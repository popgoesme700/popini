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
	/* Test temporarily sending signal */
	popinisig_raise(signal,2,"You");
	return;
}

/* End of signal managers */