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

# AWAITING TO ADD MORE DOCS FOR THE LIBRARY FUNCTIONS...