#pragma once

struct Alphabet {
  string pos, word;
  Alphabet(){}
  Alphabet(string p, string w) : pos(p), word(w) {}
};

bool operator==(const Alphabet&a, const Alphabet&b);
bool operator!=(const Alphabet&a, const Alphabet&b);
bool operator<(const Alphabet&a, const Alphabet&b);
std::ostream& operator<<(std::ostream&, const Alphabet&);

using Text = vector<Alphabet>;
