#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;
#define rep(i,n) for(int i=0;i<(n);++i)
#define trace(var) cerr<<">>> "<<#var<<" = "<<var<<endl;
const int inf = 1e9;

#include "pattern.h"
#include "text.h"
#include "ges.h"
#include "read.h"
#include "minl.h"
#include "util.h"

const int GOOD_COVERING_FROM_POOL = 2; // これ以上被覆してたら良いとする
const int GOOD_COVERING_FROM_BOOK = 2;

vector<tuple<Pattern, vector<Text*>, int>> *book_ptr = nullptr;
vector<Text*> *pool_ptr = nullptr;
bool BOOK_ONLY = false;
bool FREQ_OUTPUT = false;

void sig_handler(int signo)
{
  if (signo != SIGINT) return;
  if (book_ptr == nullptr) return;
  auto book = *book_ptr;
  auto pool = *pool_ptr;
  if (not BOOK_ONLY) {
    cout << "# book" << endl;
  }
  {
    for (size_t j = 0; j < book.size(); ++j) {
      if (get<1>(book[j]).size() == 0) continue;
      if (FREQ_OUTPUT) cout << get<1>(book[j]).size() << ' ';
      cout << get<0>(book[j]) << endl;
    }
  }
  if (not BOOK_ONLY) {
    cout << "# pool" << endl;
    for (size_t j = 0; j < pool.size(); ++j) {
      cout << *(pool[j]) << endl;
    }
  }
  exit(0);
}

void usage() {
  cerr << "Usage: alice [options] [file]" << endl;
  cerr << "  if file not specified, stdin will be read" << endl;
  cerr << "options:" << endl;
  cerr << "  -d, --delimiter                    delimiter character between word and POS in text" << endl;
  cerr << "  -l, --limit                        preserved option" << endl;
  cerr << "  -S, --seed <filename=>             seed pattern" << endl;
  cerr << "  -P, --pool-size <int>              maximum pool size (= 30)" << endl;
  cerr << "  -B, --book-size <int>              maximum size of a book (= 20)" << endl;
  cerr << "  -L, --log <filename=>              output logfile" << endl;
  cerr << "  --book-only                        output only book patterns" << endl;
  cerr << "  --frequency                        output frequency of each pattern before it" << endl;
  cerr << "  -I  <int>                          only when (time < I) or (T - time < I) (= T)" << endl;
  cerr << "  -D, --debug                        debug mode" << endl;
  cerr << "  -q, --quiet                        quiet mode" << endl;
  cerr << "  -?, --help                         it is this" << endl;
  exit(0);
}

/*
 * book[idx] を division します
 */
void book_div(int idx, ostream*logptr) {
  assert(book_ptr != nullptr);
  assert(pool_ptr != nullptr);
  assert(idx >= 0);
  auto&book = *book_ptr;
  auto&pool = *pool_ptr;

  if (is_text(get<0>(book[idx]))) return;

  assert(idx < book.size());
  bool logging = logptr != nullptr;
  ostream&log = *logptr;

  auto&p = get<0>(book[idx]);
  auto&C = get<1>(book[idx]);
  auto c = iota(C.size());

  // is seed pattern?
  if (get<2>(book[idx]) == inf) {
    // cerr << p << " -> ";
    p = mmg::tighten(p, C);
    // cerr << p << endl;
    int max_len = 0;
    for (auto&t: C) max_len = max<int>(max_len, t->size());
    get<2>(book[idx]) = max_len;
  }

  auto div = mmg::kdivision(C.size() /2, p, C, c, true);
  bool ok = div.size() >= 2;

  if (ok) {
    ok = false;
    for (size_t i = 0; i < div.size(); ++i) {
      int cs = get<2>(div[i]).size();
      if (cs >= GOOD_COVERING_FROM_BOOK) ok = true;
    }
  }

  if (logging) {
    log << endl;
    log << "* div of book[" << idx << "] (" << get<0>(book[idx]) << ") {{{" << endl;
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

  set<Text*> pushed;

  if (ok) {
    book.erase(begin(book) + idx);
    for (size_t j = 0; j < div.size(); ++j) {
      auto&p = get<0>(div[j]);
      auto&C = get<1>(div[j]);
      vector<Text*> S;

      bool ok = false;
      if (C.size() > GOOD_COVERING_FROM_BOOK) {
        for (auto&t: C) {
          if (not pushed.count(t)) {
            S.push_back(t);
          }
        }
        if (S.size() > GOOD_COVERING_FROM_BOOK) {
          int uplen = 0;
          for (auto&t: S) {
            uplen = max<int>(uplen, t->size());
            pushed.insert(t);
          }
          p = mmg::tighten(p, S);
          book.push_back(make_tuple(p, S, uplen));
          ok = true;
        }
      }

      if (not ok) {
        for (auto&t: C) if (not pushed.count(t)) {
          pushed.insert(t);
          pool.push_back(t);
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{

  random_device dev;
  mt19937 rand(dev());

  if (signal(SIGINT, sig_handler) == SIG_ERR) {
    fprintf(stderr, "\ncan't catch SIGINT\n");
  }

  /*
   * parse cmdline
   */

  bool DEBUG = false;
  bool QUIET = false;
  char delimiter = '_';
  double limit = 1.0;
  bool log_mode = false;
  ofstream log;
  int log_I = -1;
  int POOL_SIZE = 30;
  int BOOK_SIZE = 20;
  string seedfile = "";

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
    else if (arg == "-S" or arg == "--seed") {
      seedfile = nextarg;
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
    else if (arg == "--book-only") {
      BOOK_ONLY = true;
    }
    else if (arg == "--freq" or arg == "--frequency") {
      FREQ_OUTPUT = true;
    }
    else if (arg == "-D" or arg == "--debug") {
      DEBUG = true;
    }
    else if (arg == "-q" or arg == "--quiet") {
      QUIET = true;
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
    trace(seedfile);
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
    read_doc(sin, doc, delimiter);
  } else {
    cerr << "scan from stdin" << " ... ";
    if (log_mode) log << "scan from stdin" << endl;
    read_doc(cin, doc, delimiter);
  }
  cerr << "finished." << endl;

  if (DEBUG) trace(doc.size());
  if (log_mode) log << "doc.size = " << doc.size() << endl;

  // collection of (Pattern, covering, up-length-in-covering)
  // (パターン、被覆集合、被覆集合における最長)
  vector<tuple<Pattern, vector<Text*>, int>> book;

  // collection of Text-s not yet learned
  // 学習できていないテキスト
  vector<Text*> pool;

  book_ptr = &book;
  pool_ptr = &pool;

  /*
   * Scan Seed Patterns
   * and put into book
   */
  if (seedfile != "")
  {
    ifstream sin(seedfile);
    for (Pattern p; p = read_pattern(sin, '/'), sin; ) {
      book.push_back(make_tuple(p, vector<Text*>(), inf));
    }
    if (log_mode) {
      log << book.size() << " patterns specified as seeds" << endl;
    }
  }

  /*
   * stream learning
   */

  const size_t T = doc.size();
  for (size_t time = 0; time < T; ++time)
  {
    bool logging = log_mode and ( (time < log_I) or ((T - time) < log_I) );

    Text&t = doc[time];

    if (not QUIET) cerr << "\rtime=" << time;
    if (logging) {
      log << "#-- time=" << time << " : " << t << endl;
      log << endl;
    }

    if (DEBUG) {
      int c = pool.size();
      rep (i, book.size()) {
        c += get<1>(book[i]).size();
      }
      assert(time == c);
    }

    // exists? a pattern `p` s.t. `t <= p`
    vector<int> matching_indices;
    {
      for (int j = 0; j < book.size(); ++j) {
        if (preceq(t, get<0>(book[j]))) matching_indices.push_back(j);
      }
    }

    if (not matching_indices.empty()) {
      // put into book
      // for (int idx: matching_indices) {
      { int idx = matching_indices[0];//rand() % matching_indices.size()];
        if (logging) log << "# put into book[" << idx << "]" << endl;
        get<1>(book[idx]).push_back(&doc[time]);
        get<2>(book[idx]) = max<int>(get<2>(book[idx]), t.size());
        if (get<1>(book[idx]).size() > BOOK_SIZE) {
          book_div(idx, (logging ? &log : nullptr));
        }
      }
    } else {
      // put into pool
      if (logging) log << "# put into pool" << endl;
      pool.push_back(&doc[time]);

      if (pool.size() % 5 == 0) // ちょっとずるいけど、どうせ変わらん
      {
        int cx = 0; // 5回連続で失敗したら失敗にする
        while (cx < 5 and pool.size() > POOL_SIZE) {
          vector<Text*> textbook(POOL_SIZE); // random sampled from pool
          vector<int> textbook_idx(POOL_SIZE);
          for (size_t j = 0; j < POOL_SIZE-1; ++j) {
            textbook[j] = pool[j];
            textbook_idx[j] = j;
          }
          for (size_t j = POOL_SIZE-1; j < pool.size() - 1; ++j) {
            int k = rand() % j;
            if (k < POOL_SIZE) {
              textbook[k] = pool[j];
              textbook_idx[k] = j;
            }
          }
          textbook[POOL_SIZE-1] = pool[pool.size()-1];
          textbook_idx[POOL_SIZE-1] = pool.size() - 1;

          auto result = mmg::kmmg(POOL_SIZE /2, textbook, false);

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
            for (int id: c) {
              learned.push_back(textbook_idx[id]);
            }
          }

          bool learned_success = learned.size() > 0;

          if (logging) {
            log << endl;
            log << "* learning from pool:" << (learned_success ? "success" : "failed") << " {{{" << endl;
            log << "** textbook (" << textbook.size() << ")" << endl;
            // rep (i, textbook.size()) log << i << ". " << *(textbook[i]) << endl;
            log << "** kmmg result" << endl;
            rep (i, result.size())
              log << i << ". " << (get<0>(result[i])) << " (" << (get<1>(result[i]).size()) << ')' << endl;
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
      for (size_t j = 0; j < pool.size(); ++j) {
        log << "  " << j << ". " << *(pool[j]) << endl;
      }
      log << "}}}" << endl;
      log << endl;
    }

  } // end of stream learning
  if (not QUIET) cerr << endl;

  for (size_t j = 0; j < book.size(); ++j) {
    if (get<2>(book[j]) == inf and get<1>(book[j]).size() > 0)
      get<0>(book[j]) = mmg::tighten(get<0>(book[j]), get<1>(book[j]));
  }

  if (not BOOK_ONLY) {
    cout << "# book" << endl;
  }
  {
    for (size_t j = 0; j < book.size(); ++j) {
      if (get<1>(book[j]).size() == 0) continue;
      if (FREQ_OUTPUT) cout << get<1>(book[j]).size() << ' ';
      cout << get<0>(book[j]) << endl;
    }
  }
  if (not BOOK_ONLY) {
    cout << "# pool" << endl;
    for (size_t j = 0; j < pool.size(); ++j) {
      cout << *(pool[j]) << endl;
    }
  }

  return 0;
}

