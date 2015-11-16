#include <bits/stdc++.h>
using namespace std;

static map<string, int> table;
static vector<string> rev_table;

using Symbol = int;

int quote(const string&s) {
  if (table.count(s)) return table[s];
  int i = rev_table.size();
  table[s] = i;
  rev_table.push_back(s);
  return i;
}

string unquote(int i) {
  if (i < rev_table.size()) return rev_table[i];
  return "";
}


