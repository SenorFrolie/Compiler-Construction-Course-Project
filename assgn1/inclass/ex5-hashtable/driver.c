/*  driver.c */

#include "tokendef.h"
#include <stdio.h>

extern char* yytext;
extern int yylval;
extern void printStringTable();
extern void deleteHashTable();

int main() {   
   int val;
   while ( (val = yylex()) ) {
      if (val == KWD_IF)
         printf("<KEYWORD, if>\n");
      else if (val == KWD_ELSE)
         printf("<KEYWORD,  else>\n");
      else if (val == KWD_WHILE)
         printf("<KEYWORD, while>\n");
      else if (val == KWD_INT)
         printf("<KEYWORD, int>\n");
      else if (val == KWD_STRING)
         printf("<KEYWORD, string>\n");
      else if (val == KWD_CHAR)
         printf("<KEYWORD, char>\n");
      else if (val == KWD_RETURN)
         printf("<KEYWORD, return>\n");
      else if (val == KWD_VOID)
         printf("<KEYWORD, void>\n");
      else if (val == ID) {
	printf("<ID, %s>\n", yytext);
	//         printf(">\n");
      }
      else if (val == INTCONST) {
         printf("<INT, %d", yylval);
         printf(">\n");
      }
      else if (val == CHARCONST) {
         printf("<CHAR, %s", yytext);
         printf(" ascii = %d", yylval);
         printf(">\n");
      }
      else if (val == STRCONST) {
      }
      else if (val == OPER_ADD)
         printf("<OP, +>\n");
      else if (val == OPER_SUB)
         printf("<OP, ->\n");
      else if (val == OPER_MUL)
         printf("<OP, *>\n");
      else if (val == OPER_DIV)
         printf("<OP, />\n");
      else if (val == OPER_LT)
         printf("<OP, <>\n");
      else if (val == OPER_GT)
         printf("<OP, >>\n");
      else if (val == OPER_GTE)
         printf("<OP, >=>\n");
      else if (val == OPER_LTE)
         printf("<OP, <=>\n");
      else if (val == OPER_EQ)
         printf("<OP, ==>\n");
      else if (val == OPER_NEQ)
         printf("<OP, !=>\n");
      else if (val == OPER_ASGN)
         printf("<OP, =>\n");
      else if (val == LSQ_BRKT)
         printf("<OP, [>\n");
      else if (val == RSQ_BRKT)
         printf("<OP, ]>\n");
      else if (val == LCRLY_BRKT)
         printf("<OP, {>\n");
      else if (val == RCRLY_BRKT)
         printf("<OP, }>\n");
      else if (val == LPAREN)
         printf("<OP, (>\n");
      else if (val == RPAREN)
         printf("<OP, )>\n");
      else if (val == COMMA)
         printf("<SEP, ,>\n");
      else if (val == SEMICLN)
         printf("<SEP, ;>\n");

   }
   //   printStringTable();
   deleteHashTable();
}
