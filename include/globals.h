/*************************************************************************
	> File Name: globals.h
	> Author: gaoyu
	> Mail: 1400012705@pku.edu.cn
	> Created Time: 2017年10月19日 星期四 16时34分13秒
 ************************************************************************/

#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    ELSE = 259,
    WHILE = 260,
    RETURN = 261,
    INT = 262,
    MAIN = 263,
    ASSIGN = 264,
    PLUS = 265,
    MINUS = 266,
    TIMES = 267,
    OVER = 268,
    MOD = 269,
    EQ = 270,
    LT = 271,
    GT = 272,
    COMMA = 273,
    NE = 274,
    NOT = 275,
    OR = 276,
    AND = 277,
    LPAREN = 278,
    RPAREN = 279,
    LPARENX = 280,
    RPARENX = 281,
    SEMI = 282,
    LBLOCK = 283,
    RBLOCK = 284,
    ID = 285,
    NUM = 286,
    ERROR = 287
  };
  typedef enum yytokentype TokenType;   
#endif


extern int lineno;

/****************  Syntax tree ******************/

typedef enum {StmtK,ExpK,TurpleK,FuncK} NodeKind;
typedef enum {IfK,WhileK,AssignK,GetK,PutK} StmtKind;
typedef enum {DefK,DecK} FuncKind;
typedef enum {OpK,ConstK,IdK,CallK} ExpKind;

/* ExpType is used for type checking */
typedef enum {Void,Integer,Boolean} ExpType;

#define MAXCHILDREN 5

typedef struct Treenode
{ 
    struct Treenode* child[MAXCHILDREN];
    struct Treenode* sibling;
    int lineno;
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp; FuncKind func; } kind;
    struct { 
        TokenType op;
        int val;
        char * name; 
    } attr;
    ExpType type; /* for type checking of exps */
} TreeNode;

extern int Error;

typedef const char* String;

typedef struct _symbol 
{
    int depth, id, len;
    const char* name;
    struct _symbol* next; 
} Symbol;

#endif
