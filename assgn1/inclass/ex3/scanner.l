
/* definitions */

%{
#include<stdio.h>
#include"tokendef.h"

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

\".*\"            {printf("***\n"); return CHARCONST;} 

{number}       {return INTCONST;}
int|INT        {return KWD_INT;}
"/*"           {
                  char c1, c2;
                  c2 = input();
                  if (c2 <= 0 )
		     printf("error : unfinished comment\n");
		  else { 
		    c1 = c2; 
		    c2 = input();
		    while ((c1 != '*') && (c2 != '/') && (c2  > 0)) {
		      c1 = c2; 
		      c2 = input();
		    }
		    if (c2 <= 0) 
		      printf("error : unfinished comment\n");
		    else 
		      return COMMENT;
		    }
                }
"+"             {return OPER_ADD;}
{newline}       {yyline++;}
{identifier}    {return ID;}
{whitespace}    {/* skip whitespace */}
.               {return ILLEGAL_TOK;}

%%

// user routines 

