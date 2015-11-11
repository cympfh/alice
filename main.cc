#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;
#define rep(i,n) for(int i=0;i<(n);++i)
#define trace(var) cerr<<">>> "<<#var<<" = "<<var<<endl;

#include "./pattern.h"
#include "./text.h"
#include "./ges.h"
#include "./read.h"
#include "./minl.h"
#include "./util.h"

void usage() {
  cerr << "Usage: alice [options] [file]" << endl;
  cerr << "  if file not specified, stdin will be read" << endl;
  cerr << "options:" << endl;
  cerr << "  -D, --delimiter            delimiter character between word and POS in text" << endl;
  cerr << "  -l, --limit                preserved option" << endl;
  cerr << "  -P, --pool-size            maximum pool size (= 20)" << endl;
  cerr << "  -B, --book-size            maximum size of a book (= 5)" << endl;
  cerr << "  -L, --log <filename=>      output logfile" << endl;
  cerr << "  -I  <int>                  only when (time < I) or (T - time < I) (= T)" << endl;
  cerr << "  -D, --debug                debug mode" << endl;
  cerr << "  -?, --help                 it is this" << endl;
  exit(0);
}

int main(int argc, char *argv[])
{

  random_device dev;
  mt19937 rand(dev());

  /*
   * parse cmdline
   */

  bool DEBUG = false;
  char delimiter = '_';
  double limit = 1.0;
  bool log_mode = false;
  ofstream log;
  int log_I = -1;
  int POOL_SIZE = 20;
  int BOOK_SIZE = 5;

  int GOOD_COVERING_FROM_POOL = 2; // これ以上被覆してたら良いとする
  int GOOD_COVERING_FROM_BOOK = 3;

  vector<string> restargs;
  for (size_t i = 1; i < argc; ++i) {
    string arg = argv[i];
    string nextarg = (i < argc-1) ? argv[i+1] : "";
    if (arg == "-d" or arg == "--delimiter") {
      delimiter = nextarg[0];
      ++i;
    }
    else if (arg == "-l" or arg == "--limit") {
      limit = atof(argv[i+1]);
      ++i;
    }
    else if (arg == "-P" or arg == "--pool-size") {
      POOL_SIZE = atoi(argv[i+1]);
      ++i;
    }
    else if (arg == "-B" or arg == "--book-size") {
      BOOK_SIZE = atoi(argv[i+1]);
      ++i;
    }
    else if (arg == "-L" or arg == "--log") {
      log_mode = true;
      log.open(nextarg);
      ++i;
    }
    else if (arg == "-I") {
      log_I = atoi(argv[i+1]);
      ++i;
    }
    else if (arg == "-D" or arg == "--debug") {
      DEBUG = true;
    }
    else if (arg == "-?" or arg == "--help") {
      usage();
    }
    else if (arg[0] == '-') {
      cerr << "unknown option:" << arg << endl;
      usage();
    }
    else {
      restargs.push_back(arg);
    }
  }

  if (DEBUG) {
    trace(delimiter);
    trace(limit);
    trace(log_mode);
    if (log_mode) trace(log_I);
    trace(restargs);
    trace(POOL_SIZE);
    trace(BOOK_SIZE);
  }

  if (log_mode) {
    log << "POOL_SIZE=" << POOL_SIZE << endl;
    log << "BOOK_SIZE=" << BOOK_SIZE << endl;
  }

  /*
   * scan text
   */
  vector<Text> doc;
  if (restargs.size() > 0) {
    cerr << "scan from text:" << restargs[0] << " ... ";
    if (log_mode) log << "scan from text:" << restargs[0] << endl;
    ifstream sin(restargs[0]);
    for (Text t; t = read_text(sin, delimiter, 2), sin;) doc.push_back(t);
  } else {
    // cin
    cerr << "scan from stdin" << " ... ";
    if (log_mode) log << "scan from stdin" << endl;
    for (Text t; t = read_text(cin, delimiter, 2), cin;) doc.push_back(t);
  }
  cerr << "finished." << endl;

  if (DEBUG) trace(doc.size());
  if (log_mode) log << "doc.size = " << doc.size() << endl;

  /*
   * streaming inference
   */

  // collection of (Pattern, covering, up-length-in-covering)
  // (パターン、被覆集合、被覆集合における最長)
  vector<tuple<Pattern, vector<Text*>, int>> book;

  // collection of Text-s not yet learned
  // 学習できていないテキスト
  vector<Text*> pool;

  vector<Text*> textbook(POOL_SIZE); // pool から選んできて kmmg に投げる用

  const size_t T = doc.size();
  for (size_t time = 0; time < T; ++time)
  {
    bool logging = log_mode and ( (time < log_I) or ((T - time) < log_I) );

    Text&t = doc[time];

    cerr << "\rtime=" << time;
    if (logging) {
      log << "#-- time=" << time << " : " << t << endl;
      log << endl;
    }

    // exists? a pattern `p` s.t. `t <= p`
    int idx = -1;
    {
      int mf = t.size() + 2;
      for (size_t j = 0; j < book.size(); ++j) {
        if (preceq(t, get<0>(book[j]))) {
          int f = count_filling(t, get<0>(book[j]));
          if (f < mf) {
            mf = f;
            idx = j;
          }
        }
      }
    }

    if (idx >= 0) // exists `p` (t <= p) in book
    {
      // put into book
      if (logging) log << "# put into book" << endl;
      get<1>(book[idx]).push_back(&doc[time]);
      get<2>(book[idx]) = max<int>(get<2>(book[idx]), t.size());

      // divide
      int m = get<1>(book[idx]).size();
      if (m > BOOK_SIZE and (not is_text(get<0>(book[idx]))))
      {
        auto&p = get<0>(book[idx]);
        auto&C = get<1>(book[idx]);
        auto c = iota(C.size());

        auto div = kdivision(C.size(), p, C, c);
        bool ok = div.size() >= 2;

        if (ok) {
          ok = false;
          for (size_t i = 0; i < div.size(); ++i) {
            if (get<2>(div[i]).size() >= GOOD_COVERING_FROM_BOOK) ok = true;
          }
        }

        if (logging) {
          log << endl;
          log << "* div of book[" << idx << "] {{{" << endl;
          for (int i = 0; i < get<1>(book[idx]).size(); ++i)
            log << i << ". " << *(get<1>(book[idx])[i]) << endl;
          log << "div result" << endl;
          for (int i = 0; i < div.size(); ++i) {
            log << i << ". " << (get<0>(div[i])) << " (" << (get<1>(div[i]).size()) << ')' << endl;
          }
          log << "* div_success=" << ok << endl;
          log << "}}}" << endl;
          log << endl;
        }

        if (ok) {
          book.erase(begin(book) + idx);
          for (size_t j = 0; j < div.size(); ++j) {
            auto&p = get<0>(div[j]);
            auto&C = get<1>(div[j]);
            if (C.size() >= GOOD_COVERING_FROM_BOOK) {
              int uplen = 0;
              for (auto&t: C) uplen = max<int>(uplen, t->size());
              book.push_back(make_tuple(p, C, uplen));
            } else {
              for (auto&t: C) pool.push_back(t);
            }
          }
        }
      }
    }
    else
    {
      // put into pool
      if (logging) log << "# put into pool" << endl;
      pool.push_back(&doc[time]);

      if (pool.size() % 5 == 0) // ちょっとずるいけど、どうせ変わらん
      {

        int cx = 0;
        while (cx < 5 and pool.size() > POOL_SIZE) {

          for (size_t j = 0; j < POOL_SIZE; ++j) {
            textbook[j] = pool[j];
          }
          for (size_t j = POOL_SIZE; j < pool.size() - 1; ++j) {
            int k = rand() % j;
            if (k < POOL_SIZE) textbook[k] = pool[j];
          }
          textbook[POOL_SIZE-1] = pool[pool.size()-1];

          auto result = kmmg(POOL_SIZE *2/3, textbook, false);

          vector<int> learned; // indices on pool
          for (size_t j = 0; j < result.size(); ++j) {
            auto&p = get<0>(result[j]);
            auto&C = get<1>(result[j]);
            auto&c = get<2>(result[j]);
            if (c.size() < GOOD_COVERING_FROM_POOL) continue;
            bool flg = false;
            for (size_t k = 0; k < book.size(); ++k) {
              if (preceq(get<0>(book[k]), p)) { flg=true; break; }
            }
            if (flg) continue;
            int uplen = 0;
            for (auto&t: C) uplen = max<int>(uplen, t->size());
            book.push_back(make_tuple(p, C, uplen));
            for (int idx: c) learned.push_back(idx);
          }

          bool learned_success = learned.size() > 0;

          if (logging) {
            log << endl;
            log << "* learning from pool:" << (learned_success ? "success" : "failed") << " {{{" << endl;
            log << "** textbook" << endl;
            for (int i = 0; i < textbook.size(); ++i)
              log << i << ". " << *(textbook[i]) << endl;
            log << "** kmmg result" << endl;
            for (int i = 0; i < result.size(); ++i) {
              log << i << ". " << (get<0>(result[i])) << " (" << (get<1>(result[i]).size()) << ')' << endl;
            }
            log << "}}}" << endl;
            log << endl;
          }

          if (learned_success) {
            // erase learned texts
            sort(begin(learned), end(learned), std::greater<int>());
            for (int idx: learned) pool.erase(begin(pool) + idx);
            cx = 0;
          } else {
            ++cx;
          }

        }
      }
    }

    if (logging) {
      log << "## book:" << book.size() << " {{{" << endl;
      for (size_t j = 0; j < book.size(); ++j) {
        log << "  " << j << ". "
          << get<0>(book[j])
          << " (" << get<1>(book[j]).size()
          << ", " << get<2>(book[j])
          << ")" << endl;
      }
      log << "}}}" << endl;
      log << "## pool:" << pool.size() << " {{{" << endl;
      /*
      for (size_t j = 0; j < pool.size(); ++j) {
        log << "  " << j << ". " << *(pool[j]) << endl;
      }
      */
      log << "}}}" << endl;
      log << endl;
    }

  } // end of time
  cerr << endl;

  cout << "# book" << endl;
  for (size_t j = 0; j < book.size(); ++j) {
    cout << get<0>(book[j]) << endl;
  }
  cout << endl;
  cout << "# pool" << endl;
  for (size_t j = 0; j < pool.size(); ++j) {
    cout << *(pool[j]) << endl;
  }

  return 0;
}

