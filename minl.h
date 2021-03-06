
namespace minl {
  Text minl(const vector<Text*>&S);
}

int word_intersection(const vector<Text*>&);

namespace mmg {
  vector< tuple<Pattern, vector<Text*>, vector<int>> >
  kdivision(
      int k,
      const Pattern&p,
      const vector<Text*>&S,
      const vector<int>&s,
      bool DEBUG=false);

  vector<tuple<Pattern, vector<Text*>, vector<int>>>
  kmmg(
      int k,
      vector<Text*>&C,
      bool DEBUG=false);

  vector<tuple<Pattern, vector<Text*>, vector<int>>>
  kmmg(
      int k,
      vector<Text*>&C,
      const Pattern&p,
      bool DEBUG=false);

  Pattern tighten(const Pattern&p, const vector<Text*>&C);
}
