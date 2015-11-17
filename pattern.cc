#include <bits/stdc++.h>
using namespace std;

#include "pattern.h"

bool operator!=(const PUnit&a, const PUnit&b) {
  if (a.t != b.t) return true;
  switch (a.t) {
    case VAR:
      return false;
    case POS:
      return a.pos != b.pos;
    case WORD:
      return a.word != b.word;
  }
}

bool operator==(const PUnit&a, const PUnit&b) {
  return not (a != b);
}

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

bool is_text(const Pattern&p) {
  for (size_t i = 0; i < p.size(); ++i) if (p[i].t != WORD) return false;
  return true;
}

int fixed_size(const Pattern&p) {
  int n = 0;
  for (size_t i = 0; i < p.size(); ++i) if (p[i].t == WORD) ++n;
  return n;
}


