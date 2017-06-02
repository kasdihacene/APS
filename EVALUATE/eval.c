#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../PROLOG_GEN/prolog_gen.h"
#include "../AST/ast.h"
#include "eval.h"

#ifdef DEBUG_EV
#define debug_ev(str) { fprintf(stderr, "%s\n", str); fflush(stderr); }
#else
#define debug_ev(str) ;
#endif

// EVALUATE INSTRUCTIONS===============================
void eval_program(Program* prog);
void eval_block(AST* block, Env ** env);
void eval_cmds(AST* cmds, Env ** env);
void eval_seq(Cmds* seq, Env ** env);
void eval_dec(AST* dec, Env ** env);
void eval_stat(AST* stat, Env ** env);
ValType eval_type(AST* type, Env ** env);
Box* eval_expr(AST* expr, Env ** env);

// PRINTING ENV ========================================
void print_env(Env * env);
void print_env_cell(Env * env);
Box * make_true();
Box * make_false();
Box * make_int(int num);
Box * search_env(char * varName, Env * env);
Env * get_env(char * varName, Env * env);

// TRACE FOR PROGRAM ===================================
void eval_program(Program* prog) {
  debug_ev("Program init");
  Env * env = NULL;
  eval_block(prog->cmds, &env);
}

// TRACE FOR BLOCK =========================================
void eval_block(AST* prog, Env ** env) {
  debug_ev("Block");

  // Work on an environment "copy" to keep scope
  Env * blockEnv = malloc(sizeof(*blockEnv));
  blockEnv = *env;
  eval_cmds(prog->content.asBlock->cmds, &blockEnv);
  fprintf(stdout, "Env values at block end\n");
  print_env(blockEnv);
}

// TRACE FOR COMMANDS ===================================
void eval_cmds(AST* cmds, Env ** env) {
  debug_ev("Cmds");
  eval_seq(cmds->content.asCmds, env);
}

//TRACE FOR SEQUENCE ====================================
void eval_seq(Cmds* cmds, Env ** env) {
  debug_ev("Seq");
  while (cmds) {
    AST* statdec = cmds->statDec;

    switch (statdec->type) {

    case T_STAT: //STATEMENT
      eval_stat(statdec, env);
      break;

    case T_DEC: //DECLARATION
      eval_dec(statdec, env);
      break;

    default:
      fprintf(stderr, "Unrecognized seq instruction\n");
    }
    if (!cmds->next)
      break;
    cmds = cmds->next->content.asCmds;
  }
}

// TRACE FOR DECLARATIONS
void eval_dec(AST* dec, Env ** env) {
  debug_ev("Dec");
  Dec * decl = dec->content.asDec;
  Env * newValue = malloc(sizeof(*newValue));
  
  newValue->name = decl->ident;
  switch (decl->type) {
  case T_CONST: {
    Box * b = eval_expr(decl->expr, env);
    newValue->valType = b->type;
    newValue->varType = EVAL_CNST;
    switch (b->type) {
    case EVAL_INT:
      newValue->contents.asInt = b->contents.num;
      break;
    case EVAL_BOOL:
      newValue->contents.asBool = b->contents.bool;
      break;
    }
    break;
  }
  case T_VAR:
    newValue->varType = EVAL_VAR;
    newValue->valType = eval_type(decl->decType, env);
    break;
  default:
    fprintf(stderr, "Unknown declaration type!\n");
    exit(EXIT_FAILURE);
  }

  newValue->next = *env;
  *env = newValue;
}

// TRACE FOR STATEMENT =============================
void eval_stat(AST* st, Env ** env) {
  debug_ev("Stat");
  Stat * stat = st->content.asStat;
  Box * box = eval_expr(stat->expr, env);
  switch (stat->type) {
  case T_SET:
    {
      Env * myVar = get_env(stat->ident, *env);
      switch (box->type) {
      case EVAL_BOOL:
	myVar->contents.asBool = box->contents.bool;
      break;
      case EVAL_INT:
	// fprintf(stderr, "Setting var %s to %d\n", myVar->name, box->contents.num);
	myVar->contents.asInt = box->contents.num;
      }
      break;
    }
  case T_IF:
    if (box->contents.bool == TRUE)
      eval_block(stat->prog1, env);
    else
      eval_block(stat->prog2, env);
    break;
  case T_WHILE: 
    {
      // Work on an environment "copy" to keep scope
      Env * whileEnv = malloc(sizeof(*whileEnv));
      whileEnv = *env;
      while (box->contents.bool == TRUE) {
	eval_block(stat->prog1, &whileEnv);
	box = eval_expr(stat->expr, &whileEnv);
      }
      break;
    }

	  case T_PRINT:
    switch (box->type) {
   
	 case EVAL_BOOL:
      if (box->contents.bool == FALSE)
	fprintf(stdout, "FALSE\n");
      else 
	fprintf(stdout, "FALSE\n");

	    case EVAL_INT:
      fprintf(stdout, "%d\n", box->contents.num);
    }
    break;

	  default:
    fprintf(stderr, "Unknown statement type!\n");
    exit(EXIT_FAILURE);
  }
}

// EVAL EXPRESSIONS ==========================================
Box * eval_expr(AST * exp, Env ** env) {
  debug_ev("Expr");
  Expr* expr = exp->content.asExpr;
	// EXPRESSION TYPE  
	switch (expr->type) {

	//BOOLEAN
	  case T_E_BOOL:
    if (expr->contents.bool == T_TRUE)
      return make_true();
    else
      return make_false();
	//NUMS
	  case T_NUM:
    return make_int(expr->contents.num);
	//IDENT
	  case T_IDENT:
    return search_env(expr->contents.ident, *env);
	//UNIRAY OPERATION
	  case T_UNOP:
    {
      UnOp * un = expr->contents.unop;
      Box * arg = eval_expr(un->arg, env);
      switch (un->op) {
      case T_NOT:
        arg->contents.bool = (arg->contents.bool + 1) % 2;
	break;

      default:
	fprintf(stderr, "Unknown operand!\n");
	exit(EXIT_FAILURE);
      }
      return arg;
    }
	//BINARY OPERATION  
	case T_BINOP:
    {
      BinOp* bin = expr->contents.binop;
      Box * arg1 = eval_expr(bin->arg1, env);
      Box * arg2 = eval_expr(bin->arg2, env);
      switch (bin->op) {
      case T_AND:
	if (arg1->contents.bool == TRUE && arg2->contents.bool == TRUE)
	  return make_true();
	else
	  return make_false();
      case T_OR:
	if (arg1->contents.bool == TRUE || arg2->contents.bool == TRUE)
	  return make_true();
	else
	  return make_false();
      case T_EQ:
	if (arg1->contents.num == arg2->contents.num)
	  return make_true();
	else
	  return make_false();
      case T_LT:
	if (arg1->contents.num < arg2->contents.num)
	  return make_true();
	else
	  return make_false();	
      case T_ADD:
	return make_int(arg1->contents.num + arg2->contents.num);
      case T_SUB:
	return make_int(arg1->contents.num - arg2->contents.num);	
      case T_MUL:
	return make_int(arg1->contents.num * arg2->contents.num);
      case T_DIV:
	return make_int(arg1->contents.num / arg2->contents.num);
      case T_MOD:
	return make_int(arg1->contents.num % arg2->contents.num);
      default:
	fprintf(stderr, "Unknown operand!\n");
	exit(EXIT_FAILURE);
      }
    }
    break;
  default:
    fprintf(stderr, "Unknown expression type!\n");
    exit(EXIT_FAILURE);
  }
}

// Only needed for typing - not necessary, but w/e
ValType eval_type(AST* expr, Env ** env) {
  debug_ev("Type");
  switch (*(expr->content.asType)) {
  case T_BOOL: return EVAL_BOOL;
  case T_INT : return EVAL_INT;
  default: fprintf(stderr, "Returning void value\n"); exit(EXIT_FAILURE);
  }
}

Box * make_box(ValType type, int asNum, boolean asBool) {
  Box * ret = malloc(sizeof(*ret));
  ret->type = type;
  switch (type) {
  case EVAL_BOOL:
    ret->contents.bool = asBool;
    break;
  case EVAL_INT:
    ret->contents.num = asNum;
    break;
  }
  return ret;
}

Box * make_true() {
  return make_box(EVAL_BOOL, -1, TRUE);
}

Box * make_false() {
  return make_box(EVAL_BOOL, -1, FALSE);
}

Box * make_int(int val) {
  return make_box(EVAL_INT, val, -1);
}

Env * get_env(char * varName, Env * env) {
  while (env) {
    if (strcmp(varName, env->name) == 0) 
      return env;
    env = env->next;
  }
  fprintf(stderr, "Error: symbol %s not in environment\n", varName);
  exit(EXIT_FAILURE);
}

// SEEKING FOR ENVIRONMENT ==================================================
Box * search_env(char * varName, Env * env) {
  while (env) {
    if (strcmp(varName, env->name) == 0) {
      switch (env->valType) {
      case EVAL_BOOL:
	return make_box(env->valType, -1, env->contents.asBool);
      case EVAL_INT:
	return make_box(env->valType, env->contents.asInt, -1);
      default: 
	fprintf(stderr, "Environment type error (searchenv)\n");
	exit(EXIT_FAILURE);
      }
    }
    env = env->next;
  }
  fprintf(stderr, "Symbol not in environment (searchenv)\n");
  exit(EXIT_FAILURE);
}

// PRINTING ENV CELL =========================================================
void print_env_cell(Env * env) {
  char res [1024];
  char valbuf [5];
  res[0] = '\0';
  strcat(res, "[");
  strcat(res, env->name);
  strcat(res, ":");
 
 switch (env->varType) {

	  case EVAL_CNST:
    strcat(res, "cst ");
    break;
	
	  case EVAL_VAR:
    strcat(res, "var ");
  }

  switch (env->valType) {

	  case EVAL_INT:
    strcat(res, "int ");
    sprintf(valbuf, "%d", env->contents.asInt);
    strcat(res, valbuf);
    break;

	  case EVAL_BOOL:
    strcat(res, " bool ");

    
  switch (env->contents.asBool){
  
	  case FALSE:
      strcat(res, "false");
      break;

	    case TRUE:
      strcat(res, "true");
      break;
    }
    break;

	  default:
    fprintf(stdout, "Error: variable neither cst nor var\n");
    exit(EXIT_FAILURE);
  }
  strcat(res, "]\n");
  fprintf(stdout, "%s", res);
}

// PRINT ENV ===============================================================
void print_env(Env * env) {
  while (env) {
    print_env_cell(env);
    env = env->next;
  }
}
