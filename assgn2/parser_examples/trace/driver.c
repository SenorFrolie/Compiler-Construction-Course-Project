#include<strtab.h>
#include<stdio.h>



int main() {
    if (yyparse())
	printf("REJECT\n");
    else
	printf("ACCEPT\n");

  return 0;
}
