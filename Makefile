CC=clang
CFLAGS=-g -no-pie
FUZZFLAGS=-O1 -fsanitize=fuzzer,address
DEPS=fuzzapp.c datastructures.c parser.c Makefile

CORPUSDIR=corpus/
ARTIFACTDIR=artifacts/
SEED=1337

GCOVFLAGS=-fprofile-arcs -ftest-coverage
COVFLAGS=-fprofile-instr-generate -fcoverage-mapping
COVERAGE=coverage/
COVBIN=fuzzapp.cov



clean: 
	rm *.o fuzzapp

cleanfuzz: FORCE
	rm -f corpus/*
	rm -f artifacts/*

fuzzing:
	mkdir -p $(CORPUSDIR) $(ARTIFACTDIR)
	cp example.dob $(CORPUSDIR)
	./libfuzzertarget -detect_leaks=0 -max_len=1024 -artifact_prefix=$(ARTIFACTDIR) -seed=$(SEED) $(CORPUSDIR)

fuzztarget: $(DEPS)
	rm -f *.o libfuzzertarget
	clang $(FUZZFLAGS) $(CFLAGS) -c libfuzzertarget.c 
	clang $(FUZZFLAGS) $(CFLAGS) -c datastructures.c 
	clang $(FUZZFLAGS) $(CFLAGS) -c parser.c 
	clang $(FUZZFLAGS) $(CFLAGS) libfuzzertarget.o datastructures.o parser.o -o libfuzzertarget


coveragetarget:
	rm -f *.o $(COVBIN)
	mkdir -p $(COVERAGE)
	clang $(COVFLAGS) $(CFLAGS) -c libfuzzertarget.c 
	clang $(COVFLAGS) $(CFLAGS) -c datastructures.c 
	clang $(COVFLAGS) $(CFLAGS) -c parser.c 
	clang $(COVFLAGS) $(CFLAGS) -c fuzzapp.c 
	clang $(COVFLAGS) $(CFLAGS) fuzzapp.o datastructures.o parser.o -o $(COVBIN) 


coverage: $(CORPUSDIR)/* FORCE
	for file in $^ ; do echo ./$(COVBIN) readFile $${file} ; done
	llvm-profdata-7 merge -o testcov.profdata default.profraw
	llvm-cov-7 show $(COVBIN) -instr-profile=testcov.profdata

FORCE:

coverage2:
	#mkdir -p $(COVERAGE)
	#cp fuzzapp.cov example.dob coverage/
	#cd coverage; ./fuzzapp.cov
	./fuzzapp.cov 
	lcov --capture --directory . --output-file test_coverage.info
	genhtml test_coverage.info --output-directory out


fuzzapp: $(DEPS)
	rm -f *.o fuzzapp
	clang -O1 -fsanitize=address $(CFLAGS) -c fuzzapp.c datastructures.c parser.c
	clang -O1 -fsanitize=address $(CFLAGS) fuzzapp.o datastructures.o parser.o -o fuzzapp

