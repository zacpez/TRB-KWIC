/********** Output module---implementation **********/

#include <stdio.h>
#include "kwic.h"
#include "Output.h"
#include "ShiftSort.h"


#define MAXLINE 40
/***** local constants *****/

/***** local types *****/

/***** exported functions *****/

void OUInit(void)
{
	/* intentionally empty */
}

void OUPrint(void)
{
	int curLine;
	int curWord;
	int numLines = SSNumLines();
	int numChars = 0;
	const char* getWord;
	int numCharsGetWord = 0;
	for (curLine = 0; curLine < numLines; curLine++) {
		int numWords = SSNumWords(curLine);
                int shiftNum = SSGetShiftNum(curLine);
		numChars = 0;
		// get num of chars after rotate
		for (curWord = numWords-shiftNum; curWord < numWords; curWord++) {
			getWord = SSGetWord(curLine, curWord);
			numCharsGetWord = 0;
			while(getWord[numCharsGetWord++] != '\0')
				numChars++;
			// +1 for space after word
			numChars++;
		}
		// add spaces
		while(41 - numChars++ > 0)
			printf(" ");
		for (curWord = numWords-shiftNum; curWord < numWords; curWord++)
			printf("%s ", SSGetWord(curLine, curWord));
		printf("%s", SSGetWord(curLine, 0));
		for(curWord = 1; curWord < numWords-shiftNum; curWord++)
			printf(" %s", SSGetWord(curLine, curWord));
		printf("\n");
	}
	
}
