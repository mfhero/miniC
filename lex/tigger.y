%{
#include <cstdio>
#include <cstring>

#include "Riscv/method.hpp"

#define YYSTYPE_IS_DECLARED 
typedef std::string YYSTYPE;

extern int yylineno;
int yylex(void);
void yyerror(const char*);

%}

%token REGISTER NUMBER
%token IF GOTO END CALL RETURN
%token STORE LOAD MALLOC LOADADDR 
%token LABEL
%token FUNCTION
%token OP

%%

goal    : 
        | goal functiondefinition 
        | goal globaldefinition 
        ;

functiondefinition : functionhead expressions functiontail
                   ;

functionhead       : FUNCTION '[' NUMBER ']' '[' NUMBER ']' 
                   {
                       PrintFunctionHead($1, $3, $6);
                   }
                   ;

functiontail       : END FUNCTION 
                   {
                        PrintFunctionTail($2);
                   }
                   ;

expressions :
            | expressions expression 
            ;

integer : '-' NUMBER { $$ = "-" + $2; }
        | NUMBER { $$ = $1; }
        ;

globaldefinition : REGISTER '=' MALLOC NUMBER 
                 { PrintGlobalArray($1, $4); }
                 | REGISTER '=' integer 
                 { PrintGlobal($1, $2); }
                 ;

expression : REGISTER '=' integer 
           { printline("li", $1, $3); }
           | REGISTER '=' REGISTER OP REGISTER 
           { PrintOp($4, $1, $3, $5); }
           | REGISTER '=' REGISTER OP integer 
           { PrintOp($4, $1, $3, $5); }
           | REGISTER '=' OP REGISTER 
           { Print1Op($3, $1, $4); }
           | REGISTER '=' REGISTER 
           { printline("mv", $1, $3); }
           | REGISTER '[' integer ']' '=' REGISTER 
           { printline2("sw", $6, $3, $1); }
           | REGISTER '=' REGISTER '[' integer ']' 
           { printline2("lw", $1, $5, $3); }
           | IF REGISTER OP REGISTER GOTO LABEL 
           { PrintConditionJump($3, $2, $4, $6); }
           | GOTO LABEL 
           { printline("j", "." + $2); }
           | LABEL ':' 
           { printf(".%s:\n", $1.c_str()); } 
           | CALL FUNCTION 
           { printline("call", $2.substr(2)); }
           | STORE REGISTER NUMBER
           { printline2("sw", $2, $3, "sp"); }
           | LOAD NUMBER REGISTER 
           { printline2("lw", $3, $2, "sp"); }
           | LOAD REGISTER REGISTER 
           { PrintLoadGlobal($2, $3); }
           | LOADADDR NUMBER REGISTER 
           { printline("add", $3, std::string("sp"), $2); }
           | LOADADDR REGISTER REGISTER 
           { PrintLoadGlobalAddr($2, $3); }
           | RETURN
           { PrintReturn(); }
           ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Tigger : Line %d: %s\n", yylineno, s);
}

extern "C" int yywrap(void) 
{ 
   return 1; 
} 
