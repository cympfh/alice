#pragma once

#define rep(i,n) for(int i=0;i<(n);++i)
#define trace(var) cerr<<">>> "<<#var<<" = "<<var<<endl;

template<class T>
inline
std::ostream& operator<<(std::ostream&os, const vector<T>&v) {
  if (v.size() == 0) return os;
  os << v[0];
  for (size_t i = 1; i < v.size(); ++i) os << ' ' << v[i];
  return os;
}


template<class T, class U>
inline
std::ostream& operator<<(std::ostream&os, const pair<T,U>&p) {
  return os << '(' << p.first << ", " << p.second << ')';
}

template<class T, class U>
inline
std::ostream& operator<<(std::ostream&os, const tuple<T,U>&t) {
  return os << '(' << get<0>(t) << ", " << get<1>(t) << ')';
}

template<class S, class T, class U>
inline
std::ostream& operator<<(std::ostream&os, const tuple<S,T,U>&t) {
  return os << '(' << get<0>(t) << ", " << get<1>(t) << ", " << get<2>(t) << ')';
}

inline
vector<int> iota(int n) {
  vector<int> v(n);
  for (int i = 0; i < n; ++i) v[i] = i;
  return v;
}

