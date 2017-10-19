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

typedef enum {
    /* book-keeping tokens */
    ENDFILE,ERROR,
    /* reserved words */
    IF,THEN,ELSE,END,REPEAT,UNTIL,READ,WRITE,
    /* multicharacter tokens */
    ID,NUM,
    /* special symbols */
    ASSIGN,EQ,LT,PLUS,MINUS,TIMES,OVER,LPAREN,RPAREN,SEMI
} TokenType;

extern int lineno;

/****************  Syntax tree ******************/

typedef enum {StmtK,ExpK} NodeKind;
typedef enum {IfK,WhileK,AssignK,GetK,PutK} StmtKind;
typedef enum {OpK,ConstK,IdK} ExpKind;

/* ExpType is used for type checking */
typedef enum {Void,Integer,Boolean} ExpType;

static const int MAXCHILDREN = 3;

struct TreeNode
{ 
    struct TreeNode* child[MAXCHILDREN];
    struct TreeNode* sibling;
    int lineno;
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp;} kind;
    union { 
        TokenType op;
        int val;
        char * name; 
    } attr;
    ExpType type; /* for type checking of exps */
};

extern int Error;

#endif
