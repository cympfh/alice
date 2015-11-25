CXXFLAGS=-O3 -std=c++11

SEED=../PubMed/alice/seeds/a
mytest: alice
	cat ~/Corpus/abstracts/train-sentences/CONCLUSION.post | head -n 2000 | ./alice -D --seed $(SEED) -P 30 -B 20 --book-only --freq -L test.log -I 300 >test.out
	cat ./mytest.base
	cat ./test.out | sort -nr -k1,1 | sed 's/^[0-9]* //g' | ../PubMed/eval-alice/eval.exe ~/Corpus/abstracts/test-post.txt | tee -a test.log
	cp test.log log/$$(date "+%m%d.%H%M.%S.log")

OBJS=read.o ges.o pattern.o text.o setcover.o minl.o ngram.o
alice: main.cc $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

minl.o: minl.cc util.h
setcover.o: setcover.cc
read.o: pattern.h text.h read.cc
ges.o: pattern.o text.o ges.cc
pattern.o: pattern.cc
text.o: text.cc
ngram.o: ngram.cc text.o

profile:
	g++ -std=c++11 -pg main.cc $(OBJS)
	head -n 100 ~/Corpus/abstracts/train-sentences/CONCLUSION.post | ./a.out -D >/dev/null
	gprof a.out gmon.out | less

.PHONY: test clean

test: test.cc $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	./test

clean:
	rm *.out *.log alice test
