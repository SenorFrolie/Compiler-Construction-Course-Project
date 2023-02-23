#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "strtab.h"


/* Provided is a hash function that you may call to get an integer back. */
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int ST_insert(char *id, char *scope, int data_type, int symbol_type){
    
//	printf("Top of ST_insert\n");

	// TODO: Concatenate the scope and id and use that to create the hash key
	int key = makeKey(id,scope);
    // TODO: Use ST_lookup to check if the id is already in the symbol table. If yes, ST_lookup will return an index that is not -1. if index != -1, that means the variable is already in the hashtable. Hence, no need to insert that variable again. However, if index == -1, then use linear probing to find an empty spot and insert there. Then return that index.
   
	
	if(!strcmp(strTable[key].id, "")){
		strcpy(strTable[key].id, id);
		strcpy(strTable[key].scope,scope);
		strTable[key].data_type = data_type;
		strTable[key].symbol_type = symbol_type;
		return key;
	}

	while(strcmp(strTable[key].id,"")){
		key++;
		if(!strcmp(strTable[key].id,"")){
			strcpy(strTable[key].id, id);
			strcpy(strTable[key].scope, scope);
			strTable[key].data_type = data_type;
			strTable[key].symbol_type = symbol_type;
			return key;
		}	
	}
	return key;
}

int ST_lookup(char *id, char *scope) {
    // TODO: Concatenate the scope and id and use that to create the hash key
	int key = makeKey(id,scope);
	int found = search(id,scope);
    // TODO: Use the hash value to check if the index position has the "id". If not, keep looking for id until you find an empty spot. If you find "id", return that index. If you arrive at an empty spot, that means "id" is not there. Then return -1. 
	if(found != -1){
		return found;
	}
	else{
		found = search(id, "global");
		return found;
	}
	//return -1;
}

int search(char *id, char *scope){
	int start = 0;
	int key = makeKey(id,scope);

	if(!strcmp(strTable[key].id,id) && !strcmp(strTable[key].scope,scope)){
		return key;
	}

	start = key;
	key = (key+1) % MAXIDS;

	while(key != start){
		if(!strcmp(strTable[key].id,id) && !strcmp(strTable[key].scope,scope)){
			return key;
		}
		key = (key+1) % MAXIDS;
	}	
	return -1;
}

int makeKey(char *id, char *scope){
	char *newStr = (char *) malloc(strlen(scope) + strlen(id) + 1);
	strcpy(newStr, scope);
	strcat(newStr,id);
	int key = hash(newStr) % MAXIDS;
	return key;

}
void output_entry(int i){
//    printf("%d: %s ", i, types[strTable[i].data_type]);
//   printf("%s:%s%s\n", strTable[i].scope, strTable[i].id, symTypeMod[strTable[i].symbol_type]);
}

