*This code has very different syntax to the old version of popini, keep that in mind when thinking about updating your library.*
# popini
A C ini file parser and writer. Parses ini files into structures that hold individual values, like sections, keys, and comments.
## Keep note of this lighter and faster popini.
Other then the api being very very different, its also missing some features that the original popini contained. Think of this library being the [jsmn](https://github.com/zserge/jsmn) of .ini files. This means no searching for sections, and no writing back into a file with the strutures (in this case the tokens). This means all handling is done on your end, and no memory is used without your consent. Downside is the reason i made popini is kind of lost. The main reason i made popini was for the very feature of writing back into the file with the modified structures. While these changes may seem for the worse, keep in mind you gain in no compilation, and very light parser (which is actually more of a [lexical analysis like system](https://en.wikipedia.org/wiki/Lexical_analysis).)
## Using library inside of your own project.
Because popini has moved on to a single header library based system, all you really need to add to your project is the `popini.h` file inside of the `src` folder. After that include the header inside of your code and you are done! Keep in mind best practice is to make a C file that just does the task of compiling the header, something like `popini.c`, this will help prevent recomplilation of the code when including the header in other code files. After you have the header included inside of `popini.c` you are done, inside your own code files, when including the header make sure you add the line `#define POPLIBS_POPINICOMPILED` so it wont recompile the code again. Otherwise if you want to have a seperate compile of the program in two code files, add the this line before include the header to compile all the functions as static `#define POPLIBS_POPINI_STATIC`

### Before you use.
As stated in the about of this library, it uses a token like system. Tokens contain 3 keys inside of their structure, `type`, `start`, and `end`. `type` describes the token type, and it can be any of these 5 types `poplibs_popinitype_section`, `poplibs_popinitype_key`, `poplibs_popinitype_string`, `poplibs_popinitype_primi`, and `poplibs_popinitype_table`. All token types, except `poplibs_popinitype_table` set the `start` and `end`, these are positions in the string you passed it that this token pertains to, aka if the token type is `poplibs_popinitype_section` and the string you passed it equals `[Section]` then the token's start and end would be 1 and 7 respectively. `poplibs_popinitype_table` only sets the `end` key, and it stores the amount of tokens after it that are apart of the table. Say you have `{Val1 Val2 Val3}` then the token's `end` key would be 3.
Keep this in mind when you are trying to make sense of the tokens returned from the function.

### Functions.
```c
void poplibs_popiniparser_init(poplibs_popiniparser *parser);
```
This function initalizes all of the data of the passed `poplibs_popiniparser` to 0, essentially resetting it.

```c
unsigned poplibs_popiniparser_parse(poplibs_popiniparser *parser,const char *str,const unsigned strlen,poplibs_popinitoken_t *tokens,const unsigned tokenlen);
```
This function does the actual parser, you pass the structure `poplibs_popiniparser` that you initalized (above), then the string, and the length of the string, an array of your tokens, and the length of that array. This function will return an integer which is the number of tokens it has made/added into the tokens on its run, but doesnt keep track of tokens it has made total, that is your job of keeping track.

### Structures.
```c
typedef struct poplibs_popini_parser{
	enum poplibs_popinierror err;
	unsigned pos;
	unsigned errPos;
	unsigned nexttok;
	char errSym;
} poplibs_popiniparser;
```
This is the parser object, make sure after calling `poplibs_popiniparser_parse()` that you check your parser object's `err` key.
The key `pos` stores the parser objects current position in the string, the key `errPos` stores the position that invoked the error, only set when there is an error, the key `nexttok` stores the index in which to "allocate" the next token, the key `errSym` stores a character that it recommends you place at `errPos` (or before) to fix the error, only applies to all errors except `poplibs_popinierror_nomem` and if there is no error.

```c
typedef struct poplibs_popinitoken{
	enum poplibs_popinitype type;
	unsigned end;
	unsigned start;
} poplibs_popinitoken_t;
```
This is the token object, make sure after calling `poplibs_popiniparser_parse()` you dont go over the amount of tokens it has made, because after that point the data could be anything. The key `type` stores the token type, the key `end` stores the end position in string for every type except `poplibs_popinitype_table`, in which case stores the amount of tokens afterwards are about of the table, the key `start` stores the start position in the string for every type except `poplibs_popinitype_table`, in which it stores nothing.

### Enums.
```c
enum poplibs_popinierror{
	poplibs_popinierror_none= 0,
	poplibs_popinierror_inval= -1,
	poplibs_popinierror_missi= -2,
	poplibs_popinierror_nomem= -3,
	poplibs_popinierror_part= -4
};
```
Stores all of the errorcodes. the enum item `poplibs_popinierror_none` means that there was no error, the enum item `poplibs_popinierror_inval` means that there is an invalid character, the enum item `poplibs_popinierror_missi` means there its missing a crucial character, the enum item `poplibs_popinierror_part` means that the string doesnt seem to be finished, aka is part of an ini string.

```c
enum poplibs_popinitype{
	poplibs_popinitype_unknown= 0,
	poplibs_popinitype_section= 1,
	poplibs_popinitype_key= 2,
	poplibs_popinitype_string= 3,
	poplibs_popinitype_primi= 4,
	poplibs_popinitype_table= 5
};
```
Stores all token types. The enum item `poplibs_popinitype_unknown` is a generic type, any tokens with this type are tokens that the parser did not make. The enum item `poplibs_popinitype_section` is a section type, a section is defined as any string inside of brackets `[]`. The enum item `poplibs_popinitype_key` is a key type, a key is defined as a string with no spaces and no `=` before a value, aka the word `Key` in this line is a key type `Key = "Value"`. The enum item `poplibs_popinitype_string` is a string type, only applies when after a key token or apart of a table token, an example of a string would be the word `Value` inside the line `Key = "Value"`. The enum item `poplibs_popinitype_primi` is a primitive type, a primitive is a number or boolean, and only applies when after a key token or apart of a table token, an example of a primitive would be the number `6430` inside the line `Key = 6430`, a boolean can be `true` or `false` and is not case sensitive. The enum item `poplibs_popinitype_table` is a table type, and only applies after a key token, and stores an unlimited number of `poplibs_popinitype_string`, `poplibs_popinitype_primi`, and `poplibs_popinitype_table` tokens, and stores its token count in the key `end`, the count only counts the `poplibs_popinitype_table` as a token in the table, not all other `poplibs_popinitype_string`, `poplibs_popinitype_primi`, and `poplibs_popinitype_table` tokens that are apart of that token.

### Examples!
It might seem fairly confusing on how popini works, but this quick example which parses the string `[Section]\nKey = {Table in {a table}} ; This is a comment, and is ignored` and prints out all the tokens the parser made isnt too complicated.

```c
#include "popini.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	const char ini[]= "[Section]\nKey = {Table in {a table}} ; This is a comment, and is ignored";
	poplibs_popiniparser parser;
	poplibs_popinitoken_t tokens[64];
	unsigned madeTokens;

	poplibs_popiniparser_init(&parser);
	madeTokens= poplibs_popiniparser_parse(&parser,ini,strlen(ini),tokens,64);
	if(parser.err==poplibs_popinierror_none){
		unsigned i;
		for(i=0;i<madeTokens;i++){
			poplibs_popinitoken_t *token= &tokens[i];

			if(token->type==poplibs_popinitype_table){
				printf("Token %u is a table\nThe next %u tokens are apart of this token\n\n",i,token->end);
			}else{
				char *str= substr(ini,token->start,token->end);
				printf("Token %u is ",i);
				switch(token->type){
					case poplibs_popinitype_section:
						printf("a section\n");
						break;
					case poplibs_popinitype_key:
						printf("a key\n");
						break;
					case poplibs_popinitype_string:
						printf("a string\n");
						break;
					case poplibs_popinitype_primi:
						printf("a primitive\n");
						break;
					default:
						printf("unknown\n");
						break;
				}
				printf("String (%s)\n\n",str);
				free(str);
			}
		}
	}

	return 0;
}
```