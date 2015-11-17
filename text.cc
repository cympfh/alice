#include <bits/stdc++.h>
using namespace std;

#include "text.h"

bool operator==(const Alphabet&a, const Alphabet&b) {
  return a.pos == b.pos and a.word == b.word;
}

bool operator!=(const Alphabet&a, const Alphabet&b) {
  return a.pos != b.pos or a.word != b.word;
}

bool operator<(const Alphabet&a, const Alphabet&b) {
  if (a.pos != b.pos) return a.pos < b.pos;
  return a.word < b.word;
}

std::ostream& operator<<(std::ostream&os, const Alphabet&a) {
  return os << a.word << '/' << a.pos;
}

using Text = vector<Alphabet>;
