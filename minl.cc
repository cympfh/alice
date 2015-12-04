#include <bits/stdc++.h>
using namespace std;

#include "./pattern.h"
#include "./text.h"
#include "./ges.h"
#include "./util.h"
#include "./setcover.h"

/*
 * Longest Common Sub-Sequence
 * by DP O(n*m)
 */

namespace minl {
  bool string_include(const Text&text, const Text&sub) {
    int n = text.size();
    int m = sub.size();
    int i = 0, j = 0;
    for (;;) {
      if (text[i] == sub[j]) {
        ++i; ++j;
      } else {
        ++i;
      }
      if (j >= m) return true;
      if (i >= n) return false;
    }
    return false;
  }

  bool language_include(const vector<Text*>&S, const Text&sigma) {
    for (Text*s : S) {
      if (not string_include(*s, sigma)) return false;
    }
    return true;
  }

  inline Text shortest_one(const vector<Text*>&S) {
    int len = S[0]->size();
    int idx = 0;
    rep (i, S.size()) if (len > S[i]->size()) len = S[i]->size();
    return *S[idx];
  }

  // string sigma2 = sigma.substr(0, j) + s.substr(i, n) + sigma.substr(j, m-j);
  Text append(const Text&sigma, int j, int m, const Text&s, int i, int n) {
    Text r;
    for (int k = 0; k < j; ++k) r.push_back(sigma[k]);
    for (int k = i; k < i+n; ++k) r.push_back(s[k]);
    for (int k = j; k < m; ++k) r.push_back(sigma[k]);
    return r;
  }

  Text minl(const vector<Text*>&S) {
    Text s = shortest_one(S);
    Text sigma = {};
    int n = s.size();
    int m = 0;

    while (n > 0) {
      for (int i = 0; i == 0 || i < s.size() - n + 1; ++i) {
more:;
        for (int j = 0; j == 0 || j < m + 1; ++j) {
          // string sigma2 = sigma.substr(0, j) + s.substr(i, n) + sigma.substr(j, m-j);
          Text sigma2 = append(sigma, j, m, s, i, n);
          if (language_include(S, sigma2)) {
            sigma = sigma2;
            m = sigma2.size();
            goto more;
          }
        }
      }
      n = min<int>(s.size() - sigma.size(), n - 1);
    }

    return sigma;
  }
}

/*
 * minl::minl の近似
 */
int word_intersection(const vector<Text*>&S) {
  if (S.size() == 0) return 0;
  if (S.size() == 1) return S[0]->size();
  vector<Text> d(S.size());
  rep (i, S.size()) {
    d[i] = *S[i];
    sort(d[i].begin(), d[i].end());
  }

  int n = d[0].size();
  for (int i = 1; i < d.size(); ++i) {
    for (int j = 0, k = 0;;) {
      if (d[0][j] == Alphabet()) {
        ++j;
      }
      else if (d[0][j] == d[i][k]) {
        ++j; ++k;
      }
      else if (d[0][j] < d[i][k]) {
        d[0][j] = Alphabet();
        ++j;
      }
      else {
        ++k;
      }
      if (j >= n) break;
      if (k >= d[i].size()) {
        for (;j<n; ++j) d[0][j] = Alphabet();
        break;
      }
    }
  }

  int r = 0;
  rep (i, n) if (d[0][i] != Alphabet()) ++r;
  return r;
}

/*
 * k-mmg
 */
namespace mmg {

  bool language_include(const Pattern&p, const vector<Text*>&doc) {
    for (auto&t: doc) if (not preceq(*t, p)) return false;
    return true;
  }

  vector<Text*> coverset(const Pattern&p, const vector<Text*>&doc) {
    vector<Text*> S;
    for (auto&t: doc) {
      if (preceq(*t, p)) S.push_back(t);
    }
    return S;
  }

  pair<vector<Text*>, vector<int>>
    coverset(const Pattern&p, const vector<Text*>&C, const vector<int>&c) {
      vector<Text*> S;
      vector<int> s;
      for (size_t i = 0; i < c.size(); ++i) {
        if (preceq(*C[i], p)) {
          S.push_back(C[i]);
          s.push_back(c[i]);
        }
      }
      return make_pair(S, s);
    }

  Pattern tighten(const Pattern&p, const vector<Text*>&C) {
    // assert ( C subseteq L(p) )
    Pattern q = p;
    Pattern r;
    assert(C.size() > 0);
    int n;

    map<string, vector<string>> dict; // dict[pos] = { word }
    for (Text* t: C) for (const Alphabet& a: *t) dict[a.pos].push_back(a.word);
    vector<string> poss;
    for (auto&kv: dict) poss.push_back(kv.first);

tighten_begin:
    if (is_text(q)) return q;
    n = q.size();

    // <A> -> a/A
    for (const string& pos: poss) {
      for (size_t i = 0; i < n; ++i) {
        if (q[i].t != POS or q[i].pos != pos) continue;
        q[i].t = WORD;
        for (string&w: dict[q[i].pos]) {
          q[i].word = w;
          if (language_include(q, C)) goto tighten_begin;
        }
        q[i].t = POS;
      }
    }

    // <> -> <> <A>
    r.resize(n+1);
    for (const string&pos: poss) {
      for (size_t i = 0; i < n; ++i) {
        r[i] = q[i];
        if (q[i].t == VAR) {
          r[i+1] = PUnit(pos);
          for (int j = i+1; j < n; ++j) r[j+1] = q[j];
          if (language_include(r, C)) {
            q = r;
            goto tighten_begin;
          }
        }
      }
    }

    // <> -> <A> <>
    r.resize(n+1);
    for (const string&pos: poss) {
      for (size_t i = 0; i < n; ++i) {
        r[i] = q[i];
        if (q[i].t == VAR and (i == 0 or q[i-1].t != VAR)) {
          r[i] = PUnit(pos);
          r[i+1] = PUnit();
          for (int j = i+1; j < n; ++j) r[j+1] = q[j];
          if (language_include(r, C)) {
            q = r;
            goto tighten_begin;
          }
        }
      }
    }

    // <> -> <A>
    for (const string& pos: poss) {
      for (size_t i = 0; i < n; ++i) {
        if (q[i].t != VAR) continue;
        q[i].t = POS;
        q[i].pos = pos;
        if (language_include(q, C)) goto tighten_begin;
        q[i].t = VAR;
      }
    }

    // <> -> <> <>
    r.resize(n+1);
    for (size_t i = 0; i < n; ++i) {
      r[i] = q[i];
      if (r[i].t == VAR && (i == 0 or r[i-1].t != VAR)) {
        r[i+1] = PUnit();
        for (int j = i + 1; j < n; ++j) r[j+1] = q[j];
        if (language_include(r, C)) {
          q = r;
          goto tighten_begin;
        }
      }
    }

    return q;
  }

  vector<tuple<Pattern, vector<Text*>, vector<int>> >
    cspc(const Pattern&p, const vector<Text*>&C, const vector<int>&c) {
      assert(C.size() == c.size());

      const int n = p.size();
      Pattern q = p;
      vector<tuple<Pattern, vector<Text*>, vector<int>> > ret;

      map<string, vector<string>> dict; // dict[pos] = { word }
    for (auto&t: C) for (const Alphabet& a: *t) dict[a.pos].push_back(a.word);

    vector<string> poss;
    for (auto&kv: dict) poss.push_back(kv.first);

    // <A> -> a/A
    for (const string& pos: poss) {
      for (size_t i = 0; i < n; ++i) {
        if (q[i].t != POS or q[i].pos != pos) continue;
        q[i].t = WORD;
        for (string&w: dict[q[i].pos]) {
          q[i].word = w;
          auto Ss = coverset(q, C, c);
          assert(Ss.second.size() <= C.size() && "<A> -> a/A");
          if (Ss.second.size() > 0) {
            ret.push_back(make_tuple(q, Ss.first, Ss.second));
          }
        }
        q[i].t = WORD;
      }
    }

    // <> -> <> <A>
    for (const string&pos: poss) {
      q.resize(n+1);
      for (size_t i = 0; i < n; ++i) {
        q[i] = p[i];
        if (p[i].t == VAR) {
          q[i+1] = PUnit(pos);
          for (int j = i+1; j < n; ++j) q[j+1] = p[j];
          auto Ss = coverset(q, C, c);
          assert(Ss.second.size() <= C.size() && "<> -> <> <A>");
          if (Ss.second.size() > 0) {
            ret.push_back(make_tuple(q, Ss.first, Ss.second));
          }
        }
      }
    }

    // <> -> <A> <>
    for (const string&pos: poss) {
      for (size_t i = 0; i < n; ++i) {
        q[i] = p[i];
        if (p[i].t == VAR and (i == 0 or p[i-1].t != VAR)) {
          q[i+1] = PUnit();
          q[i] = PUnit(pos);
          for (int j = i+1; j < n; ++j) q[j+1] = p[j];
          auto Ss = coverset(q, C, c);
          assert(Ss.second.size() <= C.size() && "<> -> <A> <>");
          if (Ss.second.size() > 0) {
            ret.push_back(make_tuple(q, Ss.first, Ss.second));
          }
        }
      }
    }

    // <> -> <A>
    q.resize(n);
    q = p;
    for (const string& pos: poss) {
      for (size_t i = 0; i < n; ++i) {
        if (q[i].t != VAR) continue;
        q[i].t = POS;
        q[i].pos = pos;
        auto Ss = coverset(q, C, c);
        assert(Ss.second.size() <= C.size() && "<> -> <A>");
        if (Ss.second.size() > 0) {
          ret.push_back(make_tuple(q, Ss.first, Ss.second));
        }
        q[i].t = VAR;
      }
    }

    // <> -> <> <>
    q.resize(n+1);
    for (size_t i = 0; i < n; ++i) {
      q[i] = p[i];
      if (p[i].t == VAR && (i == 0 or p[i-1].t != VAR)) {
        q[i+1] = PUnit();
        for (int j = i + 1; j < n; ++j) q[j+1] = p[j];
        auto Ss = coverset(q, C, c);
        assert(Ss.second.size() <= C.size() && "<> -> <> <>");
        if (Ss.second.size() > 0) {
          ret.push_back(make_tuple(q, Ss.first, Ss.second));
        }
      }
    }

    return ret;
  }

  // クラスタのまとまり具合 [0, 1]
  double completion(const vector<Text*>&S) {
    if (S.size() < 2) return 1.0;
    int uplen = S[0]->size(); rep (i, S.size()) uplen = max<int>(uplen, S[i]->size());
    // int n = minl::minl(S).size();
    int n = word_intersection(S);
    return double(n) / double(uplen);
  }

  inline
    vector< tuple<Pattern, vector<Text*>, vector<int>> >
    kdivision(
        int k,
        const Pattern&p,
        const vector<Text*>&S,
        const vector<int>&s,
        bool DEBUG)
    {
      if (DEBUG) {
        cerr << "* k-division of " << p << " k=" << k << " for (" << s.size() << "; " << s << ")" << endl;
      }
      if (is_text(p)) { // not divisible clearly
        if (DEBUG) {{{ cerr   <<   "* not divisible clearly: " << p << endl; }}}
        return {make_tuple(p, S, s)};
      }

      vector<tuple<Pattern, vector<Text*>, vector<int>> > ps;
      ps = cspc(p, S, s);

      /*
       * SetCoverProblem
       */
      vector< tuple<Pattern, vector<Text*>, vector<int>> > div;
      {{{
          vector<pair<set<int>, double>> ls(ps.size());
          for (size_t i = 0; i < ps.size(); ++i) {
            auto&p = get<0>(ps[i]);
            auto&C = get<1>(ps[i]);

            double w;
            {
              double s = double(C.size()) / S.size();
              double c = completion(C);
              // w = s * log(s) * log(c);
              w = s * log(s * .9) * log(c * .9);
            }

            set<int> s; for (int j: get<2>(ps[i])) s.insert(j);
            int sc = s.size();
            ls[i] = { s, -w };
            // if (DEBUG) cerr << s.size() << ' ' << completion(C) << endl;
          }

          if (DEBUG) {{{
            const int NN=200;
            cerr << "* cspc:" << ps.size() << endl;
            cerr << "{{{" << endl;
            for (size_t i = 0; i < ps.size(); ++i) {
              if (ps.size() > NN and NN/2 <= i and i < ps.size() - NN/2) {
                if (i == NN/2) cerr << " : (omitted)" << endl;
                else continue;
              } else {
                cerr << i << ". " << get<0>(ps[i]) << " (" << get<2>(ps[i]).size() << "; " << get<2>(ps[i]) << ')';
                cerr << "; w = " << ls[i].second;
                cerr << endl;
              }
            }
            cerr << "}}}" << endl;
          }}}

          set<int> sol;
          sol = setcover(ls);
          if (sol.size() > k) {
            sol = unweighted_setcover(ls);
          }
          for (auto i: sol) div.push_back(ps[i]);
        }}}

      if (DEBUG) {{{
        cerr << "* div:" << div.size() << endl;
        cerr << "{{{" << endl;
        for (size_t i = 0; i < div.size(); ++i) {
          cerr << i << ". " << get<0>(div[i]) << " (" << get<1>(div[i]).size() << "; " << get<2>(div[i]) << ')' << endl;
        }
        cerr << "}}}" << endl;
      }}}

      // when not divisible
      if (div.size() < 2) {
        if (DEBUG) {{{ cerr << "* not divisible: " << p << endl; }}}
        return {make_tuple(p, S, s)};
      }

      return div;
    }

  vector< tuple<Pattern, vector<Text*>, vector<int>> >
    kmmg(int k, vector<Text*>&doc, Pattern p0, bool DEBUG)
    {
      if (DEBUG) {
        cerr << "# kmmg from " << p0 << endl;
        trace(doc.size());
        trace(k);
      }

      vector< tuple<Pattern, vector<Text*>, vector<int>> > ret;

      p0 = tighten(p0, doc);

      vector<int> cover_count(doc.size(), 1); // num of patterns covering doc[i]
      vector<int> doc_i = iota(doc.size());

      priority_queue<
        pair<int, tuple<Pattern, vector<Text*>, vector<int>> >
        > pcs;
      pcs.push({ 1, make_tuple(p0, doc, doc_i)});

      int cx = 0;
      while (not pcs.empty()) {
        auto it         = pcs.top().second; pcs.pop();
        Pattern&p       = get<0>(it);
        vector<Text*>&C = get<1>(it);
        vector<int>&c   = get<2>(it);

        vector<int> s; // = c - L(P - p)
        for (int i: c) {
          if (cover_count[i] == 1) s.push_back(i);
          --cover_count[i];
        }
        if (s.size() == 0) continue;

        vector<Text*> S; // = s
        for (int i: s) S.push_back(doc[i]);

        p = tighten(p, S);
        auto div = kdivision((k - ret.size() - pcs.size()), p, S, s, DEBUG);

        if (div.size() < 2 or ret.size() + pcs.size() + div.size() > k) {
          ret.push_back(make_tuple(p, S, s));
          continue;
        }

        { // push to queue
          for (auto&qc: div) {
            for (int i: get<2>(qc)) ++cover_count[i];
            int covering_size = get<2>(qc).size();
            pcs.push({ covering_size, qc });
          }
        }
      }

      if (DEBUG) {
        cerr << "* " << ret.size() << " patterns derived" << endl;
        for (size_t i = 0; i < ret.size(); ++i) {
          cerr << (i+1) << ". " << ret[i] << endl;
        }
      }

      return ret;
    }

  vector< tuple<Pattern, vector<Text*>, vector<int>> >
    kmmg(int k, vector<Text*>&doc, bool DEBUG)
    {
      Pattern top = { PUnit() };
      return kmmg(k, doc, top, DEBUG);
    }

}
