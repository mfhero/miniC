%{
    #include <stdio.h>
    #define YYSTYPE int 
    extern YYSTYPE yylval;

    int calc(int a, int b, int oper) {
        switch ((char)(oper)) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return a / b;
        }
    }
%}

%token OPERATOR DIGITS OTHER 

%%
/*file : para file  
    |   para 
;

para :   CHARAC { printf("c"); }
    |   WORD { printf("w"); }
    |   LINE { printf("l\n"); }
;*/

expression : expression OPERATOR expression { 
                $$ = calc($1, $3, $2); 
                printf("%d%c%d=%d\n", $1, (char)($2), $3, $$);
            }
            | DIGITS { $$ = $1; }
            | OTHER expression { $$ = $2; }
            | expression OTHER { $$ = $1; } 

%%

int main(int argc, char *args[]) 
{
    yyparse();
 //   printf("c : %d, w : %d, l : %d\n", charno, wordno, lineno);
    return 0;
}

int yyerror(const char *s) {
    printf("%s\n", s);
    return 1;
} 
