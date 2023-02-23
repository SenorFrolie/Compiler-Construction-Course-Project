/*  scanner.l: Example of using a hashtable to implement Symbol Table */


/* definitions */
%{

#include "tokendef.h"
#include <string.h>

#define STRINGTABLESIZE 20000
#define HASHTABLESIZE 997

struct HashEntry {
   int index;
   struct HashEntry* next;
};

/* This function returns a key for the hash table for the string s */
int hashFunction(char* s);

/* This function searches the string table to see if the string s
has already been stored.  If so, it returns the index into the string
table where it is found.  If s is a new string, it is inserted into the
string table and the index is returned.  */
int hashInsert(char* s);

/* This function prints out all of the entries in the string table, 
one per line */
void printStringTable();

/* This function releases the dynamic memory that was allocated for 
the hash table. */
void deleteHashTable();

int yyline = 1;
int yycol = 1;
int yylval;
%}

%x str1 str2

%%

	char string_buf[100];
	char* buf_ptr;

 /* rules */
if { yycol += 2; return KWD_IF; }
else { yycol += 4; return KWD_ELSE; }
while { yycol += 5; return KWD_WHILE; }
int { yycol += 3; return KWD_INT; }
string { yycol += 6; return KWD_STRING; }
char { yycol += 4; return KWD_CHAR; }
return { yycol += 6; return KWD_RETURN; }
void { yycol += 4; return KWD_VOID; }
\+ { yycol++; return OPER_ADD; }
\- { yycol++; return OPER_SUB; }
\* { yycol++; return OPER_MUL; }
\/ { yycol++; return OPER_DIV; }
\< { yycol++; return OPER_LT; }
\> { yycol++; return OPER_GT; }
\>\= { yycol += 2; return OPER_GTE; }
\<\= { yycol += 2; return OPER_LTE; }
\=\= { yycol += 2; return OPER_EQ; }
\!\= { yycol += 2; return OPER_NEQ; }
\= { yycol++; return OPER_ASGN; }
\[ { yycol++; return LSQ_BRKT; }
\] { yycol++; return RSQ_BRKT; }
\{ { yycol++; return LCRLY_BRKT; }
\} { yycol++; return RCRLY_BRKT; }
\( { yycol++; return LPAREN; }
\) { yycol++; return RPAREN; }
\, { yycol++; return COMMA; }
\; { yycol++; return SEMICLN; }
[a-zA-Z][0-9a-zA-Z]* { yycol += yyleng; 
		       yylval = hashInsert(yytext);
		       return ID; }
[1-9][0-9]*|0/[^0-9] { yycol += yyleng; yylval = atoi(yytext); return INTCONST; }
'[^\n]' { yycol+=3; char c = yytext[1]; yylval = (int)c; return CHARCONST; }

\" { buf_ptr = string_buf; BEGIN(str1); }
<str1>[^\n\\\"]+ { char* yptr = yytext;
		   while (*yptr)
		      *buf_ptr++ = *yptr++; }
<str1>\n {  printf("Error: unterminated string constant. ");
            printf("Starts on line: %d", yyline);
            printf(", column: %d\n", yycol);
            BEGIN(0); }   
<str1>\" { *buf_ptr = '\0';
            int length = strlen(string_buf);
	    buf_ptr = string_buf;
            printf("<STRING>: %s\n", buf_ptr);	  
            yycol += length + 2;
            yylval = hashInsert(buf_ptr);
	    BEGIN(0);
            return STRCONST; }
<str1>\\ { yycol++; BEGIN(str2); }
<str2>t { *buf_ptr++ = '\t'; BEGIN(str1); }
<str2>n { *buf_ptr++ = '\n'; BEGIN(str1); }
<str2>\" { *buf_ptr++ = '"'; BEGIN(str1); }
<str2>\\ { *buf_ptr++ = '\\'; BEGIN(str1); }
<str2>[^tn\"\\] { *buf_ptr++ = yytext[0]; BEGIN(str1); }

\/\* { char current;
       char next = input();
       int lines = 0;
       int col = yycol + 3;
       if (next == '\n') { 
          lines++;
          col = 1;
       }
       if (next <= 0) {
          printf("Error: unterminated comment. ");
          printf("Starts on line: %d", yyline);
          printf(", column: %d\n", yycol);
       }
       else  {
          current = next;
          next = input();
          col++;
          while ((next > 0) && !((current == '*') && (next == '/'))) {
             if (next == '\n') {
                lines++;
                col = 1; 
             }
             current = next;
             next = input();
             col++;
          }
          if (next <= 0) {
             printf("Error: unterminated comment. ");
             printf("Starts on line: %d", yyline);
             printf(", column: %d\n", yycol);
          }
          else {
             yyline = yyline + lines;
             yycol = col;
             printf("comment %s", yytext);
          }
       }
     }

" " { yycol++; }
\t { yycol += 8; }
\n { yycol = 1; yyline++; }
. { printf("Error: illegal token.  Line: %d", yyline);
    printf(", Column: %d\n", yycol);
    char c = input();
    yycol += 2;
    while ((c > 0) && (c != ' ') && (c != '\t') && (c != '\n')) { 
       c = input();
       yycol++;
    }
    if (c == '\n') {
      yycol = 1;
      yyline++;
    }
  } 
%%

/* code */

struct HashEntry* hashTable[HASHTABLESIZE];
char stringTable[STRINGTABLESIZE];
int stIndex = 0;

int hashFunction(char* s) {
   int total = 0;
   int i = 0;
   while (s[i] !='\0') {
      total = ((total * 17) + s[i]) % HASHTABLESIZE;
      i++;
   }
   return total;
}

int hashInsert(char* s) {
   int tableIndex;  /* return value */
   int i = hashFunction(s);
   int length = strlen(s);

   if (hashTable[i] == NULL) {
      hashTable[i] = (struct HashEntry*) malloc(sizeof(struct HashEntry));
      hashTable[i]->index = stIndex;
      hashTable[i]->next = NULL; 
      strcpy(stringTable+stIndex, s);
      tableIndex = stIndex;
      stIndex += length + 1;
   }
   else {     /* search present entries */
      struct HashEntry* current = hashTable[i];
      int found = 0;
      int marker;

      while ((found == 0) && (current != NULL)) {
         int match = strncmp(stringTable+current->index, s, length);
         if ((match == 0) && stringTable[current->index+length] == '\0') {
            found = 1;
            marker = current->index;
         }
         current = current->next;   
      }

      if (found == 1) 
         tableIndex = marker;
      else {  /*insert at head of linked list */
         current = (struct HashEntry*) malloc(sizeof(struct HashEntry));
         current->next = hashTable[i]->next;
         hashTable[i]->next = current;
         current->index = stIndex;
         strcpy(stringTable+stIndex, s);
         tableIndex = stIndex;
         stIndex += length + 1;         
      }
   } 
   return tableIndex;
}

void printStringTable() {
   int i;
   printf("String Table:\n");
   for (i = 0; i < stIndex; i++) {
      if (stringTable[i] == '\0')
         putchar('\n');
      else
         putchar(stringTable[i]);
   }
}


void deleteHashTable() {
   int i;
   for (i = 0; i < HASHTABLESIZE; i++) {
      if (hashTable[i]) {
         struct HashEntry* h_ptr1 = hashTable[i];
	 struct HashEntry* h_ptr2 = hashTable[i];
	 while (h_ptr1) {
            h_ptr1 = h_ptr1->next;
            free(h_ptr2);
            h_ptr2 = h_ptr1;
         } 
      }

   }

}
