#include <bits/stdc++.h>
using namespace std;
#include "./pattern.h"
#include "./text.h"

pair<string, string> split(const string&s, char d) {
  string w, p;
  const int n = s.size();
  int i;
  for (i = n - 1; i >= 0; --i) if (s[i] == d) break;
  if (i <= 0) { return make_pair(s, s); }
  w = s.substr(0, i);
  p = s.substr(i+1, 2);
  return make_pair(w, p);
}

Pattern read_pattern(istream&is, char d) {
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
      auto wp = split(s, d);
      p.push_back(PUnit(wp.second, wp.first));
    }
  }
  return p;
}

void read_doc(istream&is, vector<Text>&doc, char d) {
  string line;
  for (;getline(is, line);) {
    if (line == "") continue;
    Text t;
    stringstream ss(line);
    string w;
    while (ss >> w) {
      auto wp = split(w, d);
      t.push_back(Alphabet(wp.second, wp.first));
    }
    doc.push_back(t);
  }
}
