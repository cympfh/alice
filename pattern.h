#pragma once

enum PUnitType {
  VAR = 0, POS, WORD
};

struct PUnit
{
  PUnitType t;
  string pos, word;

  PUnit() : t(VAR) {}
  PUnit(string p) : t(POS), pos(p) {}
  PUnit(string p, string w) : t(WORD), pos(p), word(w) {}
};

bool operator!=(const PUnit&a, const PUnit&b);
bool operator==(const PUnit&a, const PUnit&b);
bool operator<(const PUnit&, const PUnit&);
std::ostream& operator<<(std::ostream&, const PUnit&);

using Pattern = vector<PUnit>;
bool is_text(const Pattern&);
int fixed_size(const Pattern&);
