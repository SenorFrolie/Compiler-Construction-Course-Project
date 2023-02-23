%{
#include<stdio.h>
#include<tree.h>

int yylineno;
  
enum nodeTypes {PROGRAM, VARDECL, TYPESPEC, ASSIGNSTMT, IFSTMT, EXP, INTEGER, IDENTIFIER};
enum dataType {INT_TYPE};


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
%token <value> SEMI_COLON 
%token <value> KEYWORD_INT
%token <value> ID
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
		printf("%s\n", yylval.strval);
		tree *declNode = maketree(VARDECL);
                addChild(declNode, $1);
                addChild(declNode, maketreeWithVal(IDENTIFIER, $2));
		$$ = declNode;
	      }
            ;  

typeSpecifier : KEYWORD_INT 
                {
		  $$ = maketreeWithVal(TYPESPEC, INT_TYPE);
		}
            ;

stmt        : assignStmt SEMI_COLON
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
             | IFTOKEN LPAREN exp RPAREN stmt ELSETOKEN stmt 
	     {
	        tree *elseNode = maketree(IFSTMT);
		addChild(elseNode, $3);
                addChild(elseNode, $5);
                addChild(elseNode, $7);
		$$ = elseNode;
              }
	    ;

exp 	    : NUM
              {
		tree* expNode = maketree(EXP);
		/* using token value of NUM ($1) in the tree 
                   you should store the actual integer value */
		addChild(expNode, maketreeWithVal(INTEGER, $1));
		$$ = expNode;
              }

            | ID
	      {
		tree* expNode = maketree(EXP);
		addChild(expNode, maketreeWithVal(INTEGER, $1));
		$$ = expNode;
	      }
            ;

var         : ID
              {
		/* using token value of ID ($1) in the tree 
                   you should probably use index of symbol table, or yylval */
		$$ = maketreeWithVal(IDENTIFIER, $1);
	      }
            ;

%%

int yyerror(char * msg) {
  printf("error: %d: %s\n", yylineno, msg);
  return 0;
}


int main() {
  if (!yyparse())
    printAst(ast, 1);
  return 0;
}

     


