#include "../LEXPARSER/aps.h"
#include "../PROLOG_GEN/prolog_gen.h"
#include <stdio.h>
#include <stdlib.h>

#define PL_PATH "SRC/pl_generated.pl"

int main(int argc, char* argv[]) {
  FILE* fp;
  // Open program
  if (! (fp = fopen(argv[1], "r"))) {
    fprintf(stderr, "Error, can't open %s\n", argv[1]);
    exit(1);
  }
  // AST!
  Program* prog = ast_from_program(fp);

  // Typing!
  FILE* typefp;

  if (! (typefp = fopen(PL_PATH, "w"))) {
    fprintf(stderr, "Error, can't write prolog typing file %s\n", PL_PATH);
    exit(EXIT_FAILURE);
  }
	  fprintf(stdout, "PRINTING INTO FILE TYPE ...\n");

// CALLING PROLOG_GENERATOR TO PRINT A TYPING PROGRAM IN PROLOG 

  generate_program(typefp, prog);
  fprintf(stdout, "SUCCESS !\n");
  fprintf(stdout, "LOOK AT SRC/pl_generated.pl\n");

  fclose(typefp);
  return 0;
}
