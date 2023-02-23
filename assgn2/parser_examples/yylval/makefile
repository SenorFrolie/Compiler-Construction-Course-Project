GRAMMAR = parser.y

CFLAGS = -w -I. -funsigned-char -g -DYYDEBUG 	
YFLAGS = -v -d

mcc: y.tab.o lex.yy.o driver.o
	gcc $(CFLAGS) -o mcc y.tab.o lex.yy.o driver.o -ll -ly

y.tab.o: y.tab.c y.tab.h 
	gcc $(CFLAGS) -c y.tab.c 

y.tab.c: $(GRAMMAR)
	yacc $(YFLAGS) $(GRAMMAR)

lex.yy.o: lex.yy.c y.tab.h 
	gcc $(CFLAGS) -c lex.yy.c

lex.yy.c: scanner.l
	lex scanner.l

driver.o: driver.c 
	gcc $(CFLAGS) -c driver.c

clean:
	rm -f y.tab.* y.output lex.yy.* *.o *~ mcc     



