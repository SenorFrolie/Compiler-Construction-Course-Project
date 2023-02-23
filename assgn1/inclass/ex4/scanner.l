
/* definitions */

%{
#include<stdio.h>
#include"tokendef.h"

int scancol = 1;
int yycol = 1;
int scanlineno = 1;


void updateCol();
%}

newline         \n
whitespace      [ \t]+
integer         0|[1-9][0-9]*
integerlead0    0[0-9]+
character       \'([^\']|\\[\\nt\'])\'

identifier      [a-zA-Z][a-zA-Z0-9]*
illidentifier   [0-9]+[a-zA-Z][a-zA-Z0-9]*


%%

 /* rules */

 /* Keywords */;
"if"            {updateCol(); return KWD_IF;}
"void"          {updateCol(); return KWD_VOID;}

 /* Operators */;
\+              {updateCol(); return OPER_ADD;}

 /* Additional Operators */;
@               {updateCol(); return OPER_AT;}
\+\+            {updateCol(); return OPER_INC;}

 /* Punctuation */;
\[              {updateCol(); return LSQ_BRKT;}
;               {updateCol(); return SEMICLN;}

 /* Identifiers */;
{identifier}    {updateCol(); return ID;}
 
 /* Other */;
{whitespace}    {updateCol(); /* skip whitespace */}
.               {return ILLEGAL_TOK;}

%%

/* user routines */

void updateCol(){
    yycol = scancol;
    scancol += yyleng;
}


