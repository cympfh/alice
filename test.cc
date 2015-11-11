#include <bits/stdc++.h>
using namespace std;
#define rep(i,n) for(int i=0;i<(n);++i)
#define trace(var) cerr<<">>> "<<#var<<" = "<<var<<endl;

#include "./pattern.h"
#include "./text.h"
#include "./ges.h"
#include "./read.h"
#include "./minl.h"
#include "./util.h"

int main() {
  /*
   * text <= pattern
   */
  {
    Text t = { Alphabet("A", "a"), Alphabet("B", "b"), Alphabet("B", "b"), Alphabet("C", "c") };
    Pattern p = { PUnit("A"), PUnit(), PUnit("B"), PUnit("C") };
    assert(preceq(t, p));
  }
  {
    Text t = { Alphabet("A", "a"), Alphabet("B", "b"), Alphabet("C", "c"), Alphabet("C", "c") };
    Pattern p = { PUnit("A"), PUnit(), PUnit("B"), PUnit("C") };
    Pattern q = { PUnit("A"), PUnit(), PUnit("B"), PUnit() };
    assert(not preceq(t, p));
    assert(not preceq(t, q));
  }
  {
    Text t = { Alphabet("NN", "name") };
    Pattern p = { PUnit("NN", "name") };
    Pattern p2 = { PUnit("NN") };
    Pattern p3 = { PUnit() };
    Pattern q = { PUnit("NN", "name"), PUnit() };
    Pattern r = { PUnit(), PUnit("NN", "name") };
    assert(preceq(t, p));
    assert(preceq(t, p2));
    assert(preceq(t, p3));
    assert(not preceq(t, q));
    assert(not preceq(t, r));
  }

  {
    Text t = { Alphabet("NN", "name"), Alphabet("BE", "is") };
    Pattern p = { PUnit("NN", "name") };
    Pattern q = { PUnit("NN", "name"), PUnit("BE") };
    Pattern r = { PUnit("NN", "name"), PUnit("BE", "are") };
    assert(not preceq(t, p));
    assert(preceq(t, q));
    assert(not preceq(t, r));
  }

  {
    Text t = { Alphabet("A", "a"), Alphabet("B", "b"), Alphabet("C", "c"), Alphabet("D", "d") };
    assert(not preceq(t, { PUnit(), PUnit("C"), PUnit(), PUnit() }));
  }

  clock_t a,b;
  a = clock();
  rep (_, 100)
  {
    Text t = {
      Alphabet("A", "a"), Alphabet("B", "b"),
      Alphabet("A", "a"), Alphabet("B", "b"),
      Alphabet("A", "a"), Alphabet("B", "b")
    };
    assert(preceq(t, { PUnit("A"), PUnit(), PUnit("B") }));
    assert(preceq(t, { PUnit("A"), PUnit("B"), PUnit(), PUnit("B") }));
    assert(preceq(t, { PUnit("A"), PUnit(), PUnit("A"), PUnit("B") }));
    assert(preceq(t, { PUnit(), PUnit("A"), PUnit("B") }));
    assert(preceq(t, { PUnit("A"), PUnit("B"), PUnit() }));
    assert(preceq(t, { PUnit(), PUnit("A"), PUnit("B"), PUnit() }));
    assert(preceq(t, { PUnit("A"), PUnit(), PUnit("B"), PUnit(), PUnit("B") }));
    assert(preceq(t, { PUnit(), PUnit("A"), PUnit("B"), PUnit() }));
    assert(preceq(t, { PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B") }));
    assert(preceq(t, { PUnit(), PUnit("B"), PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B") }));
    assert(preceq(t, { PUnit("A"), PUnit("B"), PUnit("A"), PUnit(), PUnit("A"), PUnit("B") }));
    assert(preceq(t, { PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B"), PUnit("A"), PUnit() }));

    assert(not preceq(t, { PUnit("C"), PUnit("B"), PUnit() }));
    assert(not preceq(t, { PUnit("A"), PUnit("B") }));
    assert(not preceq(t, { PUnit(), PUnit("B"), PUnit("B"), PUnit() }));
    assert(not preceq(t, { PUnit(), PUnit("A"), PUnit("B"), PUnit("B") }));
    assert(not preceq(t, { PUnit("B"), PUnit(), PUnit("A"), PUnit("B") }));
    assert(not preceq(t, { PUnit("A"), PUnit("B"), PUnit(), PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B") }));

  }
  b = clock();
  cerr << "prec(Text, Pattern) : clock=" << (b-a) << endl;

  /*
   * pattern <= pattern
   */
  {
    Pattern top = { PUnit() };
    Pattern q = { PUnit("A"), PUnit("B"), PUnit(), PUnit("B"), PUnit("B", "b") };
    Pattern p = { PUnit("A"), PUnit(), PUnit("B"), PUnit("B") };
    assert(preceq(p, top));
    assert(preceq(q, top));
    assert(preceq(q, p));
    assert(not preceq(p, q));
    assert(preceq(q, q));
    assert(preceq(p, p));
  }
  {
    Pattern p = { PUnit(), PUnit("A", "a"), PUnit(), PUnit() };
    Pattern q = { PUnit(), PUnit("A"), PUnit("B"), PUnit() };
    Pattern r = { PUnit(), PUnit("A", "a"), PUnit() };
    assert(not preceq(p, q));
    assert(not preceq(q, p));
    assert(not preceq(r, p));
    assert(preceq(p, r));
    assert(not preceq(q, r));
    assert(not preceq(r, q));
  }
}
