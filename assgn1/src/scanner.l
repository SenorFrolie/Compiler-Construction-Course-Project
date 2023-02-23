/* definitions */

%{
#include <stdio.h>
#include <stdlib.h>
#include "tokendef.h"

int scancol = 1;
int yycol = 0;
int yyline = 1;



char* yyerror;

void updateCol(); //TODO: Implement this function to keep track of column numbers
void countLines(); //TODO: Implement this function to keep track of line numbers
int processString(); //TODO: Implement this function to check for illegal escape sequences in string literals and print out escape sequences correctly.
%}

newline        \n
whitespace     [ \t]+
digit 	       [0-9]
integer        0|[1-9]{digit}*
character      \'([^\']|\\[\\nt\'])\'
integerlead0   0[0-9]+

/* String can contain any characters between the double quotes other than a newline or
/* unescaped doublequotes. */
string      \"([^"\n]|\\.)*\"

/* multiline string has \n somewhere in the middle */
multlnstring  	\"([^"\n]|\\.|[\n]+)*\"

/* If the end quote is found, one of the two regexes above will be matched. Otherwise,
/* the string is unterminated. * /
/* */
untermstring 	\"([^"\n]|\\.)*$

comment         \/[\/]+.*
multlncomment   \/*[^\*]\*[^\/]*\/
untermcomment   \/\*([^*]|\*[^/])*

identifier      [A-Za-z_][A-Za-z0-9_]+
illidentifier   [0-9]+[a-zA-Z][a-zA-Z0-9]*

yylineno = 1;

%%

 /* rules */

 /* Keywords */
"if"		{updateCol(); return KWD_IF;}
"else"		{updateCol(); return KWD_ELSE;}
"while"		{updateCol(); return KWD_WHILE;}
"int"		{updateCol(); return KWD_INT;}
"string"	{updateCol(); return KWD_STRING;}
"char"		{updateCol(); return KWD_CHAR;}
"return"	{updateCol(); return KWD_RETURN;}
"void"		{updateCol(); return KWD_VOID;}



 

 /* Operators */
\+	{updateCol(); return OPER_ADD;}
\*	{updateCol(); return OPER_MUL;}
\-    	{updateCol(); return OPER_SUB;}
\/    	{updateCol(); return OPER_DIV;}
\<    	{updateCol(); return OPER_LT;}
\<\=  	{updateCol(); return OPER_LTE;}
\>    	{updateCol(); return OPER_GT;}
\>\=  	{updateCol(); return OPER_GTE;}
\=\=  	{updateCol(); return OPER_EQ;}
\!\=  	{updateCol(); return OPER_NEQ;}
\=    	{updateCol(); return OPER_ASGN;}
\@    	{updateCol(); return OPER_AT;}
\+\+  	{updateCol(); return OPER_INC;}
\-\-  	{updateCol(); return OPER_DEC;}
\&\&  	{updateCol(); return OPER_AND;}
\|\|  	{updateCol(); return OPER_OR;}
\!    	{updateCol(); return OPER_NOT;}
\%\%  	{updateCol(); return OPER_MOD;}		/* check on this one */

 /* Brackets & Parenthesis */
\[      {updateCol(); return LSQ_BRKT;}
\]      {updateCol(); return RSQ_BRKT;}
\{      {updateCol(); return LCRLY_BRKT;}
\}      {updateCol(); return RSQ_BRKT;}
\(      {updateCol(); return LPAREN;}
\)      {updateCol(); return RPAREN;}

 /* Punctuation */
 \,     {updateCol(); return COMMA;}
 \;     {updateCol(); return SEMICLN;}




 /* Identifiers */
{identifier}     {updateCol(); return ID;} 
{illidentifier}  {updateCol();return ERROR;}    

 /* Constants */
{integer}       {updateCol(); return INTCONST;}
{integerlead0}  {updateCol(); yyerror = "Integers may not have leading zeros"; return ERROR;}
{character}     {updateCol(); return CHARCONST;}
{string}        {updateCol(); return processString();}
{multlnstring}  {updateCol(); countLines(); yyerror = "String spans multiple lines"; return ERROR;} // Catching a multi-line string and generating an error.

 /* Comments */
{comment}      	 {updateCol();}//TODO
{untermcomment}  {yyerror = "unterminated comment";  return ERROR;}//TODO
{multlncomment}  {updateCol(); countLines();}

 /* Other */
{newline}       {countLines();}			
{whitespace}    {updateCol();}		
.               {return ILLEGAL_TOK;}

%%

/* user routines */

void updateCol(){
	yycol = scancol;
	scancol += yyleng;
}

void countLines(){
	//TODO: Count the number of lines
	for(int i = 0; i < yyleng;i++){
		if(yytext[i] == '\n'){
			yylineno++;
			yycol = 1;
			scancol =1;
		}
	}
}

int processString(){
    char* stringLiteral = (char*)malloc(yyleng*sizeof(char)); 
    char c1, c2;
    int i = 0; 
    c2 = yytext[0];
    int numEscSeq = 0; //tracks number of escape sequences in yytext
    int escSeq; //tracks whether c1 and c2 detect an escape sequence or not
    while(yytext[i]){
        escSeq=0;
        c1 = yytext[i];
        c2 = yytext[i+1];
        if (c1 == '\\' && (c2 == 't' || c2 == 'n' || c2 == '"' || c2 == '\\')){
		switch(c2){
                case 't':
                    stringLiteral[i-numEscSeq] = '\t';
                    i++;
                    escSeq=1;
                    break;
                case 'n':
                    stringLiteral[i-numEscSeq] = '\n';
                    i++;
                    escSeq=1;
                    break;
                case '"':
                    stringLiteral[i-numEscSeq] = '"';
                    i++;
                    escSeq=1;
                    break;
                case '\\':
                    stringLiteral[i-numEscSeq] = '\\';
                    i++;
                    escSeq=1;
                    break; 
            }
        }
	else if (c1 == '\\' && c2 != 't' && c2 != 'n' && c2 != '"' && c2 != '\\'){ //if escape sequence is invalid
            yyerror = "Unrecognized escape character in String";
            yycol += i+1+numEscSeq;
            return ERROR;
        }
        else{
            stringLiteral[i-numEscSeq] = yytext[i];
        }
        if(escSeq == 1){
        numEscSeq++;
        }
    i++;
    }
    stringLiteral[yyleng-numEscSeq] = '\0'; 
    yytext = stringLiteral; 
    return STRCONST;
}





