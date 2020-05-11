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
#include "popini.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* fgets() chunk size, the amount of characters to get each time fgets() is called, if \n isnt the last char (before \0), then will recall increasing size by this. */
#define H_FILE_POPINI_GETLINE_CHUNCKSIZE 32
/* Defined whitespace chars to check for in the string, to skip when retrieving value,comment,key,and section from line. */
#define H_FILE_POPINI_WHITESPACECHARS " \n\t\v\f\r"
/* Defined `dilema` characters to check for in a line for comment starts. */
#define H_FILE_POPINI_DILEMACHARS ";#"
/* Defined `quote` characters to check for quotes that specify a value's start and end. */
#define H_FILE_POPINI_QUOTECHARS "\"'"

/* Reads a line of the file passed. */
static char *readLine(FILE *read){
	/* Next is the pointer to add characters from `fgets` returns. Line is the whole line. */
	char *line= NULL;
	char *next= NULL;
	/* Set the amount of characters we retrieve to the macro above. */
	long unsigned lineSize=H_FILE_POPINI_GETLINE_CHUNCKSIZE;
	/* If the file doesnt exist, or if the pointer is at then end of the file, return NULL */
	if((read==NULL) || feof(read)){
		return NULL;
	}
	/* Allocate lineSize bytes to line so we can actually store the line! */
	line= (char*)calloc(lineSize,sizeof(*line));
	if(line==NULL){
		return NULL;
	}
	line[0]= '\0';
	next= line;

	/* While loop fgets line via with the size of chunk. If the last character held is a newline, then that means we got the line, so we can break off! */
	while(fgets(next,lineSize-strlen(line),read)!=NULL){
		if(line[strlen(line)-1]=='\n'){
			line[strlen(line)-1]= '\0';
			break;
		}else{
			lineSize+=H_FILE_POPINI_GETLINE_CHUNCKSIZE;
			line= (char*)realloc(line,lineSize*sizeof(*line));
			if(line==NULL){
				return NULL;
			}
			next= line+strlen(line);
		}
	}
	return line;
}

/* Returns 1 if the character passed is one of the characters defined in the macro "DILEMACHARS", else it returns 0. */
static char char_is_dilema(const char dilema){
	char is= 0;
	long unsigned i;
	long unsigned len= strlen(H_FILE_POPINI_DILEMACHARS);
	for(i=0;i<len;i++){
		if(H_FILE_POPINI_DILEMACHARS[i]==dilema){
			is= 1;
			break;
		}
	}
	return is;
}

/* Returns 1 if the characters passed is one of the characters defined in the macro "WHITESPACECHARS", else it returns 0. */
static char char_is_whitespace(const char chr){
	char is= 0;
	long unsigned i;
	long unsigned len= strlen(H_FILE_POPINI_WHITESPACECHARS);
	for(i=0;i<len;i++){
		if(H_FILE_POPINI_WHITESPACECHARS[i]==chr){
			is= 1;
			break;
		}
	}
	return is;
}

/* Returns 1 if the characters passed is one of the characters defined in the macro "QUOTECHARS", else it returns 0. */
static char char_is_quote(const char quote){
	char is= 0;
	long unsigned i;
	long unsigned len= strlen(H_FILE_POPINI_QUOTECHARS);
	for(i=0;i<len;i++){
		if(H_FILE_POPINI_QUOTECHARS[i]==quote){
			is= 1;
			break;
		}
	}
	return is;
}

/* Goes through each character of string until it finds the passed char, starting from the start of the string to the end, itll return the position in the string it lies, otherwise NULL. */
static char *char_in_string(char *str,const char chr){
	while(*str!='\0'){
		if(*str==chr){
			break;
		}
		str++;
	}
	if(*str=='\0'){
		return NULL;
	}
	return str;
}

/* Goes through each character of string until it finds the passed char, starting from the start of the end of the string to the start, itll return the position in the string it lies, otherwise NULL. */
static char *char_in_string_end(char *str,char chr){
	const char *originptr= str;
	str= &str[strlen(str)-1];
	while(str!=originptr){
		if(*str==chr){
			break;
		}
		str--;
	}
	if(*str=='\0' || str==originptr){
		return NULL;
	}
	return str;
}

/* Goes through the whole string, until there is no longer a `Whitespace` characeter. Returns the pointer in which it is no longer a whitespace character. */
static char *skip_whitespaces(char *str){
	long unsigned i;
	while(char_is_whitespace(*str) && *str!='\0'){
		str++;
	}
	if(*str=='\0'){
		str--;
	}
	return str;
}

/* Duplicates a string */
static char *dup_string(const char *str){
	long unsigned size= strlen(str)+1;
	char *nStr= (char*)calloc(size,sizeof(*nStr));
	nStr= memcpy(nStr,str,size);
	return nStr;
}

/* Like char_in_string_end, but you also pass an array of chars to check against instead of one character. */
static char *char_in_string_end_list(char *str,char *list){
	char *originptr= str;
	str= &str[strlen(str)-1];
	while(str!=originptr){
		char whewWeDone= 0;
		char *ptr= list;
		while(*ptr!='\0'){
			if(*ptr==*str){
				whewWeDone= 1;
				break;
			}
			ptr++;
		}
		if(whewWeDone){
			break;
		}
		str--;
	}
	if(*str=='\0' || str==originptr){
		return NULL;
	}
	return str;
}

/* Like char_in_string, but you also pass an array of chars to check against instead of one character. */
static char *char_in_string_list(char *str,char *list){
	while(*str!='\0'){
		char whewWeDone= 0;
		char *ptr= list;
		while(*ptr!='\0'){
			if(*ptr==*str){
				whewWeDone= 1;
				break;
			}
			ptr++;
		}
		if(whewWeDone){
			break;
		}
		str++;
	}
	if(*str=='\0'){
		return NULL;
	}
	return str;
}

/* Remove character pointed by string, then shift all characters after it back one. */
static void string_remove_current_char(char *str){
	while(*str!='\0'){
		*(str)= *(str+1);
		str++;
	}
}

/* Adds character to string, shifting all characters after it to the right one. Dont forget to realloc the string before calling this! This wont realloc the string for you! */
static void string_add_char(char *str,char chr){
	char *originptr= str;
	str= &str[strlen(str)];
	while(str!=originptr){
		*str= *(str-1);
		str--;
	}
	*originptr= chr;
}

/* Like char_in_string, but doesnt count characters that have a \ before it... also shifts string for every unescaped backslash. (So if a value needs/wants to have a backslash, put one before it.) */
static char *char_in_string_not_escaped(char *str,char chr){
	char backslashed= 0;
	while(*str!='\0'){
		if(*str==chr && !backslashed){
			break;
		}else if(*str==chr){
			string_remove_current_char((str-1));
			str--;
			backslashed= 0;
		}else if(*str=='\\' && !backslashed){
			backslashed= 1;
		}else if(backslashed){
			backslashed= 0;
		}
		str++;
	}
	if(*str=='\0'){
		return NULL;
	}
	return str;
}

/* Like char_in_string, but adds escaping backslash before character. */
static char *char_in_string_escape(char *str,char chr){
	long unsigned size= strlen(str)+1;
	char *allo= (char*)calloc(size,sizeof(*allo));
	allo= memcpy(allo,str,size);
	while(*str!='\0'){
		if(*str==chr){
			size++;
			allo= (char*)realloc(allo,size*sizeof(*allo));
			allo--;
			string_add_char(allo,'\\');
			allo++;
		}
		str++;
	}
	return allo;
}

/* Frees memory used by a COMMENT structure */
static void free_comment(POPINI_ST_COMMENT *comment){
	free(comment->str);
	comment->str= NULL;
	comment->dilema= '\0';
	comment->line= 0;
}
/* Frees memory used by a KEY structure */
static void free_key(POPINI_ST_KEY *key){
	free(key->key);
	key->key= NULL;
	if(key->values!=NULL){
		long unsigned i;
		for(i=0;i<=key->valLen;i++){
			if(key->values[i]!=NULL){
				free(key->values[i]);
			}
		}
		free(key->values);
	}
	if(key->comment!=NULL){
		free_comment(key->comment);
		free(key->comment);
	}
	key->comment= NULL;
	key->values= NULL;
	key->valLen= 0;
}
/* Frees memory used by a SECTION structure */
static void free_section(POPINI_ST_SECTION *section){
	free(section->name);
	section->name= NULL;
	if(section->keys!=NULL){
		long unsigned i;
		for(i=0;i<=section->keyLen;i++){
			if(section->keys[i]!=NULL){
				free_key(section->keys[i]);
				free(section->keys[i]);
			}
		}
		free(section->keys);
	}
	if(section->comment!=NULL){
		free_comment(section->comment);
		free(section->comment);
	}
	section->comment= NULL;
	section->keys= NULL;
	section->keyLen= 0;
}
/* Frees memory used by an INIFILE structure */
static void free_ini_file(POPINI_ST_INIFILE *ini){
	free(ini->file);
	ini->file= NULL;
	if(ini->sections!=NULL){
		long unsigned i;
		for(i=0;i<=ini->secLen;i++){
			if(ini->sections[i]!=NULL){
				free_section(ini->sections[i]);
				free(ini->sections[i]);
			}
		}
		free(ini->sections);
	}
	ini->sections= NULL;
	ini->secLen= 0;
	if(ini->comments!=NULL){
		long unsigned i;
		for(i=0;i<=ini->comLen;i++){
			if(ini->comments[i]!=NULL){
				free_comment(ini->comments[i]);
				free(ini->comments[i]);
			}
		}
		free(ini->comments);
	}
	ini->comments= NULL;
	ini->comLen= 0;
}

/* Allocates memory for a new INIFILE structure */
static POPINI_ST_INIFILE *new_ini_file(const char *file){
	POPINI_ST_INIFILE *ini= (POPINI_ST_INIFILE*)calloc(1,sizeof(*ini));
	long unsigned size= strlen(file)+1;
	ini->file= (char*)calloc(size,sizeof(*ini->file));
	ini->file= memcpy(ini->file,file,size);
	ini->sections= NULL;
	ini->comments= NULL;
	ini->keys= NULL;
	ini->secLen= 0;
	ini->keyLen= 0;
	ini->comLen= 0;

	return ini;
}

/* Allocates memory for a new SECTION structure */
static POPINI_ST_SECTION *new_section(const char *name){
	POPINI_ST_SECTION *section= (POPINI_ST_SECTION*)calloc(1,sizeof(*section));
	long unsigned size= strlen(name)+1;
	section->name= (char*)calloc(size,sizeof(*section->name));
	section->name= memcpy(section->name,name,size);
	section->keys= NULL;
	section->keyLen= 0;

	return section;
}

/* Allocates memory for a new KEY structure */
static POPINI_ST_KEY *new_key(const char *keyy){
	POPINI_ST_KEY *key= (POPINI_ST_KEY*)calloc(1,sizeof(*key));
	long unsigned size= strlen(keyy)+1;
	key->key= (char*)calloc(size,sizeof(*key->key));
	key->key= memcpy(key->key,keyy,size);
	key->values= NULL;
	key->valLen= 0;

	return key;
}

/* Allocates memory for a new COMMENT structure */
static POPINI_ST_COMMENT *new_comment(const char *str){
	POPINI_ST_COMMENT *comment= (POPINI_ST_COMMENT*)calloc(1,sizeof(*comment));
	if(comment==NULL){
		return NULL;
	}
	long unsigned size= strlen(str)+1;
	comment->str= (char*)calloc(size,sizeof(*comment->str));
	comment->str= memcpy(comment->str,str,size);
	comment->dilema= '\0';
	comment->line= 0;

	return comment;
}

/* Adds SECTION structure to INIFILE structure */
static void add_section(POPINI_ST_INIFILE *ini,POPINI_ST_SECTION *section,long unsigned pos){
	if(pos>ini->secLen){
		if(ini->sections==NULL || ini->sections[ini->secLen]==NULL){
			pos= ini->secLen;
		}else{
			pos= ini->secLen+1;
		}
	}

	if(ini->sections==NULL){
		ini->sections= (POPINI_ST_SECTION**)calloc((pos+1),sizeof(**ini->sections));
		ini->sections[pos]= NULL;
	}else if(ini->secLen<pos){
		ini->sections= (POPINI_ST_SECTION**)realloc(ini->sections,(pos+1)*sizeof(**ini->sections));
		ini->sections[pos]= NULL;
	}

	if(ini->sections[pos]!=NULL){
		long unsigned nPos;
		for(nPos=ini->secLen+1;nPos>pos;nPos--){
			ini->sections[nPos]= ini->sections[nPos-1];
		}
		ini->sections[pos]= section;
		ini->secLen++;
	}else{
		ini->sections[pos]= section;
		ini->secLen=pos;
	}
}

/* Adds KEY structure to SECTION structure */
static void add_key(POPINI_ST_SECTION *section,POPINI_ST_KEY *key,long unsigned pos){
	if(pos>section->keyLen){
		if(section->keys==NULL || section->keys[section->keyLen]==NULL){
			pos= section->keyLen;
		}else{
			pos= section->keyLen+1;
		}
	}

	if(section->keys==NULL){
		section->keys= (POPINI_ST_KEY**)calloc((pos+1),sizeof(**section->keys));
		section->keys[pos]= NULL;
	}else if(section->keyLen<pos){
		section->keys= (POPINI_ST_KEY**)realloc(section->keys,(pos+1)*sizeof(**section->keys));
		section->keys[pos]= NULL;
	}

	if(section->keys[pos]!=NULL){
		long unsigned nPos;
		for(nPos=section->keyLen+1;nPos>pos;nPos--){
			section->keys[nPos]= section->keys[nPos-1];
		}
		section->keys[pos]= key;
		section->keyLen++;
	}else{
		section->keys[pos]= key;
		section->keyLen=pos;
	}
}

/* Adds KEY structure to INIFILE structure, as an unsectioned key, which are placed at top of file. */
static void add_unsectioned_key(POPINI_ST_INIFILE *ini,POPINI_ST_KEY *key,long unsigned pos){
	if(pos>ini->keyLen){
		if(ini->keys==NULL || ini->keys[ini->keyLen]==NULL){
			pos= ini->keyLen;
		}else{
			pos= ini->keyLen+1;
		}
	}

	if(ini->keys==NULL){
		ini->keys= (POPINI_ST_KEY**)calloc((pos+1),sizeof(**ini->keys));
		ini->keys[pos]= NULL;
	}else if(ini->keyLen<pos){
		ini->keys= (POPINI_ST_KEY**)realloc(ini->keys,(pos+1)*sizeof(**ini->keys));
		ini->keys[pos]= NULL;
	}

	if(ini->keys[pos]!=NULL){
		long unsigned nPos;
		for(nPos=ini->keyLen+1;nPos>pos;nPos--){
			ini->keys[nPos]= ini->keys[nPos-1];
		}
		ini->keys[pos]= key;
		ini->keyLen++;
	}else{
		ini->keys[pos]= key;
		ini->keyLen=pos;
	}
}

/* Adds a value string to KEY structure */
static void add_value(POPINI_ST_KEY *key,char *str,long unsigned pos){
	if(pos>key->valLen){
		if(key->values==NULL || key->values[key->valLen]==NULL){
			pos= key->valLen;
		}else{
			pos= key->valLen+1;
		}
	}

	if(key->values==NULL){
		key->values= (char**)calloc((pos+1),sizeof(*key->values));
		key->values[pos]= NULL;
	}else if(pos>key->valLen){
		key->values= (char**)realloc(key->values,(pos+1)*sizeof(*key->values));
		key->values[pos]= NULL;
	}

	long unsigned size= strlen(str)+1;
	if(key->values[pos]!=NULL){
		long unsigned nPos;
		for(nPos=key->valLen+1;nPos>pos;nPos--){
			key->values[nPos]= key->values[nPos-1];
		}
		key->values[pos]= (char*)calloc(size,sizeof(*key->values[pos]));
		key->values[pos]= memcpy(key->values[pos],str,size);
		key->valLen++;
	}else{
		key->values[pos]= (char*)calloc(size,sizeof(*key->values[pos]));
		key->values[pos]= memcpy(key->values[pos],str,size);
		key->valLen=pos;
	}
}

/* Adds a comment to the INIFILE structure */
static void add_inifile_comment(POPINI_ST_INIFILE *ini,POPINI_ST_COMMENT *com,long unsigned pos){
	if(pos>ini->comLen){
		if(ini->comments==NULL || ini->comments[ini->comLen]==NULL){
			pos= ini->comLen;
		}else{
			pos= ini->comLen+1;
		}
	}

	if(ini->comments==NULL){
		ini->comments= (POPINI_ST_COMMENT**)calloc((pos+1),sizeof(**ini->comments));
		ini->comments[pos]= NULL;
	}else if(pos>ini->comLen){
		ini->comments= (POPINI_ST_COMMENT**)realloc(ini->comments,(pos+1)*sizeof(**ini->comments));
		ini->comments[pos]= NULL;
	}

	if(ini->comments[pos]!=NULL){
		long unsigned nPos;
		for(nPos=ini->comLen+1;nPos>pos;nPos--){
			ini->comments[nPos]= ini->comments[nPos-1];
		}
		ini->comments[pos]= com;
		ini->comLen++;
	}else{
		ini->comments[pos]= com;
		ini->comLen= pos;
	}
}

/* Sets the comment on the SECTION structure */
static void set_section_comment(POPINI_ST_SECTION *section,POPINI_ST_COMMENT *com){
	if(section->comment!=NULL){
		free_comment(section->comment);
		free(section->comment);
	}
	section->comment= (POPINI_ST_COMMENT*)calloc(1,sizeof(*section->comment));
	section->comment= com;
}

/* Sets the comment on the KEY structure */
static void set_key_comment(POPINI_ST_KEY *key,POPINI_ST_COMMENT *com){
	if(key->comment!=NULL){
		free_comment(key->comment);
		free(key->comment);
	}
	key->comment= (POPINI_ST_COMMENT*)calloc(1,sizeof(*key->comment));
	key->comment= com;
}

/* Removes a value from a key, with specified position */
static void remove_value(POPINI_ST_KEY *key,long unsigned pos){
	if(pos>key->valLen){
		pos= key->valLen;
	}
	if(key->values!=NULL){
		if(pos>=key->valLen){
			free(key->values[key->valLen]);
			key->values[key->valLen]= NULL;
			if(key->valLen>0){
				key->valLen--;
			}
		}else{
			long unsigned nPos;
			free(key->values[pos]);
			key->values[pos]= NULL;
			for(nPos=pos;nPos<=key->valLen;nPos++){
				key->values[nPos]= key->values[nPos+1];
			}
			key->valLen--;
		}
		if(key->values[key->valLen]!=NULL){
			key->values= (char**)realloc(key->values,(key->valLen+1)*sizeof(*key->values));
		}else{
			free(key->values);
			key->values= NULL;
		}
	}
}

/* Removes a key from a section, with specified position */
static void remove_sectioned_key(POPINI_ST_SECTION *sec,long unsigned pos){
	if(pos>sec->keyLen){
		pos= sec->keyLen;
	}
	if(sec->keys!=NULL){
		if(pos>=sec->keyLen){
			free_key(sec->keys[sec->keyLen]);
			free(sec->keys[sec->keyLen]);
			sec->keys[sec->keyLen]= NULL;
			if(sec->keyLen>0){
				sec->keyLen--;
			}
		}else{
			long unsigned nPos;
			free_key(sec->keys[pos]);
			free(sec->keys[pos]);
			sec->keys[pos]= NULL;
			for(nPos=pos;nPos<=sec->keyLen;nPos++){
				sec->keys[nPos]= sec->keys[nPos+1];
			}
			sec->keyLen--;
		}
		if(sec->keys[sec->keyLen]!=NULL){
			sec->keys= (POPINI_ST_KEY**)realloc(sec->keys,(sec->keyLen+1)*sizeof(**sec->keys));
		}else{
			free(sec->keys);
			sec->keys= NULL;
		}
	}
}

/* Removes a key from an inifile, with specified position */
static void remove_unsectioned_key(POPINI_ST_INIFILE *ini,long unsigned pos){
	if(pos>ini->keyLen){
		pos= ini->keyLen;
	}
	if(ini->keys!=NULL){
		if(pos>=ini->keyLen){
			free_key(ini->keys[ini->keyLen]);
			free(ini->keys[ini->keyLen]);
			ini->keys[ini->keyLen]= NULL;
			if(ini->keyLen>0){
				ini->keyLen--;
			}
		}else{
			long unsigned nPos;
			free_key(ini->keys[pos]);
			free(ini->keys[pos]);
			ini->keys[pos]= NULL;
			ini->keyLen--;
			for(nPos=pos;nPos<=ini->keyLen;nPos++){
				ini->keys[nPos]= ini->keys[nPos+1];
			}
		}
		if(ini->keys[ini->keyLen]!=NULL){
			ini->keys= (POPINI_ST_KEY**)realloc(ini->keys,(ini->keyLen+1)*sizeof(**ini->keys));
		}else{
			free(ini->keys);
			ini->keys= NULL;
		}
	}
}

/* Removes a key from an inifile, with specified position. */
static void remove_newline_comment(POPINI_ST_INIFILE *ini,long unsigned pos){
	if(pos>ini->comLen){
		pos= ini->comLen;
	}
	if(ini->comments!=NULL){
		if(pos>=ini->comLen){
			free_comment(ini->comments[ini->comLen]);
			free(ini->comments[ini->comLen]);
			ini->comments[ini->comLen]= NULL;
			if(ini->comLen>0){
				ini->comLen--;
			}
		}else{
			long unsigned nPos;

			free_comment(ini->comments[pos]);
			free(ini->comments[pos]);
			ini->comments[pos]= NULL;
			for(nPos=pos;nPos<=ini->comLen;nPos++){
				ini->comments[nPos]= ini->comments[nPos+1];
			}
			ini->comLen--;
		}
		if(ini->comments[ini->comLen]!=NULL){
			ini->comments= (POPINI_ST_COMMENT**)realloc(ini->comments,(ini->comLen+1)*sizeof(**ini->comments));
		}else{
			free(ini->comments);
			ini->comments= NULL;
		}
	}
}

static void remove_section(POPINI_ST_INIFILE *ini,long unsigned pos){
	if(pos>ini->secLen){
		pos= ini->secLen;
	}
	if(ini->sections!=NULL){
		if(pos>=ini->secLen){
			free_section(ini->sections[ini->secLen]);
			free(ini->sections[ini->secLen]);
			ini->sections[ini->secLen]= NULL;
			if(ini->secLen>0){
				ini->secLen--;
			}
		}else{
			long unsigned nPos;

			free_section(ini->sections[pos]);
			free(ini->sections[pos]);
			ini->sections[pos]= NULL;
			for(nPos=pos;nPos<=ini->secLen;nPos++){
				ini->sections[nPos]= ini->sections[nPos+1];
			}
			ini->secLen--;
		}
		if(ini->secLen>0){
			ini->sections= (POPINI_ST_SECTION**)realloc(ini->sections,ini->secLen*sizeof(**ini->sections));
		}else if(ini->sections[ini->secLen]==NULL){
			free(ini->sections);
			ini->sections= NULL;
		}else{
			ini->sections= (POPINI_ST_SECTION**)realloc(ini->sections,sizeof(**ini->sections));
		}
	}
}

/* Writes the newline comment to the file */
static long unsigned write_comment_newline(POPINI_ST_INIFILE *ini,FILE *write,long unsigned pos){
	long unsigned lineAdd=0;
	if(ini->comments!=NULL){
		long unsigned i;
		for(i=0;i<=ini->comLen;i++){
			if(ini->comments[i]!=NULL){
				if(ini->comments[i]->line==pos){
					fprintf(write,"%c%s\n",ini->comments[i]->dilema,ini->comments[i]->str);
					pos++;
					lineAdd++;
				}
			}
		}
	}
	return lineAdd;
}

/* Reads the file passed (if it exists) and creates an inifile structure holding the info. */
POPINI_ST_INIFILE *POPINI_FU_PARSE_INIFILE(const char *file){
	/* Allocate memory for the INIFILE struct that will be returned! */
	POPINI_ST_INIFILE *ini= new_ini_file(file);

	/* Open our file in read mode, if it exists it wont return NULL, if it doesnt, then we just return the INIFILE allocated above, which contains nothing. */
	FILE *read= fopen(file,"r");
	if(read!=NULL){
		/* Keeps track of our line number and line */
		char *line;
		long unsigned lineNum= 0;
		/* Keep track of our current section so we know where to place keys! */
		POPINI_ST_SECTION *curSection= NULL;
		while((line=readLine(read))!=NULL){
			/* Duplicate our line pointer to ptr for manipulation. */
			char *ptr= line;
			/* check if the first character in the line isnt just a newline. or a terminating char. */
			if(*ptr!='\n' && *ptr!='\r' && *ptr!='\0'){
				/* An if-else statement that holds check for first character. First if is to check if the line is a comment, if so place it inside of the INIFILE->comments key. The second if is to check if the line starts with a `[` which means that it is a section. The third if is for keys. */
				if(char_is_dilema(*ptr)){
					ptr++;
					POPINI_ST_COMMENT *com= new_comment(ptr);
					com->line= lineNum;
					com->dilema= *(ptr-1);
					add_inifile_comment(ini,com,-1);
				}else if(*ptr=='['){
					ptr++;
					/* To prevent the issue that aries when you place closing brackets inside of a comment, we first check for the comment, then we are free to check for the closing bracket! */
					char *comment= char_in_string_list(ptr,H_FILE_POPINI_DILEMACHARS);
					char dilema= '\0';
					/* Because we need to place a termination char on the comment dilema character, we need to save it inside of the dilema variable, then we can set it to a termination character, and then increment the comment pointer by one character. */
					if(comment!=NULL){
						dilema= *comment;
						*comment= '\0';
						comment++;
					}
					/* Now we are safe to find the closing bracket! */
					char *ptrEnd= char_in_string_end(ptr,']');
					if(ptrEnd==NULL){
						free_ini_file(ini);
						free(line);
						ini= NULL;
						break;
					}
					/* And there it goes! Becoming a useful point in the line to specify that the section name defination is done! */
					*ptrEnd= '\0';

					/* Now we are all set up! We can allocate the memory needed to define a section, and allocate the memory for the comment if there was one! */
					POPINI_ST_SECTION *section= new_section(ptr);
					if(comment!=NULL){
						POPINI_ST_COMMENT *comm= new_comment(comment);
						comm->dilema= dilema;
						set_section_comment(section,comm);
					}
					curSection= section;
					/* Now just add the section to the bottom of the array inside of the inifile structure created in the first line! */
					add_section(ini,section,-1);
				}else{
					/* Assume, because the first character isnt a comment dilema character and an opening bracket character, that it is a key. */
					/* Values are defined with an =, so keys cant have =s. */
					printf("%d\n",*ptr);
					char *valPtr= char_in_string(ptr,'=');
					if(valPtr==NULL){
						free_ini_file(ini);
						free(line);
						ini= NULL;
						break;
					}
					/* Now, we need to seperate the key and the value, so we need to set the pointer back one, incase there is whitespace. */
					valPtr--;
					/* Keep moving pointer back until the character is no longer a whitespace. This will go as far back as until the first character, if no key is defined, it will still work, just not as expected... */
					while(char_is_whitespace(*valPtr) && valPtr!=ptr){
						valPtr--;
					}
					/* Because the pointer will point to a non whitespace character, we need to push it up one so its back on a whitespace. Then we set it to a terminating string, as the key ends here. Then push up one again, as the value itll point to =s a termination char, and that cant be. */
					valPtr++;
					*valPtr= '\0';
					valPtr++;
					/* Skip whitespaces now! */
					valPtr= skip_whitespaces(valPtr);
					/* If our value =s an `=` then we need to push it, as that was the dilema that defined it was a string, as a result, if you dont define your value with a quote, then you cant have an = as your first character. */
					if(*valPtr=='='){
						valPtr++;
					}
					/* Skip whitespaces so we can reach the first character of the value! */
					valPtr= skip_whitespaces(valPtr);

					/* Now we have the key, we can allocate memory for its struct! */
					POPINI_ST_KEY *key= new_key(ptr);

					/* Check if this value is a table, if so we will need to loop through each of the chars, otherwise we can easily retrieve the value. */
					if(*valPtr=='{'){
						/* Alright, this is a table, time to do some looping! */
						/* We cant do any guesses on when the table ends, atleast if we want to allow `}` characters inside of comments... */
						/* to keep our original pointer safe... */
						char *vPtr= valPtr;
						vPtr++;
						do{
							/* Do some checks, if it is a quote or not. */
							if(char_is_quote(*vPtr)){
								/* Ok, so now we know it is a quote, we save it in a char variable, then we increment the pointer by one. */
								char quote= *vPtr;
								vPtr++;
								/* get the closing quote... Once again, if it is escaped, skip that bad boy! */
								char *vPtrEnd= char_in_string_not_escaped(vPtr,quote);
								if(vPtrEnd==NULL){
									free_ini_file(ini);
									free(line);
									ini= NULL;
									return NULL;
								}
								*vPtrEnd= '\0';

								/* now we can add it. */
								add_value(key,vPtr,-1);
								vPtr= vPtrEnd+1;
								if(*vPtr=='}'){
									break;
								}
							}else if(*vPtr!=' '){
								/* A little like retreiving the quote version. but we loop through char until we reach space (which is the dilema for a new value). */
								char *vPtrEnd= vPtr;
								while(*vPtrEnd!='\0' && *vPtrEnd!='}' && *vPtrEnd!=' '){
									vPtrEnd++;
								}
								/* Whew, is it the end of the table, yes? Well then we may need to remember that for the future... */
								char isEnd= 0;
								if(*vPtrEnd=='}'){
									isEnd= 1;
								}
								*(vPtrEnd)= '\0';

								/* now we can add it. */
								add_value(key,vPtr,-1);
								vPtr= vPtrEnd;
								if(isEnd){
									break;
								}
							}
							vPtr++;
						}while(*vPtr!='\0' && *vPtr!='}');
						if(*vPtr=='\0'){
							vPtr++;
							if(*vPtr=='\0'){
								free_ini_file(ini);
								free(line);
								ini= NULL;
								break;
							}
						}
						char *valEndPtr= char_in_string_list(vPtr,H_FILE_POPINI_DILEMACHARS);
						if(valEndPtr!=NULL){
							POPINI_ST_COMMENT *comm= new_comment((valEndPtr+1));
							comm->dilema= *valEndPtr;
							set_key_comment(key,comm);
						}
					}else{
						/* Well, this is as easy as... oh wait quotes are a thing... */
						/* Alright! So now that we know quotes can contain the values, what we do is check if the first character is a quote character, if so then find the end quote (that isnt escaped with a backslash), If we failed to find it, then that seems wrong, so free up memory, and break off. */
						char *valEndPtr= valPtr;
						if(char_is_quote(*valPtr)){
							valPtr++;
							char quote= *valEndPtr;
							valEndPtr++;
							valEndPtr= char_in_string_not_escaped(valEndPtr,quote);
							if(valEndPtr==NULL){
								free_ini_file(ini);
								free(line);
								ini= NULL;
								break;
							}
							*valEndPtr= '\0';
							valEndPtr++;
						}
						/* Alright, now it is safe to find the comment! remember, if the value isnt quoted, the comment will be the first dilema character. An almost copy of the code for retrieve section comments aswell.. */
						valEndPtr= char_in_string_list(valEndPtr,H_FILE_POPINI_DILEMACHARS);
						char dilema= '\0';
						if(valEndPtr!=NULL){
							dilema= *valEndPtr;
							valEndPtr++;
						}

						add_value(key,valPtr,-1);
						if(valEndPtr!=NULL){
							POPINI_ST_COMMENT *comm= new_comment(valEndPtr);
							comm->dilema= dilema;
							set_key_comment(key,comm);
						}
					}
					if(curSection!=NULL){
						add_key(curSection,key,-1);
					}else{
						add_unsectioned_key(ini,key,-1);
					}
				}
				/* Increment the `lineNum` vairable, which holds the line we are in (excluding empty newlines). */
				lineNum++;
			}
			/* Free the memory taken up via the line */
			free(line);
		}
		fclose(read);
	}
	return ini;
}

/* Creates an inifile structure, the passed file string is what file it will output to if you call POPINI_FU_WRITE_INIFILE on it, the file wont be read, so if you want the sections,keys, and comments to save, you would use POPINI_FU_PARSE_INIFILE. */
POPINI_ST_INIFILE *POPINI_FU_NEW_INIFILE(const char *file){
	POPINI_ST_INIFILE *ini= new_ini_file(file);
	return ini;
}

/* Writes the data inside of the inifile structure passed to the inifile structure's file key. */
void POPINI_FU_WRITE_INIFILE(POPINI_ST_INIFILE *ini){
	/* Open our file, then check if it opened it in write mode successfully. */
	FILE *write= fopen(ini->file,"w");

	if(write!=NULL){
		/* Set lineNum so we can keep track of lines written, as `newline` comments relies on this. */
		long unsigned lineNum=0;
		/* Check if the INIFILE structure has any `unsectioned` keys. */
		if(ini->keys!=NULL){
			/* Set i to loop through all of the keys. */
			long unsigned i;
			for(i=0;i<=ini->keyLen;i++){
				/* write the newline comments on our linenum and also add the value it returns (which is the amount of lines it wrote) to linenum */
				lineNum+= write_comment_newline(ini,write,lineNum);
				/* Check if the key isnt NULL */
				if(ini->keys[i]!=NULL){
					/* Do key preset, which holds the key, and the =, after this we need to check if there is multiple values inside of the key, or just one. */
					/* Because users can be idiots, we need to check if there are even values. */
					if(ini->keys[i]->values==NULL){ continue; }

					fprintf(write,"%s = ",ini->keys[i]->key);
					if(ini->keys[i]->valLen<=0 && ini->keys[i]->values!=NULL && ini->keys[i]->values[0]!=NULL){
						/* Ez pz for writing a one value key! What we do is escape any quotes written, as if we didnt the value may very well break, quotes used is the first letter inside of the macro defined in one of the first lines of this c file. */
						char *newEscaped= char_in_string_escape(ini->keys[i]->values[0],*H_FILE_POPINI_QUOTECHARS);
						/* Add that baby! */
						fprintf(write,"%c%s%c",*H_FILE_POPINI_QUOTECHARS,newEscaped,*H_FILE_POPINI_QUOTECHARS);
						/* Free up memory used by `newEscaped` as it was dynamically allocated. */
						free(newEscaped);
					}else{
						/* Oof, we got ourselves a table. Write the table definition of {. */
						fprintf(write,"{");
						long unsigned j;
						/* Loop through all of the values */
						for(j=0;j<=ini->keys[i]->valLen;j++){
							if(ini->keys[i]->values[j]!=NULL){
								/* Now we do what we did for a one value key! */
								char *newEscaped= char_in_string_escape(ini->keys[i]->values[j],*H_FILE_POPINI_QUOTECHARS);
								fprintf(write,"%c%s%c",*H_FILE_POPINI_QUOTECHARS,newEscaped,*H_FILE_POPINI_QUOTECHARS);
								if(j<ini->keys[i]->valLen){
									fprintf(write," ");
								}
								free(newEscaped);
							}
						}
						/* Close off our table. */
						fprintf(write,"}");
					}
					if(ini->keys[i]->comment!=NULL){
						/* This key had a comment! Add it now... */
						fprintf(write," %c%s",ini->keys[i]->comment->dilema,ini->keys[i]->comment->str);
					}
					/* Now we just need a newline, and boom key written! */
					fprintf(write,"\n");
					lineNum++;
				}
			}
		}
		/* Ooh... does the INIFILE structure have sections? */
		if(ini->sections!=NULL){
			long unsigned i;
			/* Loop through the sections */
			for(i=0;i<=ini->secLen;i++){
				/* Same as used before, writes newline comments. */
				lineNum+= write_comment_newline(ini,write,lineNum);
				if(ini->sections[i]!=NULL){
					/* Alright, sections need some organization, so place a new line before it, this adds a nice empty newline before section definition! */
					fprintf(write,"\n[%s]",ini->sections[i]->name);
					/* Oh boy, we got a section comment? Write that now! */
					if(ini->sections[i]->comment!=NULL){
						fprintf(write," %c%s",ini->sections[i]->comment->dilema,ini->sections[i]->comment->str);
					}
					/* Section offictial done being written, add a newline char and add one to lineNum. */
					fprintf(write,"\n");
					lineNum++;
					/* Wow, this section even had keys! */
					if(ini->sections[i]->keys!=NULL){
						long unsigned j;
						/* Loop through all of the keys inside of the section. */
						for(j=0;j<=ini->sections[i]->keyLen;j++){
							/* An almost exact replica as to writing `unsectioned` keys! */
							lineNum+= write_comment_newline(ini,write,lineNum);
							if(ini->sections[i]->keys[j]!=NULL){
								/* Idiot check. */
								if(ini->sections[i]->keys[j]->values==NULL){ continue; }

								/* Print our first half, which conain the key string. */
								fprintf(write,"%s = ",ini->sections[i]->keys[j]->key);
								/* Check if there is just one value, if so, then we can just write the one value as a normal string. */
								if(ini->sections[i]->keys[j]->valLen<=0 && ini->sections[i]->keys[j]->values!=NULL && ini->sections[i]->keys[j]->values[0]!=NULL){
									char *newEscaped= char_in_string_escape(ini->sections[i]->keys[j]->values[0],'"');
									fprintf(write,"\"%s\"",newEscaped);
									free(newEscaped);
								}else{
									/* Well then, we got that table... */
									fprintf(write,"{");
									long unsigned o;
									for(o=0;o<=ini->sections[i]->keys[j]->valLen;o++){
										if(ini->sections[i]->keys[j]->values[o]!=NULL){
											char *newEscaped= char_in_string_escape(ini->sections[i]->keys[j]->values[o],'"');
											fprintf(write,"\"%s\"",newEscaped);
											if(o<ini->sections[i]->keys[j]->valLen){
												fprintf(write," ");
											}
											free(newEscaped);
										}
									}
									fprintf(write,"}");
								}
								/* Comment, add it... */
								if(ini->sections[i]->keys[j]->comment!=NULL){
									fprintf(write," %c%s",ini->sections[i]->keys[j]->comment->dilema,ini->sections[i]->keys[j]->comment->str);
								}
								/* Back to the top again! */
								fprintf(write,"\n");
							}
							lineNum++;
						}
					}
				}
			}
		}
		/* close our file, it no longer needs to hog our memory. */
		fclose(write);
	}
}

/* Creates a new section structure inside of a inifile structure */
POPINI_ST_SECTION *POPINI_FU_NEW_SECTION(POPINI_ST_INIFILE *ini,char *name,long unsigned pos){
	POPINI_ST_SECTION *section= new_section(name);
	add_section(ini,section,pos);
	return section;
}

/* Creates a new unsectioned key. */
POPINI_ST_KEY *POPINI_FU_NEW_UNSECTIONED_KEY(POPINI_ST_INIFILE *ini,char *key,long unsigned pos){
	POPINI_ST_KEY *keyy= new_key(key);
	add_unsectioned_key(ini,keyy,pos);
	return keyy;
}

/* Creates a new sectioned key. */
POPINI_ST_KEY *POPINI_FU_NEW_KEY(POPINI_ST_SECTION *section,char *key,long unsigned pos){
	POPINI_ST_KEY *keyy= new_key(key);
	add_key(section,keyy,pos);
	return keyy;
}

/* Adds a new value to a key structure. */
void POPINI_FU_NEW_VALUE(POPINI_ST_KEY *key,char *value,long unsigned pos){
	add_value(key,value,pos);
}

/* Sets a key structure's comment. */
POPINI_ST_COMMENT *POPINI_FU_SET_KEY_COMMENT(POPINI_ST_KEY *key,char *comm){
	POPINI_ST_COMMENT *comment= new_comment(comm);
	set_key_comment(key,comment);
	return comment;
}

/* Sets a section structure's comment. */
POPINI_ST_COMMENT *POPINI_FU_SET_SECTION_COMMENT(POPINI_ST_SECTION *section,char *comm){
	POPINI_ST_COMMENT *comment= new_comment(comm);
	set_section_comment(section,comment);
	return comment;
}

/* Adds a newline comment to the inifile structure at position `pos` */
POPINI_ST_COMMENT *POPINI_FU_NEW_NEWLINE_COMMENT(POPINI_ST_INIFILE *ini,char *comm,long unsigned pos){
	POPINI_ST_COMMENT *comment= new_comment(comm);
	add_inifile_comment(ini,comment,pos);
	return comment;
}

/* Free up the memory hogged by an INIFILE structure. It's recommended that you dont use the pointer again after it has been freed. */
void POPINI_FU_FREE_INIFILE(POPINI_ST_INIFILE *ini){
	free_ini_file(ini);
	free(ini);
}

/* Retrieves a section from the position passed. */
POPINI_ST_SECTION *POPINI_FU_SECTION_FROM_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos){
	if(ini->sections!=NULL){
		if(pos>ini->secLen){
			pos= ini->secLen;
		}
		return ini->sections[pos];
	}
	return NULL;
}

/* Retrieves a section from its name, the number passed is the `number` the section is (default is 0), as sections can have same names, this number dictates how many of the same named sections to skip, before returning (if there are no other sections, but the skips havent been fulfilled, will return last section with the name). */
POPINI_ST_SECTION *POPINI_FU_SECTION_FROM_NAME(POPINI_ST_INIFILE *ini,char *name,long unsigned skip){
	POPINI_ST_SECTION *curSection= NULL;
	long unsigned i=0;
	if(ini->sections==NULL){
		return NULL;
	}
	if(skip==0){
		skip= 1;
	}
	long unsigned size= strlen(name);
	while(i<=ini->secLen && (skip>0 && curSection==NULL)){
		if(ini->sections[i]!=NULL){
			if(strncmp(ini->sections[i]->name,name,size)==0){
				if(skip>0){
					skip--;
				}
				curSection= ini->sections[i];
			}
		}
		i++;
	}
	return curSection;
}

/* Removes the value at position passed inside of key. */
void POPINI_FU_REMOVE_VALUE_POSITION(POPINI_ST_KEY *key,long unsigned pos){
	if(key->values!=NULL){
		if(pos>key->valLen){
			pos= key->valLen;
		}
		remove_value(key,pos);
	}
}

/* Removes the key at the position passed inside of section. */
void POPINI_FU_REMOVE_SECTIONED_KEY_POSITION(POPINI_ST_SECTION *section,long unsigned pos){
	if(section->keys!=NULL){
		if(pos>section->keyLen){
			pos= section->keyLen;
		}
		remove_sectioned_key(section,pos);
	}
}

/* Removes the key at the position  passed inside of an inifile structure. */
void POPINI_FU_REMOVE_UNSECTIONED_KEY_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos){
	if(ini->keys!=NULL){
		if(pos>ini->keyLen){
			pos= ini->keyLen;
		}
		remove_unsectioned_key(ini,pos);
	}
}

/* Unset the key's comment */
void POPINI_FU_UNSET_KEY_COMMENT(POPINI_ST_KEY *key){
	if(key->comment!=NULL){
		free_comment(key->comment);
		free(key->comment);
		key->comment= NULL;
	}
}

/* Unset the section's comment */
void POPINI_FU_UNSET_SECTION_COMMENT(POPINI_ST_SECTION *sec){
	if(sec->comment!=NULL){
		free_comment(sec->comment);
		free(sec->comment);
		sec->comment= NULL;
	}
}

/* Removes a newline comment inside of an inifile */
void POPINI_FU_REMOVE_NEWLINE_COMMENT_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos){
	if(ini->comments!=NULL){
		if(pos>ini->comLen){
			pos= ini->comLen;
		}
		remove_newline_comment(ini,pos);
	}
}

/* Removes an unsectioned key inside of an infile, with a key structure pointer instead of a position. */
void POPINI_FU_REMOVE_UNSECTIONED_KEY(POPINI_ST_INIFILE *ini,POPINI_ST_KEY *key){
	if(ini->keys!=NULL){
		long unsigned posF;
		char found= 0;
		for(posF=0;posF<=ini->keyLen;posF++){
			if(ini->keys[posF]!=NULL && ini->keys[posF]==key){
				found= 1;
				break;
			}
		}
		if(found){
			POPINI_FU_REMOVE_UNSECTIONED_KEY_POSITION(ini,posF);
		}
	}
}

/* Removes a newline comment inside of an inifile, with a comment structure pointer instead of a position. */
void POPINI_FU_REMOVE_NEWLINE_COMMENT(POPINI_ST_INIFILE *ini,POPINI_ST_COMMENT *comm){
	if(ini->comments!=NULL){
		long unsigned posF;
		char found= 0;
		for(posF=0;posF<=ini->comLen;posF++){
			if(ini->comments[posF]!=NULL && ini->comments[posF]==comm){
				found= 1;
				break;
			}
		}
		if(found){
			POPINI_FU_REMOVE_NEWLINE_COMMENT_POSITION(ini,posF);
		}
	}
}

/* Removes a sectioned key inside of a section, with a key structure pointer instead of a position. */
void POPINI_FU_REMOVE_SECTIONED_KEY(POPINI_ST_SECTION *sec,POPINI_ST_KEY *key){
	if(sec->keys!=NULL){
		long unsigned posF;
		char found= 0;
		for(posF=0;posF<=sec->keyLen;posF++){
			if(sec->keys[posF]!=NULL && sec->keys[posF]==key){
				found= 1;
				break;
			}
		}
		if(found){
			POPINI_FU_REMOVE_SECTIONED_KEY_POSITION(sec,posF);
		}
	}
}

/* Retrieves key from section at position */
POPINI_ST_KEY *POPINI_FU_SECTIONED_KEY_FROM_POSITION(POPINI_ST_SECTION *sec,long unsigned pos){
	if(sec->keys!=NULL){
		if(pos>sec->keyLen){
			pos= sec->keyLen;
		}
		return sec->keys[pos];
	}
	return NULL;
}

/* Retrieves a key from its name, the number passed is the `number` the key is (default is 0), as keys can have same names, this number dictates how many of the same named keys to skip, before returning (if there are no other keys, but the skips havent been fulfilled, will return last key with the name). */
POPINI_ST_KEY *POPINI_FU_SECTIONED_KEY_FROM_KEYNAME(POPINI_ST_SECTION *sec,char *key,long unsigned skip){
	POPINI_ST_KEY *curKey= NULL;
	long unsigned i=0;
	if(sec->keys==NULL){
		return NULL;
	}
	if(skip==0){
		skip= 1;
	}
	long unsigned size= strlen(key);
	while(i<=sec->keyLen && (skip>0 && curKey==NULL)){
		if(sec->keys[i]!=NULL){
			if(strncmp(sec->keys[i]->key,key,size)==0){
				if(skip>0){
					skip--;
				}
				curKey= sec->keys[i];
			}
		}
		i++;
	}
	return curKey;
}

/* Retrieves key from inifile at position */
POPINI_ST_KEY *POPINI_FU_UNSECTIONED_KEY_FROM_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos){
	if(ini->keys!=NULL){
		if(pos>ini->keyLen){
			pos= ini->keyLen;
		}
		return ini->keys[pos];
	}
	return NULL;
}

/* Retrieves a key from its name, the number passed is the `number` the key is (default is 0), as keys can have same names, this number dictates how many of the same named keys to skip, before returning (if there are no other keys, but the skips havent been fulfilled, will return last key with the name). */
POPINI_ST_KEY *POPINI_FU_UNSECTIONED_KEY_FROM_KEYNAME(POPINI_ST_INIFILE *ini,char *key,long unsigned skip){
	POPINI_ST_KEY *curKey= NULL;
	long unsigned i=0;
	if(ini->keys==NULL){
		return NULL;
	}
	if(skip==0){
		skip= 1;
	}
	long unsigned size= strlen(key);
	while(i<=ini->keyLen && (skip>0 && curKey==NULL)){
		if(ini->keys[i]!=NULL){
			if(strncmp(ini->keys[i]->key,key,size)==0){
				if(skip>0){
					skip--;
				}
				curKey= ini->keys[i];
			}
		}
		i++;
	}
	return curKey;
}

void POPINI_FU_REMOVE_SECTION_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos){
	if(ini->sections!=NULL){
		if(pos>ini->secLen){
			pos= ini->secLen;
		}
		remove_section(ini,pos);
	}
}

/* Removes a section via its name. */
void POPINI_FU_REMOVE_SECTION(POPINI_ST_INIFILE *ini,POPINI_ST_SECTION *sec){
	if(ini->sections!=NULL){
		long unsigned posF;
		char found= 0;
		for(posF=0;posF<=ini->secLen;posF++){
			if(ini->sections[posF]!=NULL && ini->sections[posF]==sec){
				found= 1;
				break;
			}
		}
		if(found){
			POPINI_FU_REMOVE_SECTION_POSITION(ini,posF);
		}
	}
}