/********** LineStorage module---implementation **********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kwic.h"
#include "LineStorage.h"

/***** local constants *****/

/***** local types *****/

/***** local variables *****/

#define MAXWORD 40
#define MAXLINE 20


long unsigned int lineSize = 0;
long unsigned int lineCount = 0;

struct Line{
	long unsigned int wordSize;
	long unsigned int wordCount;
	char** word;
}* line;


#ifndef NULL
#define NULL 0
#endif


/***** state invariant *****

/***** local functions *****/

/*
* if headLinePtr contains at least i+1 LineNodes then
*	return the address of the ith LineNode
* else
*	return NULL
* Assumed: the state invariant holds
*/

/*
static char* getLine(int i)
{
	
	if (i < 0 || i > linesSize)
		return NULL;
	return lines[i];
}
*/
/*
* if wordNodePtr != NULL && the list headed by wordNodePtr has >= i words then
*	return a pointer to the ith word in the list headed by wordNodePtr
* else
*	return NULL
* Assumed: wordNodePtr is either NULL or a pointer to a list of WordNodes
*/
/*
static char* getWord(int lineIndex,int wordIndex)
{
	if (lineIndex < 0 || lineIndex > MAXLINE || wordIndex < 0 || wordIndex > MAXLINE)
		return NULL;
	return lines[lineIndex][wordIndex];
}
*/
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
	//printf("Befor lineCoutn = %i lineSize = %i\n", lineCount, lineSize);
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
		//memset(((void*)(*line) + sizeof(*line)*(lineSize)), '\0', sizeof(*line)*(lineSize));
		lineSize *= 2;
	} 

	/* create and fill a LineNode */
	line[lineCount].wordCount = 0;
	line[lineCount].wordSize = 0;
	line[lineCount].word = NULL;
	lineCount++;
        //printf("AFTER lineCount = %i\n linesSize = %i\n", lineCount, linesSize);
        //printf("AFTER lineCoutn = %i lineSize = %i\n", lineCount, lineSize);

	return KWSUCCESS;
}

KWStatus LSAddWord(char* word)
{
	if(!line||line[lineCount-1].wordCount > MAXWORD){
	//	printf("range error word = %i\n", line[lineCount-1].wordCount);
		return KWRANGEERROR;
	//	return KWMEMORYERROR;
	}

	if(line[lineCount-1].word == NULL){
	//	printf("gothere\n");
		line[lineCount-1].word = malloc(sizeof(char**)*5);
	//	printf("please\n");
		if(!line){
	//		printf("mem1\n");
			return KWMEMORYERROR;
		}
		line[lineCount-1].wordSize = 5;
	} else if(line[lineCount-1].wordCount >= line[lineCount-1].wordSize){
		if(!(line[lineCount-1].word = realloc(line[lineCount-1].word, sizeof(char**) * line[lineCount-1].wordSize * 2 ))){
	//		printf("mem2\n");
			return KWMEMORYERROR;
		}
		line[lineCount-1].wordSize *= 2;
	}
	

	int charCount;

	if(!(line[lineCount-1].word[line[lineCount-1].wordCount] = malloc(sizeof(char*)*5))){
	//	printf("mem3\n");
		return KWMEMORYERROR;
	}
	int stringSize = 5;

	for(charCount =0 ; word[charCount] != 0 ; charCount ++){
		if (charCount > stringSize){
			if(!(line[lineCount-1].word[line[lineCount-1].wordCount] = realloc(line[lineCount-1].word[line[lineCount-1].wordCount], sizeof(char*) * stringSize *2))){
	//			printf("mem4\n");
				return KWMEMORYERROR;
			}
			stringSize *=2;
		}
			
		line[lineCount-1].word[line[lineCount-1].wordCount][charCount] = word[charCount];
	}
	if(charCount > stringSize)
		if(!(line[lineCount-1].word[line[lineCount-1].wordCount] = realloc(line[lineCount-1].word[line[lineCount-1].wordCount], sizeof(char*) * (stringSize +1)))){
	//		printf("mem5\n");
			return KWMEMORYERROR;
		}
	line[lineCount-1].word[line[lineCount-1].wordCount][charCount] = '\0';

	//strcpy(lines[lineCount][wordCount] ,word);
	

	/* link in the new WordNode */
	line[lineCount-1].wordCount++;

	return KWSUCCESS;
}

const char* LSGetWord(int lineNum,int wordNum)
{

	if (lineNum >= lineCount || lineNum < 0 || wordNum < 0 || wordNum >= line[lineNum].wordCount)
		return NULL;
	return line[lineNum].word[wordNum];
}

int LSNumWords(int lineNum)
{
	/* find line lineNum */
	/* count the words in line lineNum */

	if(lineNum < 0 || lineNum >= lineCount){
	//	printf("ragng\n");
		return KWRANGEERROR;
	}
	
	return line[lineNum].wordCount;


}

int LSNumLines(void)
{
	return lineCount;
}

void LSPrintState(void)
{return;
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
