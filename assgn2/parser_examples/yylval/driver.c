#include<stdio.h>

int main() {
  if (!yyparse()) 
    printf("ACCEPT\n");
  return 0;
}
