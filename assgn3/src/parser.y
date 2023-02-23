%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../src/tree.h"
#include"../src/strtab.h"

int yydebug=1; 
extern int yylineno;
/* nodeTypes refer to different types of internal and external nodes that can be part of the abstract syntax tree.*/
enum nodeTypes {PROGRAM, DECLLIST, DECL, VARDECL, VARDECLP, TYPESPEC, FUNDECL, 
                FUNCTYPENAME, FORMALDECLLIST, FORMALDECLLISTP, FORMALDECL, FORMALDECLP, 
                FUNBODY, LOCALDECLLIST, STATEMENTLIST, STATEMENT, COMPOUNDSTMT, ASSIGNSTMT,
                CONDSTMT, CONDSTMTP, LOOPSTMT, RETURNSTMT, RETURNSTMTP, EXPRESSION, RELOP,
                ADDEXPR, ADDOP, TERM, MULOP, FACTOR, FUNCCALLEXPR, FUNCCALLEXPRP,
                ARGLIST, INTEGER, IDENTIFIER, VAR, VARP, ARRAYDECL, CHAR,
                };
enum opType {ADD, SUB, MUL, DIV, LT, LTE, EQ, GTE, GT, NEQ};

/* NOTE: mC has two kinds of scopes for variables : local and global. Variables declared outside any
function are considered globals, whereas variables (and parameters) declared inside a function foo are local to foo. You should update the scope variable whenever you are inside a production that matches function definition (funDecl production). The rationale is that you are entering that function, so all variables, arrays, and other functions should be within this scope. You should pass this variable whenever you are calling the ST_insert or ST_lookup functions. This variable should be updated to scope = "" to indicate global scope whenever funDecl finishes. Treat these hints as helpful directions only. You may implement all of the functions as you like and not adhere to my instructions. As long as the directory structure is correct and the file names are correct, we are okay with it. */
int scope = 0;
%}

/* the union describes the fields available in the yylval variable */
%union
{
    int value;
    struct treenode *node;
    char *strval;
}

/*Add token declarations below. The type <value> indicates that the associated token will be of a value type such as integer, float etc., and <strval> indicates that the associated token will be of string type.*/
%token <strval> ID
%token <value> INTCONST
%token <value> CHARCONST
%token <strval> STRCONST

%token <value> KWD_IF
%token <value> KWD_ELSE
%token <value> KWD_WHILE
%token <value> KWD_INT
%token <value> KWD_STRING
%token <value> KWD_CHAR
%token <value> KWD_RETURN
%token <value> KWD_VOID

%token <value> OPER_ADD
%token <value> OPER_SUB
%token <value> OPER_MUL
%token <value> OPER_DIV
%token <value> OPER_LT
%token <value> OPER_GT
%token <value> OPER_GTE
%token <value> OPER_LTE
%token <value> OPER_EQ
%token <value> OPER_NEQ
%token <value> OPER_ASGN
%token <value> OPER_AT
%token <value> OPER_INC
%token <value> OPER_DEC
%token <value> OPER_AND
%token <value> OPER_OR
%token <value> OPER_NOT
%token <value> OPER_MOD

%token <value> LSQ_BRKT
%token <value> RSQ_BRKT
%token <value> LCRLY_BRKT
%token <value> RCRLY_BRKT
%token <value> LPAREN
%token <value> RPAREN

%token <value> COMMA
%token <value> SEMICLN

%token <value> ILLEGAL_TOKEN
%token <value> ERROR


/* TODO: Add the rest of the tokens below.*/





%type <node> program declList decl varDecl varDeclP funDecl funDeclP typeSpecifier formalDeclList formalDeclListP formalDecl formalDeclP funcTypeName
%type <node> funBody localDeclList statementList statement compoundStmt assignStmt condStmt condStmtP loopStmt returnStmt returnStmtP
%type <node> var varP expression relOp addExpr addOp term mulOp factor funcCallExpr funcCallExprP argList






%start program

%%

program         : {new_scope(); scope++; } declList
                 {
                    //printf("Inside program | declList\n");
                    tree* progNode = maketree(PROGRAM);
                    addChild(progNode, $2);
                    ast = progNode;
                 }
                ;
declList        : decl
                 {
                    //printf("Inside declList | decl\n");
                    tree* declListNode = maketree(DECLLIST);
                    addChild(declListNode, $1);
                    $$ = declListNode;
                 }
                | declList decl
                 {
                    //printf("Inside declList | declList decl\n");
                    tree* declListNode = maketree(DECLLIST);
                    addChild(declListNode, $1);
                    addChild(declListNode, $2);
                    $$ = declListNode;
                 }
                ;
decl            : varDecl 
                    {
                    //printf("Inside decl | varDecl\n");
                    tree* declNode = maketree(DECL);
                    addChild(declNode, $1);
                    $$ = declNode;
                    }
                | funDecl
                    {
                    //printf("Inside decl | funDecl \n");
                    tree* declNode = maketree(DECL);
                    addChild(declNode, $1);
                    $$ = declNode;
                    }
                ;
varDecl         : typeSpecifier ID varDeclP
                    {
                    tree* varDeclNode = maketree(VARDECL);
                    addChild(varDeclNode, $1);
                    //printf("this is dumb %s\n", $2);
                    //printf("Inside varDecl | typeSpecifier ID varDeclP\n");
                    int exists = -1; 
                    int key; 
                    if($3 == NULL){
                        key = ST_insert($2, varDeclNode->children[0]->val, SCALAR, &scope, &exists);
                    }
                    else{
                        key = ST_insert($2, varDeclNode->children[0]->val, ARRAY, &scope, &exists);
                        current_scope->strTable[key]->arrayBound = $3->val; 
                    }
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = maketreeWithVal(IDENTIFIER, key);
                    childNode->data_type = current_scope->strTable[key]->data_type;
                    childNode->sym_type = current_scope->strTable[key]->symbol_type;
                    childNode->scope = current_scope->strTable[key]->scope;
                    childNode->order = current_scope->strTable[key]->order;
                    addChild(varDeclNode, childNode);
                    addChild(varDeclNode, $3);
                    $$ = varDeclNode;
                    }
                ;
varDeclP        : LSQ_BRKT INTCONST RSQ_BRKT SEMICLN
                    {
                    if($2 == 0){
                        yyerror("Array variable declared with size of zero."); 
                    }
                    //printf("Inside varDeclP | LSQ_BRKT INTCONST RSQ_BRKT\n");
                    $$ = maketreeWithVal(INTEGER, $2);
                    }
                | SEMICLN
                    {
                    //printf("Inside varDeclP | SEMICLN\n");
                    $$ = NULL; 
                    }
                ;
typeSpecifier   : KWD_INT
                    {
                    //printf("Inside typeSpecifier KWD_INT\n");
                    $$ = maketreeWithVal(TYPESPEC, INT_TYPE);
                    }
                | KWD_CHAR
                    {
                    //printf("Inside typeSpecifier KWD_CHAR\n");
                    $$ = maketreeWithVal(TYPESPEC, CHAR_TYPE);
                    }
                | KWD_VOID
                    {
                    $$ = maketreeWithVal(TYPESPEC, VOID_TYPE);
                    //printf("Inside typeSpecifier KWD_VOID\n");
                    }
                ;
funDecl         : funcTypeName LPAREN 
                    {
                    new_scope(); 
                    scope++;
                    } 
                  formalDeclList RPAREN funBody
                    {
                    up_scope();
                    scope--; 
                    connect_params($1->children[1]->val); 
                    //printf("Inside funDecl | funcTypeName LPAREN formalDeclList RPAREN funBody\n");
                    //scope = "local"; 
                    tree* parentNode = maketree(FUNDECL);
                    addChild(parentNode, $1);
                    //printf("NUM OF CHILDREN %d\n", parentNode->numChildren);
                    //addChild(parentNode, $3);
                    //addChild(parentNode, $5);
                    addChild(parentNode, $4);
                    addChild(parentNode, $6);
                    $$ = parentNode; 
                    //scope = "";
                    }
                | funcTypeName LPAREN 
                    {
                    new_scope(); 
                    scope++; 
                    //printf("this is dumb fuckery\n");
                    }
                  RPAREN funBody
                    {
                    up_scope();
                    scope--; 
                    //printf("Inside funDecl | typeSpecifier ID LPAREN funDeclP\n");
                    //scope = "local"; 
                    tree* parentNode = maketree(FUNDECL);
                    addChild(parentNode, $1);
                    //printf("NUM OF CHILDREN %d\n", parentNode->numChildren);
                    //addChild(parentNode, $4);
                    addChild(parentNode, $5);
                    $$ = parentNode; 
                    //scope = "";
                    }
                ;
funcTypeName    : typeSpecifier ID 
                    {
                    tree* parentNode = maketree(FUNCTYPENAME);
                    addChild(parentNode, $1);
                    int exists = -1; 
                    int key = ST_insert($2, parentNode->children[0]->val, FUNCTION, &scope, &exists); //global
                    //printf("key created %d\n", key);
                    //printf("address of currentscope is %x\n", current_scope->strTable); 
                    //printf("address of strTable[%d] is %x\n", key, current_scope->strTable[key]); 
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = maketreeWithVal(IDENTIFIER, key);
                    childNode->data_type = current_scope->strTable[key]->data_type;
                    childNode->sym_type = current_scope->strTable[key]->symbol_type;
                    childNode->scope = current_scope->strTable[key]->scope;
                    childNode->order = current_scope->strTable[key]->order;
                    addChild(parentNode, childNode);
                    $$ = parentNode;
                    }
                ;
formalDeclList  : formalDecl formalDeclListP
                    {
                    //printf("Inside formalDeclList | formalDecl formalDeclListP\n");
                    tree* parentNode = maketree(FORMALDECLLIST);
                    addChild(parentNode, $1);
                    addChild(parentNode, $2);
                    $$ = parentNode;
                    }
                ;
formalDeclListP :
                    {
                    //printf("Inside formalDeclListP | empty \n");
                    $$ = NULL;
                    }
                | COMMA formalDeclList
                    {
                    //printf("Inside formalDeclListP | COMMA formalDeclList\n");
                    $$ = $2;
                    }
                ;
formalDecl      : typeSpecifier ID formalDeclP
                    {
                    //printf("Inside formalDecl | typeSpecifier ID formalDeclP\n");
                    //scope = "local"; 
                    tree* parentNode = maketree(FORMALDECL);
                    addChild(parentNode, $1);
                    int exists = -1; 
                    int key;
                    if($3 == NULL){
                        key = ST_insert($2, parentNode->children[0]->val, SCALAR, &scope, &exists);
                    }
                    else{
                        key = ST_insert($2, parentNode->children[0]->val, ARRAY, &scope, &exists);
                    }
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = maketreeWithVal(IDENTIFIER, key);
                    childNode->data_type = current_scope->strTable[key]->data_type;
                    childNode->sym_type = current_scope->strTable[key]->symbol_type;
                    childNode->scope = current_scope->strTable[key]->scope;
                    childNode->order = current_scope->strTable[key]->order;
                    add_param(current_scope->strTable[key]->data_type, current_scope->strTable[key]->symbol_type);
                    addChild(parentNode, childNode);
                    addChild(parentNode, $3); 
                    $$ = parentNode;
                    //scope = ""; 
                    }
                ;
formalDeclP     : 
                    {
                    //printf("Inside formalDeclP\n");
                    $$ = NULL;
                    }
                | LSQ_BRKT RSQ_BRKT
                    {
                    //printf("Inside formalDeclP | LSQ_BRKT RSQ_BRKT\n");
                    $$ = maketree(ARRAYDECL);
                    }
                ;
funBody         : LCRLY_BRKT localDeclList statementList RCRLY_BRKT
                    {
                    //printf("Inside funBody | LCRLY_BRKT localDeclList statementList RCRLY_BRKT\n");
                    tree* parentNode = maketree(FUNBODY);
                    addChild(parentNode, $2);
                    addChild(parentNode, $3);
                    $$ = parentNode;
                    }
                ;
localDeclList   : 
                    {
                    //printf("Inside localDeclList | empty \n");
                    //tree* parentNode = maketree(LOCALDECLLIST);
                    $$ = NULL;
                    }
                | varDecl localDeclList 
                    {
                    //printf("Inside localDeclList | varDecl localDeclList\n");
                    tree* parentNode = maketree(LOCALDECLLIST);
                    addChild(parentNode, $1);
                    addChild(parentNode, $2);
                    $$ = parentNode;
                    }
                ;
statementList   :
                    {
                    //printf("Inside statementList | statement\n");
                    $$ = NULL;
                    }
                | statement statementList 
                    {   
                    //printf("Inside statementList | statement statementList \n");
                    tree* parentNode = maketree(STATEMENTLIST);
                    addChild(parentNode, $1);
                    addChild(parentNode, $2);
                    $$ = parentNode;
                    }
                ;
statement       : compoundStmt
                    {
                    //printf("Inside statement | compoundStmt\n");
                    tree* parentNode = maketree(STATEMENT);
                    addChild(parentNode, $1);
                    $$ = parentNode;
                    }
                | assignStmt
                    {   
                    //printf("Inside statement | assignStmt\n");
                    tree* parentNode = maketree(STATEMENT);
                    addChild(parentNode, $1);
                    $$ = parentNode;    
                    }
                | condStmt
                    {
                    //printf("Inside statement | condStmt\n");
                    tree* parentNode = maketree(STATEMENT);
                    addChild(parentNode, $1);
                    $$ = parentNode;
                    }
                | loopStmt
                    {
                    //printf("Inside statement | loopStmt\n");
                    tree* parentNode = maketree(STATEMENT);
                    addChild(parentNode, $1);
                    $$ = parentNode;
                    }
                | returnStmt
                    {
                    //printf("Inside statement | returnStmt\n");
                    tree* parentNode = maketree(STATEMENT);
                    addChild(parentNode, $1);
                    $$ = parentNode;
                    }
                ;
compoundStmt    : LCRLY_BRKT statementList RCRLY_BRKT
                    {
                    //printf("Inside compoundStmt | LCRLY_BRKT statementList RCRLY_BRKT\n");
                    tree* parentNode = maketree(COMPOUNDSTMT);
                    addChild(parentNode, $2);
                    $$ = parentNode;
                    }
                ;
assignStmt      : var OPER_ASGN expression SEMICLN
                    {
                    //printf("Inside assignStmt | var OPER_ASGN expression SEMICLN\n");
                    tree* parentNode = maketree(ASSIGNSTMT);
                    addChild(parentNode, $1);
                    addChild(parentNode, $3); 
                    $$ = parentNode;

                    tree* current = $1; 
                    while(current->numChildren > 0){
                        current = current->children[0];
                    }

                    int varDataType = current->data_type;

                    if(isMismatch($3, varDataType) == 1){
                        yyerror("Type mismatch in assignment.");
                    }
                    }
                | expression SEMICLN
                    {
                    //printf("Inside assignStmt | expression SEMICLN\n");
                    tree* parentNode = maketree(ASSIGNSTMT);
                    addChild(parentNode, $1);
                    $$ = parentNode;
                    }
                ;
condStmt        : KWD_IF LPAREN expression RPAREN statement condStmtP
                    {
                    //printf("Inside condStmt | KWD_IF LPAREN expression RPAREN statement condStmtP\n");
                    tree* parentNode = maketree(CONDSTMT);
                    addChild(parentNode, $3);
                    addChild(parentNode, $5);
                    addChild(parentNode, $6);
                    $$ = parentNode;
                    }
                ;
condStmtP       : 
                    {
                    //printf("Inside condStmtP | empty\n");
                    $$ = NULL; 
                    }
                | KWD_ELSE statement
                    {
                    //printf("Inside condStmtP | KWD_ELSE statement\n");
                    $$ = $2;
                    }
                ;
loopStmt        : KWD_WHILE LPAREN expression RPAREN statement
                    {
                    printf("Inside loopStmt | KWD_WHILE LPAREN expression RPAREN statement\n");
                    tree* parentNode = maketree(LOOPSTMT);
                    addChild(parentNode, $3);
                    addChild(parentNode, $5);
                    $$ = parentNode;
                    }
                ;
returnStmt      : KWD_RETURN returnStmtP
                    {
                    //printf("Inside returnStmt | KWD_RETURN returnStmtP\n");
                    tree* parentNode = maketree(RETURNSTMT);
                    addChild(parentNode, $2); 
                    $$ = parentNode;
                    }
                ;
returnStmtP     : SEMICLN
                    {
                    //printf("Inside returnStmtP | SEMICLN\n");
                    $$ = NULL; 
                    }
                | expression SEMICLN
                    {
                    //printf("Inside returnStmtP | expression SEMICLN\n");
                    $$ = $1;
                    }
                ;
var             : ID varP
                    {
                    //printf("Inside var | ID varP\n");
                    tree* parentNode = maketree(VAR);
                    int key = -1; 
                    symEntry* entry = ST_lookup($1, &key); 
                    tree* childNode = maketreeWithVal(IDENTIFIER, key);
                    //printf("yylval is %d\n", yylval.value); 
                    if($2 != NULL){
                        if(entry->symbol_type != ARRAY){
                            yyerror("Non-array identifier used as an array.");
                        }
                        else{
                            tree* temp = $2; 
                            while(temp->numChildren > 0){
                                temp = temp->children[0];
                                //printf("temp nodekind is %d\n", temp->nodeKind);
                            }
                            //printf("temp nodekind is %d\n\n", temp->nodeKind);

                            if(temp->nodeKind == IDENTIFIER){
                                if(temp->data_type != INT_TYPE){
                                    yyerror("Array indexed using non-integer expression.");
                                }
                            }
                            else if(temp->nodeKind == INTEGER){
                                //printf("temp->val is %d, arrayBound is %d\n", temp->val, entry->arrayBound); 
                                if(yylval.value >= entry->arrayBound){
                                    yyerror("Statically sized array indexed with constant, out-of-bounds expression.");
                                }
                            }
                            else{
                                yyerror("Array indexed using non-integer expression.");
                            }
                        }
                    }
                    if(key == -1){
                        yyerror("Undeclared variable");
                    }
                    else{
                        childNode->data_type = entry->data_type;
                        childNode->sym_type = entry->symbol_type;
                        childNode->scope = entry->scope;
                        childNode->order = entry->order;
                    }
                    addChild(parentNode, childNode);
                    addChild(parentNode, $2);
                    $$ = parentNode;
                    }
                ;
varP            : 
                    {
                    //printf("Inside varP | empty\n");
                    $$ = NULL;
                    }
                | LSQ_BRKT addExpr RSQ_BRKT
                    {
                    //printf("Inside varP | LSQ_BRKT addExpr RSQ_BRKT\n");
                    $$ = $2;
                    }
                ;
expression      : addExpr
                    {
                    //printf("Inside expression | addExpr\n");
                    tree* parentNode = maketree(EXPRESSION);
                    addChild(parentNode, $1);
                    $$ = parentNode;
                    }
                | expression relOp addExpr
                    {
                    //printf("Inside expression | expression relOp addExpr\n");
                    tree* parentNode = maketree(EXPRESSION);
                    addChild(parentNode, $1);
                    addChild(parentNode, $2);
                    addChild(parentNode, $3);
                    $$ = parentNode;
                    }
                ;          
relOp           : OPER_LTE
                    {
                    //printf("Inside relOp | OPER_LTE\n");
                    $$ = maketreeWithVal(RELOP, LTE);
                    }
                | OPER_LT
                    {
                    //printf("Inside relOp | OPER_LT\n");
                    $$ = maketreeWithVal(RELOP, LT);
                    }  
                | OPER_GT
                    {
                    //printf("Inside relOp | OPER_GT\n");
                    $$ = maketreeWithVal(RELOP, GT);
                    }
                | OPER_GTE
                    {
                    //printf("Inside relOp | OPER_GTE\n");
                    $$ = maketreeWithVal(RELOP, GTE);
                    }
                | OPER_EQ
                    {
                    //printf("Inside relOp | OPER_EQ\n");
                    $$ = maketreeWithVal(RELOP, EQ);
                    }
                | OPER_NEQ
                    {
                    //printf("Inside relOp | OPER_NEQ\n");
                    $$ = maketreeWithVal(RELOP, NEQ);
                    }
                ;
addExpr         : term 
                    {
                    //printf("Inside addExpr | term\n");
                    tree* parentNode = maketree(ADDEXPR);
                    addChild(parentNode, $1);
                    $$ = parentNode;
                    }
                | addExpr addOp term 
                    {
                    //printf("Inside addExpr | addExpr addOp term\n");
                    tree* parentNode = maketree(ADDEXPR);
                    addChild(parentNode, $1);
                    addChild(parentNode, $2);
                    addChild(parentNode, $3);
                    $$ = parentNode;
                    }
                ;
addOp           : OPER_ADD
                    {
                    //printf("Inside addOp | OPER_ADD\n");
                    $$ = maketreeWithVal(ADDOP, ADD);
                    }
                | OPER_SUB
                    {
                    //printf("Inside addOp | OPER_SUB\n");
                    $$ = maketreeWithVal(ADDOP, SUB);
                    }
                ;
term            : factor
                    {
                    //printf("Inside term | factor\n");
                    tree* parentNode = maketree(TERM);
                    addChild(parentNode, $1);
                    $$ = parentNode;
                    }
                | term mulOp factor 
                    {
                    //printf("Inside term | term mulOp factor\n");
                    tree* parentNode = maketree(TERM);
                    addChild(parentNode, $1);
                    addChild(parentNode, $2);
                    addChild(parentNode, $3);
                    $$ = parentNode;
                    }
                ;
mulOp           : OPER_MUL
                    {
                    //printf("Inside mulOp | OPER_MUL\n");
                    $$ = maketreeWithVal(MULOP, MUL);
                    }
                | OPER_DIV
                    {
                    //printf("Inside mulOp | OPER_DIV\n");
                    $$ = maketreeWithVal(MULOP, DIV); 
                    }
                ;
factor          : LPAREN expression RPAREN
                    {
                    //printf("Inside factor | LPAREN expression RPAREN\n");
                    tree* parentNode = maketree(FACTOR);
                    addChild(parentNode, $2);
                    $$ = parentNode;
                    }
                | var 
                    {
                    //printf("Inside factor | var\n");
                    tree* parentNode = maketree(FACTOR);
                    addChild(parentNode, $1);
                    $$ = parentNode;
                    }
                | funcCallExpr
                    {
                    //printf("Inside factor | funcCallExpr\n");
                    tree* parentNode = maketree(FACTOR);
                    addChild(parentNode, $1);
                    $$ = parentNode;
                    }
                | INTCONST
                    {
                    //printf("Inside factor | INTCONST\n");
                    tree* parentNode = maketree(FACTOR);
                    addChild(parentNode, maketreeWithVal(INTEGER, $1));
                    $$ = parentNode;
                    }
                | CHARCONST
                    {
                    //printf("Inside factor | CHARCONST\n");
                    tree* parentNode = maketree(FACTOR);
                    addChild(parentNode, maketreeWithVal(CHAR, $1));
                    $$ = parentNode;
                    }
                | STRCONST
                    {
                    //printf("Inside factor | STRCONST\n");
                    tree* parentNode = maketree(FACTOR);
                    int exists = -1; 
                    int key = ST_insert($1, CHAR_TYPE, SCALAR, &scope, &exists);
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = maketreeWithVal(CHAR, key);
                    childNode->data_type = current_scope->strTable[key]->data_type;
                    childNode->sym_type = current_scope->strTable[key]->symbol_type;
                    childNode->scope = current_scope->strTable[key]->scope;
                    childNode->order = current_scope->strTable[key]->order;
                    addChild(parentNode, childNode);
                    $$ = parentNode;
                    }
                ;
funcCallExpr    : ID LPAREN funcCallExprP
                    {
                    //printf("Inside funcCallExpr | ID LPAREN funcCallExprP\n");
                    tree* parentNode = maketree(FUNCCALLEXPR);
                    int key = -1; 
                    symEntry* entry = ST_lookup($1, &key); 
                    tree* childNode = maketreeWithVal(IDENTIFIER, key);
                    if(key == -1){
                        yyerror("Undefined function");
                    }
                    else{
                        childNode->data_type = entry->data_type;
                        childNode->sym_type = entry->symbol_type;
                        childNode->scope = entry->scope;
                        childNode->order = entry->order;

                        if(count_args() < symTableTreeHead->strTable[key]->size){
                            yyerror("Too few arguments provided in function call.");
                        }
                        else if(count_args() > symTableTreeHead->strTable[key]->size){
                            yyerror("Too many arguments provided in function call.");
                        }
                        else{
                            
                            param* currentParam = symTableTreeHead->strTable[key]->params; 
                            arg* currentArg = arg_list_head; 
                            while(currentParam != NULL && currentArg != NULL){
                                //printf("param datatype %d\n", currentParam->data_type);
                                //printf("param symbol_type %d\n", currentParam->symbol_type);
                                //printf("arg datatype %d\n", currentArg->data_type);
                                //printf("arg symbol_type %d\n\n", currentArg->symbol_type);
                                if(currentParam->data_type != currentArg->data_type){
                                    yyerror("Argument type mismatch in function call.");
                                    break;
                                }
                                if(currentParam->symbol_type != currentArg->symbol_type){
                                    yyerror("Argument type mismatch in function call.");
                                    break;
                                }
                                currentParam = currentParam->next; 
                                currentArg = currentArg->next; 
                            }
                        }
                        clear_args();
                    }
                    addChild(parentNode, childNode);
                    addChild(parentNode, $3);
                    $$ = parentNode;
                    }
                ;
funcCallExprP   : argList RPAREN
                    {
                    //printf("Inside funcCallExprP | argList RPAREN\n");
                    $$ = $1;
                    }
                | RPAREN
                    {
                    //printf("Inside funcCallExprP | RPAREN\n");
                    $$ = NULL; 
                    }
                ;
argList         : expression
                    {
                    //printf("Inside argList | expression\n");
                    tree* parentNode = maketree(ARGLIST);
                    addChild(parentNode, $1);
                        int tempDataType = -1;
                        int tempSymType = -1;
                        tree *temp = parentNode;
						while (temp->numChildren > 0) {
							temp = temp->children[0];
						}
                        //var
                        if (temp->nodeKind == IDENTIFIER) {
                            //printf("id %s\n", current_scope->strTable[temp->val]);
						  if (current_scope->strTable[temp->val]->symbol_type == FUNCTION) {
						    tempDataType = INT_TYPE;    //function call in another function call
						  } 
                          else {    //ARRAY OR SCALAR
                            tempDataType = current_scope->strTable[temp->val]->data_type;
						    tempSymType = current_scope->strTable[temp->val]->symbol_type;
						  }
						} 
                        //const
                        else {
                            if(temp->nodeKind == INTEGER){
                                tempDataType = INT_TYPE; 
                                tempSymType = SCALAR;
                            }
                            else if(temp->nodeKind == CHAR){
                                tempDataType = CHAR_TYPE; 
                                tempSymType = SCALAR;
                            }
						}
                        add_arg(tempDataType, tempSymType);
                    $$ = parentNode;
                    }
                | argList COMMA expression
                    {
                    //printf("Inside argList | argList COMMA expression\n");
                    tree* parentNode = maketree(ARGLIST);
                    addChild(parentNode, $1);
                    addChild(parentNode, $3);
                        int tempDataType = -1;
                        int tempSymType = -1;
                        tree *temp = parentNode->children[1];
						while (temp->numChildren > 0) {
							temp = temp->children[0];
						}
                        //var
                        if (temp->nodeKind == IDENTIFIER) {
                            //printf("id %s\n", current_scope->strTable[temp->val]);
						  if (current_scope->strTable[temp->val]->symbol_type == FUNCTION) {
						    tempDataType = INT_TYPE;    //function call in another function call
						  } 
                          else {    //ARRAY OR SCALAR
                            tempDataType = current_scope->strTable[temp->val]->data_type;
						    tempSymType = current_scope->strTable[temp->val]->symbol_type;
						  }
						} 
                        //const
                        else {
                            if(temp->nodeKind == INTEGER){
                                tempDataType = INT_TYPE; 
                                tempSymType = SCALAR;
                            }
                            else if(temp->nodeKind == CHAR){
                                tempDataType = CHAR_TYPE; 
                                tempSymType = SCALAR;
                            }
						}
                        add_arg(tempDataType, tempSymType);
                    $$ = parentNode;
                    }
                ;
%%

int yywarning(char * msg){
    printf("warning: line %d: %s\n", yylineno, msg);
    return 0;
}

int yyerror(char * msg){
    printf("error: line %d: %s\n", yylineno, msg);
    return 0;
}
