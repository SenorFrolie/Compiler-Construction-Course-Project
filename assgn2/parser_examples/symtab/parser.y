%{
#include<stdio.h>
#include<tree.h>

int yylineno;
  

enum dataType {INT};


tree *ast;  /* pointer to AST root */

%}

%union 
{
  int value;
  struct treenode *node;
  char *strval;
}

%token <value> IFTOKEN ELSETOKEN
%token <value> LPAREN RPAREN 
%token <value> ASSIGN_OP 
%token <value> ADD_OP 
%token <value> SEMI_COLON 
%token <value> KEYWORD_INT
%token <strval> ID
%token <value> NUM
%token <value> ERROR
%token <value> ILLEGAL_TOK

%type <node> program varDecl typeSpecifier stmt assignStmt ifStmt exp var 

%start program 

%%

program     : varDecl stmt 
              {
		tree *progNode = maketree(PROGRAM);
                addChild(progNode, $1);
		addChild(progNode, $2);
	        ast = progNode;
	      }
            ;

varDecl     : typeSpecifier ID SEMI_COLON
              {
	        
		tree *declNode = maketree(VARDECL);
                addChild(declNode, $1);
		int index = ST_insert($2);
                addChild(declNode, maketreeWithVal(IDENTIFIER, index));
		$$ = declNode;
	      }
            ;  

typeSpecifier : KEYWORD_INT 
                {
		  $$ = maketreeWithVal(TYPESPEC, INT);
		}
            ;

stmt        : assignStmt SEMI_COLON {
 }
            | ifStmt
            ;

assignStmt  : var ASSIGN_OP exp 
              {
		tree *assignNode = maketree(ASSIGNSTMT);
		addChild(assignNode, $1);
                addChild(assignNode, $3);
		$$ = assignNode;
	      }
            ;
ifStmt      : IFTOKEN LPAREN exp RPAREN stmt
              {
		tree *ifNode = maketree(IFSTMT);
		addChild(ifNode, $3);
                addChild(ifNode, $5);
		$$ = ifNode;
	      }
	    ;

exp 	    : NUM 
              {
		tree* expNode = maketree(EXP);
		addChild(expNode, maketreeWithVal(INTEGER, $1));
		$$ = expNode;
              }
              | exp ADD_OP exp
              {
		tree* expNode = maketree(ADD_EXP);
		addChild(expNode, $1);
		addChild(expNode, $3);
		$$ = expNode;
              }

            | ID
	      {
		tree* expNode = maketree(EXP);
		int index = ST_lookup($1);
		addChild(expNode, maketreeWithVal(IDENTIFIER, index));
		$$ = expNode;
	      }
            ;

var         : ID
              {
		int index = ST_lookup($1);
		$$ = maketreeWithVal(IDENTIFIER, index);
	      }
            ;

%%

int ST_insert(char *id) {
  return 1;
}

int ST_lookup(char *id) {
  return 1;
}

int yyerror(char * msg) {
  printf("error: %d: %s\n", yylineno, msg);
  return 0;
}


int main() {
  if (!yyparse())
    printAst(ast, 1);
  return 0;
}

