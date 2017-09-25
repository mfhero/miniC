%{
    #include <stdio.h>

    #include "miniC.yacc.c"
    int lineno = 0, charno = 0, wordno = 0;
%}

digits [0-9]+
operator [+*/-]

%%
{digits} {
    yylval = atoi(yytext);
//    printf("lex find dights:%d\n", yylval);
    return DIGITS;
}

{operator} {
    yylval = (int)(yytext[0]);
//    printf("lex find operator:%c\n", (int)(yylval));
    return OPERATOR;
}

. {
    return OTHER;
}

%%


