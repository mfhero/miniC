%{
%}

%token CHARAC WORD LINE

%%
file : para file  
    |   para 
;

para :   CHARAC { printf("c"); }
    |   WORD { printf("w"); }
    |   LINE { printf("l\n"); }
;
%%
