#ifndef POPLIBS_POPINI_H_FILE
#	define POPLIBS_POPINI_H_FILE

#	ifdef POPLIBS_POPINI_STATIC
#		define POPLIBS_POPINIAPI static
#	else
#		define POPLIBS_POPINIAPI extern
#	endif

#	ifdef NULL
#		define POPLIBS_POPININULL NULL
#	else
#		define POPLIBS_POPININULL (void*)0
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
	poplibs_popinitype_primi= 4,
	poplibs_popinitype_table= 5
};

typedef struct poplibs_popini_parser{
	enum poplibs_popinierror err;
	unsigned pos;
	unsigned errPos;
	unsigned nexttok;
	char errSym;
} poplibs_popiniparser;

typedef struct poplibs_popinitoken{
	enum poplibs_popinitype type;
	unsigned end;
	unsigned start;
} poplibs_popinitoken_t;

POPLIBS_POPINIAPI void poplibs_popiniparser_init(poplibs_popiniparser *parser);
POPLIBS_POPINIAPI unsigned poplibs_popiniparser_parse(poplibs_popiniparser *parser,const char *str,const unsigned strsize,poplibs_popinitoken_t *tokens,const unsigned tokenlen);

#	ifndef POPLIBS_POPINICOMPILED

static poplibs_popinitoken_t *popini_alloctoken(poplibs_popiniparser *parser,poplibs_popinitoken_t *tokens,const unsigned tokenlen){
	poplibs_popinitoken_t *token= POPLIBS_POPININULL;
	if(parser->nexttok<tokenlen){
		token= &tokens[parser->nexttok++];
		token->type= poplibs_popinitype_unknown;
		token->start= 0;
		token->end= 0;
	}
	return token;
}

static char popini_skipspace(poplibs_popiniparser *parser,const char *str,const unsigned strsize){
	char chr= '\0';
	int leav= 0;

	for(;parser->pos<strsize && (chr= str[parser->pos])!='\0';parser->pos++){
		switch(chr){
			case ' ':
			case '\t':
			case '\v':
			case '\f':
			case '\r':
				break;
			default:
				leav= 1;
				break;
		}
		if(leav){
			break;
		}
	}
	return chr;
}

static void popini_value(poplibs_popiniparser *parser,const char *str,const unsigned strsize,poplibs_popinitoken_t *tokens,const unsigned tokenlen,unsigned *made,int dirM){
	char chr= '\0';
	char tmp;
	char tmpt[]= {'r','u','e','\0'};
	char tmpf[]= {'a','l','s','e','\0'};
	int i= 0;
	unsigned start;
	unsigned lMade= 0;
	int mTok= 0;
	int isN= 0;
	int hDot= 0;
	poplibs_popinitoken_t *token;
	
	chr= popini_skipspace(parser,str,strsize);
	start= parser->pos;
	switch(chr){
		case '\'':
		case '"':
			tmp= chr;
			parser->pos++;
			for(;parser->pos<strsize && (chr= str[parser->pos])!='\0';parser->pos++){
				switch(chr){
					case '\'':
					case '"':
						if(chr==tmp && !isN){
							parser->pos++;
							if(tokens!=POPLIBS_POPININULL){
								if((token= popini_alloctoken(parser,tokens,tokenlen))!=POPLIBS_POPININULL){
									token->type= poplibs_popinitype_string;
									token->start= start+1;
									token->end= parser->pos-2;
									mTok= 1;
									(*made)++;
								}else{
									parser->err= poplibs_popinierror_nomem;
									parser->errSym= '\0';
									parser->errPos= parser->pos-1;
									parser->pos= start;
									mTok= 2;
								}
							}else{
								(*made)++;
								mTok= 1;
							}
						}
						break;

					case '\\':
						isN= !isN;
						break;

					default:
						isN= 0;
						break;
				}
				if(mTok>0){
					break;
				}
			}
			if((chr=='\0' || parser->pos>=strsize) && mTok!=1){
				parser->err= poplibs_popinierror_part;
				parser->errPos= parser->pos-1;
				parser->pos= start;
				parser->errSym= tmp;
			}
			break;
		
		case '{':
			if(tokens!=POPLIBS_POPININULL){
				if((token= popini_alloctoken(parser,tokens,tokenlen))!=POPLIBS_POPININULL){
					token->type= poplibs_popinitype_table;
					token->start= 0;
					(*made)++;
					mTok= 1;
				}else{
					parser->err= poplibs_popinierror_nomem;
					parser->errSym= '\0';
					parser->errPos= parser->pos;
					parser->pos= start;
					mTok= 2;
				}
			}else{
				mTok= 1;
				(*made)++;
			}
			if(mTok==1){
				int cAdd= 1;
				
				parser->pos++;
				mTok= 0;
				for(;parser->pos<strsize && (chr= str[parser->pos])!='\0';parser->pos++){
					switch(chr){
						case ' ':
						case '\f':
						case '\v':
						case '\t':
						case '\r':
							cAdd= 1;
							break;
						
						case ';':
						case '#':
							parser->err= poplibs_popinierror_missi;
							parser->errSym= '}';
							parser->errPos= parser->pos-1;
							parser->pos= start;
							mTok= 2;
							break;
						
						case '}':
							parser->pos++;
							if(token!=POPLIBS_POPININULL){
								token->end= lMade;
							}
							mTok= 1;
							break;
						
						default:
							if(cAdd){
								popini_value(parser,str,strsize,tokens,tokenlen,made,1);
								lMade++;
								if(parser->err!=poplibs_popinierror_none){
									mTok= 2;
								}else{
									parser->pos--;
								}
							}else{
								parser->err= poplibs_popinierror_inval;
								parser->errSym= '"';
								parser->errPos= parser->pos;
								parser->pos= start;
								mTok= 2;
							}
							break;
					}
					if(mTok>0){
						break;
					}
				}
				if((chr=='\0' || parser->pos>=strsize) && mTok!=1){
					parser->err= poplibs_popinierror_part;
					parser->errPos= parser->pos-1;
					parser->pos= start;
					parser->errSym= '}';
				}
			}
			break;
		
		case ';':
		case '#':
			parser->err= poplibs_popinierror_missi;
			parser->errSym= '"';
			parser->errPos= parser->pos;
			parser->pos= start;
			mTok= 2;
			break;

		default:
			if(chr=='-' || chr=='.' || (chr>=48 && chr<=57)){
				if(chr=='.'){
					hDot= 1;
				}
				isN= 1;
			}
			if(chr=='t' || chr=='T'){
				isN= 2;
			}else if(chr=='f' || chr=='F'){
				isN= 3;
			}
			parser->pos++;
			for(;parser->pos<strsize && (chr= str[parser->pos])!='\0';parser->pos++){
				switch(chr){
					case ' ':
					case '\r':
					case '\t':
					case '\v':
					case '\f':
					case ';':
					case '#':
					case '}':
					case '\n':
						if((!dirM || chr!='}') || (chr!=';' && chr!='#' && chr!='\n' && chr!='\0')){
							enum poplibs_popinitype typ= poplibs_popinitype_string;
							if(isN>0){
								typ= poplibs_popinitype_primi;
							}
							if(tokens!=POPLIBS_POPININULL){
								if((token= popini_alloctoken(parser,tokens,tokenlen))!=POPLIBS_POPININULL){
									token->type= typ;
									token->start= start;
									token->end= parser->pos-1;
									mTok= 1;
									(*made)++;
								}else{
									parser->err= poplibs_popinierror_nomem;
									parser->errSym= '\0';
									parser->errPos= parser->pos;
									parser->pos= start;
									mTok= 2;
								}
							}else{
								(*made)++;
								mTok= 1;
							}
						}else if(dirM){
							parser->err= poplibs_popinierror_missi;
							parser->errSym= '}';
							parser->errPos= parser->pos;
							parser->pos= start;
							mTok= 2;
						}
						break;
					
					default:
						if(isN==1 && (((chr=='.' && hDot) || chr!='.') && (chr<48 || chr>57))){
							isN= 0;
						}else if(isN==1 && chr=='.'){
							hDot= 1;
						}else if(isN==2 || isN==3){
							if(isN==2){
								if(chr!=tmpt[i] && chr!=(tmpt[i]-32)){
									isN= 0;
								}
								if(i<3){
									i++;
								}
							}else{
								if(chr!=tmpf[i] && chr!=(tmpf[i]-32)){
									isN= 0;
								}
								if(i<4){
									i++;
								}
							}
						}
						break;
				}
				if(mTok>0){
					break;
				}
			}
			if((chr=='\0' || parser->pos>=strsize) && mTok!=1){
				if((!dirM || chr!='}') || (chr!=';' && chr!='#' && chr!='\n' && chr!='\0')){
					enum poplibs_popinitype typ= poplibs_popinitype_string;
					if(isN>0){
						typ= poplibs_popinitype_primi;
					}
					if(tokens!=POPLIBS_POPININULL){
						if((token= popini_alloctoken(parser,tokens,tokenlen))!=POPLIBS_POPININULL){
							token->type= typ;
							token->start= start;
							token->end= parser->pos-1;
							mTok= 1;
							(*made)++;
						}else{
							parser->err= poplibs_popinierror_nomem;
							parser->errSym= '\0';
							parser->errPos= parser->pos;
							parser->pos= start;
							mTok= 2;
						}
					}else{
						(*made)++;
						mTok= 1;
					}
				}else if(dirM){
					parser->err= poplibs_popinierror_missi;
					parser->errSym= '}';
					parser->errPos= parser->pos;
					parser->pos= start;
					mTok= 2;
				}
			}
			break;
	}

	return;
}

static void popini_keyvalpair(poplibs_popiniparser *parser,const char *str,const unsigned strsize,poplibs_popinitoken_t *tokens,const unsigned tokenlen,unsigned *made){
	char chr= '\0';
	unsigned start= parser->pos;
	unsigned end;
	int mTok= 0;
	int skptoeql= 0;
	unsigned lMade= 0;
	poplibs_popinitoken_t *token;

	for(;parser->pos<strsize && (chr= str[parser->pos])!='\0';parser->pos++){
		switch(chr){
			case ' ':
			case '\t':
			case '\v':
			case '\f':
			case '\r':
				if(!skptoeql){
					end= parser->pos-1;
					skptoeql= 1;
				}
				break;
			
			case '=':
				if(!skptoeql){
					end= parser->pos-1;
				}
				if(tokens!=POPLIBS_POPININULL){
					if((token= popini_alloctoken(parser,tokens,tokenlen))!=POPLIBS_POPININULL){
						token->type= poplibs_popinitype_key;
						token->start= start;
						token->end= end;
						mTok= 1;
						lMade++;
						parser->pos++;
						popini_value(parser,str,strsize,tokens,tokenlen,&lMade,0);
					}else{
						parser->err= poplibs_popinierror_nomem;
						parser->errSym= '\0';
						parser->errPos= parser->pos;
						parser->pos= start;
					}
				}else{
					mTok= 1;
					lMade++;
					parser->pos++;
					popini_value(parser,str,strsize,tokens,tokenlen,&lMade,0);
				}
				if(parser->err!=poplibs_popinierror_none){
					parser->nexttok-= lMade;
					parser->pos= start;
					mTok= 2;
				}else{
					(*made)+= lMade;
				}
				break;
			
			case '\n':
				parser->err= poplibs_popinierror_missi;
				parser->errSym= '=';
				parser->errPos= parser->pos-1;
				parser->pos= start;
				mTok= 2;
				break;
			
			default:
				if(skptoeql){
					parser->err= poplibs_popinierror_inval;
					parser->errSym= '=';
					parser->errPos= parser->pos;
					parser->pos= start;
					mTok= 2;
				}
				break;
		}
		if(mTok>0){
			break;
		}
	}
	if((chr=='\0' || parser->pos>=strsize) && mTok!=1){
		parser->err= poplibs_popinierror_part;
		parser->errPos= parser->pos-1;
		parser->pos= start;
		parser->errSym= '=';
	}
	return;
}

static void popini_section(poplibs_popiniparser *parser,const char *str,const unsigned strsize,poplibs_popinitoken_t *tokens,const unsigned tokenlen,unsigned *made){
	char chr= '\0';
	unsigned start= parser->pos;
	int mTok= 0;
	int backslashed= 0;
	poplibs_popinitoken_t *token;

	for(;parser->pos<strsize && (chr= str[parser->pos])!='\0';parser->pos++){
		switch(chr){
			case ']':
				if(!backslashed){
					if(tokens!=POPLIBS_POPININULL){
						if((token= popini_alloctoken(parser,tokens,tokenlen))!=POPLIBS_POPININULL){
							token->type= poplibs_popinitype_section;
							token->start= start+1;
							token->end= parser->pos-1;
							mTok= 1;
							(*made)++;
						}
					}else{
						mTok= 1;
						(*made)++;
					}
				}
				break;
			
			case '\\':
				backslashed= !backslashed;
				break;
			
			case '\n':
				parser->err= poplibs_popinierror_missi;
				parser->errSym= ']';
				parser->errPos= parser->pos-1;
				parser->pos= start;
				mTok= 2;
				break;
			
			default:
				backslashed= 0;
				break;
		}
		if(mTok>0){
			break;
		}
	}
	if((chr=='\0' || parser->pos>=strsize) && mTok!=1){
		parser->err= poplibs_popinierror_part;
		parser->errPos= parser->pos-1;
		parser->pos= start;
		parser->errSym= ']';
	}
	return;	
}

POPLIBS_POPINIAPI unsigned poplibs_popiniparser_parse(poplibs_popiniparser *parser,const char *str,const unsigned strsize,poplibs_popinitoken_t *tokens,const unsigned tokenlen){
	char chr= '\0';
	unsigned start= parser->pos;
	unsigned made= 0;
	int nline= 0;
	int commed= 0;
	parser->err= poplibs_popinierror_none;

	for(;parser->pos<=strsize && (chr= str[parser->pos])!='\0';parser->pos++){
		switch(chr){
			case '#':
			case ';':
				nline= 1;
				commed= 1;
				break;
			
			case '\n':
				nline= 0;
				break;
			
			case '[':
				popini_section(parser,str,strsize,tokens,tokenlen,&made);
				start= parser->pos;
				nline= 1;
				break;
			
			case ' ':
			case '\t':
			case '\f':
			case '\v':
			case '\r':
				break;
			
			default:
				if(nline && !commed){
					parser->err= poplibs_popinierror_inval;
					parser->errPos= parser->pos;
					parser->pos= start;
					parser->errSym= ';';
				}else if(!nline){
					popini_keyvalpair(parser,str,strsize,tokens,tokenlen,&made);
					start= parser->pos;
					if((chr= str[parser->pos])!='\n'){
						nline= 1;
					}
				}
				break;
		}
		if(parser->err!=poplibs_popinierror_none){
			break;
		}
	}
	return made;
}

POPLIBS_POPINIAPI void poplibs_popiniparser_init(poplibs_popiniparser *parser){
	parser->err= poplibs_popinierror_none;
	parser->errPos= 0;
	parser->nexttok= 0;
	parser->pos= 0;
	parser->errSym= '\0';
	return;
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
