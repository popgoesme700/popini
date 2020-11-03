#ifndef POPLIBS_POPINI_H_FILE
#	define POPLIBS_POPINI_H_FILE

#	ifdef POPLIBS_POPINI_STATIC
#		define POPLIBS_POPINIAPI static
#	else
#		define POPLIBS_POPINIAPI extern
#	endif

#	ifndef NULL
#		define NULL (void*)0
#	endif

enum poplibs_popinierror{
	poplibs_popinierror_none= 0,
	poplibs_popinierror_inval= -1,
	poplibs_popinierror_missi= -2,
	poplibs_popinierror_nomem= -3,
	poplibs_popinierror_part= -4
};

enum poplibs_popinitype{
	poplibs_popinitype_unknown= 0,
	poplibs_popinitype_section= 1,
	poplibs_popinitype_key= 2,
	poplibs_popinitype_string= 3,
	poplibs_popinitype_primit= 4,
	poplibs_popinitype_table= 5
};

typedef struct poplibs_popini_parser{
	enum poplibs_popinierror err;
	unsigned pos;
	unsigned errPos;
	unsigned nexttok;
} poplibs_popiniparser;

typedef struct poplibs_popinitoken{
	enum poplibs_popinitype type;
	unsigned start;
	unsigned end;
} poplibs_popinitoken_t;

POPLIBS_POPINIAPI void poplibs_popiniparser_init(poplibs_popiniparser *parser);
POPLIBS_POPINIAPI unsigned poplibs_popiniparser_parse(poplibs_popiniparser *parser,const char *str,const unsigned strlen,poplibs_popinitoken_t *tokens,const unsigned tokenlen);

#	ifndef POPLIBS_POPINICOMPILED

POPLIBS_POPINIAPI void poplibs_popiniparser_init(poplibs_popiniparser *parser){
	parser->err= poplibs_popinierror_none;
	parser->errPos= 0;
	parser->nexttok= 0;
	parser->pos= 0;
	return;
}

static poplibs_popinitoken_t *popini_alloctoken(poplibs_popiniparser *parser,poplibs_popinitoken_t *tokens,const unsigned tokenlen){
	poplibs_popinitoken_t *token= NULL;
	if(parser->nexttok<tokenlen){
		token= &tokens[parser->nexttok++];
		token->type= poplibs_popinitype_unknown;
		token->start= 0;
		token->end= 0;
	}
	return token;
}

static void popini_section(poplibs_popiniparser *parser,const char *str,const unsigned strlen,poplibs_popinitoken_t *tokens,const unsigned tokenlen,unsigned *made){
	char chr= '\0';
	unsigned start= parser->pos;
	poplibs_popinitoken_t *token= NULL;
	
	for(;parser->pos<=strlen && (chr= str[parser->pos])!='\0';parser->pos++){
		if(token!=NULL){
			break;
		}
		switch(chr){
			case '\n':
				parser->err= poplibs_popinierror_missi;
				parser->errPos= parser->pos;
				parser->pos= start;
				break;
			case ']':
				if(tokens!=NULL){
					if((token= popini_alloctoken(parser,tokens,tokenlen))!=NULL){
						token->type= poplibs_popinitype_section;
						token->start= start+1;
						token->end= parser->pos-1;
					}else{
						parser->err= poplibs_popinierror_nomem;
						parser->errPos= parser->pos;
						parser->pos= start;
					}
				}
				(*made)++;
				break;
			default:
				break;
		}
	}
	return;
}



POPLIBS_POPINIAPI unsigned poplibs_popiniparser_parse(poplibs_popiniparser *parser,const char *str,const unsigned strlen,poplibs_popinitoken_t *tokens,const unsigned tokenlen){
	char chr= '\0';
	unsigned start= parser->pos;
	unsigned made= 0;
	int nline= 0;
	int commed= 0;
	parser->err= poplibs_popinierror_none;

	for(;parser->pos<=strlen && (chr= str[parser->pos])!='\0';parser->pos++){
		if(parser->err!=poplibs_popinierror_none){
			break;
		}
		switch(chr){
			case ' ':
			case '\t':
			case '\f':
			case '\v':
			case '\r':
				break;
			
			case '#':
			case ';':
				nline= 1;
				commed= 1;
				break;
			
			case '\n':
				nline= 0;
				break;
			
			case '[':
				popini_section(parser,str,strlen,tokens,tokenlen,&made);
				start= parser->pos;
				nline= 1;
				break;
			
			default:
				if(nline && !commed){
					parser->err= poplibs_popinierror_inval;
					parser->errPos= parser->pos;
					parser->pos= start;
				}else if(!nline){

				}
				break;
		}
	}
	return made;
}

#	endif

#endif

/*
MIT License

Copyright (c) 2020 Azrael James Addy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
