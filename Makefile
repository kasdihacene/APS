CC=gcc
CFLAGS=-Wall -g
LXP = LEXPARSER
TYP = TYPER

all: eval type

parserlexer: 
	cd $(LXP) && $(MAKE)

typer: 
	cd $(TYP) && $(MAKE)

evaluate: LEXPARSER/aps.tab.c AST/ast.c LEXPARSER/lex.yy.c EVALUATE/eval.c
	$(CC) $(CFLAGS) -o evaluator LEXPARSER/aps.tab.c AST/ast.c LEXPARSER/lex.yy.c EVALUATE/eval.c -lfl

clean:
	rm -f LEXPARSER/*tab.* LEXPARSER/lex.yy.c
	rm -f typerProgram
	rm -f pl_generated.pl
	rm -f evaluator
