
%error-verbose

%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include "../PROLOG_GEN/prolog_gen.h"
  #include "../AST/ast.h"

  extern FILE *yyin;
  extern int charat;
  extern int lineat;  

  Program* ast_result;

  int yyerror(const char *message) {
    fprintf(stderr, "l.%d, char.%d, %s\n", lineat, charat, message);
    return 0;
  }
%}

%union {
    int entier;
    char* str;
    struct _ast* ast;
    struct _program* program;
}

%token TOKEN_WHILE TOKEN_IF TOKEN_PRINT TOKEN_SET TOKEN_VAR TOKEN_CONST
%token TOKEN_TRUE TOKEN_FALSE
%token TOKEN_AND TOKEN_OR TOKEN_NOT TOKEN_GIVES TOKEN_RETURN
%token TOKEN_EQ TOKEN_LT TOKEN_ADD TOKEN_SUB TOKEN_DIV TOKEN_MUL TOKEN_MOD
%token TOKEN_VOID TOKEN_BOOL TOKEN_INT

%token	<entier>	TOKEN_NOMBRE
%token	<str>		TOKEN_IDENT

/* EXTESION APS1 */
%type	<ast>		Block Cmds Dec Stat Expr Type Exprs Stats Args Arg Types Ret
%type	<program>	Entry

%start Entry

%%

/************************** EXTENSION FOR APS 0  ***************************/
Entry: '[' Cmds ']'              { $$ = make_program($2); ast_result = $$; }

Block: '[' Cmds ']'              { $$ = make_block($2); }
	     
Cmds:		Ret 		 { $$ = $1; }
	|	Stat Stats       { $$ = make_cmds($1, $2); }
	|      	Dec ';' Cmds     { $$ = make_cmds($1, $3); }

Stats:                       { $$ = NULL; }
	| ';' 	Cmds         { $$ = $2 ;  }

Dec: 		TOKEN_VAR TOKEN_IDENT Type	   { $$ = make_var(strdup($2), $3); }
	| 	TOKEN_CONST TOKEN_IDENT Type Expr  { $$ = make_cst(strdup($2), $3, $4); }
		    
Stat: TOKEN_SET TOKEN_IDENT Expr               { $$ = make_set(strdup($2), $3); }
	| 	TOKEN_IF Expr Block Block  { $$ = make_cond($2, $3, $4); }
	| 	TOKEN_WHILE Expr Block    { $$ = make_loop($2, $3); }
	|	TOKEN_PRINT Expr          { $$ = make_print($2);}
			  
Expr: 		TOKEN_TRUE		 	{ $$ = make_bool_expr(T_TRUE);  }
	| 	TOKEN_FALSE              	{ $$ = make_bool_expr(T_FALSE); }
	| 	TOKEN_NOMBRE             	{ $$ = make_integer_expr($1); }
	| 	TOKEN_IDENT              	{ $$ = make_ident_expr(strdup($1)); }
 	|	'(' TOKEN_NOT Expr ')'          { $$ = make_unary_expr(T_NOT, $3);      }
	|  	'(' TOKEN_OR Expr Expr ')'      { $$ = make_binary_expr(T_OR, $3, $4);  }
	|  	'(' TOKEN_AND Expr Expr ')'     { $$ = make_binary_expr(T_AND, $3, $4); }
	|  	'(' TOKEN_EQ Expr Expr ')'      { $$ = make_binary_expr(T_EQ, $3, $4);  }
	|  	'(' TOKEN_LT Expr Expr ')'      { $$ = make_binary_expr(T_LT, $3, $4);  }
	|  	'(' TOKEN_ADD Expr Expr ')'     { $$ = make_binary_expr(T_ADD, $3, $4); }
	|  	'(' TOKEN_SUB Expr Expr ')'     { $$ = make_binary_expr(T_SUB, $3, $4); }
	|  	'(' TOKEN_DIV Expr Expr ')'     { $$ = make_binary_expr(T_DIV, $3, $4); }
	|  	'(' TOKEN_MUL Expr Expr ')'     { $$ = make_binary_expr(T_MUL, $3, $4); }
	|	'(' TOKEN_MOD Expr Expr ')'     { $$ = make_binary_expr(T_MOD, $3, $4); }
/************************** EXTENSION FOR APS 01 ***************************/
	|	'[' Args ']' Expr	 { $$ = make_abstraction($2,$4);} 
	|	'(' Exprs ')'		 { $$ = $2; }

Exprs:
	Expr				 { $$ = $1; }
	|Expr Exprs 			 {$$ = make_application($1,$2);}
	
Args:
	Arg				 { $$ = $1; }
	| Arg ',' Args			 { $$ = make_args($1,$3);}

Arg:
	TOKEN_IDENT ':' Type		{$$ = make_affect_arg(strdup($1), $3);}

Type:    	TOKEN_BOOL      		{ $$ = make_type(T_BOOL); }
	| 	TOKEN_INT       		{ $$ = make_type(T_INT); }
	|	'(' Types TOKEN_GIVES Type ')'	{ $$ = make_type_func_in_out($2,$4); }

/************************** EXTENSION FOR APS 01 ***************************/
Types : 	Type				{$$ = make_a_type($1);}
	|	Type '*' Types			{$$ = make_type_func_in($1,$3);}
/***************************************************************************/

/************************** EXTENSION FOR APS 02 ***************************/
Ret:	TOKEN_RETURN Expr		{$$ = make_return($2);}/* not implemented yet */
/***************************************************************************/
%%

Program* ast_from_program(FILE* fp) {
    yyin = fp;
    yyparse();
    fclose(fp);
    return ast_result;
}
