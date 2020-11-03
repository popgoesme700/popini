#define POPLIBS_POPINI_STRICT
#include <popini.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char *substr(const char *str,const unsigned start,const unsigned end){
	unsigned i;
	char cChr= '\0';
	char *rtn= NULL;
	if((rtn= malloc(sizeof(*rtn)*((end-start)+2)))!=NULL){
		for(i=start;i<=end && (cChr= str[i])!='\0';i++){
			rtn[i-start]= cChr;
		}
		rtn[(end-start)+1]= '\0';
	}
	return rtn;
}

int main(int argc,const char *argv[]){
	const char ini[]= "[Test]\nKey=Value\nKey= Value\nKey =Value\nKey = Value\n Key= Value Value\nKey= Value ;Value\nKey= \"String Test ;Wow\"\nKey= 'String Test ;Wow'\nKey= 'String Test ;Wow' ;Wow\n[Test 2]\n#Key = Test me!\n# Key = Test me!\n;Key = Test me!\n; Key = Test me!";

	poplibs_popiniparser parser;
	poplibs_popinitoken_t tokens[64];

	unsigned madeTokens= 0;

	poplibs_popiniparser_init(&parser);
	madeTokens= poplibs_popiniparser_parse(&parser,ini,strlen(ini),tokens,64);
	return 0;
}
