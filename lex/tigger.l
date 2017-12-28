%{
#include <cstring>
#include <cstdio>
#include "tigger.yacc.hpp"
%}

digit       [0-9]
number      {digit}+
identifier  [0-9a-zA-Z_]+
whitespace  [ \t\r\n]+
register    x{digit}+|s{digit}+|a{digit}+|t{digit}+
OP \+|\-|\*|\/|\%|\!|\|\||&&|<|>|!=|==

%%

"malloc"        {return MALLOC;}
"if"            {return IF;}
"goto"          {return GOTO;}
"call"          {return call}
"return"        {return RETURN;}
"end"           {return END;}
"load"          {return LOAD;}
"store"         {return STORE;}
"loadaddr"      {return LOADADDR;}
{register}      {return REGISTER;}
{op}            {return OP;}
{whitespace}    {}

%%

int yywrap(void) {
    return 1;
}
