BISON= bison -d
FLEX= flex

all: parser lexer

parser: aps.y
	$(BISON) aps.y

lexer: aps.l
	$(FLEX) aps.l

clean:
	rm -f *tab.* lex.yy.c
