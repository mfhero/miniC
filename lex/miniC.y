%{
#ifndef YYPARSER 
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
//#include "scan.h"
//#include "parse.h"

#define YYSTYPE TreeNode *
static char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
extern TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void); // added 11/2/11 to ensure no conflict with lex
static void saveStatus(void); // save token name and lineno 

%}

%token IF ELSE WHILE RETURN INT MAIN
%token ASSIGN PLUS MINUS TIMES OVER MOD EQ LT GT COMMA
%token NE NOT OR AND 
%token LPAREN RPAREN LPARENX RPARENX SEMI LBLOCK RBLOCK  
%token ID NUM 
%token ERROR 
//%token END REPEAT UNTIL READ WRITE

%% /* Grammar for TINY */

program     : goal
                 { savedTree = $1;} 
            ;

goal        : definitions mainfunc
                 { 
                    $$ = newTurpleNode();
                    $$->child[0] = $1;
                    $$->child[1] = $2;
                 }
            ;

definitions :  { $$ = NULL; }
            | definitions definition {
                $$ = newTurpleNode();
                $$->child[0] = $1;
                $$->child[1] = $2;
            }
            ;

definition  : vardefinition SEMI { $$ = $1; } 
            | functiondefition { $$ = $1; }
            | functiondeclare { $$ = $1; }
            ;

vardefinition : INT ID {
                  //  $$ = newStmtNode(VarK); 
                  //  $$->attr.name = StrClone($2->attr.name);
                  //  $$->attr.val = 0;
                  //  RegisterSymbol($2->attr.name, 0);
                  $$ = $2;
                  $$->attr.val = 0;
                } 
              | INT ID LPARENX RPARENX { 
                  //  $$ = newStmtNode(VarK); 
                  //  $$->attr.name = StrClone($2->attr.name);
                  //  $$->attr.val = -1;
                  //  RegisterSymbol($2->attr.name, -1);
                  $$ = $2;
                  $$->attr.val = -1;
                } 
              | INT ID LPARENX NUM RPARENX 
                {
                  //  $$ = newStmtNode(VarK); 
                  //  $$->attr.name = StrClone($2->attr.name);
                  //  $$->attr.val = atoi($2->attr.name);
                  //  RegisterSymbol($2->attr.name, atoi($2->attr.name));
                  $$ = $2;
                  $$->attr.val = $4->attr.val;
                }
              ;

functiondefition : INT ID LPAREN vardefinitions RPAREN 
                   LBLOCK stmts RBLOCK                 
                   {
                      $$ = newFuncNode(DefK);
                      $$->attr.name = StrClone($2->attr.name);
                      $$->child[0] = $4;
                      $$->child[1] = $7;
                   }
                 ;

functiondeclare  : INT ID LPAREN vardefinitions RPAREN SEMI 
                 {
                     $$ = newFuncNode(DecK);
                     $$->attr.name = StrClone($2->attr.name);
                     $$->child[0] = $4;
                 }
                 ;

vardefinitions : {  $$ = NULL; }
                 | vardefinition { $$ = newTurpleNode(); $$->child[0] = $1; }
                 | vardefinitions COMMA vardefinition {
                    $$ = newTurpleNode();
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                }
                ;

stmts       : { $$ = NULL; }
            | stmt stmts { 
                $$ = newTurpleNode();
                $$->child[0] = $1;
                $$->child[1] = $2;
                } 
            ;

mainfunc    : INT MAIN LPAREN RPAREN LBLOCK stmts RBLOCK { 
                $$ = newFuncNode(DefK);
                $$->attr.name = "main";
                $$->child[0] = NULL;
                $$->child[1] = $6;
                }
            ;

stmt        : if_stmt { $$ = $1; }
            | while_stmt { $$ = $1; }
            | assign_stmt { $$ = $1; }
            | assign_list_stmt { $$ = $1; }
            | return_stmt { $$ = $1; }
            | vardefinition SEMI { $$ = $1; }
            | LBLOCK stmts RBLOCK { $$ = $2; }
            | expression SEMI { $$ = $1; }
            ;

if_stmt     : IF LPAREN expression RPAREN stmt 
                 { $$ = newStmtNode(IfK);
                   $$->child[0] = $3;
                   $$->child[1] = $5;
                 }
            | IF LPAREN expression RPAREN stmt ELSE stmt
                 { $$ = newStmtNode(IfK);
                   $$->child[0] = $3;
                   $$->child[1] = $5;
                   $$->child[2] = $7;
                 }
            ;

while_stmt : WHILE LPAREN expression RPAREN stmt 
                 { $$ = newStmtNode(WhileK);
                   $$->child[0] = $3;
                   $$->child[1] = $5;
                 }
            ;

assign_stmt : ID ASSIGN expression SEMI 
                 { 
                   $$ = newStmtNode(AssignK); 
                   $$->attr.name = StrClone($1->attr.name);
                   $$->lineno = lineno;
                   $$->child[0] = $3;
                 }
            ;

assign_list_stmt: ID  LPARENX expression RPARENX ASSIGN expression 
                   SEMI
                    {
                      $$ = newStmtNode(AssignK); 
                      $$->attr.name = StrClone($1->attr.name);
                      $$->lineno = lineno;
                      $$->child[0] = $6;
                      $$->child[1] = $3;
                    }
            ;

return_stmt : RETURN expression SEMI {
                       $$ = newStmtNode(AssignK);
                       $$->child[0] = $2;
                       $$->attr.name = "return";
                       $$->lineno = lineno;
                    }
            ;

expression  : boolean_exp { $$ = $1; }
            | int_exp { $$ = $1; }
            ;

boolean_exp : boolean_exp AND simple_bool_exp 
                { $$ = newExpNode(OpK);
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                  $$->attr.op = AND;
                }
            | boolean_exp OR simple_bool_exp 
                { $$ = newExpNode(OpK);
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                  $$->attr.op = OR;
                }
            | simple_bool_exp { $$ = $1; }
            ;

simple_bool_exp 
            : int_exp LT int_exp 
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = LT;
                 }
            | int_exp EQ int_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = EQ;
                 }
            | int_exp GT int_exp 
                { $$ = newExpNode(OpK); 
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                  $$->attr.op = GT;
                }
            | int_exp NE int_exp 
                { $$ = newExpNode(OpK); 
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                  $$->attr.op = NE;
                }
            | NOT simple_bool_exp 
                { $$ = newExpNode(OpK); 
                  $$->child[0] = $2;
                  $$->attr.op = NOT;
                }
            | int_exp { $$ = $1; }
            ;

int_exp    : int_exp PLUS term 
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = PLUS;
                 }
            | int_exp MINUS term
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = MINUS;
                 } 
            | term { $$ = $1; }
            ;

term        : term TIMES factor 
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = TIMES;
                 }
            | term OVER factor
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = OVER;
                 }
           | term MOD factor
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = MOD;
                 }
           
            | factor { $$ = $1; }
            ;

factor      : LPAREN expression RPAREN { $$ = $2; }
            | NUM { $$ = $1; }
            | ID { $$ = $1; }
            | ID LPARENX expression RPARENX {
                $$ = $1;
                $$->child[0] = $3;
              }
            | ID LPAREN args RPAREN {
                $$ = newExpNode(CallK);
                $$->attr.name = StrClone($1->attr.name);
                $$->child[0] = $3;
              }    
            | MINUS factor {
                $$ = newExpNode(OpK);
                $$->child[0] = $2;
                $$->attr.op = MINUS;
              }
            ;

args        : { $$ = NULL; }
            | expression {
                $$ = $1;
                }
            | args COMMA expression {
                $$ = newTurpleNode();
                $$->child[0] = $1;
                $$->child[1] = $3;
            }
            ;

%%

int yyerror(char * message)
{ 
    raiseException(message, lineno);
    printf("Syntax Error\n");
//    printf("Current token: ");
//    printToken(yychar,tokenString);
//  Error = 1;
    return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
/*static int yylex(void)
{ 
    return getToken(); 
}*/

TreeNode * parse(void)
{ 
    yyparse();
    return savedTree;
}

void saveStatus(void) {
    savedLineNo = lineno;
    savedName = StrClone(tokenString);
}

int yywrap() 
{ 
   return 1; 
} 
#endif
