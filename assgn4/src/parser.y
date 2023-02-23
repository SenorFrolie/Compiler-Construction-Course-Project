%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../src/tree.h"
#include"../src/strtab.h"

int yydebug=1; 
extern int yylineno;
/* nodeTypes refer to different types of internal and external nodes that can be part of the abstract syntax tree.*/
enum nodeTypes {PROGRAM, DECLLIST, DECL, VARDECL, VARDECLNT, TYPESPEC, FUNDECL, 
                FUNCTYPENAME, FORMALDECLLIST, FORMALDECLLISTNT, FORMALDECL, FORMALDECLNT, 
                FUNBODY, LOCALDECLLIST, STATEMENTLIST, STATEMENT, COMPOUNDSTMT, ASSIGNSTMT,
                CONDSTMT, CONDSTMTNT, LOOPSTMT, RETURNSTMT, RETURNSTMTNT, EXPRESSION, RELOP,
                ADDEXPR, ADDOP, TERM, MULOP, FACTOR, FUNCCALLEXPR, FUNCCALLEXPRNT,
                ARGLIST, INTEGER, IDENTIFIER, VAR, VARNT, ARRAYDECL, CHAR,
                };
enum opType {ADD, SUB, MUL, DIV, LT, LTE, EQ, GTE, GT, NEQ};

/* NOTE: mC has two kinds of scopes for variables : local and global. Variables declared outside any
function are considered globals, whereas variables (and parameters) declared inside a function foo are local to foo. You should update the scope variable whenever you are inside a production that matches function definition (funDecl production). The rationale is that you are entering that function, so all variables, arrays, and other functions should be within this scope. You should pass this variable whenever you are calling the STInsert or STLookup functions. This variable should be updated to scope = "" to indicate global scope whenever funDecl finishes. Treat these hints as helpful directions only. You may implement all of the functions as you like and not adhere to my instructions. As long as the directory structure is correct and the file names are correct, we are okay with it. */
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
/* TODO: Add the rest of the tokens below.*/
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

/* TODO: Declate non-terminal symbols as of type node. Provided below is one example. node is defined as 'struct treenode *node' in the above union data structure. This declaration indicates to parser that these non-terminal variables will be implemented using a 'treenode *' type data structure. Hence, the circles you draw when drawing a parse tree, the following lines are telling yacc that these will eventually become circles in an AST. This is one of the connections between the AST you draw by hand and how yacc implements code to concretize that. We provide with two examples: program and declList from the grammar. Make sure to add the rest.  */

%type <node> program declList decl varDecl varDeclNT funDecl funDeclP typeSpecifier formalDeclList formalDeclListNT formalDecl formalDeclNT funcTypeName funBody localDeclList statementList statement compoundStmt assignStmt condStmt condStmtNT loopStmt returnStmt returnStmtNT var varNT expression relOp addExpr addOp term mulOp factor funcCallExpr funcCallExprNT argList

%start program

%%
program         : { NewScope(); scope++; } declList
                 {
 //                   printf("top of program\n");
                    tree* progNode = MakeTree(PROGRAM);
                    AddChild(progNode, $2);
                    ast = progNode;
                 }
                ;
declList        : decl
                 {
                    tree* declListNode = MakeTree(DECLLIST);
                    AddChild(declListNode, $1);
                    $$ = declListNode;
                 }
                | declList decl
                 {
                    tree* declListNode = MakeTree(DECLLIST);
                    AddChild(declListNode, $1);
                    AddChild(declListNode, $2);
                    $$ = declListNode;
                 }
                ;
decl            : varDecl 
                 {
                    tree* declNode = MakeTree(DECL);
                    AddChild(declNode, $1);
                    $$ = declNode;
                 }
                | funDecl
                 {
                    tree* declNode = MakeTree(DECL);
                    AddChild(declNode, $1);
                    $$ = declNode;
                 }
                ;
varDecl         : typeSpecifier ID varDeclNT
                 {
                    tree* varDeclNode = MakeTree(VARDECL);
                    AddChild(varDeclNode, $1);
                    int exists = -1; 
                    int key; 
                    if($3 == NULL){
                        key = STInsert($2, varDeclNode->children[0]->val, SCALAR, &scope, &exists);
                    }
                    else{
                        key = STInsert($2, varDeclNode->children[0]->val, ARRAY, &scope, &exists);
                        currentScope->strTable[key]->arrayBound = $3->val; 
                    }
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = MakeTreeWithVal(IDENTIFIER, key);
                    childNode->data_type   = currentScope->strTable[key]->data_type;
                    childNode->sym_type    = currentScope->strTable[key]->symbol_type;
                    childNode->scope       = currentScope->strTable[key]->scope;
                    childNode->childNumber = currentScope->strTable[key]->childNumber;
                    AddChild(varDeclNode, childNode);
                    AddChild(varDeclNode, $3);
                    $$ = varDeclNode;
                 }
                ;
varDeclNT        : LSQ_BRKT INTCONST RSQ_BRKT SEMICLN
                 {
                    if($2 == 0){
                        yyerror("Array variable declared with size of zero."); 
                    }
                    $$ = MakeTreeWithVal(INTEGER, $2);
                 }
                | SEMICLN
                 {
                    $$ = NULL; 
                 }
                ;
typeSpecifier   : KWD_INT
                 {
                    $$ = MakeTreeWithVal(TYPESPEC, INT_TYPE);
                 }
                | KWD_CHAR
                 {
                    $$ = MakeTreeWithVal(TYPESPEC, CHAR_TYPE);
                 }
                | KWD_VOID
                 {
                    $$ = MakeTreeWithVal(TYPESPEC, VOID_TYPE);
                 }
                ;
funDecl         : funcTypeName LPAREN { NewScope(); scope++; } formalDeclList RPAREN funBody
                 {
                    UpScope();
                    scope--; 

                    ConnectParams($1->children[1]->val); 
                    tree* funDeclNode = MakeTree(FUNDECL);
                    AddChild(funDeclNode, $1);
                    AddChild(funDeclNode, $4);
                    AddChild(funDeclNode, $6);
                    $$ = funDeclNode; 
                 }
                | funcTypeName LPAREN { NewScope(); scope++; } RPAREN funBody
                 {
                    UpScope();
                    scope--; 

                    tree* funDeclNode = MakeTree(FUNDECL);
                    AddChild(funDeclNode, $1);
                    AddChild(funDeclNode, $5);
                    $$ = funDeclNode; 
                 }
                ;
funcTypeName    : typeSpecifier ID 
                 {
                    tree* funcTypeNameNode = MakeTree(FUNCTYPENAME);
                    AddChild(funcTypeNameNode, $1);
                    int exists = -1; 
                    int key = STInsert($2, funcTypeNameNode->children[0]->val, FUNCTION, &scope, &exists); //global
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = MakeTreeWithVal(IDENTIFIER, key);
                    childNode->data_type   = currentScope->strTable[key]->data_type;
                    childNode->sym_type    = currentScope->strTable[key]->symbol_type;
                    childNode->scope       = currentScope->strTable[key]->scope;
                    childNode->childNumber = currentScope->strTable[key]->childNumber;
                    AddChild(funcTypeNameNode, childNode);
                    $$ = funcTypeNameNode;
                 }
                ;
formalDeclList  : formalDecl formalDeclListNT
                 {
                    tree* formalDeclListNode = MakeTree(FORMALDECLLIST);
                    AddChild(formalDeclListNode, $1);
                    AddChild(formalDeclListNode, $2);
                    $$ = formalDeclListNode;
                 }
                ;
formalDeclListNT :
                 {
                    $$ = NULL;
                 }
                | COMMA formalDeclList
                 {
                    $$ = $2;
                 }
                ;
formalDecl      : typeSpecifier ID formalDeclNT
                 {
                    tree* formalDeclNode = MakeTree(FORMALDECL);
                    AddChild(formalDeclNode, $1);
                    int exists = -1; 
                    int key;
                    if($3 == NULL){
                        key = STInsert($2, formalDeclNode->children[0]->val, SCALAR, &scope, &exists);
                    }
                    else{
                        key = STInsert($2, formalDeclNode->children[0]->val, ARRAY, &scope, &exists);
                    }
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = MakeTreeWithVal(IDENTIFIER, key);
                    childNode->data_type   = currentScope->strTable[key]->data_type;
                    childNode->sym_type    = currentScope->strTable[key]->symbol_type;
                    childNode->scope       = currentScope->strTable[key]->scope;
                    childNode->childNumber = currentScope->strTable[key]->childNumber;
                    AddParam(currentScope->strTable[key]->data_type, currentScope->strTable[key]->symbol_type);
                    AddChild(formalDeclNode, childNode);
                    AddChild(formalDeclNode, $3); 
                    $$ = formalDeclNode;
                 }
                ;
formalDeclNT     : 
                 {
                    $$ = NULL;
                 }
                | LSQ_BRKT RSQ_BRKT
                 {
                    $$ = MakeTree(ARRAYDECL);
                 }
                ;
funBody         : LCRLY_BRKT localDeclList statementList RCRLY_BRKT
                 {
                    tree* funBodyNode = MakeTree(FUNBODY);
                    AddChild(funBodyNode, $2);
                    AddChild(funBodyNode, $3);
                    $$ = funBodyNode;
                 }
                ;
localDeclList   : 
                 {
                    $$ = NULL;
                 }
                | varDecl localDeclList 
                 {
                    tree* localDeclListNode = MakeTree(LOCALDECLLIST);
                    AddChild(localDeclListNode, $1);
                    AddChild(localDeclListNode, $2);
                    $$ = localDeclListNode;
                 }
                ;
statementList   :
                 {
                    $$ = NULL;
                 }
                | statement statementList 
                 {   
                    tree* statementListNode = MakeTree(STATEMENTLIST);
                    AddChild(statementListNode, $1);
                    AddChild(statementListNode, $2);
                    $$ = statementListNode;
                 }
                ;
statement       : compoundStmt
                 {
                    tree* statementNode = MakeTree(STATEMENT);
                    AddChild(statementNode, $1);
                    $$ = statementNode;
                 }
                | assignStmt
                 {   
                    tree* statementNode = MakeTree(STATEMENT);
                    AddChild(statementNode, $1);
                    $$ = statementNode;    
                 }
                | condStmt
                 {
                    tree* statementNode = MakeTree(STATEMENT);
                    AddChild(statementNode, $1);
                    $$ = statementNode;
                 }
                | loopStmt
                 {
                    tree* statementNode = MakeTree(STATEMENT);
                    AddChild(statementNode, $1);
                    $$ = statementNode;
                 }
                | returnStmt
                 {
                    tree* statementNode = MakeTree(STATEMENT);
                    AddChild(statementNode, $1);
                    $$ = statementNode;
                 }
                ;
compoundStmt    : LCRLY_BRKT statementList RCRLY_BRKT
                 {
                    tree* compoundStmtNode = MakeTree(COMPOUNDSTMT);
                    AddChild(compoundStmtNode, $2);
                    $$ = compoundStmtNode;
                 }
                ;
assignStmt      : var OPER_ASGN expression SEMICLN
                 {
 //                   printf("top of assignStmt\n");
                    tree* assignStmtNode = MakeTree(ASSIGNSTMT);
                    AddChild(assignStmtNode, $1);
                    AddChild(assignStmtNode, $3); 
                    $$ = assignStmtNode;

                    tree* current = $1; 
                    while(current->numChildren > 0){
                        current = current->children[0];
                    }
                    int varDataType = current->data_type;
                    if(CheckTypeMismatch($3, varDataType) == 1){
                        yyerror("Type mismatch in assignment.");
                    }
                 }
                | expression SEMICLN
                 {
                    tree* assignStmtNode = MakeTree(ASSIGNSTMT);
                    AddChild(assignStmtNode, $1);
                    $$ = assignStmtNode;
                 }
                ;
condStmt        : KWD_IF LPAREN expression RPAREN statement condStmtNT
                 {
                    tree* condStmtNode = MakeTree(CONDSTMT);
                    AddChild(condStmtNode, $3);
                    AddChild(condStmtNode, $5);
                    AddChild(condStmtNode, $6);
                    $$ = condStmtNode;
                 }
                ;
condStmtNT       : 
                 {
                    $$ = NULL; 
                 }
                | KWD_ELSE statement
                 {
                    $$ = $2;
                 }
                ;
loopStmt        : KWD_WHILE LPAREN expression RPAREN statement
                 {
                    tree* loopStmtNode = MakeTree(LOOPSTMT);
                    AddChild(loopStmtNode, $3);
                    AddChild(loopStmtNode, $5);
                    $$ = loopStmtNode;
                 }
                ;
returnStmt      : KWD_RETURN returnStmtNT
                 {
                    tree* returnStmtNode = MakeTree(RETURNSTMT);
                    AddChild(returnStmtNode, $2); 
                    $$ = returnStmtNode;
                 }
                ;
returnStmtNT     : SEMICLN
                 {
                    $$ = NULL; 
                 }
                | expression SEMICLN
                 {
                    $$ = $1;
                 }
                ;
var             : ID varNT
                 {
                    tree* varNode = MakeTree(VAR);
                    int key = -1; 
                    symEntry* entry = STLookup($1, &key); 
                    tree* childNode = MakeTreeWithVal(IDENTIFIER, key);
                    if($2 != NULL){
                        if(entry->symbol_type != ARRAY){
                            yyerror("Non-array identifier used as an array.");
                        }
                        else{
                            tree* placeholderTreeNode = $2; 
                            while(placeholderTreeNode->numChildren > 0){
                                placeholderTreeNode = placeholderTreeNode->children[0];
                            }
                            if(placeholderTreeNode->nodeKind == IDENTIFIER){
                                if(placeholderTreeNode->data_type != INT_TYPE){
                                    yyerror("Array indexed using non-integer expression.");
                                }
                            }
                            else if(placeholderTreeNode->nodeKind == INTEGER){
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
                        childNode->data_type   = entry->data_type;
                        childNode->sym_type    = entry->symbol_type;
                        childNode->scope       = entry->scope;
                        childNode->childNumber = entry->childNumber;
                    }
                    AddChild(varNode, childNode);
                    AddChild(varNode, $2);
                    $$ = varNode;
                 }
                ;
varNT            : 
                 {
                    $$ = NULL;
                 }
                | LSQ_BRKT addExpr RSQ_BRKT
                 {
                    $$ = $2;
                 }
                ;
expression      : addExpr
                 {
                    tree* expressionNode = MakeTree(EXPRESSION);
                    AddChild(expressionNode, $1);
                    $$ = expressionNode;

                 }
                | expression relOp addExpr
                 {
                    /*
                    tree* expressionNode = MakeTree(EXPRESSION);
                    AddChild(expressionNode, $1);
                    AddChild(expressionNode, $2);
                    AddChild(expressionNode, $3);
                    $$ = expressionNode;
                    */
                    tree* expressionNode = $2;
                    AddChild(expressionNode, $1);
                    AddChild(expressionNode, $3);
                    $$ = expressionNode;

                 }
                ;          
relOp           : OPER_LTE
                 {
                    $$ = MakeTreeWithVal(RELOP, LTE);
                 }
                | OPER_LT
                 {
                    $$ = MakeTreeWithVal(RELOP, LT);
                 }  
                | OPER_GT
                 {
                    $$ = MakeTreeWithVal(RELOP, GT);
                 }
                | OPER_GTE
                 {
                    $$ = MakeTreeWithVal(RELOP, GTE);
                 }
                | OPER_EQ
                 {
                    $$ = MakeTreeWithVal(RELOP, EQ);
                 }
                | OPER_NEQ
                 {
                    $$ = MakeTreeWithVal(RELOP, NEQ);
                 }
                ;
addExpr         : term 
                 {
                    /*
                    tree* addExprNode = MakeTree(ADDEXPR);
                    AddChild(addExprNode, $1);
                    $$ = addExprNode;
                    */
                    $$ = $1;
                 }
                | addExpr addOp term 
                 {
                    /*
                    tree* addExprNode = MakeTree(ADDEXPR);
                    AddChild(addExprNode, $1);
                    AddChild(addExprNode, $2);
                    AddChild(addExprNode, $3);
                    $$ = addExprNode;
                    */
                    tree* addOpNode = $2;
                    AddChild(addOpNode, $1);
                    AddChild(addOpNode, $3);
                    $$ = addOpNode;
                 }
                ;
addOp           : OPER_ADD
                 {
                    $$ = MakeTreeWithVal(ADDOP, ADD);
                 }
                | OPER_SUB
                 {
                    $$ = MakeTreeWithVal(ADDOP, SUB);
                 }
                ;
term            : factor
                 {
                    /*
                    tree* termNode = MakeTree(TERM);
                    AddChild(termNode, $1);
                    $$ = termNode;
                    */
                    $$ = $1;
                 }
                | term mulOp factor 
                 {
                    /*
                    tree* termNode = MakeTree(TERM);
                    AddChild(termNode, $1);
                    AddChild(termNode, $2);
                    AddChild(termNode, $3);
                    $$ = termNode;
                    */
                    tree* mulOpNode = $2;
                    AddChild(mulOpNode, $1);
                    AddChild(mulOpNode, $3);
                    $$ = mulOpNode;
                 }
                ;
mulOp           : OPER_MUL
                 {
                    $$ = MakeTreeWithVal(MULOP, MUL);
                 }
                | OPER_DIV
                 {
                    $$ = MakeTreeWithVal(MULOP, DIV); 
                 }
                ;
factor          : LPAREN expression RPAREN
                 {
                    tree* factorNode = MakeTree(FACTOR);
                    AddChild(factorNode, $2);
                    $$ = factorNode;
                 }
                | var 
                 {
                    tree* factorNode = MakeTree(FACTOR);
                    AddChild(factorNode, $1);
                    $$ = factorNode;
                 }
                | funcCallExpr
                 {
                    tree* factorNode = MakeTree(FACTOR);
                    AddChild(factorNode, $1);
                    $$ = factorNode;
                 }
                | INTCONST
                 {
                    /*
                    tree* factorNode = MakeTree(FACTOR);
                    AddChild(factorNode, MakeTreeWithVal(INTEGER, $1));
                    $$ = factorNode;
                    */
                    tree* intConstNode = MakeTreeWithVal(INTEGER, $1);
                    $$ = intConstNode;
                 }
                | CHARCONST
                 {
                    tree* factorNode = MakeTree(FACTOR);
                    AddChild(factorNode, MakeTreeWithVal(CHAR, $1));
                    $$ = factorNode;
                 }
                | STRCONST
                 {
                    tree* factorNode = MakeTree(FACTOR);
                    int exists = -1; 
                    int key = STInsert($1, CHAR_TYPE, SCALAR, &scope, &exists);
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = MakeTreeWithVal(CHAR, key);
                    childNode->data_type   = currentScope->strTable[key]->data_type;
                    childNode->sym_type    = currentScope->strTable[key]->symbol_type;
                    childNode->scope       = currentScope->strTable[key]->scope;
                    childNode->childNumber = currentScope->strTable[key]->childNumber;
                    AddChild(factorNode, childNode);
                    $$ = factorNode;
                 }
                ;
funcCallExpr    : ID LPAREN funcCallExprNT
                 {
                    tree* funcCallExprNode = MakeTree(FUNCCALLEXPR);
                    int key = -1; 
                    symEntry* entry = STLookup($1, &key); 

                     //Make sure that the output() function does not throw any errors
                     if(!strcmp($1, "output") && key == -1){
                        table_node* temp = currentScope;
                        int tempScope = 1;
                        int exists = -1;
                        //set scope to be global
                        currentScope = symbolTableHead;
                        key = STInsert("output", INT_TYPE, FUNCTION, &tempScope, &exists);
                        NewScope();
                        tempScope++;
                        //insert arg (a single integer)
                        STInsert("arg1", INT_TYPE, SCALAR, &tempScope, &exists);
                        AddParam(INT_TYPE, SCALAR);
                        UpScope();
                        ConnectParams(key);
                        //return to original scope
                        currentScope = temp;
                        entry = STLookup($1, &key);
                     }

                    tree* childNode = MakeTreeWithVal(IDENTIFIER, key);
                    if(key == -1){
                        yyerror("Undefined function");
                    }
                    else{
                        childNode->data_type   = entry->data_type;
                        childNode->sym_type    = entry->symbol_type;
                        childNode->scope       = entry->scope;
                        childNode->childNumber = entry->childNumber;

                        if(CountArguments() < symbolTableHead->strTable[key]->size){
                            yyerror("Too few arguments provided in function call.");
                        }
                        else if(CountArguments() > symbolTableHead->strTable[key]->size){
                            yyerror("Too many arguments provided in function call.");
                        }
                        else{
                            param* currentParam = symbolTableHead->strTable[key]->params; 
                            arg* currentArg = argumentListHead; 
                            while(currentParam != NULL && currentArg != NULL){
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
                        ResetArguments();
                    }
                    AddChild(funcCallExprNode, childNode);
                    AddChild(funcCallExprNode, $3);
                    $$ = funcCallExprNode;
                 }
                ;
funcCallExprNT   : argList RPAREN
                 {
                    $$ = $1;
                 }
                | RPAREN
                 {
                    $$ = NULL; 
                 }
                ;
argList         : expression
                 {
                    tree* argListNode = MakeTree(ARGLIST);
                    AddChild(argListNode, $1);
                        int desiredArgumentDataType = -1;
                        int desiredArgumentSymbolType = -1;
                        tree *placeholderTreeNode = argListNode;
						while (placeholderTreeNode->numChildren > 0) {
							placeholderTreeNode = placeholderTreeNode->children[0];
						}
                        if (placeholderTreeNode->nodeKind == IDENTIFIER) { //"var" non-terminal
						  if (currentScope->strTable[placeholderTreeNode->val]->symbol_type == FUNCTION) {
						    desiredArgumentDataType = INT_TYPE; 
						  } 
                          else { //scalar or array
                            desiredArgumentDataType = currentScope->strTable[placeholderTreeNode->val]->data_type;
						    desiredArgumentSymbolType = currentScope->strTable[placeholderTreeNode->val]->symbol_type;
						  }
						} 
                        else { //constant 
                            if(placeholderTreeNode->nodeKind == INTEGER){
                                desiredArgumentDataType = INT_TYPE; 
                                desiredArgumentSymbolType = SCALAR;
                            }
                            else if(placeholderTreeNode->nodeKind == CHAR){
                                desiredArgumentDataType = CHAR_TYPE; 
                                desiredArgumentSymbolType = SCALAR;
                            }
						}
                        AddArgument(desiredArgumentDataType, desiredArgumentSymbolType);
                    $$ = argListNode;
                 }
                | argList COMMA expression
                 {
                    tree* argListNode = MakeTree(ARGLIST);
                    AddChild(argListNode, $1);
                    AddChild(argListNode, $3);
                    int desiredArgumentDataType = -1;
                    int desiredArgumentSymbolType = -1;
                    tree *placeholderTreeNode = argListNode->children[1];
					while (placeholderTreeNode->numChildren > 0) {
						placeholderTreeNode = placeholderTreeNode->children[0];
					}
                    if (placeholderTreeNode->nodeKind == IDENTIFIER) { //"var" non-terminal
					  if (currentScope->strTable[placeholderTreeNode->val]->symbol_type == FUNCTION) {
					    desiredArgumentDataType = INT_TYPE;
					  } 
                      else { //scalar or array
                        desiredArgumentDataType = currentScope->strTable[placeholderTreeNode->val]->data_type;
					    desiredArgumentSymbolType = currentScope->strTable[placeholderTreeNode->val]->symbol_type;
					  }
					} 
                    else { //constant
                        if(placeholderTreeNode->nodeKind == INTEGER){
                            desiredArgumentDataType = INT_TYPE; 
                            desiredArgumentSymbolType = SCALAR;
                        }
                        else if(placeholderTreeNode->nodeKind == CHAR){
                            desiredArgumentDataType = CHAR_TYPE; 
                            desiredArgumentSymbolType = SCALAR;
                        }
					}
                    AddArgument(desiredArgumentDataType, desiredArgumentSymbolType);
                    $$ = argListNode;
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
