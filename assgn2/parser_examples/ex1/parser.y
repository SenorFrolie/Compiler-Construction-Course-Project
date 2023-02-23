%{
#include<stdio.h>
extern int yylineno;
%}

%token ID
%token OPER_ADD
%token NUM
%token ILLEGAL_TOK


%%

program    :  exprList
           ;

exprList   : expr exprList
           | 

expr       :  expr OPER_ADD expr
	   |  ID
	   |  NUM
	   ;

%%

int yyerror(char *msg) {
  printf("Syntax error at line %d\n ", yylineno);
}

