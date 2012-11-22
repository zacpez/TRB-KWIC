/********** LineStorage module---implementation **********/

//#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include "kwic.h"
#include "LineStorage.h"

/***** local constants *****/

/***** local types *****/

/***** local variables *****/

#define MAXWORD 40
#define MAXLINE 20

#ifndef NULL
#define NULL 0
#endif



// Currently allocated lines
long unsigned int lineSize = 0;

// Number of lines;
long unsigned int lineCount = 0;

struct Line{
	// Number of words word can hold, used for dynamic allocation of word, uses char to save memory
	unsigned char wordSize;
	// Number of words in Line, uses char to save memory
	unsigned char wordCount;

	// points to dynamic array of words
	char** word;
}* line;



/***** state invariant *****

/***** local functions *****/

/***** exported functions *****/

void LSInit(void)
{
	line = NULL;
	lineSize = 0;
	lineCount = 0;
}


// TODO: Test for mem leaks
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
	if(line == NULL){
		if(!(line = malloc(sizeof(*line)))){
	//		printf("mem5\n");
			return KWMEMORYERROR;
		}
		lineSize = 1;
	} else if (lineCount >= lineSize){
		line = realloc(line, sizeof(*line)*lineSize*2);
		if(!line){
	//		printf("Out of mem\n");
			return KWMEMORYERROR;
		}
		lineSize *= 2;
	} 

	/* create and fill a LineNode */
	line[lineCount].wordCount = 0;
	line[lineCount].wordSize = 0;
	line[lineCount].word = NULL;
	lineCount++;

	return KWSUCCESS;
}

KWStatus LSAddWord(char* word)
{

	// if line has not yet been allocated or wordCount is full for the current line return KWRANGERROR
	if(!line||line[lineCount-1].wordCount > MAXLINE){
	//	printf("range error word = %i\n", line[lineCount-1].wordCount);
		return KWRANGEERROR;
	}

	// if first word on line, allocate space for 5 words.
	if(line[lineCount-1].word == NULL){
		line[lineCount-1].word = malloc(sizeof(char**)*5);
		if(!line){
	//		printf("mem1\n");
			return KWMEMORYERROR;
		}
		line[lineCount-1].wordSize = 5;

	// if current allocated space is full, reallocate 2 times current size
	} else if(line[lineCount-1].wordCount >= line[lineCount-1].wordSize){
		if(!(line[lineCount-1].word = realloc(line[lineCount-1].word, sizeof(char**) * line[lineCount-1].wordSize * 2 ))){
	//		printf("mem2\n");
			return KWMEMORYERROR;
		}
		line[lineCount-1].wordSize *= 2;
	}
	
	// allocate space for 5 characters for the current word
	if(!(line[lineCount-1].word[line[lineCount-1].wordCount] = malloc(sizeof(char*)*5))){
	//	printf("mem3\n");
		return KWMEMORYERROR;
	}
	// allocated size for characters
	int stringSize = 5;

	int charCount;
	for(charCount = 0; word[charCount] != 0 ; charCount++){

		// if out of space for string, reallocate 2 times current size
		if (charCount > stringSize){
			if(!(line[lineCount-1].word[line[lineCount-1].wordCount] = realloc(line[lineCount-1].word[line[lineCount-1].wordCount], sizeof(char*) * stringSize *2))){
	//			printf("mem4\n");
				return KWMEMORYERROR;
			}
			stringSize *=2;
		}
		// copy char to string from given word
		line[lineCount-1].word[line[lineCount-1].wordCount][charCount] = word[charCount];
	}

	// if no room for null char, allocate 1 extra char
	if(charCount > stringSize)
		if(!(line[lineCount-1].word[line[lineCount-1].wordCount] = realloc(line[lineCount-1].word[line[lineCount-1].wordCount], sizeof(char*) * (stringSize +1)))){
	//		printf("mem5\n");
			return KWMEMORYERROR;
		}
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
	if(lineNum < 0 || lineNum >= lineCount){
	//	printf("range\n");
		return KWRANGEERROR;
	}
	
	return line[lineNum].wordCount;


}

int LSNumLines(void)
{
	return lineCount;
}


// TODO: program useless fuction
void LSPrintState(void)
{
return;
}
/*
	LineNodePtr tmpLinePtr;
	WordNodePtr tmpWordPtr;

	printf("lineCount:%d\n",lineCount);
	for (tmpLinePtr = headLinePtr;
			tmpLinePtr != NULL;
			tmpLinePtr = tmpLinePtr->nextLinePtr) {
		printf("\twordCount:%d\n\t",tmpLinePtr->wordCount);
		for (tmpWordPtr = tmpLinePtr->headWordPtr;
				tmpWordPtr != NULL;
				tmpWordPtr = tmpWordPtr->nextWordPtr) {
			printf("!%s",tmpWordPtr->word);
		}
		printf("!\n");
	}
}*/
