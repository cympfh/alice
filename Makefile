CXXFLAGS=-O3 -std=c++11

mytest: alice
	cat ~/Corpus/abstracts/train-sentences/CONCLUSION.pos | head -n 3000 | ./alice -D -N 10:200 -P 50 -B 10 --book-only --freq -L test.log -I 300 >test.out
	cp test.log log/$$(date "+%m%d.%H%M.%S.log")

alice: main.cc read.o ges.o pattern.o text.o setcover.o minl.o ngram.o
	$(CXX) $(CXXFLAGS) -o $@ $^

minl.o: minl.cc util.h
setcover.o: setcover.cc
read.o: pattern.h text.h read.cc
ges.o: pattern.o text.o ges.cc
pattern.o: pattern.cc
text.o: text.cc
ngram.o: ngram.cc text.o

profile:
	g++ -std=c++11 -pg read.o ges.o pattern.o text.o setcover.o minl.o main.cc
	head -n 200 ~/Corpus/abstracts/train-sentences/CONCLUSION.pos | ./a.out -D >/dev/null
	gprof a.out gmon.out | less

.PHONY: test clean

test: test.cc read.o ges.o pattern.o text.o setcover.o minl.o ngram.o
	$(CXX) $(CXXFLAGS) -o $@ $^
	./test

clean:
	rm *.out *.log alice test
