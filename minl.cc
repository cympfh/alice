#include <bits/stdc++.h>
using namespace std;

#include "./pattern.h"
#include "./text.h"
#include "./ges.h"
#include "./util.h"
#include "./setcover.h"

/*
 * k-mmg
 */

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
  vector<string> poss = { "NN" };
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
  for (const string&pos: poss) {
    r.resize(n+1);
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
    if (pos == "CD") continue;
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

  vector<string> poss = { "NN" };
  for (auto&kv: dict) poss.push_back(kv.first);

  // <A> -> a/A
  for (const string& pos: poss) {
    if (pos == "CD") continue;
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

  if (DEBUG) {{{
    cerr << "* cspc:" << ps.size() << endl;
    cerr << "{{{" << endl;
    for (size_t i = 0; i < 10; ++i) {
      if (i >= ps.size()) continue;
      cerr << i << ". " << get<0>(ps[i]) << " (" << get<2>(ps[i]) << ')' << endl;
    }
    if (ps.size() > 20) {
      cerr << " : (omitted)" << endl;
    }
    for (size_t i = ps.size() - 10; i < ps.size(); ++i) {
      if (i < 10) continue;
      cerr << i << ". " << get<0>(ps[i]) << " (" << get<2>(ps[i]) << ')' << endl;
    }
    cerr << "}}}" << endl;
  }}}

  /*
   * SetCoverProblem
   */
  vector< tuple<Pattern, vector<Text*>, vector<int>> > div;
  {{{
    vector<pair<set<int>, int>> ls(ps.size());
    for (size_t i = 0; i < ps.size(); ++i) {
      int w = 1e9;
      auto&p = get<0>(ps[i]);
      auto&C = get<1>(ps[i]);
      for (auto&t: C) w = max<int>(w, t->size());
      w -= fixed_size(p);
      set<int> s; for (int j: get<2>(ps[i])) s.insert(j);
      ls[i] = { s, w };
    }

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
