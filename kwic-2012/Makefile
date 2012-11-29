# master Makefile for KWIC program

# Make targets:
#
#   make kwic		-- creates kwic program
#   make runtest	-- runs tests on each module and on kwic
#   make clean		-- removes all .o files
#   make gdb		-- invokes the gdb debugger on kwic

# For the kwic target, different versions make be created by supplying
# different settings for the CFLAGS parameter.  Possible versions are:
#
# 1. For a debugging version (usable with the gdb debuggers)
#    while developing the code (this is the default):
#	make 'CFLAGS=-g' kwic
#
# 2. For a version of kwic that generates execution profiles:
#	make 'CFLAGS=-pg' kwic
#
# 3. For an (optimized) production version of the program:
#	make 'CFLAGS=-O' kwic
#
# NOTE:	If you want to switch from one version of kwic to another (e.g.
#	after having produced the debugging version, you now want a
#	profiling version), you must execute
#		make clean
#	first.

# List of directories holding code and tests for each module
MODULES = Input LineStorage ShiftSort Output WordTable

# Default compilation flags for IBM xlc compiler
CFLAGS = -p -g

# Directory holding all the .h files
INC = include

XCFLAGS = -I$(INC)  ## extra C flags

CC = gcc $(XCFLAGS)

IMPINC = $(INC)/Input.h $(INC)/LineStorage.h \
	$(INC)/ShiftSort.h $(INC)/Output.h \
	$(INC)/WordTable.h

IMPOBJ = Input/Input.o LineStorage/LineStorage.o \
	ShiftSort/ShiftSort.o Output/Output.o \
	WordTable/WordTable.o

kwic: main.o impMod
	$(CC) -o ./kwic $(CFLAGS) main.o $(IMPOBJ)

impMod:
	for m in $(MODULES); do \
		( cd $$m; echo $$m; \
		make 'CFLAGS=$(CFLAGS)' $(MFLAGS) export ) \
	done

gdb:	kwic
	gdb -d ShiftSort -d WordTable -d Input -d Output \
		-d LineStorage kwic
prof: kwic
# test each module first
	for m in $(MODULES); do \
		( cd $$m; $(MAKE) 'CFLAGS=$(CFLAGS)' runtest; ) \
	done
#
# use a large input file
	./kwic -n noiseWords < input/test4 > actualOutput/test4
	gprof kwic > ProfileReport

runtest: kwic
# test each module first
	rm -f gcov_summary.txt
	for m in $(MODULES); do \
		( cd $$m; $(MAKE) 'CFLAGS=$(CFLAGS)' gcov; cat gcov_summary.txt >> ../gcov_summary.txt) \
	done
#
# now test the whole kwic program
	rm -r -f actualOutput/
	mkdir actualOutput
#
# ***** use different lines files
# input/test1 from stdin
	./kwic -n noiseWords < input/test1 > actualOutput/test1
	-diff actualOutput/test1 expectedOutput/test1
	gprof kwic > ProfileReportTest1

# input/test2 as command line argument
	./kwic -n noiseWords input/test2 > actualOutput/test2
	-diff actualOutput/test2 expectedOutput/test2
	gprof kwic > ProfileReportTest2

# input/test3 as command line arguments
	./kwic -n noiseWords input/test3 > actualOutput/test3
	-diff actualOutput/test3 expectedOutput/test3
	gprof kwic > ProfileReportTest3
#
# ***** use different noise words files
# -n argument
	./kwic -n noiseWordsThe input/test0 > actualOutput/noiseWords1
	-diff actualOutput/noiseWords1 expectedOutput/noiseWords1
#
# environment variable
# Note: parentheses needed to retain environment variable definition
	( \
	KWICNOISEWORDS=noiseWordsIn; \
	export KWICNOISEWORDS; \
	./kwic input/test0 > actualOutput/noiseWords2; \
	)
	-diff actualOutput/noiseWords2 expectedOutput/noiseWords2

main.o: $(INC)/kwic.h $(IMPINC)
	$(CC) -c $(CFLAGS) main.c

clean:
	for m in $(MODULES); do \
		( cd $$m; $(MAKE) clean; ) \
	done
	rm -f kwic core mon.out gmon.out *.o actualOutput/* ProfileReport gcov_summary.txt
