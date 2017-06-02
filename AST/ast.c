#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

#ifdef DEBUG_AST
#define debug_ast(str) {fprintf(stderr, "%s\n", str); fflush(stderr);}
#else
#define debug_ast(str) ;
#endif

Program* make_program(AST* cmds) {
  Program * prog = malloc(sizeof(*prog));
  prog->cmds = make_block(cmds);
  return prog;
}

AST* make_block (AST* cmds) {
  debug_ast("Block");
  AST* ast = malloc(sizeof(*ast));
  ast->type = T_PROG;
  ast->content.asBlock = malloc(sizeof(Block));
  ast->content.asBlock->cmds = cmds;
  return ast;
}

AST* make_cmds (AST* statDec, AST* next) {
  debug_ast("Cmds");
  AST* ast = malloc(sizeof(*ast));
  ast->type = T_CMDS;
  ast->content.asCmds = malloc(sizeof(Cmds));
  ast->content.asCmds->statDec = statDec;
  ast->content.asCmds->next = next;
  return ast;
}

AST* make_dec (TypeDec ast_type, AST* dec_type, char* ident, AST* expr) {
  AST* ast = malloc(sizeof(*ast));
  ast->type = T_DEC;
  ast->content.asDec = malloc(sizeof(Dec));
  ast->content.asDec->type = ast_type;
  ast->content.asDec->decType = dec_type;
  ast->content.asDec->ident = ident;
  ast->content.asDec->expr = expr;
  return ast;
}

AST* make_stat (TypeStat statType, char* ident, AST* expr, AST* prog1, AST* prog2) {
  AST* ast = malloc(sizeof(*ast));
  ast->type = T_STAT;
  ast->content.asStat = malloc(sizeof(Stat));
  ast->content.asStat->type = statType;
  ast->content.asStat->ident = ident;
  ast->content.asStat->expr = expr;
  ast->content.asStat->prog1 = prog1;
  ast->content.asStat->prog2 = prog2;
  return ast;
}

AST* make_expr (TypeExpr exprType, Bool bool, int num, char* ident, Operators op, AST* expr1, AST* expr2) {
  AST* ast = malloc(sizeof(*ast));
  ast->type = T_EXPR;
  ast->content.asExpr = malloc(sizeof(Expr));
  ast->content.asExpr->type = exprType;
  switch (exprType) {
  case T_E_BOOL:

    ast->content.asExpr->contents.bool = bool;
    break;

  case T_NUM:
    ast->content.asExpr->contents.num = num;
    break;

  case T_IDENT:
    ast->content.asExpr->contents.ident = ident;
    break;

  case T_UNOP:
    ast->content.asExpr->contents.unop = malloc(sizeof(UnOp));
    ast->content.asExpr->contents.unop->op = op;
    ast->content.asExpr->contents.unop->arg = expr1;

  case T_BINOP:
    ast->content.asExpr->contents.binop = malloc(sizeof(BinOp));;
    ast->content.asExpr->contents.binop->op = op;
    ast->content.asExpr->contents.binop->arg1 = expr1;
    ast->content.asExpr->contents.binop->arg2 = expr2;
    break;
  }
  return ast;
}

AST* make_var(char* ident, AST* type) {
  debug_ast("Var");
  return make_dec(T_VAR, type, ident, NULL);
}

AST* make_cst(char* ident, AST* type, AST* expr) {
  debug_ast("Cst");
  return make_dec(T_CONST, type, ident, expr);
}

AST* make_set(char* ident, AST* expr) {
  debug_ast("Set");
  return make_stat(T_SET, ident, expr, NULL, NULL);
}

AST* make_cond(AST* cond, AST* cons, AST* alt) {
  debug_ast("Cond");
  return make_stat(T_IF, NULL, cond, cons, alt);
}

AST* make_loop(AST* cond, AST* body) {
  debug_ast("Loop");
  return make_stat(T_WHILE, NULL, cond, body, NULL);
}

AST* make_print(AST* expr) {
  debug_ast("Echo");
  return make_stat(T_PRINT, NULL, expr, NULL, NULL);
}

AST* make_type (PrimitiveType t) {
  debug_ast("Type");
  Type res = malloc(sizeof(*res));
  *res = t;
  AST* ast = malloc(sizeof(*ast));
  ast->type = T_TYPE;
  ast->content.asType = res;
  return ast;
}

AST* make_bool_expr(Bool b) {
  debug_ast("Bool_Expr");
  return make_expr(T_E_BOOL, b, -1, NULL, -1, NULL, NULL);
}

AST* make_integer_expr(int num) {
  debug_ast("Int_Expr");
  return make_expr(T_NUM, -1, num, NULL, -1, NULL, NULL);
}

AST* make_ident_expr(char* ident) {
  debug_ast("Ident_Expr");
  return make_expr(T_IDENT, -1, -1, ident, -1, NULL, NULL);
}

AST* make_unary_expr(Operators op, AST* expr) {
  debug_ast("Unary_Expr");
  return make_expr(T_UNOP, -1, -1, NULL, op, expr, NULL);
}

AST* make_binary_expr(Operators op, AST* expr1, AST* expr2) {
  debug_ast("Binary_Expr");
  return make_expr(T_BINOP, -1, -1, NULL, op, expr1, expr2);
}

/********* APS 1 **********/
AST* make_abstraction(AST* args, AST* expr){
debug_ast("Abs");
return NULL;} /* NOT IMPLEMENTED */

AST* make_application(AST* expr,AST* exprs){
debug_ast("Appl");
return NULL;} /* NOT IMPLEMENTED */

AST* make_args(AST* arg,AST* args){
debug_ast("Args");
return NULL;} /* NOT IMPLEMENTED */

AST* make_affect_arg(char* ident,AST* type){
debug_ast("Arg");
return NULL;} /* NOT IMPLEMENTED */

AST* make_type_func_in_out(AST* types, AST* type){
debug_ast("TypesFunc");
return NULL;} /* NOT IMPLEMENTED */

AST* make_a_type(AST* type){
debug_ast("NewType");
return NULL;} /* NOT IMPLEMENTED */

AST* make_type_func_in(AST* type, AST* types){
debug_ast("FuncType");
return NULL;} /* NOT IMPLEMENTED */

/********* APS 2 **********/
AST* make_return(AST* expr){
debug_ast("Return");
return NULL;}/* NOT IMPLEMENTED */


