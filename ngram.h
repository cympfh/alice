#pragma once

#include "text.h"

// collection of (bos?, eos?, inner-gram)
using Gram = tuple<bool, bool, vector<Alphabet>>;
vector<pair<int, Gram>> ngram(const vector<int>&ns, const vector<Text>&doc);

