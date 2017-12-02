%{
#include <cstdio>
#include <cstring>

#include <vector>
#include <map>

#include "RegAllocate/function_block.hpp"
#include "RegAllocate/expression_types.hpp"
#include "RegAllocate/util.hpp"

extern std::vector<function_block*> functions;

extern std::map<std::string, int> symbol_table, global_symbol;
static int global_symbol_cnt = 0;

static function_block* active_function;
static base_expression* last_expression;
static int param_cnt = 0, begin_label = 1001, availabel_label = 1001;

extern int lineno;
int yylex(void);
void yyerror(const char*);

#define YYSTYPE std::string

%}

%token SEMI
%token IF
%token GOTO
%token VAR
%token PARAM
%token CALL
%token RETURN
%token ENDF

%token LABEL 
%token NUM 
%token SYMBOL 
%token FUNCTION
%token OP
%token ASSIGN

%%

Program : {}
        | Program SEMI
        | Program FunctionDef 
        | Program Vardefinition SEMI {
            global_symbol[std::string($2)] = global_symbol_cnt++;
        }
;

FunctionDef : FuncBegin stmts FuncEnd 
          ;

FuncBegin : FUNCTION '[' NUM ']' SEMI {
                active_function = new function_block(
                        std::string($1), 
                        std::stoi(std::string($3)));
                functions.push_back(active_function);
                begin_label = availabel_label;
            }
          ;

FuncEnd    : ENDF FUNCTION SEMI {
                //active_function = global_function;
                for (int i = begin_label; i < availabel_label; i++)
                    active_function->build_label(i);
            }
            ;

Vardefinition : VAR SYMBOL {
                /* not pass */
                $$ = $2;
                }
              | VAR NUM SYMBOL {
                global_symbol[std::string($3)] = global_symbol_cnt++; 
                symbol_table[std::string($3)] = 
                        std::stoi(std::string($2));
                $$ = $3;
                }
              ;
stmts : 
      | stmts stmt SEMI  
      ;
stmt : 
     | assign_stmt 
     | label_stmt
     | goto_stmt
     | param_stmt 
     | return_stmt 
     | Vardefinition 
     ;
assign_stmt : SYMBOL '=' expression {
        active_function->statements.push_back(
                new Statement(std::string($1), last_expression)
                );
            }
            | SYMBOL '[' factor ']' '=' expression {
        active_function->statements.push_back(
                new Statement(std::string($1), 
                    last_expression, 
                    rall::factor($3))
            );
            }     
            ;

label_stmt : LABEL ':' {
           active_function->build_label(rall::realnum(std::string($1)));
           }
           ;

goto_stmt : GOTO LABEL {
          active_function->jump_label(rall::realnum(std::string($2)));
          }
          | IF factor OP factor GOTO LABEL {
          active_function->jump_label(rall::realnum(std::string($6)), rall::factor($2), rall::op($3), rall::factor($4));
          }
          ;

param_stmt : PARAM factor {
    active_function->statements.push_back(
        new Statement("a" + std::to_string(param_cnt++),
                    new symbol_expression(std::string($2)))
                );
            }
            ;
return_stmt : RETURN factor {
    active_function->statements.push_back(
        new Statement("a0", new symbol_expression($2)));
    active_function->jump_label(availabel_label++);
        }
expression : {}
           | factor {
        last_expression = new symbol_expression(rall::factor($1));
        }
           | CALL FUNCTION {
        last_expression = new call_expression(std::string($2), param_cnt);
        param_cnt = 0;
        }
           | OP factor {
        last_expression = new op_expression(
                rall::op($1),
                rall::factor($2));
        }
           | factor OP factor {
        last_expression = new op_expression(
                rall::op($2),
                rall::factor($1),
                rall::factor($3));
        }
           | SYMBOL '[' factor ']' {
        last_expression = new array_expression(
                rall::factor($1),
                rall::factor($3));
        }
        ;

factor : SYMBOL { $$ = $1; }
       | NUM { $$ = $1; }
%%
//Miss some check, because Eeyore must correct from my code

void yyerror(const char *s) {
    fprintf(stderr, "Line %d: %s\n", lineno, s);
}

extern "C" int yywrap(void) 
{ 
   return 1; 
} 
