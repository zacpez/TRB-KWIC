/********** LineStorage module---test driver **********/

#include <stdio.h>
#include "kwic.h"
#include "LineStorage.h"

#define NUMLINES 18
#define MAXWORDS 20

struct {
	int numWords;
	char* wordList[MAXWORDS];
} lineList[NUMLINES] = {
	{20, "1","2","3","4","5","6","7","8","9","1","1","2","3","4","5","6","7","8","9","2"},
	{1,"1234567891123456789212345678931234567894"},
	{5,"The","Cat","in","the","Hat"},
	{4,"The","C","Programming","Language"},
	{0},
	{1,"xxx"},
	{3,"","x","01234567890123456789012345678901234567890123456789"},
	{7,"There","is","no","future","in","time","travel"},
	{6,"I","bought","a","portable","cable","TV"},
	{7,"Entropy","isn't","what","it","used","to","be"},
	{7,"You","can","observe","a","lot","by","watching"},
	{7,"Learning","is","the","evolutions","of","the","mind"},
	{8,"Windows","is","a","colorful","clown","suit","for","DOS"},
	{7,"I'd","kill","for","a","Nobel","Peace","Prize"},
	{5,"Hindsight","is","an","exact","science"},
	{6,"One","planet","is","all","you","get"},
	{6,"Two","wrongs","are","only","the","beginning"},
	{5,"Plan","to","be","spontaneous","tomorrow"}
};

static int errorCount = 0;

static void runTest()
{
	int i,j,status;
	int rangeErr[4];
	const char* word;

	printf("check that LSAddWord properly signals KWRANGEERROR\n");
	status = LSAddWord("");
	if (status != KWRANGEERROR) {
		printf("Error in LSAddWord return value.");
		printf(" Actual: %d Expected: %d\n",status,KWRANGEERROR);
		errorCount++;
	}

	printf("load lineList into LineStorage\n");
	for (i = 0; i < NUMLINES; i++) {
		printf("\tline %d\n",i);
		status = LSAddLine();
		if (status != KWSUCCESS) {
			printf("Error in LSAddLine return value.");
			printf(" Actual: %d Expected: %d\n",status,KWSUCCESS);
			errorCount++;
		}
		for (j = 0; j < lineList[i].numWords; j++) {
			printf("\t\tword %d\n",j);
			status = LSAddWord(lineList[i].wordList[j]);
			if (status != KWSUCCESS) {
				printf("Error in LSAddWord return value.");
				printf(" Actual: %d Expected: %d\n",
					status,KWSUCCESS);
				errorCount++;
			}
		}
	}

	printf("check that the lines have been properly stored\n");
	if (LSNumLines() != NUMLINES) {
		printf("Error in LSNumLines. Actual: %d Expected: %d\n",
			LSNumLines(),NUMLINES);
		errorCount++;
	}
	for (i = 0; i < NUMLINES; i++) {
		printf("\tline %d\n",i);
		if (LSNumWords(i) != lineList[i].numWords) {
			printf("Error in LSNumWord. Actual: %d Expected: %d\n",
				LSNumWords(i),lineList[i].numWords);
			errorCount++;
		}
		for (j = 0; j < lineList[i].numWords; j++) {
			printf("\t\tword %d\n",j);
			if (strcmp(LSGetWord(i,j),lineList[i].wordList[j])) {
				printf("Error in LSGetWords(%d,%d).",i,j);
				printf(" Actual: !%s! Expected: !%s!\n",
					LSGetWord(i,j),lineList[i].wordList[j]);
				errorCount++;
			}
		}
	}

	printf("check that line number range errors are properly signaled\n");
	rangeErr[0] = -1000;
	rangeErr[1] = -1;
	rangeErr[2] = NUMLINES;
	rangeErr[3] = NUMLINES+1000;
	for (i = 0; i < 4; i++) {
		printf("\tline %d\n",rangeErr[i]);
		status = LSNumWords(rangeErr[i]);
		if (status != KWRANGEERROR) {
			printf("Error in LSNumWords return value.");
			printf(" Actual: %d Expected: %d\n",
				status,KWRANGEERROR);
			errorCount++;
		}
		word = LSGetWord(rangeErr[i],0);
		if (word != 0) {
			printf("Error in LSGetWord return value.");
			printf(" Actual: !%s! Expected: %d\n",word,0);
			errorCount++;
		}
	}

	printf("check that word number range errors are properly signaled\n");
	rangeErr[0] = -1000;
	rangeErr[1] = -1;
	rangeErr[2] = lineList[0].numWords;
	rangeErr[3] = lineList[0].numWords+1000;
	for (i = 0; i < 4; i++) {
		printf("\tword %d\n",rangeErr[i]);
		word = LSGetWord(0,rangeErr[i]);
		if (word != 0) {
			printf("Error in LSGetWord return value.");
			printf(" Actual: !%s! Expected: %d\n",word,0);
			errorCount++;
		}
	}
}

int main()
{
	LSInit();

	printf("run the tests once\n");
	runTest();

	printf("check that LSReset runs without crashing\n");
	LSReset();

	printf("run the tests again\n");
	runTest();
	LSPrintState();

	if (errorCount == 0)
		printf("\n\nNo errors detected!\n");
	return errorCount;
}
