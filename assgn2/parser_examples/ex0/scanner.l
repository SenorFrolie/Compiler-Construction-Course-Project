
/* definitions */

%{
#include<y.tab.h>
#include<stdio.h>

int yyline = 1;

%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+


%%

 /* rules */ 

{number}        {return NUM;}
{identifier}    {return ID;}
"+"             {return OPER_ADD;}

{newline}       {yyline++;}
{whitespace}    {/* skip whitespace */}
.               {return ILLEGAL_TOK;}

%%

// user routines 

