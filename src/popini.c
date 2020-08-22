/* Include needed system headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Include needed local headers */
#include "popini.h"


/* Start of signal managers */

/* Static variables that store our signal callback functions, and its length. */
static PopIniSigFunc *funcs= NULL;
static unsigned funccount;

/* Define PopIniSig internal api to raise a signal. */
static char popinisig_raise(const PopIniSig signal,const long unsigned line,const char *lineStr){
	if(funcs!=NULL && funccount>=(unsigned)signal-1 && funcs[(unsigned)signal-1]!=NULL){
		return funcs[(unsigned)signal-1](signal,line,lineStr);
	}
	return -1;
}

/* Define external api to set a callback function to a signal */
void PopIniSig_OnSignal(const PopIniSig signal,const PopIniSigFunc func){
	if(funcs==NULL){
		funccount= ((unsigned)signal-1);
		if((funcs= calloc(sizeof(*funcs),(funccount+1)))==NULL){
			funccount= 0;
			return;
		}
	}else if(funccount<(unsigned)signal-1){
		funccount= ((unsigned)signal-1);
		if((funcs= realloc(funcs,sizeof(*funcs)*(funccount+1)))==NULL){
			funccount= 0;
			return;
		}
	}
	funcs[funccount]= func;
	return;
}

/* End of signal managers */

/* Define macros */
#ifndef POPINI_MACRO_CHUNKSIZE
/* How many characters to allocate memory for each chunk. */
#	define POPINI_MACRO_CHUNKSIZE 32
#endif

/* Reads a line from a file */
static char *readFileLine(FILE *file){
	char *line= NULL;
	char *next;

	size_t lineSize= POPINI_MACRO_CHUNKSIZE;
	if(file!=NULL && !feof(file)){
		if((line= malloc(sizeof(*line)*lineSize))!=NULL){
			*line= '\0';
			next= line;
			size_t lineLen= strlen(line);

			while(fgets(next,lineSize-(lineLen-1),file)!=NULL){
				lineLen= strlen(line);
				if(line[lineLen-1]=='\n'){
					line[lineLen-1]= '\0';
					break;
				}else if(lineLen>=lineSize){
					lineSize+= POPINI_MACRO_CHUNKSIZE;
					printf("Reallocate memory: %lu\n",lineSize);
					if((line= realloc(line,sizeof(*line)*lineSize))==NULL){
						break;
					}
					line[lineLen]= '\0';
				}
				next= &line[lineLen];
			}
		}
	}
	return line;
}

/* Reads a line from a string */
static char *readStringLine(const char *str,size_t *cont){
	char *line= NULL;
	if(str!=NULL && *str!='\0' && *str!='\n'){
		char *tmpDup= NULL;
		size_t strLen= strlen(str);
		if((tmpDup= malloc(sizeof(*tmpDup)*strLen))!=NULL){
			if(strncpy(tmpDup,str,strLen)!=NULL){
				char *ptr= NULL;
				if((ptr= strchr(tmpDup,'\n'))!=NULL){
					*ptr= '\0';
					size_t len= strlen(tmpDup);
					if((line= malloc(sizeof(*line)*len))!=NULL){
						if(strncpy(line,str,len)==NULL){
							free(line);
						}
						*cont= len+1;
					}
				}else{
					size_t len= strlen(str);
					if((line= malloc(sizeof(*line)*len))!=NULL){
						if(strncpy(line,str,len)==NULL){
							free(line);
						}
						*cont= len+1;
					}
				}
			}
		}
	}else if(str!=NULL && *str=='\n'){
		*cont++;
	}
	return line;
}