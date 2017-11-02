%{
#ifndef _MINIC_LEX_C
#define _MINIC_LEX_C

#include "globals.h"
#include "util.h"
//#include "scan.h"
/* lexeme of identifier or reserved word */

#define MAXTOKENLEN 40
char tokenString[MAXTOKENLEN+1];

#include "miniC.yacc.c"
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t\r]+
hints \/\/(.)*\n

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
"%"             {return MOD;}
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
{number}        {yylval = newExpNode(ConstK); yylval->attr.name = StrClone(yytext); yylval->attr.val = atof(yytext); return NUM;}
{identifier}    {yylval = newExpNode(IdK); yylval->attr.name = StrClone(yytext); return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
{hints}            { char c;
                  do {
                      break;
                      c = input();
                  } while (c != '\n' && c != EOF);
                     lineno++;
                }
.               {return ERROR;}

%%

TokenType getToken(void){ 
    static int firstTime = 1;
    TokenType currentToken;
    if (firstTime) { 
        firstTime = 0;
    //    lineno++;
    //    yyin = source;
    //    yyout = listing;
    }
    currentToken = yylex();
    strncpy(tokenString,yytext,MAXTOKENLEN);
    return currentToken;
}

#endif
