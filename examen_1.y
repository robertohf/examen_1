%require "3.2"

%code requires{
   #include "ast.h"

   extern StmtPtr root;
}

%{
   #include <cstdio>
   #include <stdio.h>
   #include <list>
   #include "ast.h"
   int yylex();
   extern int yyparse();
   extern int yylineno;

   void yyerror(const char * s);
%}

%union {
  int int_t;
  float float_t;
  char *string_t;

  StmtList *stmtList_t;
  ArgumentList *argumentList_t;
  StmtPtr stmt_t;
  ExprPtr expr_t;
};

/*
BlockP: %empty { }
      | ';' Stmt BlockP { $$ = new StmtList; $$->push_back($2); }
;

Expr: Term ExprP { $$ = $1; }
;

ExprP: %empty { }
      | '+' Term ExprP { $$ = new AddExpr($2, $3); }
      | '-' Term ExprP { $$ = new SubExpr($2, $3); }
;

Term: Factor TermP { $$ = $1; }
;

TermP: %empty { }
      |  '*' Factor TermP { $$ = new MultExpr($2, $3); }
      |  '/' Factor TermP { $$ = new DivExpr($2, $3); }
;
*/

%token KW_WRITE KW_INT KW_DEF KW_END KW_LET KW_BEGIN KW_IF TK_EQUAL_EQUAL KW_PRINT KW_RETURN
%token<int_t> TK_INT
%token<string_t> TK_IDENT
%token<float_t> TK_FLOAT

%type<stmtList_t> Block
%type<argumentList_t> Params

%type<stmt_t> Program Stmt Print AssignDecl MethodDecl IfStmt ReturnStmt
%type<expr_t> EqualityExpr RelationalExpr Expr Term Factor MethodInvoc

%start Program

%%
Program: Block { root = new BlockStmt($1); root->isTrue = true; }
;

Block: Block Stmt { $$ = $1; $$->push_back($2); }
      | Stmt { $$ = new StmtList(); $$->push_back($1); }
;

Stmt: Print ';' { $$ = $1; }
      | AssignDecl ';' { $$ = $1; }
      | MethodDecl { $$ = $1; }
      | IfStmt { $$ = $1; }
      | ReturnStmt ';' { $$ = $1; }
;

Print: KW_PRINT EqualityExpr { $$ = new WriteStmt($2); }
;

AssignDecl: KW_LET TK_IDENT '=' EqualityExpr { $$ = new AssignStmt($2, $4); }
      | TK_IDENT '=' EqualityExpr { $$ = new AssignStmt($1, $3); }
      | KW_INT TK_IDENT '='  EqualityExpr { $$ = new AssignStmt($2, $4); }
      | AssignDecl ';'
;

MethodDecl: KW_LET TK_IDENT '(' Params ')' KW_BEGIN Block KW_END { $$ = new MethodDecl($2, $4, $7); }
      | KW_INT TK_IDENT '(' Params ')' KW_BEGIN Block KW_END { $$ = new MethodDecl($2, $4, $7); }
;

MethodInvoc: TK_IDENT '(' Params ')' { $$ = new MethodInvoc($1, $3); }
;

Params: Params ',' EqualityExpr { $$ = $1; $$->push_back($3); }
      | EqualityExpr { $$ = new ArgumentList(); $$->push_back($1); }
;

IfStmt: KW_IF '(' EqualityExpr ')' KW_BEGIN Block KW_END { $$ = new IfStmt($3, $6); }
;

ReturnStmt: KW_RETURN EqualityExpr { $$ = new ReturnStmt($2); }
;

EqualityExpr: RelationalExpr { $$ = $1; }
;

RelationalExpr: Expr { $$ = $1; }
;

Expr: Expr TK_EQUAL_EQUAL Term { $$ = new EqualsExpr($1, $3); }
      | Expr '>' Term { $$ = new GtExpr($1, $3); }
      | Expr '<' Term { $$ = new LtExpr($1, $3); }
      | Expr '+' Term { $$ = new AddExpr($1, $3); }
      | Expr '-' Term { $$ = new SubExpr($1, $3); }
      | Term { $$ = $1; }
;


Term: Term '*' Factor { $$ = new MultExpr($1, $3); }
      | Term '/' Factor { $$ = new DivExpr($1, $3); }
      | Factor { $$ = $1; }
;

Factor: TK_INT { $$ = new IntExpr($1); }
      | TK_IDENT { $$ = new IdentExpr($1); }
      | TK_FLOAT { $$ = new FloatExpr($1); }
      | '(' Expr ')' { $$ = $2; }
      | MethodInvoc { }
;
%%

void yyerror(const char * s){
      fprintf(stderr, "Line %d, error: %s\n", yylineno, s);
}