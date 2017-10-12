%{
    #include <stdio.h>
    #define YYSTYPE float
    extern YYSTYPE yylval;

    YYSTYPE calc(YYSTYPE a, YYSTYPE b, YYSTYPE oper) {
        switch ((char)(int)(oper)) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return a / b;
        }
    }
%}

%token OPERATOR1 OPERATOR2 NUMBER SPLITCHAR LB RB 

%%
program    : SPLITCHAR {}
            | expression { printf("result:%.2f\n", $1); }
            | program SPLITCHAR expression { printf("result:%.2f\n", $3); }
            | program SPLITCHAR {}

expression : expression OPERATOR1 factor { 
                $$ = calc($1, $3, $2); 
                printf("%.1f%c%.1f=%.1f\n", $1, (char)($2), $3, $$);
            }
            | factor { 
                $$ = $1;
            }

factor : factor OPERATOR2 id {  
                $$ = calc($1, $3, $2); 
                printf("%.1f%c%.1f=%.1f\n", $1, (char)($2), $3, $$);
            }
         | id { $$ = $1; } 

id : NUMBER { $$ = $1; }
   | LB expression RB { $$ = $2; }
   | OPERATOR1 id { if ($1 == (int)'+') $$ = $2; else $$ = -$2; }
 
         
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
