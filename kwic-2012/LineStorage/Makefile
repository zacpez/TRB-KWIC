INC = ../include

XCFLAGS = -I$(INC) ## extra C flags

CC = gcc $(XCFLAGS)

CFLAGS = -g -p

driver: driver.o LineStorage.o
	$(CC) -o driver driver.o LineStorage.o

memtest: memtest.o LineStorage.o
	$(CC) -o memtest memtest.o LineStorage.o

runtest: driver
	./driver

gcov: driver.o LineStorage.c
	$(CC) -fprofile-arcs -ftest-coverage -c LineStorage.c
	$(CC) -fprofile-arcs -ftest-coverage -o driver driver.o LineStorage.o
	./driver
	gcov LineStorage.c > gcov_summary.txt
	cat gcov_summary.txt
	rm -f *.o # gcov version is dangerous to leave around

export: LineStorage.o

driver.o: $(INC)/kwic.h $(INC)/LineStorage.h driver.c
	$(CC) -c $(CFLAGS) driver.c

LineStorage.o: $(INC)/kwic.h $(INC)/LineStorage.h LineStorage.c
	$(CC) -c $(CFLAGS) LineStorage.c

memtest.o: $(INC)/kwic.h $(INC)/LineStorage.h memtest.c
	$(CC) -c $(CFLAGS) memtest.c

clean:
	rm -f *.o act/* driver *.gcov *.gcda *.gcno memtest gcov_summary.txt
