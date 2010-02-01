BINDIR	= ./
DBG		= $(CFLAGS) -g3 -D__DEBUG__
REL		= $(CFLAGS) -O2
BIN		= primer
CC		= gcc
CFLAGS	= -std=gnu99

main: parser lexer utils vm eval
	$(CC) $(DBG) $(CFLAGS) -c y.tab.c lex.yy.c
	$(CC) $(DBG) *.o -o $(BINDIR)$(BIN)

eval: eval.c eval.h
	$(CC) $(DBG) $(CFLAGS) -c eval.c

vm: vm.c vm.h
	$(CC) $(DBG) $(CFLAGS) -c vm.c

utils: main.c main.h
	$(CC) $(DBG) $(CFLAGS) -c main.c
	
lexer: parser
	flex lexer.l

parser:
	bison -y -d parser.y

clean:
	rm lex.yy.c y.tab.c y.tab.h *.o primer
	
cleanwin:
	del lex.yy.c y.tab.c y.tab.h *.o primer.exe