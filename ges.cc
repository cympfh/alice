#include <bits/stdc++.h>
using namespace std;
#define rep(i,n) for(int i=0;i<(n);++i)
#define trace(var) cerr<<">>> "<<#var<<" = "<<var<<endl;

#include "./pattern.h"
#include "./text.h"
#include "./util.h"

/*
 * non-erasing generalization system <=
 */

inline
bool preceq(const Alphabet&a, const PUnit&u) {
  if (u.t == VAR) return true;
  return (a.pos == u.pos) and (u.t == POS or a.word == u.word);
}

int table[200][200];

bool search_preceq(const Text&s, const Pattern&p) {
  int n = s.size();
  int m = p.size();
  if (n < m) return false;
  if (m == 0) return false;
  // trace(make_pair(n, s)); trace(make_pair(m, p));

  rep (i, n) rep (j, m) table[i][j] = 0;
  queue<pair<int,int>> q;
  if (preceq(s[0], p[0])) q.push({0, 0});

  while (not q.empty()) {
    auto P = q.front(); q.pop();
    int i = P.first;
    int j = P.second;
    if (i == n-1 and j == m-1) return true;
    if (table[i][j]) continue;
    if (i >= n or j >= m) continue;

    if (i < n-1 and j < m-1)
      if (preceq(s[i+1], p[j+1])) {
        if (table[i+1][j+1]) continue;
        q.push({i+1, j+1});
      }
    if (i < n-1)
      if (p[j].t != WORD and preceq(s[i+1], p[j])) {
        if (table[i+1][j]) continue;
        q.push({i+1, j});
      }
    table[i][j] = 1;
  }

  return false;
}

bool naiive_preceq(const Text&s, const Pattern&p) {
  int n = s.size();
  int m = p.size();
  if (n < m) return false;
  if (m == 0) return false;

  // tail matching
  while (m > 0 and p[m-1].t != VAR) {
    if (preceq(s[n-1], p[m-1])) {
      --n; --m;
    } else {
      return false;
    }
  }
  if (n == 0 and m == 0) return true;
  if (m == 0) return false;

  // `p` ends with "<>": "<>[.<>]*<>" or "[.<>]*<>"

  int __pos = 0; // of text
  int __begin = 0; // of pattern

  // head matching
  while (p[__begin].t != VAR) {
    if (preceq(s[__pos], p[__begin])) {
      ++__pos; ++__begin;
    } else {
      return false;
    }
    if (__pos >= n) return false;
  }

  // `p` begins with "<>": "<>[.<>]*<>"
  
  int __end; // of pattern
  for (int _ = p.size(); _ > 0; --_) { // for a segment
    while (__begin < m and p[__begin].t == VAR) {
      ++__pos; ++__begin;
    }
    if (__begin >= m and __pos <= n) return true;
    if (__pos >= n) return false;

    for (__end = __begin + 1; __end < m; ++__end)
      if (p[__end].t == VAR) break;

    for (; __pos < n - (__end - __begin); ++__pos) {
      bool res = true;
      for (int i = 0; i < __end - __begin; ++i) {
        if (not preceq(s[__pos + i], p[__begin + i])) {
          res = false;
          break;
        }
      }
      if (res) {
        __pos += (__end - __begin);
        __begin = __end;
        break;
      }
    }
    if (__pos >= n - (__end - __begin)) return false;
  }
  return false;
}

bool preceq(const Text&s, const Pattern&p) {
  return naiive_preceq(s, p);
}

bool preceq(const PUnit&a, const PUnit&b) {
  if (b.t == VAR) return true;
  return a.pos == b.pos and (b.t == POS or a.word == b.word);
}

bool preceq(const Pattern&q, const Pattern&p) {
  int n = q.size();
  int m = p.size();
  if (n < m) return false;

  // tail matching
  while (m > 0 and p[m-1].t != VAR) {
    if (preceq(q[n-1], p[m-1])) {
      --n; --m;
    } else {
      return false;
    }
  }
  if (n == 0 and m == 0) return true;
  if (m == 0) return false;

  // p should be "<>[.<>]*<>" or "[.<>]*<>"

  int __pos = 0; // of text
  int __begin = 0; // of pattern

  // head matching
  while (p[__begin].t != VAR) {
    if (preceq(q[__pos], p[__begin])) {
      ++__pos; ++__begin;
    } else {
      return false;
    }
    if (__pos >= n) return false;
  }

  // p should be "<>[.<>]*<>"
  
  int __end;
  for (int i = p.size();i > 0; --i) {
    while (__begin < m and p[__begin].t == VAR) {
      ++__pos; ++__begin;
    }
    if (__begin >= m and __pos <= n) return true;
    if (__pos >= n) return false;

    for (__end = __begin + 1; __end < m; ++__end)
      if (p[__end].t == VAR) break;

    for (; __pos < n - (__end - __begin); ++__pos) {
      bool res = true;
      for (int i = 0; i < __end - __begin; ++i) {
        if (not preceq(q[__pos + i], p[__begin + i])) {
          res = false;
          break;
        }
      }
      if (res) {
        __pos += (__end - __begin);
        __begin = __end;
        break;
      }
    }
    if (__pos >= n - (__end - __begin)) return false;
  }
  return false;
}

Pattern text_to_pattern(const Text&t) {
  Pattern p(t.size());
  for (size_t i = 0; i < t.size(); ++i)
    p[i] = PUnit(t[i].pos, t[i].word);
  return p;
}

// num of words required to fill
int count_filling(const Text&t, const Pattern&p) {
  // assert (t <= p)
  int n = t.size();
  for (size_t i = 0; i < p.size(); ++i) if (p[i].t == WORD) --n;
  return n;
}

