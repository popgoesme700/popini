# popini
A C ini file parser and writer. Parses ini files into structures that hold individual values, like sections, keys, and comments.
## Getting started
This library allows easy implementation for reading and writing ini configuration files... Allowing you direct access to the structures modified by the functions inside the script allow you to further control how you will read and write to the ini file specified! This library is coded in the C90 standard, to allow it to be compiled with almost any C compiler that uses the standard c header files, that include actions like fopen and fprintf.
## Usage
### Executable
If you are using the executable for testing and deminstration purposes, the following is how it works.
When you first execute the program, itll output the following `Enter an action (enter ? or help for a list of actions you can do):` Entering `?` or `help` will output a few "actions" you can run.
By default, the program has no inifile, section, or key selected. This means that the actions outputted when called `?` or `help` will be short, as you create keys and sections, this list will enlarge to show more actions you can do with the newly selection structures.
The first actions output contains the following:
```
Actions you can do:
q or quit: Closes the program.
? or help: Lists actions you can do.
oi or openini: Opens an inifile, reading it aswell.
ci or createini: Creates an inifile, if the file exists, it still wont read it.
```
The action `q` or `quit` just exits the loop and frees up any dynamically allocated memory and then closes the program, this is the recommend way of closing the program, but isnt enforced.
The action `?` or `help` lists every action you can do, which changes when you create/select sections, inifiles, and/or keys.
The action `oi` or `openini` Creates an inifile structure and selects it. If the file specified exists, itll parse it, in turn keeping its data.
The action `ci` or `createini` Creates an inifile structure and selects it. No matter if the file exists or not, the selected structure will always be empty.

After you selected an inifile structure by doing `oi` or `ci`, when you do `?` itll list more actions you can do... the output is below.
```
Actions you can do:
q or quit: Closes the program.
? or help: Lists actions you can do.
oi or openini: Opens an inifile, reading it aswell.
ci or createini: Creates an inifile, if the file exists, it still wont read it.
wi or writeini: Writes the inifile to file.
cli or closeini: Closes the inifile.
ns or newsection: Creates a new section structure.
ss or selectsection: Select a section structure.
ls or listsections: Lists all of the sections.
lk or listkeys: Lists all keys, if a section selected, lists all section keys, otherwise unsectioned keys.
nk or newkey: Creates a new key structure if you have a section selected, itll create the key inside of the section, otherwise itll make an unsectioned key.
sk or selectkey: Select a key structure if you have a section selected, selects key inside of the section, otherwise selects unsectioned key.
anc or addnewlinecomment: Adds a newline comment.
dnc or deletenewlinecomment: Deletes a newline comment.
lnc or listnewlinecomments: Lists all newline comments.
```
The default four actions do the same as before (with an added fact that it frees up the selected inifile before recreating it), but now we have a bunch more to go through.
The action `wi` or `writeini` writes the selected inifile structure to the file you specified when you created the structure.
The action `cli` or `closeini` frees up the memory of the currently selected inifile structure, then sets it's pointer to NULL, resulting in the inifile structure being unselected.
The action `ns` or `newsection` creates a new section structure inside of the inifile at the end of the sections array. You specify the name afterwards, without spaces.
The action `ss` or `selectsection` selects a section. Itll ask a question if you want to select it by name or position, if you select name, you enter the name of the section (no spaces), and then itll ask how many skips you want to do... skips specify that, if there are multiple sections of the same name, itll skip that many sections of the same name.
The action `ls` or `listsections` lists all of the sections contained inside of the inifile structure.
The action `lk` or `listkeys` lists all of the keys, either inside of the inifile structure, or if selected, the section structure.
The action `nk` or `newkey` creates a new key, either inside of the inifile structure, or if selected, the section structure.
The action `sk` or `selectkey` selects a key, somewhat similar to how you select sections, but can select a key inside of the inifile if no section is selected.
The action `anc` or `addnewlinecomment` adds a new newline comment inside of inifile structure. The newline comment will have you specify a line number, which is what line the comment will be placed at afterwards.
The action `dnc` or `deletenewlinecomment` deletes a newline comment by the specified position in the array of the inifile structure.
The action `lnc` or `listnewlinecomments` Lists all of the newline comments inside of the inifile structure array.

The actions when you have a key or section selected are self explainitory. Each have an unselect, a delete, and a set comment action, specified with a different letter. `s` for section and `k` for key. An example is `skc` for `setkeycomment`, while a sections version of this would be `ssc` or `setsectioncomment`. Unselcting a section or key means that the key or section pointers stored in a variable will be set to NULL. Deleting a section or key means that the key or section pointers will be freed up (and if needed, will shift the array positions), then set to NULL to make sure it is unselected. And setting a comment is as simple as ever to understand, when you call this, you will enter a string in which is the comment for that key or section.

Now that you understand the demonstrative executable program of the library's features, why dont we explain the library's functions?
### Library
#### Functions
All functions pertain `FU_` after `POPINI_`, to remember this, FU stands for `FUnction`.

First, before we mess with sections, keys, comments, and values, we need to create and/or read an inifile. This is very simple, and has two functions that do different stuff.
The function
```c
POPINI_ST_INIFILE *POPINI_FU_PARSE_INIFILE(const char *file);
```
Will create an inifile structure, return its pointer, and read the file passed if it exists (and apply all of the data in it to the structure).

The function
```c
POPINI_ST_INIFILE *POPINI_FU_NEW_INIFILE(const char *file);
```
Will create an inifile structure and return its pointer. It wont read the file passed if it exists, the file string passed is used to specify what file `POPINI_FU_WRITE_INIFILE` will write to.

After you save the pointer of the `POPINI_ST_INIFILE` structure, you can do some modifing actions to it.

The function
```c
void POPINI_FU_WRITE_INIFILE(POPINI_ST_INIFILE *ini);
```
Will write all of the structure `ini`'s data into the file that the structure has defined inside of the `file` key.

The function
```c
void POPINI_FU_FREE_INIFILE(POPINI_ST_INIFILE *ini);
```
Frees up all of the memory used by the structure `ini`, recommended to set any pointers pertaining to this structure to NULL, as it is undefined what would happen if you use the same memory after freeing it.

The function
```c
POPINI_ST_SECTION *POPINI_FU_SECTION_FROM_NAME(POPINI_ST_INIFILE *ini,char *name,long unsigned skip);
```
Will retrieve a section pointer inside of the INIFILE structure `ini`, that has teh name `name`. The argument `skip` pertains to the fact there can be multiple sections named the same name, the number passed to skip means itll skip these many sections (subtract one from the number, of course. so a skip of 1 means itll get the first section named `name`). If skip is larger then the amount of sections with the same name, itll return the last section with the name.

The function
```c
POPINI_ST_SECTION *POPINI_FU_SECTION_FROM_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos);
```
Will retrieve a section pointer inside of the INIFILE structure `ini`, that is at the position `pos`. If the value `pos` is greater then the amount of sections inside of the ini structure, itll clamp it to the amount of sections.

The function
```c
POPINI_ST_SECTION *POPINI_FU_NEW_SECTION(POPINI_ST_INIFILE *ini,char *name,long unsigned pos);
```
Creates a new section structure inside of the structure `ini` with the name `name` at the position (clamped) `pos`, then returns it's pointer.

The function
```c
POPINI_ST_KEY *POPINI_FU_NEW_UNSECTIONED_KEY(POPINI_ST_INIFILE *ini,char *key,long unsigned pos);
```
Creates a new 'unsectioned' key (which is a key that is placed at the top of the inifile, underneath no section), inside of the structure `ini` with the keyname of `key` at the position `pos` (clamped).

The function
```c
POPINI_ST_KEY *POPINI_FU_NEW_KEY(POPINI_ST_SECTION *section,char *key,long unsigned pos);
```
Creates a new 'sectioned' key (which is a key placed under a section), inside of the section strucutre `section` with the keyname `key` at the position `pos` (clamped).

The function
```c
void POPINI_FU_NEW_VALUE(POPINI_ST_KEY *key,char *value,long unsigned pos);
```
Will add a new value into the strucutre `key` with the string `value` at the position `pos` (clamped). There is no need to pass the pointer, as it is just a char array.

The function
```c
POPINI_ST_COMMENT *POPINI_FU_SET_KEY_COMMENT(POPINI_ST_KEY *key,char *comm);
```
Sets the comment on a key structure `key` with the string `comm`. No dilema is set, so its best to set the dilema character yourself with the passed pointer to the `POPINI_ST_COMMENT` structure.

The function
```c
POPINI_ST_COMMENT *POPINI_FU_SET_SECTION_COMMENT(POPINI_ST_SECTION *section,char *comm);
```
Like the function `POPINI_FU_SET_KEY_COMMENT`, this function does exactly the same, but for section structure `section` instead of a key structure.

The function
```c
POPINI_ST_COMMENT *POPINI_FU_NEW_NEWLINE_COMMENT(POPINI_ST_INIFILE *ini,char *comm,long unsigned pos);
```
Adds a newline comment into the inifile `ini` with the comment string of `comm` at the position in the array `pos` (clamped). Returns the comment structure, recommended to set dilema character, and set line position.

The function
```c
POPINI_ST_KEY *POPINI_FU_SECTIONED_KEY_FROM_POSITION(POPINI_ST_SECTION *sec,long unsigned pos);
```
Retrieves the key structure at the position `pos` inside of the section structure `sec`. The value `pos` is clamped.

The function
```c
POPINI_ST_KEY *POPINI_FU_SECTIONED_KEY_FROM_KEYNAME(POPINI_ST_SECTION *sec,char *key,long unsigned skip);
```
Retrieves the key structure by the key name `key` inside of the section structure `sec`, skipping `skip` of the same keys that have the same name, if skip is larger then the amount of keys with the same name, itll retrieve the last key in the section.

The function
```c
POPINI_ST_KEY *POPINI_FU_UNSECTIONED_KEY_FROM_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos);
```
An almost exact replica to `POPINI_FU_SECTIONED_KEY_FROM_POSITION`, but instead of retrieving the key from a section structure, it retrieves it from the inifile structure `ini`.

The function
```c
POPINI_ST_KEY *POPINI_FU_UNSECTIONED_KEY_FROM_KEYNAME(POPINI_ST_INIFILE *ini,char *key,long unsigned skip);
```
An almost exact replic to `POPINI_FU_SECTIONED_KEY_FROM_KEYNAME`, but instead of retrieving the key from a section structure, it retrieves it from the inifile structure `ini`.

The function
```c
void POPINI_FU_REMOVE_UNSECTIONED_KEY_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos);
```
Removes a key structure from the inifile structure `ini` at the position `pos` (clamped). It frees up any memory used by the key structure, so do ensure you dont use the pointer the key structure had.

The function
```c
void POPINI_FU_REMOVE_SECTIONED_KEY_POSITION(POPINI_ST_SECTION *section,long unsigned pos);
```
Removes a key structure from the section structure `section` at the position `pos` (clamped). It frees up any memory used by the key structure, so do ensure you dont use the pointer the key structure had.

The function
```c
void POPINI_FU_REMOVE_NEWLINE_COMMENT_POSITION(POPINI_ST_INIFILE *ini,long unsigned pos);
```
Removes a newline comment from the inifile structure `ini` at the position `pos` (clamped). It frees up any memory used by the comment structure, so do ensure you dont use the pointer the comment structure had.

The function
```c
void POPINI_FU_UNSET_SECTION_COMMENT(POPINI_ST_SECTION *sec);
```
Unsets the comment structure (thus freeing its memory) for the section structure `sec`, do ensure to not touch the pointer again!

The function
```c
void POPINI_FU_UNSET_KEY_COMMENT(POPINI_ST_KEY *key);
```
Unsets the comment structure (thus freeing its memory) for the key structure `key`, do ensure to not touch the pointer again!

Onto some more helpful functions for removing items, instead of needing to know its position inside of the array, you can just pass its pointer!

The function
```c
void POPINI_FU_REMOVE_UNSECTIONED_KEY(POPINI_ST_INIFILE *ini,POPINI_ST_KEY *key);
```
Removes a key structure (thus freeing its memory) inside of the inifile structure `ini`, instead of a position, you pass a pointer to it, the program will loop through all of the `ini`s key structures until a pointer =s yours, then it calls `POPINI_FU_REMOVE_UNSECTIONED_KEY_FROM_POSITION` with the newfound position. If it cant find the pointer, nothing will happen. Dont use its pointer after removing it!

The function
```c
void POPINI_FU_REMOVE_NEWLINE_COMMENT(POPINI_ST_INIFILE *ini,POPINI_ST_COMMENT *comm);
```
Does almost the same as `POPINI_FU_REMOVE_UNSECTIONED_KEY`, but instead does it for a newline comment structure `comm`. Dont use its pointer after removing it!

The function
```c
void POPINI_FU_REMOVE_SECTIONED_KEY(POPINI_ST_SECTION *sec,POPINI_ST_KEY *key);
```
Does the same as `POPINI_FU_REMOVE_UNSECTIONED_KEY`, but instead finds it inside of the section structure `sec`. Dont use its pointer after removing it!

Comments are written inside of the header file `popini.h` and inside of the c file `popini.c`. They may help you or not. If you really need new methods, simply do it yourself. The structures passed by these functions include everything allowing free modifications.

#### Structures
ALL LONG UNSIGNED `*Len` KEYS STORE THE SIZE OF ARRAYS BASED ON 0 == 1 or 0 == 0 DEPENDING ON IF THE ARRAY IS NULL, AND INCREMENTS UPWARDS

The structure
```c
typedef struct{
	char *file;
	POPINI_ST_KEY **keys;
	POPINI_ST_SECTION **sections;
	POPINI_ST_COMMENT **comments;
	long unsigned comLen;
	long unsigned secLen;
	long unsigned keyLen;
} POPINI_ST_INIFILE;
```
Contains everything about the inifile needed, like its file it should write to inside of the key `file`, and an array of the unsectioned keys `keys` and an array of its sections `sections`, aswell as an array of its newline comments `comments` (do keep in mind, you do need to set their `line` key to a number, line numbers are based on its structure, so first every unsectioned key is a line, then section and its keys are lines, throughout this all, newline comments are counted as lines for every one added). The unsigned longs `comLen`, `secLen`, and `keyLen` are the size of the arrays, its long to allow for a lot of entries (but not unlimited, like how it should be). `comLen` stores the size of the `comments` array, `secLen` stores the size of the `sections` array, and `keyLen` stores the size of the `keys` array.

The structure
```c
typedef struct{
	char *name;
	POPINI_ST_KEY **keys;
	POPINI_ST_COMMENT *comment;
	long unsigned keyLen;
} POPINI_ST_SECTION;
```
Stores info about sections. like its name `name` and its comment `comment`. the array `keys` stores all of the keys the section has, and `keyLen` stores how many keys it has in the array.

The structure
```c
typedef struct{
	char *key;
	char **values;
	POPINI_ST_COMMENT *comment;
	long unsigned valLen;
} POPINI_ST_KEY;
```
Stores info about keys. Like its keyname `key` and its comment `comment`. The array `values` stores all of the value strings, while `valLen` stores how many values are in the array.

The structure
```c
typedef struct{
	char dilema;
	long unsigned line;
	char *str;
} POPINI_ST_COMMENT;
```
Stores info about comments. Like its dilema character `dilema` (which is the character to define it as a comment), or its line in the file (if its a newline comment, aka placed inside of the inifile structure) both `dilema` and `line` need to be set manually the comment structure is passed by a function, `line` only needs to be set if you want it on a certain line, and it is a newline comment, but `dilema` needs to always be set, or the comment will never be retrievable inside of the file.

## Building
If you want to build the library, and not the executable, then go into the main directory in which this readme is held, and use cmake, latest version is needed (although you can prob simply edit a line inside of the `CMakeLists.txt` file to support your version, if it supports all of the functions i used). Make a new directory, like build or something, and just do `cmake ..` or `cmake /path/to/your/clone` and then use make `make` in your directory, itll create a static library that you can attach to your main project.

If you want to build the executable, then head into the folder `test` from the main directory in which this readme is held. After that, you use cmake just as you would for the library, with the change that you will need to do `cmake /path/to/your/clone/test` instead if you didnt place the build folder inside of the test folder, once again i set min version to my version of cmake, which is pretty recent, but if your version supports all of the functions used inside of the `CMakeLists.txt` file, it is most likely safe.