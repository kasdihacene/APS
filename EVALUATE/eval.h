#ifndef __eval__
#define __eval__

typedef enum _valtypes { EVAL_BOOL, EVAL_INT } ValType;
typedef enum _constvar { EVAL_CNST, EVAL_VAR } ConstVar;
typedef enum _boolean  { FALSE, TRUE         } boolean;

typedef struct _env {
  char* name;
  ConstVar varType;
  ValType valType;
  union {
    int asInt;
    boolean asBool;
  } contents;
  struct _env * next;
} Env;

typedef struct _box {
  ValType type;
  union {
    int num;
    boolean bool;
  } contents;
} Box;

void eval_program(Program* program);

#endif
