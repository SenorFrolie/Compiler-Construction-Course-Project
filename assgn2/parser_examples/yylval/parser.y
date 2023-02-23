%{
#include<stdio.h>

int yylineno;
%}

%union 
{
  int value;
  char *strval;
}

%token  IFTOKEN ELSETOKEN
%token  LPAREN RPAREN 
%token  ASSIGN_OP 
%token  SEMI_COLON 
%token  KEYWORD_INT
%token  ID
%token  INTCONST
%token  ERROR
%token  ILLEGAL_TOK

%start program 

%%

program     : varDecl stmt 
            ;

varDecl     : typeSpecifier ID SEMI_COLON
              {
		printf("ID = %s\n", yylval.strval);
	      }
            ;  

typeSpecifier : KEYWORD_INT 
              ;

stmt        : assignStmt SEMI_COLON
            | ifStmt
            ;

assignStmt  : var ASSIGN_OP exp 
            ;
ifStmt      : IFTOKEN LPAREN exp RPAREN stmt
            | IFTOKEN LPAREN exp RPAREN stmt ELSETOKEN stmt 
	    ;

exp 	    : INTCONST
              {
		printf("INT VAL = %d\n", yylval.value);
		
              }

            | ID
            ;

var         : ID
            ;

%%

int yyerror(char * msg) {
  printf("error: %d: %s\n", yylineno, msg);
  return 0;
}


