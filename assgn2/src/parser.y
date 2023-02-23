%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<../src/tree.h>
#include<../src/strtab.h>

int yydebug=1;
extern int yylineno;
/* nodeTypes refer to different types of internal and external nodes that can be part of the abstract syntax tree.*/

enum nodeTypes {PROGRAM, DECLLIST, DECL, VARDECL, VARDECLNT, TYPESPEC, FUNDECL, FUNCTYPENAME, 
				FUNDECLNT, FORMALDECLLIST, FDECLLISTNT, FORMALDECL, FDECLNT,
				FUNBODY, LOCALDECLLIST, STATEMENTLIST, STATEMENT, COMPOUNDSTMT, ASSIGNSTMT, 
				CONDSTMT, CONDSTMTNT, LOOPSTMT, RETURNSTMT, RETSTMTNT,  EXPRESSION, RELOP, 
				ADDEXPR, ADDOP, TERM, MULOP, FACTOR, FUNCCALLEXPR, FUNCCALLNT,  
				ARGLIST, INTEGER, IDENTIFIER, VAR, VARNT,  ARRAYDECL, CHAR, 
				};

enum opType {ADD, SUB, MUL, DIV, LT, LTE, EQ, GTE, GT, NEQ};

//enum types {program declList decl varDecl funDecl typeSpecifier formalDeclList formalDecl funBody localDeclList statementList statement compoundStmt assignStmt condStmt loopStmt returnStmt var expression relop addExpr addop term mulop factor funcCallExpr argList};

/* NOTE: mC has two kinds of scopes for variables : local and global. Variables declared outside any
function are considered globals, whereas variables (and parameters) declared inside a function foo are local to foo. You should update the scope variable whenever you are inside a production that matches function definition (funDecl production). The rationale is that you are entering that function, so all variables, arrays, and other functions should be within this scope. You should pass this variable whenever you are calling the ST_insert or ST_lookup functions. This variable should be updated to scope = "" to indicate global scope whenever funDecl finishes. Treat these hints as helpful directions only. You may implement all of the functions as you like and not adhere to my instructions. As long as the directory structure is correct and the file names are correct, we are okay with it. */
char* scope = "";
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
/* TODO: Add the rest of the tokens below.*/
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
%token <value> LSQ_BRKT
%token <value> RSQ_BRKT
%token <value> LCRLY_BRKT
%token <value> RCRLY_BRKT
%token <value> LPAREN
%token <value> RPAREN
%token <value> COMMA
%token <value> SEMICLN
%token <value> OPER_AT
%token <value> OPER_INC
%token <value> OPER_DEC
%token <value> OPER_AND
%token <value> OPER_OR
%token <value> OPER_NOT
%token <value> OPER_MOD
%token <value> ERROR
%token <value> ILLEGAL_TOKEN


/* TODO: Declate non-terminal symbols as of type node. Provided below is one example. node is defined as 'struct treenode *node' in the above union data structure. This declaration indicates to parser that these non-terminal variables will be implemented using a 'treenode *' type data structure. Hence, the circles you draw when drawing a parse tree, the following lines are telling yacc that these will eventually become circles in an AST. This is one of the connections between the AST you draw by hand and how yacc implements code to concretize that. We provide with two examples: program and declList from the grammar. Make sure to add the rest.  */

%type <node> program declList decl varDecl varDeclNT funDecl funDeclNT typeSpecifier formalDeclList 
			 fDeclListNT formalDecl fDeclNT funcTypeName funBody localDeclList statementList statement 
			compoundStmt assignStmt condStmt condStmtNT loopStmt returnStmt retStmtNT var varNT expression 
			relOp addExpr addOp term mulOp factor funcCallExpr funcCallNT argList



%start program

%%
/* TODO: Your grammar and semantic actions go here. We provide with two example productions and their associated code for adding non-terminals to the AST.*/

program         : declList
                 {
//					printf("Top of program : declList\n");
                    tree* progNode = maketree(PROGRAM);
                    addChild(progNode, $1);
                    ast = progNode;
                 }
                ;

declList        : decl
                 {
//					printf("Top of declList : decl\n");
                    tree* declListNode = maketree(DECLLIST);
                    addChild(declListNode, $1);
                    $$ = declListNode;
				}
                | declList decl
                 {
//					printf("Top of declList : declList decl\n");
                    tree* declListNode = maketree(DECLLIST);
                    addChild(declListNode, $1);
                    addChild(declListNode, $2);
                    $$ = declListNode;
				 }
                ;
decl			: varDecl
				 {
//					printf("Top of decl : varDecl\n");
					tree* declNode = maketree(DECL);
					addChild(declNode, $1);
					$$ = declNode;
				 }
				| funDecl
				 {
//					printf("Top of decl : funDecl\n");
					tree* declNode = maketree(DECL);
					addChild(declNode, $1);
					$$ = declNode;
				 }
				;
varDecl			: typeSpecifier ID varDeclNT
				 {
//					printf("Top of varDecl : typeSpecifier ID varDeclNT\n");
					tree* varDeclNode = maketree(VARDECL);
					addChild(varDeclNode, $1);
					int key = ST_insert($2,scope, varDeclNode->children[0]->val, SCALAR);
					addChild(varDeclNode, maketreeWithVal(IDENTIFIER,key));
					addChild(varDeclNode, $3);
					$$ = varDeclNode;
				 }
				;
varDeclNT		: LSQ_BRKT INTCONST RSQ_BRKT SEMICLN
				 {
//					printf("Top of varDeclNT : LSQ_BRKT INSTCONST RSQ_BRKT SEMICLN\n");
					$$ = maketreeWithVal(INTEGER,$2);
				 }
				| SEMICLN
				 {
//					printf("Top of varDeclNT : SEMICLN\n");
					$$ = NULL;
				 }
				;
typeSpecifier	: KWD_INT
				 {
//					printf("Inside typeSpecifier : KWD_INT\n");
					$$ = maketreeWithVal(TYPESPEC, INT_TYPE);
				 }
				| KWD_CHAR
				 {
//					printf("Inside typeSpecifier : KWD_CHAR\n");
					$$ = maketreeWithVal(TYPESPEC, CHAR_TYPE);
				 }
				| KWD_VOID
				 {
//					printf("Inside typeSpecifier : KWD_VOID\n");
					$$ = maketreeWithVal(TYPESPEC, VOID_TYPE);
				 }
				;				
funDecl			: funcTypeName LPAREN formalDeclList RPAREN funBody
				 {
//					printf("Top of funDecl : typeSpecifier ID LPAREN funDeclNT\n");
					scope = "local";
					tree* funDeclNode = maketree(FUNDECL);
					addChild(funDeclNode, $1);
					addChild(funDeclNode, $3);
					addChild(funDeclNode, $5);
					$$ = funDeclNode;
					scope = "";
				 }
				| funcTypeName LPAREN RPAREN funBody
				 {
					scope = "local";
					tree* funDeclNode = maketree(FUNDECL);
					addChild(funDeclNode, $1);
					addChild(funDeclNode, $4);
					$$ = funDeclNode;
					scope = "";
				 }
				;
funcTypeName	: typeSpecifier ID
				 {
					tree* funcTypeNameNode = maketree(FUNCTYPENAME);
					addChild(funcTypeNameNode, $1);
					int key = ST_insert($2, scope, funcTypeNameNode->children[0]->val, FUNCTION);
					addChild(funcTypeNameNode, maketreeWithVal(IDENTIFIER, key));
					$$ = funcTypeNameNode;
				 }
				;
funDeclNT		: formalDeclList RPAREN funBody
				 {
//					printf("Top of funDeclNT : formalDeclList RPAREN funBody\n");
					scope = "local";
					tree* funcTypeNameNode = maketree(FUNCTYPENAME);
					addChild(funcTypeNameNode, $1);
					addChild(funcTypeNameNode, $3);
					$$ = funcTypeNameNode;
					scope = "";
				 }
				| RPAREN funBody
				 {
					scope = "local";
					tree* funcTypeNameNode = maketree(FUNCTYPENAME);
					addChild(funcTypeNameNode, $2);
					$$ = funcTypeNameNode;
					scope = "";
				 }
				;
formalDeclList	: formalDecl fDeclListNT
				 {
//					printf("Top of formalDeclList : formalDecl fDeclListNT\n");
					tree* formalDeclListNode = maketree(FORMALDECLLIST);
					addChild(formalDeclListNode, $1);
					addChild(formalDeclListNode, $2);
					$$ = formalDeclListNode;
				 }
				;
fDeclListNT		:
				 {
//					printf("Top of fDeclList : empty\n");
					$$ = NULL;
				 }
				|  COMMA formalDeclList
				 {
//					printf("Top of fDeclList : COMMA formalDeclList\n");
					$$ = $2;
				 }
				;
formalDecl		: typeSpecifier ID fDeclNT
				 {
					scope = "local";
					tree* formalDeclNode = maketree(FORMALDECL);
					addChild(formalDeclNode, $1);
					int key = ST_insert($2,scope,formalDeclNode->children[0]->val, SCALAR);
					addChild(formalDeclNode, maketreeWithVal(IDENTIFIER,key));
					addChild(formalDeclNode, $3);
					$$ = formalDeclNode;
					scope = "";
				 }
				;
fDeclNT			:
				 {
					$$ = NULL;
				 }
				| LSQ_BRKT RSQ_BRKT
				 {
					$$ = maketree(ARRAYDECL);
				 }
				;
funBody			: LCRLY_BRKT localDeclList statementList RCRLY_BRKT
				 {
					tree* funBodyNode = maketree(FUNBODY);
					addChild(funBodyNode, $2);
					addChild(funBodyNode, $3);
					$$ = funBodyNode;
				 }
				;
localDeclList	:
				 {
					$$ = NULL;
				 } 
				| varDecl localDeclList
				 {
					tree* localDeclListNode = maketree(LOCALDECLLIST);
					addChild(localDeclListNode, $1);
					addChild(localDeclListNode, $2);
					$$ = localDeclListNode;
				 }
				;
statementList	:
				 {
					$$ = NULL;
				 }
				| statement statementList
				 {
					tree* statementListNode = maketree(STATEMENTLIST);
					addChild(statementListNode, $1);
					addChild(statementListNode, $2);
					$$ = statementListNode;
				 }
				;
statement		: compoundStmt
				 {
					tree* statementNode = maketree(STATEMENT);
					addChild(statementNode, $1);
					$$ = statementNode;
				 }
				| assignStmt
				 {
					tree* statementNode = maketree(STATEMENT);
					addChild(statementNode, $1);
					$$ = statementNode;
				 }
				| condStmt
				 {
					tree* statementNode = maketree(STATEMENT);
					addChild(statementNode, $1);
					$$ = statementNode;
				 }
				| loopStmt
				 {
					tree* statementNode = maketree(STATEMENT);
					addChild(statementNode, $1);
					$$ = statementNode;
				 }
				| returnStmt
				 {
					tree* statementNode = maketree(STATEMENT);
					addChild(statementNode, $1);
					$$ = statementNode;
				 }
				;
compoundStmt	: LCRLY_BRKT statementList RCRLY_BRKT
				 {
					tree* compoundStmtNode = maketree(COMPOUNDSTMT);
					addChild(compoundStmtNode, $2);
					$$ = compoundStmtNode;
				
				 }
				;
assignStmt		: var OPER_ASGN expression SEMICLN
				 {
					tree* assignStmtNode = maketree(ASSIGNSTMT);
					addChild(assignStmtNode, $1);
					addChild(assignStmtNode, $3);
					$$ = assignStmtNode;
				 }
				| expression SEMICLN
				 {
					tree* assignStmtNode = maketree(ASSIGNSTMT);
					addChild(assignStmtNode, $1);
					$$ = assignStmtNode;
				 }
				;
condStmt		: KWD_IF LPAREN expression RPAREN statement condStmtNT
				 {
					tree* condStmtNode = maketree(CONDSTMT);
					addChild(condStmtNode, $3);
					addChild(condStmtNode, $5);
					addChild(condStmtNode, $6);
					$$ = condStmtNode;
				 }
				;
condStmtNT		:
				 {
					$$ = NULL;
				 }
				| KWD_ELSE statement
				 {
					$$ = $2;
				 }
				;
loopStmt		: KWD_WHILE LPAREN expression RPAREN statement
				 {
					tree* loopStmtNode = maketree(LOOPSTMT);
					addChild(loopStmtNode, $3);
					addChild(loopStmtNode, $5);
					$$ = loopStmtNode;
				 }
				;
returnStmt		: KWD_RETURN retStmtNT
				 {
					tree* returnStmtNode = maketree(RETURNSTMT);
					addChild(returnStmtNode, $2);
					$$ = returnStmtNode;
				 }
				;
retStmtNT		: SEMICLN
				 {
					$$ = NULL;
			 	 }
				| expression SEMICLN
				 {
					$$ = $1;
				 }
				;
var				: ID varNT
				 {
					tree* varNode = maketree(VAR);
					int key = ST_lookup($1, scope);
					if (key == -1){
						yywarning("usage of undeclared variable");
					}
					addChild(varNode, maketreeWithVal(IDENTIFIER,key));
					addChild(varNode, $2);
					$$ = varNode;
				 }
				;
varNT			: 
				 {
					$$ = NULL;
				 }
				| LSQ_BRKT addExpr RSQ_BRKT
				 {
					$$ = $2;
				 }
				;
expression		: addExpr
				 {
					tree* expressionNode = maketree(EXPRESSION);
					addChild(expressionNode, $1);
					$$ = expressionNode;
				 }
				| expression relOp addExpr
				 {
					tree* expressionNode = maketree(EXPRESSION);
					addChild(expressionNode, $1);
					addChild(expressionNode, $2);
					addChild(expressionNode, $3);
					$$ = expressionNode;
				 }
				;
relOp			: OPER_LTE
				 {
					$$ = maketreeWithVal(RELOP, LTE);
				 }
				| OPER_LT
				 {
					$$ = maketreeWithVal(RELOP, LT);
				 }
				| OPER_GT
				 {
					$$ = maketreeWithVal(RELOP, GT);
				 }
				| OPER_GTE
				 {
					$$ = maketreeWithVal(RELOP, GTE);
				 }
				| OPER_EQ
				 {
					$$ = maketreeWithVal(RELOP, EQ);
				 }
				| OPER_NEQ
				 {
					$$ = maketreeWithVal(RELOP, NEQ);
				 }
				;
addExpr			: term
				 {
					tree* addExprNode = maketree(ADDEXPR);
					addChild(addExprNode, $1);
					$$ = addExprNode;
				 }
				| addExpr addOp term
				 {
					tree* addExprNode = maketree(ADDEXPR);
					addChild(addExprNode, $1);
					addChild(addExprNode, $2);
					addChild(addExprNode, $3);
					$$ = addExprNode;
				 }
				;
addOp			: OPER_ADD
				 {
					$$ = maketreeWithVal(ADDOP, ADD);
				 }
				| OPER_SUB
				 {
					$$ = maketreeWithVal(ADDOP, SUB);
				 }
				;
term			: factor
				 {
					tree* termNode = maketree(TERM);
					addChild(termNode, $1);
					$$ = termNode;
				 }
				| term mulOp factor
				 {
					tree* termNode = maketree(TERM);
					addChild(termNode, $1);
					addChild(termNode, $2);
					addChild(termNode, $3);
					$$ = termNode;
				 }
				;
mulOp			: OPER_MUL
				 {
					$$ = maketreeWithVal(MULOP, MUL);
				 }
				| OPER_DIV
				 {
					$$ = maketreeWithVal(MULOP, DIV);
				 }
				;
factor			: LPAREN expression RPAREN
				 {
					tree* factorNode = maketree(FACTOR);
					addChild(factorNode, $2);
					$$ = factorNode;
				 }
				| var
				 {
					tree* factorNode = maketree(FACTOR);
					addChild(factorNode, $1);
					$$ = factorNode;
				 }
				| funcCallExpr
				 {
					tree* factorNode = maketree(FACTOR);
					addChild(factorNode, $1);
					$$ = factorNode;
				 }
				| INTCONST
				 {
					tree* intConstNode = maketree(FACTOR);
					addChild(intConstNode, maketreeWithVal(INTEGER, $1));
					$$ = intConstNode;
				 }
				| CHARCONST
				 {
					tree* charConstNode = maketree(FACTOR);
					addChild(charConstNode, maketreeWithVal(CHAR, $1));
					$$ = charConstNode;
				 }
				| STRCONST
				 {
					tree* strConstNode = maketree(FACTOR);
					int key = ST_insert($1, scope, CHAR_TYPE, SCALAR);
					addChild(strConstNode, maketreeWithVal(CHAR, key));
					$$ = strConstNode;
				 }
				;
funcCallExpr	: ID LPAREN funcCallNT
				 {
					tree* funcCallExprNode = maketree(FUNCCALLEXPR);
					int key = ST_lookup($1, scope);
					if (key == -1){
						yywarning("usage of undeclared function");
					}
					addChild(funcCallExprNode, maketreeWithVal(IDENTIFIER,key));
					addChild(funcCallExprNode, $3);
					$$ = funcCallExprNode;
				 }
				;
funcCallNT		: argList RPAREN
				 {
					$$ = $1;
			 	 }
				| RPAREN
				 {
					$$ = NULL;
  				 }
				;
argList			: expression
				 {
					tree* argListNode = maketree(ARGLIST);
					addChild(argListNode, $1);
					$$ = argListNode;
				 }
				| argList COMMA expression
				 {
					tree* argListNode = maketree(ARGLIST);
					addChild(argListNode, $1);
					addChild(argListNode, $3);
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
