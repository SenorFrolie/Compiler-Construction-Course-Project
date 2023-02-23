#include<tokendef.h>
#include<stdio.h>
#include<stdlib.h>

extern int yycol;
extern int yylineno;

extern char* yytext;

void printToken(int tokenNum) {
  int val;
  switch(tokenNum) {
  case ID:
      printf("<ID, %s>\n", yytext);
    break;
  case INTCONST:
      val = atoi(yytext);
      printf("<INTEGER, %d>\n", val);
      break;
  case KWD_INT:
      printf("<INT_KEYWORD> : (%d:%d)\n", yylineno, yycol);
    break;
  default:
    printf("<ILLEGAL TOKEN>\n");
  }
  return;
}

int main() {

  int ret = yylex();
  while (ret) {
    printToken(ret);
    ret = yylex();    
  }
  return 0;
}
