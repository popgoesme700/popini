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

#ifndef POPLIBS_POPINI_H_FILE
#	define POPLIBS_POPINI_H_FILE

#	ifdef POPLIBS_POPINI_STATIC
#		define POPLIBS_POPINIAPI static
#	else
#		define POPLIBS_POPINIAPI extern
#	endif

/* Define an enum to hold error types */
enum poplibs_popinierror{
	POPLIBS_POPINIERROR_NONE= 0,	/* When set to error int, means that there were no errors */
	POPLIBS_POPINIERROR_INVAL=1,	/* When set to error int, means that there was an invalid character in the ini string */
	POPLIBS_POPINIERROR_NOMEM=2,	/* When set to error int, means that there is not enough tokens allocated. */
	POPLIBS_POPINIERROR_PART=3		/* When set to error int, indicates that the data recieved is not fully complete */
};

/* Define an enum to hold ini token types */
enum poplibs_popinitype{
	POPLIBS_POPINITYPE_UNKNOWN=0,	/* When a token type equals this, it means the type seems to be unknown */
	POPLIBS_POPINITYPE_SECTION=1,	/* When a token type equals this, it means the token is of a section, aka `ini [Section]` */
	POPLIBS_POPINITYPE_STRING=3,	/* When a token type equals this, it means the token is of a string, aka `ini "This is a String"` or could possibly be a no spaced string, like `ini String` */
	POPLIBS_POPINITYPE_TABLE=4,		/* When a token type equals this, it means the token is of a table. */
	POPLIBS_POPINITYPE_PRIM=5		/* When a token type equals this, it means the token is of a primitive, aka bool,int,float */
}; /* Note: string type is only applied to strings that dont start with a number (and dont contain any symbols), and arent a case insensitive "true" and "false". Strings are also any length of any characters placed inside of quotes (" or '), if one wishes to place a quote inside their string (and the quote is the same as the one they started with), they could place a backslash before it to escape it. Keep in mind, the header wont mess with your input string, so if you find an escaped quote in your token snippets, you will be in charge of removing it */

/* Define token structure */
typedef struct poplibs_popinitoken{
	enum poplibs_popinitype type;	/* 4 bytes -> Holds type enum of token */
	unsigned start;					/* 4 bytes -> Holds start index of string */
	unsigned end;					/* 4 bytes -> Holds end index of string */
	unsigned size;					/* 4 bytes -> Holds total size of the string snippet */
} poplibs_popinitoken_t;

/* Define parser structure */
typedef struct poplibs_popini_parser{
	unsigned pos;		/* Holds current index of string */
	unsigned nexttoken;	/* Holds index of next token to place data in */
} poplibs_popiniparser;

/* Initializes poplibs_popiniparser object... can simply set both keys inside of the structure 0 */
POPLIBS_POPINIAPI void poplibs_popiniparser_init(poplibs_popiniparser *parser);

/* Parses an ini string. Pass parser object pointer, then the string and its length, then the tokens array, and its length, then a pointer to an int which will be set with any parsing errors that may occur. returns the amount of tokens it has made. */
POPLIBS_POPINIAPI unsigned poplibs_popiniparser_parse(poplibs_popiniparser *parser,const char *ini,const unsigned inilen,poplibs_popinitoken_t *tokens,const unsigned tokenlen,int *errorcode);

#endif