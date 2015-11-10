#include <bits/stdc++.h>
using namespace std;

struct Alphabet {
  string pos, word;
  Alphabet(string p, string w) : pos(p), word(w) {}

  bool operator==(Alphabet&rh) {
    return this->word == rh.word and this->pos == rh.pos;
  }

  bool operator<(Alphabet&rh) {
    return this->pos < rh.pos or this->word < rh.word;
  }
};

bool operator<(const Alphabet&lh, const Alphabet&rh) {
  return lh.pos < rh.pos or lh.word < rh.word;
}

std::ostream& operator<<(std::ostream&os, const Alphabet&a) {
  return os << a.word << '/' << a.pos;
}

using Text = vector<Alphabet>;
