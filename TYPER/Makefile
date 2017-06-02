CC=gcc
CFLAGS=-Wall -g
LXP = LEXPARSER 

all: typer

typer: ../PROLOG_GEN/prolog_gen.c typer.c ../AST/ast.c ../EVALUATE/eval.c
	$(CC) $(CFLAGS) -o ../typerProgram typer.c ../PROLOG_GEN/prolog_gen.c ../AST/ast.c ../LEXPARSER/aps.tab.c ../LEXPARSER/lex.yy.c -lfl

clean:
	rm -f *tab.* lex.yy.c eval type *~ *.o tests	
