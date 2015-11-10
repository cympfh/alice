CXXFLAGS=-O3 -std=c++11

mytest: alice
	./alice -D ~/Corpus/abstracts/train-sentences/CONCLUSION.pos -L test.log >/dev/null
	cp test.log log/$$(date "+%m%d.%H%M.%S.log")

alice: main.cc read.o ges.o pattern.o text.o setcover.o minl.o
	$(CXX) $(CXXFLAGS) -o $@ $^

minl.o: minl.cc util.h
setcover.o: setcover.cc
read.o: pattern.h text.h read.cc
ges.o: pattern.o text.o ges.cc
pattern.o: pattern.cc
text.o: text.cc

profile:
	g++ -std=c++11 -pg read.o ges.o pattern.o text.o setcover.o minl.o main.cc
	head -n 200 ~/Corpus/abstracts/train-sentences/CONCLUSION.pos | ./a.out -D >/dev/null
	gprof a.out gmon.out | less

test: test.cc read.o ges.o pattern.o text.o setcover.o minl.o
	$(CXX) $(CXXFLAGS) -o $@ $^
	./test


.PHONY: clean

clean:
	rm *.out *.log alice test
