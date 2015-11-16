#include <bits/stdc++.h>
using namespace std;
#include "./pattern.h"
#include "./text.h"

pair<string, string> split(const string&s) {
  string w, p;
  const int n = s.size();
  int i;
  for (i = n - 1; i >= 0; --i) if (s[i] == '_') break;
  if (i == -1) for (i = n - 1; i >= 0; --i) if (s[i] == '/') break;
  if (i <= 0) { return make_pair(s, s); }
  w = s.substr(0, i);
  p = s.substr(i+1, 2);
  return make_pair(w, p);
}

/*
Pattern read_pattern(istream&is, const char&d, int up_pos_len) {
  Pattern p;
  string line; getline(is, line);
  stringstream ss(line);
  string s;
  while (ss >> s) {
    if (s == "<>") {
      p.push_back(PUnit());
    } else if (s[0] == '<') {
      p.push_back(PUnit(s.substr(1, s.size()-2)));
    } else {
      auto wp = split(s, d, up_pos_len);
      p.push_back(PUnit(wp.second, wp.first));
    }
  }
  return p;
}
*/

Text read_text(istream&is) {
  vector<Alphabet> as;
  string line; getline(is, line);
  stringstream ss(line);
  string s;
  while (ss >> s) {
    auto wp = split(s);
    as.push_back(Alphabet(wp.second, wp.first));
  }
  Text t;
  for (size_t i = 0; i < as.size(); ++i) {
    auto&a = as[i];
    int n = t.size();
    if (n > 0 and t[n-1].pos == a.pos) {
      t[n-1].word += '\t' + a.word;
      continue;
    }
    t.push_back(a);
  }
  return t;
}

