#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../AST/ast.h"
#include "prolog_gen.h"

#ifdef DEBUG_PLG
#define debug_plg(str) {fprintf(stderr, "%s\n", str); fflush(stderr);}
#else
#define debug_plg(str) ;
#endif

// GENERATE TYPES WITH PROLOG ================================

void generate_block(FILE* f, AST* block);
void generate_cmds(FILE* f, AST* cmds);
void generate_seq(FILE* f, Cmds* cmds);
void generate_dec(FILE* f, AST* dec);
void generate_stat(FILE* f, AST* stat);
void generate_expr(FILE* f, AST* expr);
void generate_type(FILE* f, AST* type);

// ON APS0 WE SEEK TO PROUVE THAT ALL PROGRAMS ARE TYPED VOID


// TYPING A PROGRAM
void generate_program(FILE* f, Program* prog) {
  	fprintf(f, "/*=========================== FILE GENERATED AFTER CHECKING TYPE OF APS PROGRAM =====================*/\n \n");
	debug_plg("Program");
	fprintf(f, "type(prog(");
  	generate_block(f, prog->cmds);
  	fprintf(f, "), [], void).");
}

//TYPING A BLOCK
void generate_block(FILE* f, AST* block) {
  debug_plg("Block");
  fprintf(f, "[");
  generate_cmds(f, block->content.asBlock->cmds);
  fprintf(f, "]");
}

//TYPING COMMANDS
void generate_cmds(FILE* f, AST* cmds) {
  debug_plg("Cmds");
  generate_seq(f, cmds->content.asCmds);
}

//TYPING SEQUENCE
void generate_seq(FILE* f, Cmds* cmds) {
  debug_plg("Seq");
  while(cmds) {
    AST* statdec = cmds->statDec;
    switch (statdec->type) {
    case T_STAT:
      if (cmds->next) {
	generate_stat(f, statdec);
	fprintf(f, ",");
      }
      else 
	generate_stat(f, statdec);
      break;
    case T_DEC:
      if (cmds->next) {
	generate_dec(f, statdec);
	fprintf(f, ",");
      }
      else
	generate_dec(f, statdec);
      break;
    default:
      fprintf(stderr, "SEQUENCE ERROR !\n");
      exit(EXIT_FAILURE);
    }
    if (!cmds->next)
      break;
    cmds = cmds->next->content.asCmds;
  }
}

//TYPING DECLARATION
void generate_dec(FILE* f, AST* dec) {
  debug_plg("Dec");
  Dec* decl = dec->content.asDec;
  switch (decl->type) {

  	case T_CONST:
    fprintf(f, "const(%s,", decl->ident);
    generate_type(f, decl->decType);
    fprintf(f, ",");
    generate_expr(f, decl->expr);
    fprintf(f, ")");
    break;

  	case T_VAR:
    fprintf(f, "var(%s,", decl->ident);
    generate_type(f, decl->decType);
    fprintf(f, ")");
    break;
  default:
    fprintf(stderr, "UNKNOWN DECLARATION TYPE\n");
    exit(EXIT_FAILURE);
  }
}


//TYPING STATEMENT
void generate_stat(FILE* f, AST* st) {
  debug_plg("Stat");
  Stat* stat = st->content.asStat;
  switch (stat->type) {

	  case T_SET:
    fprintf(f, "set(%s,", stat->ident);
    generate_expr(f, stat->expr);
    fprintf(f, ")");
    break;

	  case T_IF:
    fprintf(f, "if(");
    generate_expr(f, stat->expr);
    fprintf(f, ",");
    generate_block(f, stat->prog1);
    fprintf(f, ",");
    generate_block(f, stat->prog2);
    fprintf(f, ")");
    break;

	  case T_WHILE:
    fprintf(f, "while(");
    generate_expr(f, stat->expr);
    fprintf(f, ",");
    generate_block(f, stat->prog1);
    fprintf(f, ")");
    break;

	  case T_PRINT:
    fprintf(f, "echo(");
    generate_expr(f, stat->expr);
    fprintf(f, ")");
    break;

	  default:
    fprintf(stderr, "UNKNOWN STAT TYPE!\n");
    exit(EXIT_FAILURE);
  }
}

//TYPING EXPRESSIONS =====================================================
void generate_expr(FILE* f, AST* exp) {
  debug_plg("Expr");
  Expr* expr = exp->content.asExpr;
  switch (expr->type) {

	  case T_E_BOOL: 					// BOOLEAN EXPRESSION
    if (expr->contents.bool == T_TRUE)
      fprintf(f, "true");
    else
      fprintf(f, "false");
    break;

	  case T_NUM: 						// NUM EXPRESSION
    fprintf(f, "%d", expr->contents.num);
    break;
  
	case T_IDENT: 						// IDENT EXPRESSION
    fprintf(f, "%s", expr->contents.ident);
    break;
  
	case T_UNOP: 						//<<<<<<<<<<<<<<<<<<UNARY EXPRESSION >>
    {
      UnOp* un = expr->contents.unop;
      switch (un->op) {
      case T_NOT:
	fprintf(f, "not(");
	break;

	      default:
	fprintf(stderr, "UNKNOWN OPERAND!\n");
	exit(EXIT_FAILURE);
      }
      generate_expr(f, un->arg);
      fprintf(f, ")");
      break;
    }
 
	 case T_BINOP:	 					//<<<<<<<<<<<<<<<<<<BINARY EXPRESSION >>
    {
      BinOp* bin = expr->contents.binop;
      switch (bin->op) {
      case T_AND:
	fprintf(f, "and(");
	break;
      case T_OR:
	fprintf(f, "or(");
	break;
      case T_EQ:
	fprintf(f, "eq(");
	break;
      case T_LT:
	fprintf(f, "lt(");
	break;
      case T_ADD:
	fprintf(f, "add(");
	break;
      case T_SUB:
	fprintf(f, "sub(");
	break;
      case T_MUL:
	fprintf(f, "mul(");
	break;
      case T_DIV:
	fprintf(f, "div(");
	break;
      case T_MOD:
	fprintf(f, "mod(");
	break;
      default:
	fprintf(stderr, "Unknown operand!\n");
	exit(EXIT_FAILURE);
      }
      generate_expr(f, bin->arg1);
      fprintf(f, ",");
      generate_expr(f, bin->arg2);
      fprintf(f, ")");
    }
    break;
  default:
    fprintf(stderr, "Unknown expression type!\n");
    exit(EXIT_FAILURE);
  }
}

// PRINTING TYPES ==========================================================================
void generate_type(FILE* f, AST* type) {
  debug_plg("Type");
  switch (*(type->content.asType)) {
  case T_VOID:
    fprintf(f, "void");
    break;
  case T_BOOL:
    fprintf(f, "bool");
    break;
  case T_INT:
    fprintf(f, "int");
  }
}
