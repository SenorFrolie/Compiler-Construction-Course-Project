
%{
#include<y.tab.h>
extern  int yylineno;

%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"if"            {return IFTOKEN;}
"else"          {return ELSETOKEN;}
"int"           {return KEYWORD_INT;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"="             {return ASSIGN_OP;}
";"             {return SEMI_COLON;}
{number}        {yylval.value = atoi(yytext); return INTCONST;}
{identifier}    {
                   yylval.strval = (char *) malloc(yyleng *sizeof(char)); 
                   strcpy(yylval.strval, yytext);
		   return ID;
                }

{newline}       {yylineno++;}
{whitespace}    {/* skip whitespace */}
.               {return ERROR;}

%%



