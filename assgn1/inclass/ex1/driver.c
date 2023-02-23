#include<tokendef.h>
#include<stdio.h>

void printToken(int tokenNum) {
  switch(tokenNum) {
  case ID:
    printf("<ID>\n");
    break;
  case INTCONST:
    printf("<INTEGER>\n");
    break;
  case KWD_INT:
    printf("<INT KEYWORD>\n");
    break;
  case OPER_ADD:
    printf("<ADD OPERATOR>\n");
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
