#include <bits/stdc++.h>
using namespace std;

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

  bool operator!=(PUnit&rh) {
    if (this-> t != rh.t) return true;
    switch (this->t) {
      case VAR:
        return false;
      case POS:
        return this->pos != rh.pos;
      case WORD:
        return this->word != rh.word;
    }
  }

  bool operator==(PUnit&rh) {
    return not (*this == rh);
  }

};

bool operator<(const PUnit&rl, const PUnit&rh) {
  switch (rl.t) {
    case VAR:
      return (rh.t != VAR);
    case POS:
      if (rh.t == WORD) return true;
      if (rh.t == POS) return (rl.pos < rh.pos);
      return false;
    case WORD:
      if (rh.t == WORD) return (rl.word < rh.word);
      return false;
  }
}

std::ostream& operator<<(std::ostream&os, const PUnit&r) {
  switch (r.t) {
    case VAR:
      return os << "<>";
    case POS:
      return os << '<' << r.pos << '>';
    case WORD:
      if (r.word == r.pos) return os << r.word;
      return os << r.word << '/' << r.pos;
    default:
      return os << "(???)";
  }
}

using Pattern = vector<PUnit>;
bool is_text(const Pattern&p) {
  for (size_t i = 0; i < p.size(); ++i) if (p[i].t != WORD) return false;
  return true;
}

int fixed_size(const Pattern&p) {
  int n = 0;
  for (size_t i = 0; i < p.size(); ++i) if (p[i].t == WORD) ++n;
  return n;
}


