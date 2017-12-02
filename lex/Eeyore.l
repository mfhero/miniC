%{

#include <cstdio>
#include <iostream>
#include "Eeyore.yacc.hpp"

extern int lineno;

%}

NUM -?[1-9][0-9]*|0
SYMBOL [tTp][0-9]+
LABEL l[0-9]+
FUNCTION f_[_a-zA-Z][_a-zA-Z0-9]*
OP \+|\-|\*|\/|\%|\!|\|\||&&|<|>|!=|==
ASSIGN :=
hints \/\/(.)*\n

%%
" "    
{hints} { lineno++; } 
"\n"  { lineno++; return SEMI; }

"if"    { return IF;  }
"goto"  { return GOTO; }
"var"   { return VAR;  }
"param" { return PARAM;}
"call"  { return CALL; }
"return" { return RETURN; }
"end"   { return ENDF; }

{LABEL} { yylval = yytext; return LABEL; }
{NUM}   { yylval = yytext; return NUM; }
{SYMBOL} { yylval = yytext; return SYMBOL; }
{FUNCTION} {yylval = yytext; return FUNCTION; }
{OP}    { yylval = yytext; return OP; }
{ASSIGN} { yylval = yytext; return ASSIGN; }
. { return int(yytext[0]); }

%%

int yywarp() {
    return 1;
}
