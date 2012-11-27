/********** LineStorage module---implementation **********/

#include <stdio.h>
#include <stdlib.h>
#include "kwic.h"
#include "LineStorage.h"

/***** local constants *****/

#ifndef NULL
#define NULL 0
#endif

/***** local types *****/

/***** local variables *****/


// Currently allocated lines
static long unsigned int lineSize = 0;

// Number of lines;
static long unsigned int lineCount = 0;

struct Line{
	// Number of words line can hold, used for dynamic allocation of word, uses char to save memory
	unsigned char wordSize;
	// Number of words in Line, uses char to save memory
	unsigned char wordCount;

	// points to dynamic array of words
	char** word;
} static * line;



/***** state invariant *****

1. if lineCount == 0 then
        line == NULL
        lineSize == 0
   else
        line points to a dynamicly allocated array of Line structures.
        There are lineCount Lines in this line.
	lineCount <= lineSize

2. for every Line allocated by LineStorage
        if wordCount == 0 then
            wordSize == 0
            word == NULL
        else
            word points to a dynamicly allocated array of pointers that points to a null-terminated dynamic char array.
            There are wordCount words in this list.
            wordCount <= wordSize

3. All of the dynamic memory allocated by LineStorage (and not yet freed)
   is in the List structure pointed to by line.
*/

/***** local functions *****/

/***** exported functions *****/

void LSInit(void)
{
	line = NULL;
	lineSize = 0;
	lineCount = 0;
}


void LSReset(void)
{
	if(line){
		int rmline;
		int rmword;
		for(rmline = 0; rmline < lineCount; rmline++){
			for(rmword = 0; rmword < line[rmline].wordCount; rmword++){
				free(line[rmline].word[rmword]);
			}
			free(line[rmline].word);	
		}
		free(line);
	}
	line = NULL;
	lineSize = 0;
	lineCount = 0;
}



KWStatus LSAddLine(void)
{
	if(!line){
		if(!(line = malloc(sizeof(*line))))
			return KWMEMORYERROR;

		lineSize = 1;
	} else if (lineCount == lineSize){
		line = realloc(line, sizeof(*line)*lineSize*2);
		if(!line)
			return KWMEMORYERROR;

		lineSize *= 2;
	}

	// fill a Line Struct
	line[lineCount].wordCount = 0;
	line[lineCount].wordSize = 0;
	line[lineCount].word = NULL;
	lineCount++;

	return KWSUCCESS;
}

KWStatus LSAddWord(char* word)
{

	// if line has not yet been allocated, return KWRANGERROR
	if(!line)
		return KWRANGEERROR;


	// if first word on line, allocate space for 5 words.
	if(line[lineCount-1].word == NULL){
		line[lineCount-1].word = malloc(sizeof(char**)*5);
		if(!line)
			return KWMEMORYERROR;
		line[lineCount-1].wordSize = 5;

	// if current allocated space is full, reallocate 2 times current size
	} else if(line[lineCount-1].wordCount == line[lineCount-1].wordSize){
		if(!(line[lineCount-1].word = realloc(line[lineCount-1].word, sizeof(char**) * line[lineCount-1].wordSize * 2 )))
			return KWMEMORYERROR;

		line[lineCount-1].wordSize *= 2;
	}
	
	// allocate space for 5 characters for the current word
	if(!(line[lineCount-1].word[line[lineCount-1].wordCount] = malloc(sizeof(char*)*5)))
		return KWMEMORYERROR;
	// allocated size for characters
	int stringSize = 5;

	int charCount;
	for(charCount = 0; word[charCount] != 0; charCount++){

		// if out of space for string, reallocate 2 times current size
		if (charCount == stringSize){
			if(!(line[lineCount-1].word[line[lineCount-1].wordCount] = realloc(line[lineCount-1].word[line[lineCount-1].wordCount], sizeof(char*) * stringSize *2)))
				return KWMEMORYERROR;

			stringSize *=2;
		}
		// copy char to string from given word
		line[lineCount-1].word[line[lineCount-1].wordCount][charCount] = word[charCount];
	}

	// if no room for null char, allocate 1 extra char
	if(charCount == stringSize)
		if(!(line[lineCount-1].word[line[lineCount-1].wordCount] = realloc(line[lineCount-1].word[line[lineCount-1].wordCount], sizeof(char*) * (stringSize +1))))
			return KWMEMORYERROR;
	// add null termanint
	line[lineCount-1].word[line[lineCount-1].wordCount][charCount] = '\0';

	line[lineCount-1].wordCount++;

	return KWSUCCESS;
}

const char* LSGetWord(int lineNum,int wordNum)
{
	// bound check
	if (lineNum >= lineCount || lineNum < 0 || wordNum < 0 || wordNum >= line[lineNum].wordCount)
		return NULL;
	return line[lineNum].word[wordNum];
}

int LSNumWords(int lineNum)
{
	// bound check
	if(lineNum < 0 || lineNum >= lineCount)
		return KWRANGEERROR;
	
	return line[lineNum].wordCount;


}

int LSNumLines(void)
{
	return lineCount;
}


void LSPrintState(void)
{
	printf("lineCount:%d\n",lineCount);
	int i;
	int n;
        int wordCount;
	for (i = 0; i < lineCount; i++){
		wordCount = line[i].wordCount;
		printf("\twordCount:%d\n\t", wordCount);
		for (n = 0; n < wordCount; n++) {
			printf("!%s", line[i].word[n]);
		}
		printf("!\n");
	}
}
