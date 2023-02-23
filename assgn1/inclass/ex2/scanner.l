
/* definitions */

%{
#include<stdio.h>
#include<tokendef.h>

int yycol = 0;

%}

digit [0-9]
number 0|[1-9]{digit}*
letter [a-zA-Z]
identifier {letter}+

newline     \n
whitespace  [ \t]+

yylineno = 1;

%%

 /* rules */ 

"int"        {yycol = yycol + yyleng; return KWD_INT;}

{identifier} {return ID; }
{number}     {return INTCONST; }

{newline}       {yylineno++; yycol = 0;}
{whitespace}    {/* skip whitespace */}
.               {return ILLEGAL_TOK;}

%%

/* user routines */

