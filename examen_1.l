%option yylineno

%{
   #include "tokens.h"

   #define YY_DECL int yylex()

   FILE *fp;
%}

%x MultiLineComment

%%
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"%" { return '%'; }
"=" { return '='; }
"(" { return '('; }
")" { return ')'; }
";" { return ';'; }
"," { return ','; }

"<" { return '<'; }
">" { return '>'; }
"==" { return TK_EQUAL_EQUAL; }


"return" { return KW_RETURN; }
"int" { return KW_INT; }
"print" { return KW_PRINT; }
"let" { return KW_LET; }
"begin" { return KW_BEGIN; }
"end" { return KW_END; }
"if" { return KW_IF; }

[a-zA-Z][_|0-9|a-zA-Z]* { yylval.string_t = strdup(yytext); return TK_IDENT; }
[0-9]+ { yylval.int_t = atoi(yytext); return TK_INT; }
([0-9]*[.])?[0-9]+ { yylval.float_t = atof(yytext); return TK_FLOAT; }
[\t] { printf("\t"); }
[\r] { printf("\r"); }
[\n] { printf("\n"); }
[ ] { printf(" "); }
. { printf("\nInvalid Lexical Character %c, Line No. %d", yytext[0], yylineno); return YYUNDEF; }
%%

int yywrap(void) {
   yylineno = 1;
   yyrestart(yyin);
   
   yyin = fp;
   if(yyin == NULL)
      return 0;
   return 1;
}