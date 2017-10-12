%{
    #include <stdio.h>

    #include "miniC.yacc.c"
%}

digits [0-9]+
number {digits}(\.{digits})?
operator1 [\+\-]
operator2 [\*/]
splitchar [;\n]
lb [\(]
rb [\)]

%%
{number} {
    yylval = atof(yytext);
//    printf("lex find dights:%d\n", yylval);
    return NUMBER;
}

{operator1} {
    yylval = (int)(yytext[0]);
//    printf("lex find operator:%c\n", (int)(yylval));
    return OPERATOR1;
}

{operator2} {
    yylval = (int)(yytext[0]);
//    printf("lex find operator:%c\n", (int)(yylval));
    return OPERATOR2;
}
{splitchar} { return SPLITCHAR; }

{lb} { return LB; }

{rb} { return RB; }

. {}

%%


