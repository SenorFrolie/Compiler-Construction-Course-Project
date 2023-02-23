#include<tokendef.h>
#include<strtab.h>
#include<stdio.h>



int main() {
  yyparse();
  return 0;
}

// extern int yylval;

/* int main() { */
/*   int val; */

/*   while(val = yylex()){ */
/*     switch(val) { */
/*     case KWD_IF: */
/*         printf("<KWD_IF, if>\n"); */
/* 	break; */
/*     case KWD_ELSE: */
/*         printf("<KWD_ELSE, else>\n"); */
/* 	break; */
/*     case STRCONST: */
/* 	/\* add 1 to char *, so that " is not printed  */
/*            ideally, not want to store the " (double quotes) at all *\/  */
/* 	printf("<STRCONST, %s>\n", strTable[yylval] + 1); */
/*     case ERROR: */
/* 	/\* error msg generated from inside lex, do nothing here *\/ */
/* 	break; */
/*     default: */
/* 	/\* do nothing *\/; */
/*     } */
/*   } */

/*   return 0;  */
/* } */
