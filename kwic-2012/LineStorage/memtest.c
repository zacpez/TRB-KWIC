/********** LineStorage module---test Memory leaks on Reset **********/


/*******************************
        Used to test Reset function for Memory leaks

        Inserts lineList into LineStorage then resets LineStorage and loop untill error

	check for mem Increase with 'watch "ps aux|grep memtest"'
*******************************/


#include <stdio.h>
#include "kwic.h"
#include "LineStorage.h"

#define NUMLINES 16
#define MAXWORDS 8

struct {
	int numWords;
	char* wordList[MAXWORDS];
} lineList[NUMLINES] = {
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

static int error = 0;

static void runTest()
{
	int i,j,status;
	int rangeErr[4];
	const char* word;
	printf("load lineList into LineStorage\n");
	for (i = 0; i < NUMLINES; i++) {
		printf("\tline %d\n",i);
		status = LSAddLine();
		if (status != KWSUCCESS) {
			printf("Error in LSAddLine return value.");
			printf(" Actual: %d Expected: %d\n",status,KWSUCCESS);
			error = 1;
		}
		for (j = 0; j < lineList[i].numWords; j++) {
			printf("\t\tword %d\n",j);
			status = LSAddWord(lineList[i].wordList[j]);
			if (status != KWSUCCESS) {
				printf("Error in LSAddWord return value.");
				printf(" Actual: %d Expected: %d\n",
					status,KWSUCCESS);
				error = 1;
			}
		}
	}

}

int main()
{
	LSInit();

	runTest();

	while(!error){
		printf("check that LSReset runs without crashing\n");
		LSReset();

		printf("run the tests again\n");
		runTest();
	}
	return error;
}
