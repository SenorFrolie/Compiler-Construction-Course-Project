#ifndef STRTAB_H
#define STRTAB_H
#define MAXIDS 1000
#include<stdlib.h>

enum dataType {INT_TYPE, CHAR_TYPE, VOID_TYPE};
enum symbolType {SCALAR, ARRAY, FUNCTION};

typedef struct param{
    int data_type;
    int symbol_type;
    struct param* next;
} param;

// You should use a linear linklist to keep track of all parameters passed to a function. The working_list_head should point to the beginning of the linklist and working_list_end should point to the end. Whenever a parameter is passed to a function, that node should also be added in this list. */
extern param* working_list_head;
extern param* working_list_end;

typedef struct arg{
    int data_type;
    int symbol_type;
    struct arg* next;
} arg;

//linear linked-list to keep trach of the arguments for a function
extern arg* argumentListHead;
extern arg* argumentListEnd;

typedef struct strEntry{
    char id[50];
    int   scope;
    int   childNumber;
    int   data_type;
    int   symbol_type;
    int   size; //Num elements if array, num params if function
    int   arrayBound; 
    param*  params;
} symEntry;

typedef struct table_node{
    symEntry* strTable[MAXIDS];
    int numChildren;
    struct table_node* parent;
    struct table_node* first_child; // First subscope
    struct table_node* last_child;  // Most recently added subscope
    struct table_node* next; // Next subscope that shares the same parent
} table_node; // Describes each node in the symbol table tree and is used to implement a tree for the nested scope as discussed in lecture 13 and 14.

//table_node* currentScope = NULL; // A global variable that should point to the symbol table node in the scope tree as discussed in lecture 13 and 14.
extern table_node* currentScope;
extern table_node* symbolTableHead; 

/* Inserts a symbol into the current symbol table tree. Please note that this function is used to instead into the tree of symbol tables and NOT the AST. Start at the returned hash and probe until we find an empty slot or the id.  */
int STInsert(char *id, int data_type, int symbol_type, int* scope, int* alreadyExists);

/* The function for looking up if a symbol exists in the currentScope. Always start looking for the symbol from the node that is being pointed to by the currentScope variable*/
symEntry* STLookup(char *id, int *keyRef);

/* Creates a param* whenever formalDecl in the parser.y file declares a formal parameter. Please note that we are maining a separate linklist to keep track of all the formal declarations because until the function body is processed, we will not know the number of parameters in advance. Link list provides a way for the formalDecl to declare as many parameters as needed.*/
void AddParam(int data_type, int symbol_type);

/*ConnectParams is called after the funBody is processed in parser.y. At this point, the parser has already seen all the formal parameter declaration and has built the entire list of parameters to the function. This list is pointed to by the working_list_head pointer. currentScope->parent->strTable[index]->params should point to the header of that parameter list. */
void ConnectParams(int i);

//add argument to the linked-list created above - much the same as param
void AddArgument(int dataType, int symbolType);

//reset the head and end pointers of the argument linked-list
void ResetArguments();

//count the number of elements in the argument linked-list
int CountArguments();

//creates a new scope within the current scope and set that as the current scope
void NewScope();

//move up one scope in the symbol table tree
void UpScope();

//check if an element exists in a local scope
symEntry* Search(char *id, table_node *scope, int *keyRef);

//get the id of an element
char* GetSymbolID(int val, int scope, int childNumber);

//wrapper for hash function. creates a hash-key for an entry into the strTable
int MakeKey(char *id);

//prints the symbol table
void PrintSymbolTable(table_node* input, int nest);

#endif
