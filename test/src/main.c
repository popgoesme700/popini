/* Include needed system headers */
#include <stdio.h>
#include <popini.h>

/* Test PopIniSig signals */
static char popinisighandle(const PopIniSig signal,const long unsigned line,const char *str){
	printf("Line (%u) (%s)\n",line,str);
	return 0;
}

/* Define main executable function. */
int main(const int argc,const char *argv[]){
	PopIniSig_OnSignal(PopIniSig_ParseError,popinisighandle);
	return 0;
}