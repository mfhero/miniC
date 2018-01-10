%{
#include <cstring>
#include <cstdio>

#include "tigger.yacc.hpp"

%}

digit       [0-9]
number      {digit}+
identifier  [0-9a-zA-Z_]+
whitespace  [ \t\r\n]+
register    v{digit}+|s{digit}+|a{digit}+|t{digit}+
label       l{digit}+
function    f_{identifier}
op \+|\-|\*|\/|\%|\!|\|\||&&|<|>|!=|==

%option yylineno

%%

"malloc"        {return MALLOC;}
"if"            {return IF;}
"goto"          {return GOTO;}
"call"          {return CALL;}
"return"        {return RETURN;}
"end"           {return END;}
"load"          {return LOAD;}
"store"         {return STORE;}
"loadaddr"      {return LOADADDR;}
{register}      {yylval = yytext; return REGISTER;}
{number}        {yylval = yytext; return NUMBER;}
{op}            {yylval = yytext; return OP;}
{function}      {yylval = yytext; return FUNCTION;}
{label}         {yylval = yytext; return LABEL; }
{whitespace}    {}
.               {return yytext[0]; }

%%
