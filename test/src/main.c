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
	const char ini[]= "[Test]\n\tKey = True\n";

	poplibs_popiniparser parser;
	poplibs_popinitoken_t tokens[64];

	unsigned madeTokens= 0;

	poplibs_popiniparser_init(&parser);
	madeTokens= poplibs_popiniparser_parse(&parser,ini,strlen(ini),tokens,64);
	printf("Made %u tokens, Errorcode %d\n",madeTokens,parser.err);
	if(parser.err==0){
		unsigned i;
		for(i=0;i<madeTokens;i++){
			poplibs_popinitoken_t *token= &tokens[i];
			char *sub;
			printf("Token of type: %d\n",token->type);
			if(token->type==poplibs_popinitype_table){
				printf("-> Tokens upto %u are values of this table\n",token->end);
			}else{
				sub= substr(ini,token->start,token->end);
				printf("-> String (%s)\n",sub);
				free(sub);
			}
		}
	}
	return 0;
}
