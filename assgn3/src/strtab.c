#include<stdio.h>
#include<string.h>
#include "strtab.h"

param *working_list_head = NULL;
param *working_list_end = NULL;
arg *arg_list_head = NULL;
arg *arg_list_end = NULL;
table_node* current_scope = NULL; 
table_node* symTableTreeHead = NULL; 

char* symbolTypes[3] = {"scalar", "array", "function"};
char* dataTypes[3] = {"int","char","void"};

/* Provided is a hash function that you may call to get an integer back. */
unsigned long hash(unsigned char *str)
{
    int hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int ST_insert(char *id, int data_type, int symbol_type, int* scope, int* alreadyExists){
    // TODO: Concatenate the scope and id and use that to create the hash key
    symEntry* newEntry = (symEntry*) malloc(sizeof(symEntry));
    int key = makeKey(id); 
    search(id, current_scope, alreadyExists);
    //printf("top of insert\n"); 
    //printf("current scope: %x\n", current_scope->strTable);
    //if empty
    if(current_scope->strTable[key] == NULL){
        strcpy(newEntry->id, id);
        newEntry->scope = *scope; 
        newEntry->data_type = data_type;
        newEntry->symbol_type = symbol_type;
        newEntry->order = 1;
        newEntry->size = 0; 
        if(current_scope->parent != NULL){
            newEntry->order = current_scope->parent->numChildren;
            //printf("ORDER IS GOING TO BE: %d\n", newEntry->order); 
        }
        current_scope->strTable[key] = newEntry; 
        return key; 
    }

    //if something
    while(current_scope->strTable[key] != NULL){
        key++;
        if(current_scope->strTable[key] == NULL){
            //printf("-------------------------------\n");
            strcpy(newEntry->id, id);
            newEntry->scope = *scope; 
            newEntry->data_type = data_type;
            newEntry->symbol_type = symbol_type;
            newEntry->order = 1;
            newEntry->size = 0; 
            if(current_scope->parent != NULL){
                newEntry->order = current_scope->parent->numChildren;
                //printf("ORDER IS GOING TO BE: %d\n", newEntry->order); 
            }
            current_scope->strTable[key] = newEntry; 
            //printf("-------------------------------\n");
            return key; 
        }
    }
    printf("here3\n"); 
}

symEntry* ST_lookup(char *id, int *keyRef) {
    // TODO: Concatenate the scope and id and use that to create the hash key
    // TODO: Use the hash value to check if the index position has the "id". If not, keep looking for id until you find an empty spot. If you find "id", return that index. If you arrive at an empty spot, that means "id" is not there. Then return -1. 
    table_node* thisScope = current_scope; 
    //printf("address of current scope: %x\n", thisScope);
    while(thisScope != NULL){
        //printf("iteration\n");
        symEntry* found = search(id, thisScope, keyRef); 

        if(*keyRef != -1){
            //printf("keyref was changed to %d\n", *keyRef);
        }
        if (found != NULL){
            return found; 
        }
        //printf("address of parent scope: %x\n", thisScope->parent);
        thisScope = thisScope->parent; 
    }

    return NULL; 
}

symEntry* search(char *id, table_node *scope, int *keyRef){
    int start = 0; 
    int key = makeKey(id);
    //printf("strTable[%d].id = %s\n", key, strTable[key].id);
    if(scope->strTable[key] != NULL){
        //printf("occupied here\n");
        if(!strcmp(scope->strTable[key]->id, id)){
            //printf("found, setting refKey\n");
            *keyRef = key;
            return scope->strTable[key]; 
        }
    }
    else{
        //printf("strTable[%d] wasnt occupied in scope = %x\n", key, scope->strTable);
    }

    start = key;
    key = (key + 1) % MAXIDS;
    //printf(" key: %d, address: %x\n",key, &strTable[key]); 
    //linear probing
    while(key != start){
        //printf("key: %d, address: %x\n",key, &strTable[key]); 
        if(scope->strTable[key] != NULL){
            if(!strcmp(scope->strTable[key]->id, id)){
                //printf("found, setting refKey\n");
                *keyRef = key;
                return scope->strTable[key]; 
            }
        }
        //printf("comparing %s and %s\n", strTable[key].id, id);  
        key = (key + 1) % MAXIDS;
    }

    return NULL; //not found
}


//uses the hash function to make a key and return it. Written by Dylan Morales
int makeKey(char *id){
    //char* str = (char*) malloc(strlen(scope) + strlen(id) + 1);
    //strcpy(str, scope);
    //strcat(str, id);
    int key = hash(id) % MAXIDS;
    //printf("Length of |%s| is |%d|\n", str, strlen(str)); 
    //printf("key: %d, id: %s\n", key, id); 
    return key;
}

void output_entry(int i){
    //printf("%d: %s ", i, types[strTable[i].data_type]);
    //printf("%s:%s%s\n", strTable[i].scope, strTable[i].id, symTypeMod[strTable[i].symbol_type]);
}

void new_scope(){
    table_node* newNode = (table_node*) malloc(sizeof(table_node)); 
    newNode->numChildren = 0; 
    newNode->parent = NULL; 
    newNode->first_child = NULL;
    newNode->last_child = NULL;
    newNode->next = NULL;
    //printf("\naddress of new scope is: %x\n\n", newNode); 
    if (current_scope == NULL){
        symTableTreeHead = newNode;
        current_scope = newNode; 
    }
    else{
        if(current_scope->first_child == NULL){
            current_scope->first_child = newNode;
        }
        else{
            current_scope->last_child->next = newNode; 
        }
        current_scope->last_child = newNode; 
        current_scope->numChildren++; 
        newNode->parent = current_scope; 
        current_scope = newNode; 
    }
}

void up_scope(){
    current_scope = current_scope->parent;
}

void add_param(int dataType, int symbolType){
    param* newParam = (param*) malloc(sizeof(param));
    newParam->data_type = dataType; 
    newParam->symbol_type = symbolType; 
    if(working_list_head == NULL){
        working_list_head = newParam; 
    }
    else{
        working_list_end->next = newParam; 
    }
    working_list_end = newParam; 
}

void connect_params(int i){
    //printf("key: %d\n", i);
    //printf("top\n");
    param* current = working_list_head; 
    int num_params = 0; 
    while(current != NULL){
        num_params++; 
        current = current->next; 
    }
    //printf("address %x\n", current_scope);
    //printf("address %x\n", current_scope->strTable);
    //printf("address %x\n", current_scope->strTable[i]->params);
    current_scope->strTable[i]->params = working_list_head; 
    //printf("middle\n");
    current_scope->strTable[i]->size = num_params; 
    working_list_head = NULL; 
    working_list_end = NULL; 
    //printf("bottom\n");
}

void add_arg(int dataType, int symbolType){
    arg* newArg = (arg*) malloc(sizeof(arg));
    newArg->data_type = dataType; 
    newArg->symbol_type = symbolType; 
    if(arg_list_head == NULL){
        arg_list_head = newArg; 
    }
    else{
        arg_list_end->next = newArg; 
    }
    arg_list_end = newArg; 
}

int count_args(){
    arg* current = arg_list_head; 
    int num_args = 0; 
    while(current != NULL){
        num_args++; 
        current = current->next; 
    }
    return num_args;
}

void clear_args(){
    arg_list_end = NULL; 
    arg_list_head = NULL;
}

char* get_symbol_id(int val, int scope, int order){
    //printf("\tKEY: %d, SCOPE: %d, ORDER: %d\n", val, scope, order);
    table_node* thisScope = current_scope;
    while(scope > 1){
        thisScope = thisScope->first_child; 
        scope--; 
    }
    while(order > 1){
        thisScope = thisScope->next; 
        order--;
    }
    return thisScope->strTable[val]->id;
}

//on the first call of this function (for the root parent of all nodes), you would pass it 0 for numsiblings and sibling count
void print_sym_tab(table_node* input, int nest) {
    /*for(int i = 0; i < nest; i++){
        printf("    ");
    }*/
    //print symbol table
    for(int i = 0; i < MAXIDS; i++){
        if(input->strTable[i] != NULL){
            param* paramPtr = input->strTable[i]->params;
            printf("id: %s\n", input->strTable[i]->id); 
            printf("\tscope: %d\n", input->strTable[i]->scope); 
            printf("\torder: %d\n", input->strTable[i]->order); 
            printf("\tdata_type: %s\n", dataTypes[input->strTable[i]->data_type]);
            printf("\tsymbol_type: %s\n", symbolTypes[input->strTable[i]->symbol_type]);  
            printf("\tsize: %d\n", input->strTable[i]->size);  
            while(paramPtr != NULL){
                printf("\tparam: data_type: %s, symbol_type: %s\n", dataTypes[paramPtr->data_type], symbolTypes[paramPtr->symbol_type]);  
                paramPtr = paramPtr->next;
            }
        }
    }

    if(input->next != NULL){
        print_sym_tab(input->next, nest + 1); 
    }
    
    if(input->first_child != NULL){
        print_sym_tab(input->first_child, nest + 1); 
    }
	
	
}
