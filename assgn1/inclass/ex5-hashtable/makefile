# makefile

scanner: driver.o lex.yy.o
	gcc -o scanner driver.o lex.yy.o -ll

driver.o: driver.c tokendef.h
	gcc -c driver.c

lex.yy.o: lex.yy.c tokendef.h
	gcc -c lex.yy.c

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm *.o scanner
