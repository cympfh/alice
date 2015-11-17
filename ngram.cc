#include <bits/stdc++.h>
using namespace std;
#define rep(i,n) for(int i=0;i<(n);++i)
#define trace(var) cerr<<">>> "<<#var<<" = "<<var<<endl;
#include "util.h"

#include "ngram.h"

template<typename T>
bool grt(const pair<int, T>&left,const pair<int, T>&right) {
  return left.first > right.first;
}

vector<pair<int, Gram>>
ngram(const vector<int>&ns, const vector<Text>&doc) {
  map<Gram, int> dict;

  for (auto&t: doc) {
    int m = t.size();
    for (int n: ns) {
      for (int i = 0; i <= m - n; ++i) {
        Gram g;
        // pre-char
        get<0>(g) = (i == 0);
        // inner
        for (int j = 0; j < n; ++j) {
          get<2>(g).push_back(t.at(i+j));
        }
        // post-char
        get<1>(g) = (i == m-n);
        dict[g]++;
      }
    }
  }

  vector<pair<int, Gram>> ret;
  for (auto kv: dict) ret.push_back({ kv.second, kv.first });
  sort(begin(ret), end(ret), grt<Gram>);
  // sort(begin(ret), end(ret));
  // reverse(begin(ret), end(ret));

  return ret;
}
