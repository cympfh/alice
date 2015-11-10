#include <bits/stdc++.h>
using namespace std;
#include "./pattern.h"
#include "./text.h"

pair<string, string> split(const string&s, const char&d, int up_pos_len) {
  string w, p;
  const int n = s.size();
  int i;
  for (i = n; i >= 0; --i) if (s[i] == d) break;
  if (i <= 0) { return make_pair(s, s); }
  w = s.substr(0, i);
  p = s.substr(i+1);
  if (up_pos_len > 0 and p.size() > up_pos_len) {
    p = p.substr(0, up_pos_len);
  }
  return make_pair(w, p);
}

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

Text read_text(istream&is, const char&d, int up_pos_len) {
  Text t;
  string line; getline(is, line);
  stringstream ss(line);
  string s;
  while (ss >> s) {
    auto wp = split(s, d, up_pos_len);
    t.push_back(Alphabet(wp.second, wp.first));
  }
  return t;
}

