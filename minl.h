vector< tuple<Pattern, vector<Text*>, vector<int>> >
kdivision(
    int k,
    const Pattern&p,
    const vector<Text*>&S,
    const vector<int>&s,
    bool DEBUG=false);
vector<tuple<Pattern, vector<Text*>, vector<int>>>
kmmg(int, vector<Text*>&, bool DEBUG=false);
vector<tuple<Pattern, vector<Text*>, vector<int>>>
kmmg(int, vector<Text*>&, const Pattern&, bool DEBUG=false);
