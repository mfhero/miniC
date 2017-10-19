%{
#ifndef _MINIC_LEX_C
#define _MINIC_LEX_C

#include "globals.h"
#include "util.h"
//#include "scan.h"
/* lexeme of identifier or reserved word */

#include "miniC.yacc.c"

char tokenString[MAXTOKENLEN+1];
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"if"            {return IF;}
"else"          {return ELSE;}
"while"         {return WHILE;}
"main"          {return MAIN;}
"int"           {return INT;}
"return"        {return RETURN;}
"="             {return ASSIGN;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
"<"             {return LT;}
">"             {return GT;}
"=="            {return EQ;}
"!="            {return NE;}
"!"             {return NOT;}
"||"            {return OR;}
"&&"            {return AND;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LPARENX;}
"]"             {return RPARENX;}
";"             {return SEMI;}
","             {return COMMA;}
"{"             {return LBLOCK;}
"}"             {return RBLOCK;}
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
"//"            { char c;
                  do {
                      c = input();
                  } while (c != '\n' && c != EOF);
                }
.               {return ERROR;}

%%

TokenType getToken(void){ 
    static int firstTime = TRUE;
    TokenType currentToken;
    if (firstTime) { 
        firstTime = FALSE;
    //    lineno++;
    //    yyin = source;
    //    yyout = listing;
    }
    currentToken = yylex();
    strncpy(tokenString,yytext,MAXTOKENLEN);
    return currentToken;
}

#endif
