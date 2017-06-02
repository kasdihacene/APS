
% >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> les regles de typages <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
% type(Expr, Env, NewEnv, Type)
% L'environnement est une liste tr(Nom, Const, Type)
% où Const est vrai si la variable a été déclarée constante, faux sinon

% Déclarations de valeurs nommées
typeNamed(var(Name, Type), Env, NextEnv) :- 
    env_add(Env, tr(Name, false, Type), NextEnv).
typeNamed(const(Name, Type, Expr), Env, NextEnv) :- 
    type(Expr, Env, Type), env_add(Env, tr(Name, true, Type), NextEnv).

% Règles intermédiaires
type([], _, void).
type([H|T], Env, void) :- typeNamed(H, Env, NextEnv), type(T, NextEnv, void).
type([H|T], Env, void) :- type(H, Env, void), type(T, Env, void).

/*================================== Noeuds terminaux ====================================================================*/
type(N, _, int) :- integer(N).
type(true, _, bool).
type(false, _, bool).
type(Var, Env, Type) :- env_get(Env, Var, true, Type); env_get(Env, Var, false, Type).

/*================================== Noeuds non terminaux - expressions =================================================*/

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> STATEMENT :
type(if(Cond, Bloc1, Bloc2), Env, void) :- 
    type(Cond, Env, bool), type(Bloc1, Env, void), type(Bloc2, Env, void).
type(while(Cond, Bloc), Env, void) :- type(Cond, Env, bool), type(Bloc, Env, void).
type(set(Var, Expr), Env, void) :- env_get(Env, Var, false, VarType), 
				   type(Expr, Env, VarType).
type(echo(Expr), Env, void) :- type(Expr, Env, bool); type(Expr, Env, int).

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UNIRAY AND BINARY OPERATIONS :
type(not(A), Env, bool) :- type(A, Env, bool).
type(and(A, B), Env, bool) :- type(A, Env, bool), type(B, Env, bool).
type(or(A, B), Env, bool) :- type(A, Env, bool), type(B, Env, bool).

type(add(A, B), Env, int) :- type(A, Env, int), type(B, Env, int).
type(sub(A, B), Env, int) :- type(A, Env, int), type(B, Env, int).
type(mul(A, B), Env, int) :- type(A, Env, int), type(B, Env, int).
type(div(A, B), Env, int) :- type(A, Env, int), type(B, Env, int).
type(mod(A, B), Env, int) :- type(A, Env, int), type(B, Env, int).

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> PROGRAM TYPING :
type(prog(Cmds), Env, Type) :- type(Cmds, Env, Type).

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ENVIRONMENT AND EXTENSION :

% env_add(env, (name, const, type), newenv) >>> ADDING TO THE ENV :
env_add(Env, tr(VarName, IsConst, VarType), [tr(VarName, IsConst, VarType)|Env]).

% env_get(env, name, const, type) >>> GETING FROM THE ENV :
env_get([tr(LVarName, LIsConst, LVarType)], LVarName, LIsConst, LVarType).
env_get([tr(LVarName, LIsConst, LVarType)|_], LVarName, LIsConst, LVarType).
env_get([tr(LVarName, LIsConst, _)|Env], VarName, IsConst, Type) :-
    (LVarName \= VarName; LIsConst \= IsConst), env_get(Env, VarName, IsConst, Type).

    
