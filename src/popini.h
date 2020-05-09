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
#ifndef H_FILE_POPINI
#define H_FILE_POPINI

/* The structure that holds data of comments. Which contain the line (only used on newline comments), the comment str and the comment's dilema (that specify the char that occurs before the comment's str) */
typedef struct{
	char dilema;
	long unsigned line;
	char *str;
} POPINI_ST_COMMENT;

/* The structure that holds data of keys. Which contain the key string, the 2D array of values, one holding the value position, and the holding the actual value string. a comment structure, which contains the keys comment, and the valLen which is how many values this key has. */
typedef struct{
	char *key;
	char **values;
	POPINI_ST_COMMENT *comment;
	long unsigned valLen;
} POPINI_ST_KEY;

/* The structure that holds data of sections. Which contain it's name, an array of key structures, a comment structure, and the number of keys inside of the section. */
typedef struct{
	char *name;
	POPINI_ST_KEY **keys;
	POPINI_ST_COMMENT *comment;
	long unsigned keyLen;
} POPINI_ST_SECTION;

/* The structure that holds data of the parsed inifile. Which contain the file string, an array of unsectioned keys (placed at the top of the file, before any section identifiers), an array of section structures, an array of comment structures (for newline comments), a number that holds how many newline comments are stored, a number that holds how many sections are stored, and a number for how many unsectioned keys are stored. */
typedef struct{
	char *file;
	POPINI_ST_KEY **keys;
	POPINI_ST_SECTION **sections;
	POPINI_ST_COMMENT **comments;
	long unsigned comLen;
	long unsigned secLen;
	long unsigned keyLen;
} POPINI_ST_INIFILE;

/* Creates an inifile structure, then reads and parses the file `file` if it exists. */
POPINI_ST_INIFILE *POPINI_FU_PARSE_INIFILE(const char *file);
/* Creates an inifile structure, does not read and parse the file `file`. */
POPINI_ST_INIFILE *POPINI_FU_NEW_INIFILE(const char *file);
/* Writes the inifile structure to an inifile */
void POPINI_FU_WRITE_INIFILE(POPINI_ST_INIFILE *ini);
/* Frees all the memory used by the inifile structure */
void POPINI_FU_FREE_INIFILE(POPINI_ST_INIFILE *ini);
/* Retrieves a section by its name, because you can have sections with similar names, a skip is also passed, which specifies how many sections of the same name to skip, before returning the section. */
POPINI_ST_SECTION *POPINI_FU_SECTION_FROM_NAME(POPINI_ST_INIFILE *ini,char *name,long unsigned skip);
/* Retrieves a section by its position inside of the inifile structure. */
POPINI_ST_SECTION *POPINI_FU_SECTION_FROM_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos);

/* Creates a new section inside of the inifile structure. */
POPINI_ST_SECTION *POPINI_FU_NEW_SECTION(POPINI_ST_INIFILE *ini,char *name,long unsigned pos);
/* Creates a key that is placed at the top of the inifile structure, this key has no section */
POPINI_ST_KEY *POPINI_FU_NEW_UNSECTIONED_KEY(POPINI_ST_INIFILE *ini,char *key,long unsigned pos);

/* Creates a key that is placed inside of the passed section. */
POPINI_ST_KEY *POPINI_FU_NEW_KEY(POPINI_ST_SECTION *section,char *key,long unsigned pos);
/* Creates a value string that is placed inside of the passed key. */
void POPINI_FU_NEW_VALUE(POPINI_ST_KEY *key,char *value,long unsigned pos);

/* Sets a keys comment to the passed string, afterwards you can set the comment dilema char. */
POPINI_ST_COMMENT *POPINI_FU_SET_KEY_COMMENT(POPINI_ST_KEY *key,char *comm);
/* Sets a sections comment to the passed string, afterwards you can set the comment dilema char. */
POPINI_ST_COMMENT *POPINI_FU_SET_SECTION_COMMENT(POPINI_ST_SECTION *section,char *comm);

/* Creates a new `newline` comment inside of the inifile structure, at the `pos`, keep in mind, all positional items are `maxxed`, so if you do -1 you will select the last position. */
POPINI_ST_COMMENT *POPINI_FU_NEW_NEWLINE_COMMENT(POPINI_ST_INIFILE *ini,char *comm,long unsigned pos);

/* Retrieves a key by its position inside of a section. */
POPINI_ST_KEY *POPINI_FU_SECTIONED_KEY_FROM_POSITION(POPINI_ST_SECTION *sec,long unsigned pos);
/* Retrieves a key by its key name inside of a section, because you can have keys with similar names, a skip is also passed, which specifies how many keys of the same name to skip, before returning the key. */
POPINI_ST_KEY *POPINI_FU_SECTIONED_KEY_FROM_KEYNAME(POPINI_ST_SECTION *sec,char *key,long unsigned skip);

/* Retrieves an unsectioned key by its position inside of an inifile */
POPINI_ST_KEY *POPINI_FU_UNSECTIONED_KEY_FROM_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos);
/* Retrieves an unsectioned key by its key name inside of an inifile, because you can have keys with similar names, a skip is also passed, which specifies how many keys of the same name to skip, before returning the key. */
POPINI_ST_KEY *POPINI_FU_UNSECTIONED_KEY_FROM_KEYNAME(POPINI_ST_INIFILE *ini,char *key,long unsigned skip);

/* Removes an unsectioned key from the inifile, at the position pos */
void POPINI_FU_REMOVE_UNSECTIONED_KEY_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos);
/* Removes a sectioned key from the inifile at the position pos */
void POPINI_FU_REMOVE_SECTIONED_KEY_POSITION(POPINI_ST_SECTION *section,long unsigned pos);

/* Removes a newline comment at the position pos */
void POPINI_FU_REMOVE_NEWLINE_COMMENT_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos);
/* Unsets a section's comment. */
void POPINI_FU_UNSET_SECTION_COMMENT(POPINI_ST_SECTION *sec);
/* Unsets a key's comment. */
void POPINI_FU_UNSET_KEY_COMMENT(POPINI_ST_KEY *key);

/* Removes an unsectioned key from the inifile, instead of passing a number, you pass the pointer to the key. This function will loop through the inifile unsectioned keys until it finds the position of this pointer in the array, then just call the function `POPINI_FU_REMOVE_UNSECTIONED_KEY_POSITION` and pass the position it found. */
void POPINI_FU_REMOVE_UNSECTIONED_KEY(POPINI_ST_INIFILE *ini,POPINI_ST_KEY *key);
/* Removes a newline commeny from the inifile, instead of passing a number, you pass a pointer to the comment structure. This function will loop through all of the inifile newline comments until it find the position this pointer is in the array, then just calls the function `POPINI_FU_REMOVE_NEWLINE_COMMENT_POSITION` and pass the position it was found. */
void POPINI_FU_REMOVE_NEWLINE_COMMENT(POPINI_ST_INIFILE *ini,POPINI_ST_COMMENT *comm);
/* Removes a sectioned key from the section, instead of passing a number, you pass the pointer to the key. This function will loop through the inifile unsectioned keys until it finds the position of this pointer in the array, then just call the function `POPINI_FU_REMOVE_SECTIONED_KEY_POSITION` and pass the position it found. */
void POPINI_FU_REMOVE_SECTIONED_KEY(POPINI_ST_SECTION *sec,POPINI_ST_KEY *key);

#endif