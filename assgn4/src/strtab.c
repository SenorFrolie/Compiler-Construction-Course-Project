#include<stdio.h>
#include<string.h>
#include "strtab.h"

//initialize pointers to NULL
param *working_list_head = NULL;
param *working_list_end = NULL;
arg *argumentListHead = NULL;
arg *argumentListEnd = NULL;
table_node* currentScope = NULL; 
table_node* symbolTableHead = NULL; 

//for more readable outputs 
char* symbolTypes[3] = {"scalar", "array", "function"};
char* dataTypes[3] = {"int","char","void"};

/* Provided is a hash function that you may call to get an integer back. */
unsigned long hash(unsigned char *str){
    int hash = 5381; int c;
    while (c = *str++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

/* STInsert will attempt to insert an element into the symbol table, and return the hash-key where it is stored
 * PARAMETER 1: the id of the element
 * PARAMETER 2: the data type of the element
 * PARAMETER 3: the symbol type of the element
 * PARAMETER 4: the scope of the element
 * PARAMETER 5: a flag used in parser.y to check if the element already exists in the symbol table
 * RETURNS: the hash-key where the element is stored
 */
int STInsert(char *id, int data_type, int symbol_type, int* scope, int* alreadyExists){
    symEntry* newEntry = (symEntry*) malloc(sizeof(symEntry));
    int key = MakeKey(id); 
    Search(id, currentScope, alreadyExists);
    if(currentScope->strTable[key] == NULL){ //if strTable at index of key is empty
        strcpy(newEntry->id, id);
        newEntry->scope = *scope; 
        newEntry->data_type = data_type;
        newEntry->symbol_type = symbol_type;
        newEntry->childNumber = 1;
        newEntry->size = 0; 
        if(currentScope->parent != NULL){
            newEntry->childNumber = currentScope->parent->numChildren;
        }
        currentScope->strTable[key] = newEntry; 
        return key; 
    }
    while(currentScope->strTable[key] != NULL){ //if strTable at index of key is not empty (has a symEntry element)
        key++;
        if(currentScope->strTable[key] == NULL){
            strcpy(newEntry->id, id);
            newEntry->scope = *scope; 
            newEntry->data_type = data_type;
            newEntry->symbol_type = symbol_type;
            newEntry->childNumber = 1;
            newEntry->size = 0; 
            if(currentScope->parent != NULL){
                newEntry->childNumber = currentScope->parent->numChildren;
            }
            currentScope->strTable[key] = newEntry; 
            return key; 
        }
    }
}

/* STLookup will check ALL SCOPES if an element is stored in the symbol table tree, and return a pointer to the symEntry if it finds it
 * PARAMETER 1: the id of the element 
 * PARAMETER 2: a pointer to the hash-key of the element; will equal -1 if the element is not found in the symbol table tree
 * RETURNS: a pointer to the symEntry with a matching ID
 */
symEntry* STLookup(char *id, int *keyRef) {
    table_node* thisScope = currentScope; 
    while(thisScope != NULL){
        symEntry* found = Search(id, thisScope, keyRef); 
        if (found != NULL){
            return found; 
        }
        thisScope = thisScope->parent; 
    }
    return NULL; 
}

/* Search will check ONE LOCAL SCOPE if an element is stored in the symbol table of that local scope, and return a pointer to the ysmEntry if it finds it
 * PARAMETER 1: the id of the element
 * PARAMETER 2: the scope of the element
 * PARAMETER 3: a pointer to the hash-key of the element; will equal -1 if the element is not found in the local symbol table
 * RETURNS: a pointer to the symEntry with a matching ID
 */
symEntry* Search(char *id, table_node *scope, int *keyRef){
    int start = 0; 
    int key = MakeKey(id);
    if(scope->strTable[key] != NULL){
        if(!strcmp(scope->strTable[key]->id, id)){ //element was found
            *keyRef = key;
            return scope->strTable[key]; 
        }
    }
    start = key;
    key = (key + 1) % MAXIDS;
    while(key != start){ //linear probe to find unoccupied space
        if(scope->strTable[key] != NULL){
            if(!strcmp(scope->strTable[key]->id, id)){ //element was found
                *keyRef = key;
                return scope->strTable[key]; 
            }
        }
        key = (key + 1) % MAXIDS;
    }
    return NULL; //element was not found
}


/* wrapper for the hash function. MakeKey will create a hash-key given an id
 * PARAMETER 1: the id of the element
 * RETURNS: the hash-key
 */
int MakeKey(char *id){
    int key = hash(id) % MAXIDS;
    return key;
}

/* NewScope will create a new scope and change the current scope to be the new scope (move down one node in the symbol table tree)
 */
void NewScope(){
    table_node* newNode = (table_node*) malloc(sizeof(table_node)); 
    newNode->numChildren = 0; 
    newNode->parent = NULL; 
    newNode->first_child = NULL;
    newNode->last_child = NULL;
    newNode->next = NULL;
    if (currentScope == NULL){
        symbolTableHead = newNode;
        currentScope = newNode; 
    }
    else{
        if(currentScope->first_child == NULL){
            currentScope->first_child = newNode;
        }
        else{
            currentScope->last_child->next = newNode; 
        }
        currentScope->last_child = newNode; 
        currentScope->numChildren++; 
        newNode->parent = currentScope; 
        currentScope = newNode; 
    }
}

/* UpScope will change the current scope to be the parent scope (move up one node in the symbol table tree)
 */
void UpScope(){
    currentScope = currentScope->parent;
}

/* AddParam will create a new parameter to the linked-list of parameter and add it to the end of the list
 * PARAMETER 1: the data type of the parameter
 * PARAMETER 2: the symbol type of the parameter
 */
void AddParam(int dataType, int symbolType){
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

/* ConnectParams will connect the parameter linked-list to the appropriate function
 * PARAMETER 1: the hash-key of the function in the strTable of the current scope
 */
void ConnectParams(int i){
    param* current = working_list_head; 
    int num_params = 0; 
    while(current != NULL){
        num_params++; 
        current = current->next; 
    }
    currentScope->strTable[i]->params = working_list_head; 
    currentScope->strTable[i]->size = num_params; 
    working_list_head = NULL; //reset the param linked-list values
    working_list_end = NULL;  //to be used for another function
}

/* AddArgument will add arguments to the linked-list of arguments, much the same as AddParam
 * PARAMETER 1: the data type of the argument
 * PARAMETER 2: the symbol type of the argument
 */
void AddArgument(int dataType, int symbolType){
    arg* newArg = (arg*) malloc(sizeof(arg));
    newArg->data_type = dataType; 
    newArg->symbol_type = symbolType; 
    if(argumentListHead == NULL){
        argumentListHead = newArg; 
    }
    else{
        argumentListEnd->next = newArg; 
    }
    argumentListEnd = newArg; 
}

/* CountArguments will count the number of elements in the argument linked-list
 * RETURNS: the number of arguments stored in the linked-list
 */
int CountArguments(){
    arg* current = argumentListHead; 
    int num_args = 0; 
    while(current != NULL){
        num_args++; 
        current = current->next; 
    }
    return num_args;
}

/* ResetArguments will reset the argument linked-list pointers
 */
void ResetArguments(){
    argumentListEnd = NULL; 
    argumentListHead = NULL;
}

/* GetSymbolID gets the id of the desired symbol
 * PARAMETER 1: the hash-key (index of strTable) for the desired symbol
 * PARAMETER 2: the scope of the desired symbol
 * PARAMETER 3: which child of the parent scope has the desired symbol
 * RETURNS: the id of the desired symbol
 */
char* GetSymbolID(int val, int scope, int childNumber){
    table_node* thisScope = currentScope;
    while(scope > 1){
        thisScope = thisScope->first_child; 
        scope--; 
    }
    while(childNumber > 1){
        thisScope = thisScope->next; 
        childNumber--;
    }
    return thisScope->strTable[val]->id;
}

/* PrintSymbolTable will print the symbol table to the console if the --sym flag is set during execution
 * PARAMETER 1: table_node* indicating the root node of the symbol table tree
 * PARAMETER 2: nest level (for indentations)
 */
void PrintSymbolTable(table_node* input, int nest) {
    for(int i = 0; i < MAXIDS; i++){
        int j = 0;
        if(input->strTable[i] != NULL){
            param* paramPtr = input->strTable[i]->params;
            printf("SYMBOL TABLE ENTRY %d\n",i);
            printf("\tID: %s\n", input->strTable[i]->id); 
            printf("\tSCOPE: %d\n", input->strTable[i]->scope); 
            printf("\tCHILD NUMBER: %d\n", input->strTable[i]->childNumber); 
            printf("\tDATA TYPE: %s\n", dataTypes[input->strTable[i]->data_type]);
            printf("\tSYMBOL TYPE: %s\n", symbolTypes[input->strTable[i]->symbol_type]);  
            printf("\tSIZE: %d\n", input->strTable[i]->size);  
            while(paramPtr != NULL){
                j++;
                printf("\tPARAM %d: data type of %s | symbol type of %s\n",j,dataTypes[paramPtr->data_type], symbolTypes[paramPtr->symbol_type]);  
                paramPtr = paramPtr->next;
            }
        }
    }
    if(input->next != NULL){
        PrintSymbolTable(input->next, nest + 1); 
    }
    if(input->first_child != NULL){
        PrintSymbolTable(input->first_child, nest + 1); 
    }
}