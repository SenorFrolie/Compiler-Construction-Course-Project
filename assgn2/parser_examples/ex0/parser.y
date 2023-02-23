%{
#include<stdio.h>
%}

%token ID
%token OPER_ADD
%token NUM
%token ILLEGAL_TOK


%%

program    :  expr
           ;

expr       :  expr OPER_ADD expr
	   |  ID
	   |  NUM
	   ;

%%

int myerror(char *msg) {
  printf("ERROR: %s\n", msg);
}


