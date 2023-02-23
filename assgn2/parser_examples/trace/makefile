GRAMMAR = parser.y
CC = gcc
CFLAGS = -I. 
YFLAGS = -v -d 
YACC = yacc # bison

mcc: y.tab.o lex.yy.o driver.o
	gcc $(CFLAGS) -o mcc driver.o y.tab.o lex.yy.o -ll

y.tab.o: y.tab.c
	gcc $(CFLAGS) -c y.tab.c 

y.tab.c: $(GRAMMAR)
	$(YACC) $(YFLAGS) $(GRAMMAR)

lex.yy.o: lex.yy.c
	gcc $(CFLAGS) -c lex.yy.c

lex.yy.c: scanner.l
	lex scanner.l

driver.o: driver.c
	$(CC)  $(CFLAGS) -c driver.c 
clean:
	rm -f y.tab.* y.output lex.yy.* *.o *~ mcc     



