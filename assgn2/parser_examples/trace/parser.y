%{
#include<stdio.h>
extern int yylineno;
%}

%token ID
%token NUM
%token OPER_ADD
%token ILLEGAL_TOK
%token WHITESPACE


%%

program     : exprList 
              { printf("done: program -> exprList\n"); }
            ;

exprList    : expr 
                 {printf("done: exprList -> expr ...\n");}
              exprList
                 {printf("done: exprList -> expr exprList\n");}
            |
            ;

expr        : expr OPER_ADD expr
              { printf("done: expr -> expr + expr \n"); }
            | ID
              { printf("done: expr -> ID \n"); }
            | NUM
	      {  
                printf("done: expr -> NUM \n"); 
	      }
;

%%

int yyerror(char * msg) {
  printf("Syntax error at line %d\n", yylineno);
}




