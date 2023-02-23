/* definitions */

%{
#include<stdio.h>
#include<tokendef.h>
%}

digit [0-9]
number 0|[1-9]{digit}*
letter [a-zA-Z]
identifier {letter}+
whitespace [ \n\t]

%%

 /* rules */ 
			 
int|INT { return KWD_INT; }
{identifier} {return ID; }
{number} {return INTCONST; }
\+ { return OPER_ADD; }
{whitespace} { /* skip white space */ }

. { return ILLEGAL_TOK;}

%%
 // user routines 


