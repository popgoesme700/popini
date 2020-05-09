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
#include <stdio.h>
#include <popini.h>
#include <stdlib.h>
#include <string.h>

int main(int argCount,char *args[]){
	/* Specifies our inifile structure, which holds all the parsed data returned from the parsing action. */
	POPINI_ST_INIFILE *ini= NULL;
	POPINI_ST_SECTION *sec= NULL;
	POPINI_ST_KEY *key= NULL;
	char ans[100]= {};
	while(strncmp(ans,"q",1)!=0 && strncmp(ans,"quit",4)!=0){
		printf("Enter an action (enter ? or help for a list of actions you can do): ");
		int nocare= scanf("%s",ans);
		if(strncmp(ans,"?",1)==0 || strncmp(ans,"help",4)==0){
			printf("Actions you can do:\n");
			printf("q or quit: Closes the program.\n");
			printf("? or help: Lists actions you can do.\n");
			printf("oi or openini: Opens an inifile, reading it aswell.\n");
			printf("ci or createini: Creates an inifile, if the file exists, it still wont read it.\n");
			if(ini!=NULL){
				printf("wi or writeini: Writes the inifile to file.\n");
				printf("cli or closeini: Closes the inifile.\n");
				printf("ns or newsection: Creates a new section structure.\n");
				printf("ss or selectsection: Select a section structure.\n");
				printf("ls or listsections: Lists all of the sections.\n");

				printf("lk or listkeys: Lists all keys, if a section selected, lists all section keys, otherwise unsectioned keys.\n");
				printf("nk or newkey: Creates a new key structure if you have a section selected, itll create the key inside of the section, otherwise itll make an unsectioned key.\n");
				printf("sk or selectkey: Select a key structure if you have a section selected, selects key inside of the section, otherwise selects unsectioned key.\n");

				printf("anc or addnewlinecomment: Adds a newline comment.\n");
				printf("dnc or deletenewlinecomment: Deletes a newline comment.\n");
				printf("lnc or listnewlinecomments: Lists all newline comments.\n");
			}
			if(sec!=NULL){
				printf("uss or unselectsection: Unselects the currently selected section.\n");
				printf("ds or deletesection: Delete the currently selected section.\n");
				printf("ssc or setsectioncomment: Sets the currently selected section's comment.\n");
			}
			if(key!=NULL){
				printf("usk or unselectkey: Unselects the currently selected key structure.\n");
				printf("dk or deletekey: Delete the currently selected key.\n");
				printf("av or addvalue: Adds a new value to the selected key.\n");
				printf("dv or deletevalue: Deletes a value in the selected key.\n");
				printf("lv or listvalues: Lists all of the selected key's values.\n");
				printf("skc or setkeycomment: Sets the currently selected key's comment.\n");
			}
			printf("\n");
		}else if(strncmp(ans,"oi",2)==0 || strncmp(ans,"openini",7)==0){
			if(ini!=NULL){
				POPINI_FU_FREE_INIFILE(ini);
				ini= NULL;
			}
			printf("Enter the file to open: ");
			nocare= scanf("%s",ans);
			printf("Opening file `%s`...\n",ans);
			ini= POPINI_FU_PARSE_INIFILE(ans);
			printf("Read the file! Do ? or help for more actions!\n\n");
		}else if(strncmp(ans,"ci",2)==0 || strncmp(ans,"createini",9)==0){
			if(ini!=NULL){
				POPINI_FU_FREE_INIFILE(ini);
				ini= NULL;
			}
			printf("Enter the file that it will write to if you call `wi`: ");
			nocare= scanf("%s",ans);
			ini= POPINI_FU_NEW_INIFILE(ans);
			printf("Created the inifile! Do ? or help for more actions!\n\n");
		}else if(ini!=NULL && (strncmp(ans,"cli",3)==0 || strncmp(ans,"closeini",8)==0)){
			POPINI_FU_FREE_INIFILE(ini);
			ini= NULL;
			printf("Freed up the memory used by the inifile open! Do ? or help for an updated action list.\n\n");
		}else if(ini!=NULL && (strncmp(ans,"wi",2)==0 || strncmp(ans,"writeini",8)==0)){
			POPINI_FU_WRITE_INIFILE(ini);
			printf("Wrote to the file %s!\n\n",ini->file);
		}else if(ini!=NULL && (strncmp(ans,"ns",2)==0 || strncmp(ans,"newsection",10)==0)){
			printf("Name this section: ");
			nocare= scanf("%s",ans);
			sec= POPINI_FU_NEW_SECTION(ini,ans,-1);
			key= NULL;
			printf("Section created and selected! Do ? or help for more actions!\n\n");
		}else if(sec!=NULL && (strncmp(ans,"uss",3)==0 || strncmp(ans,"unselectsection",15)==0)){
			sec= NULL;
			key= NULL;
			printf("Section unselected!\n\n");
		}else if(sec!=NULL && (strncmp(ans,"ds",2)==0 || strncmp(ans,"deletesection",13)==0)){
			POPINI_FU_REMOVE_SECTION(ini,sec);
			sec= NULL;
			key= NULL;
			printf("Section deleted and unselected!\n");
		}else if(sec!=NULL && (strncmp(ans,"ssc",3)==0 || strncmp(ans,"setsectioncomment",17)==0)){
			printf("Do you want to unset the comment? [Y/N] ");
			nocare= scanf("%s",ans);
			if(strncmp(ans,"y",1)==0 || strncmp(ans,"Y",1)==0){
				POPINI_FU_UNSET_SECTION_COMMENT(sec);
			}else{
				printf("Enter comment string: ");
				while(fgets(ans,100,stdin)){
					if(strcspn(ans,"\n")>=1){
						ans[strcspn(ans,"\n")]= '\0';
						break;
					}
				}
				POPINI_ST_COMMENT *comm= POPINI_FU_SET_SECTION_COMMENT(sec,ans);
				comm->dilema= ';';
			}
			printf("Successfully did action\n\n");
		}else if(ini!=NULL && (strncmp(ans,"ss",2)==0 || strncmp(ans,"selectsection",13)==0)){
			printf("Do you want to select the section by name? [Y/N] ");
			nocare= scanf("%s",ans);
			POPINI_ST_SECTION *oldSec= sec;
			if(strncmp(ans,"y",1)==0 || strncmp(ans,"Y",1)==0){
				printf("Enter the section name: ");
				nocare= scanf("%s",ans);
				sec= NULL;
				long unsigned skipp= 0;
				printf("Enter how many sections of the same name to skip: ");
				nocare= scanf("%lu",&skipp);
				printf("Finding section...\n");
				sec= POPINI_FU_SECTION_FROM_NAME(ini,ans,skipp);
				if(sec==NULL){
					printf("Failed to find section.\n");
					sec= oldSec;
				}else{
					printf("Section selected! Do ? or help for more actions!\n");
				}
			}else{
				printf("Enter the section position: ");
				long unsigned pos= 0;
				nocare= scanf("%lu",&pos);
				sec= NULL;
				sec= POPINI_FU_SECTION_FROM_POSITION(ini,pos);
				if(sec==NULL){
					printf("Failed to find section.\n");
					sec= oldSec;
					printf("Reset the section back to what it previously was.\n");
				}else{
					printf("Section selected! Do ? or help for more actions!\n");
				}
			}
		}else if((sec!=NULL || ini!=NULL) && (strncmp(ans,"nk",2)==0 || strncmp(ans,"newkey",6)==0)){
			printf("Enter a keyname: ");
			nocare= scanf("%s",ans);
			if(sec==NULL){
				key= POPINI_FU_NEW_UNSECTIONED_KEY(ini,ans,-1);
			}else{
				key= POPINI_FU_NEW_KEY(sec,ans,-1);
			}
			printf("Key made, and key selected! Do ? or help for more actions.\n\n");
		}else if(key!=NULL && (strncmp(ans,"skc",3)==0 || strncmp(ans,"setkeycomment",13)==0)){
			printf("Do you want to unset the comment? [Y/N] ");
			nocare= scanf("%s",ans);
			if(strncmp(ans,"y",1)==0 || strncmp(ans,"Y",1)==0){
				POPINI_FU_UNSET_KEY_COMMENT(key);
			}else{
				printf("Enter comment string: ");
				while(fgets(ans,100,stdin)){
					if(strcspn(ans,"\n")>=1){
						ans[strcspn(ans,"\n")]= '\0';
						break;
					}
				}
				POPINI_ST_COMMENT *comm= POPINI_FU_SET_KEY_COMMENT(key,ans);
				comm->dilema= ';';
			}
			printf("Successfully did action\n\n");
		}else if((sec!=NULL || ini!=NULL) && (strncmp(ans,"sk",2)==0 || strncmp(ans,"selectkey",9)==0)){
			printf("Do you want to retrieve the key by its keyname? [Y/N] ");
			nocare= scanf("%s",ans);
			POPINI_ST_KEY *oldKey= key;
			if(strncmp(ans,"y",1)==0 || strncmp(ans,"Y",1)==0){
				printf("Enter the keyname: ");
				nocare= scanf("%s",ans);
				long unsigned skipp= 0;
				printf("Enter how many keys of the same name to skip: ");
				nocare= scanf("%lu",&skipp);
				key= NULL;
				if(sec!=NULL){
					key= POPINI_FU_SECTIONED_KEY_FROM_KEYNAME(sec,ans,skipp);
				}else{
					key= POPINI_FU_UNSECTIONED_KEY_FROM_KEYNAME(ini,ans,skipp);
				}
				if(key==NULL){
					printf("Failed to retrieve key...\n\n");
				}else{
					printf("Key selected!\n\n");
				}
			}else{
				long unsigned skipp= 0;
				printf("Enter key position: ");
				nocare= scanf("%lu",&skipp);
				key= NULL;
				if(sec!=NULL){
					key= POPINI_FU_SECTIONED_KEY_FROM_POSITION(sec,skipp);
				}else{
					key= POPINI_FU_UNSECTIONED_KEY_FROM_POSITION(ini,skipp);
				}
				if(key==NULL){
					printf("Failed to retrieve key...\n\n");
				}else{
					printf("Key selected!\n\n");
				}
			}
		}else if(key!=NULL && (strncmp(ans,"av",2)==0 || strncmp(ans,"addvalue",8)==0)){
			printf("Enter value string: ");
			while(fgets(ans,100,stdin)){
				if(strcspn(ans,"\n")>=1){
					ans[strcspn(ans,"\n")]= '\0';
					break;
				}
			}
			POPINI_FU_NEW_VALUE(key,ans,-1);
			printf("Value added!\n\n");
		}else if(key!=NULL && (strncmp(ans,"dv",2)==0 || strncmp(ans,"deletevalue",11)==0)){
			printf("Enter the value position: ");
			long unsigned pos= 0;
			nocare= scanf("%lu",&pos);
			POPINI_FU_REMOVE_VALUE_POSITION(key,pos);
			printf("Value deleted!\n\n");
		}else if(key!=NULL && (strncmp(ans,"usk",3)==0 || strncmp(ans,"unselectkey",11)==0)){
			key= NULL;
			printf("Key unselected! Do ? or help for an updated list of actions!\n\n");
		}else if(key!=NULL && (strncmp(ans,"dk",2)==0 || strncmp(ans,"deletekey",9)==0)){
			if(sec==NULL){
				POPINI_FU_REMOVE_UNSECTIONED_KEY(ini,key);
				key= NULL;
			}else{
				POPINI_FU_REMOVE_SECTIONED_KEY(sec,key);
				key= NULL;
			}
			printf("Key deleted!\n\n");
		}else if(key!=NULL && (strncmp(ans,"lv",2)==0 || strncmp(ans,"listvalues",10)==0)){
			if(key->values!=NULL){
				printf("Listing values...\n\n");
				long unsigned nPos;
				for(nPos=0;nPos<=key->valLen;nPos++){
					if(key->values[nPos]!=NULL){
						printf("Value %lu: %s\n",nPos,key->values[nPos]);
					}
				}
				printf("\n");
			}else{
				printf("This key has no values! Make sure to add one, or else this key wont be written to the file when you do `wi`!\n");
			}
		}else if(ini!=NULL && (strncmp(ans,"ls",2)==0 || strncmp(ans,"listsections",12)==0)){
			if(ini->sections!=NULL){
				printf("Listing sections...\n\n");
				long unsigned nPos;
				for(nPos=0;nPos<=ini->secLen;nPos++){
					if(ini->sections[nPos]!=NULL){
						printf("Section %lu: %s\n",nPos,ini->sections[nPos]->name);
					}
				}
				printf("\n");
			}else{
				printf("There are no sections...\n\n");
			}
		}else if((ini!=NULL || sec!=NULL) && (strncmp(ans,"lk",2)==0 || strncmp(ans,"listkeys",8)==0)){
			if(sec==NULL){
				if(ini->keys!=NULL){
					printf("Listing unsectioned keys..\n\n");
					long unsigned nPos;
					for(nPos=0;nPos<=ini->keyLen;nPos++){
						if(ini->keys[nPos]!=NULL){
							printf("Key %lu: %s\n",nPos,ini->keys[nPos]->key);
						}
					}
					printf("\n");
				}else{
					printf("There are no unsectioned keys...\n\n");
				}
			}else{
				if(sec->keys!=NULL){
					printf("Listing sectioned keys...\n\n");
					long unsigned nPos;
					for(nPos=0;nPos<=sec->keyLen;nPos++){
						if(sec->keys[nPos]!=NULL){
							printf("Key %lu: %s\n",nPos,sec->keys[nPos]->key);
						}
					}
					printf("\n");
				}else{
					printf("There are no sectioned keys inside of the selected section...\n\n");
				}
			}
		}else if(ini!=NULL && (strncmp(ans,"anc",3)==0 || strncmp(ans,"addnewlinecomment",17)==0)){
			printf("Enter the comment string: ");
			while(fgets(ans,100,stdin)){
				if(strcspn(ans,"\n")>=1){
					ans[strcspn(ans,"\n")]= '\0';
					break;
				}
			}
			printf("Enter the line position: ");
			long unsigned pos= 0;
			nocare= scanf("%lu",&pos);
			POPINI_ST_COMMENT *comm= POPINI_FU_NEW_NEWLINE_COMMENT(ini,ans,-1);
			comm->dilema= ';';
			comm->line= pos;
			printf("Successfully Added newline comment!\n\n");
		}else if(ini!=NULL && (strncmp(ans,"dnc",3)==0 || strncmp(ans,"deletenewlinecomment",20)==0)){
			printf("Enter the position: ");
			long unsigned pos= 0;
			nocare= scanf("%lu",&pos);
			POPINI_FU_REMOVE_NEWLINE_COMMENT_POSITION(ini,pos);
			printf("Successfully Deleted newline comment!\n\n");
		}else if(ini!=NULL && (strncmp(ans,"lnc",3)==0 || strncmp(ans,"listnewlinecomments",18)==0)){
			if(ini->comments!=NULL){
				printf("Listing newline comments...\n\n");
				long unsigned nPos;
				for(nPos=0;nPos<=ini->comLen;nPos++){
					if(ini->comments[nPos]!=NULL){
						printf("Comment %lu: %s -> Line Position %lu -> Dilema %c\n",nPos,ini->comments[nPos]->str,ini->comments[nPos]->line,ini->comments[nPos]->dilema);
					}
				}
				printf("\n");
			}else{
				printf("No newline comments exists in this infile!\n\n");
			}
		}
	}

	/* Frees up the memory used by the INIFILE structure. */
	if(ini!=NULL){
		POPINI_FU_FREE_INIFILE(ini);
		ini= NULL;
	}
	return 0;
}