#include<tokendef.h>
#include<stdio.h>

extern int yyline;
extern char *yytext;

void printError() {
  printf("Error at line no %d\n", yyline);
}

printToken(int tokenNum) {
  switch(tokenNum) {
  case ID:
    printf("<ID, %s>\n", yytext);
    break;
  case CHARCONST:
    printf("<CHAR>\n");
    break;
  case INTCONST:
    printf("<INTEGER>\n");
    break;
  case KWD_INT:
    printf("<INT KEYWORD>\n");
    break;
  case KWD_WHILE:
    printf("<INT WHILE>\n");
    break;
  case COMMENT:
    printf("<COMMENT>\n");
    break;
  default:
    printError();
    printf("<ILLEGAL TOKEN>\n");
  }
}



int main() {

  int ret = yylex();
  while (ret) {
      //    printf("%s\n", yytext);
    printToken(ret);
    ret = yylex();    
  }
  return 0;
}
