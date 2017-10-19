/*******************************************************
    > File Name: src/util.c
    > Author: gaoyu
    > Mail: 1400012705@pku.edu.cn
    > Created Time: 2017年10月19日 星期四 17时05分10秒
*******************************************************/

#include "globals.h"
#include "util.h"


void raiseException(const char *message, int lineno) {
    printf("Line %d:    %s\n", lineno, message);
    Error = 1;
}   

void printToken(const TokenType token, const char* tokenString) {
    printf("%s\n", tokenString);
}

TreeNode *newStmtNode(StmtKind kind)
{
    TreeNode* t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) 
        raiseException("Out of memory", lineno);
    else {
        for (int i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL:
        t->sibling = NULL;
        t-nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

TreeNode *newStmtNode(ExpKind kind)
{
    TreeNode* t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) 
        raiseException("Out of memory", lineno);
    else {
        for (int i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL:
        t->sibling = NULL;
        t-nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

char* strClone(char* s) {
    if (s == NULL)
        return NULL;
    char *t = malloc(strlen(s) + 1);
    if (t == NULL)
        raiseException("Out of memory", lineno);
    else 
        strcpy(t, s);
    return t;
}

static int indentno = 0;
static void printSpaces(void) {
    for (int i = 0; i < indentno - 1; i++)
        printf(" ");
    if (indentno)
        printf("|");
}

void printTree(TreeNode * tree) {
    indentno += 2;
    while (tree != NULL) {
        printSpaces();
        if (tree->nodekind==StmtK) { 
            switch (tree->kind.stmt) {
                case IfK:
                fprintf(listing,"If\n");
                break;
                case RepeatK:
                fprintf(listing,"Repeat\n");
                break;
                case AssignK:
                fprintf(listing,"Assign to: %s\n",tree->attr.name);
                break;
                case ReadK:
                fprintf(listing,"Read: %s\n",tree->attr.name);
                break;
                case WriteK:
                fprintf(listing,"Write\n");
                break;
                default:
                fprintf(listing,"Unknown ExpNode kind\n");
                break;
            }
        }
        else if (tree->nodekind==ExpK) { 
            switch (tree->kind.exp) {
                case OpK:
                fprintf(listing,"Op: ");
                printToken(tree->attr.op,"\0");
                break;
                case ConstK:
                fprintf(listing,"Const: %d\n",tree->attr.val);
                break;
                case IdK:
                fprintf(listing,"Id: %s\n",tree->attr.name);
                break;
                default:
                fprintf(listing,"Unknown ExpNode kind\n");
                break;
            }
        }
        else fprintf(listing,"Unknown node kind\n");
        for (int i = 0; i < MAXCHILDREN; i++)
        printTree(tree->child[i]);
        tree = tree->sibling;
    }
    intdentno -= 2;
}   

