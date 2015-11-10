#pragma once

struct Alphabet {
  string pos, word;
  Alphabet(string p, string w) : pos(p), word(w) {}
  bool operator==(Alphabet&);
  bool operator<(Alphabet&);
};
bool operator<(const Alphabet&, const Alphabet&);
std::ostream& operator<<(std::ostream&, const Alphabet&);

using Text = vector<Alphabet>;
