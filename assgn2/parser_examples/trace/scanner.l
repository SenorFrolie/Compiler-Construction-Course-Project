
/* definitions */

%{
#include<y.tab.h>
#include<strtab.h>
#include<stdio.h>

%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+


%%

 /* rules */ 

{number}        {yylval = atoi(yytext); return NUM;}
{identifier}    {return ID;}
"+"             {return OPER_ADD;}
{newline}       {yylineno++;}
{whitespace}    {/* skip whitespace */}
.               {return ILLEGAL_TOK;}

%%

// user routines 

