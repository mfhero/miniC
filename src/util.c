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

void printToken(const TokenType token) {
    switch (token) {
        case PLUS : printf("+"); break;
        case MINUS: printf("-"); break;    
        case TIMES: printf("*"); break;   
        case OVER: printf("/"); break; 
        case MOD: printf("%%"); break;
        case AND: printf("&&"); break; 
        case NOT: printf("!"); break; 
        case OR: printf("||"); break; 
        case EQ: printf("=="); break;
        case LT: printf("<"); break;
        case GT: printf(">"); break;
        case NE: printf("!="); break;
        default: printf("%d", (int)token); break;
    }
 //   printf("%s\n", tokenString);
}

TreeNode *newStmtNode(StmtKind kind)
{
 //   printf("build stmt node\n");
    TreeNode* t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) 
        raiseException("Out of memory", lineno);
    else {
        for (int i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

TreeNode *newExpNode(ExpKind kind)
{
 //   printf("build exp node\n");
    TreeNode* t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) 
        raiseException("Out of memory", lineno);
    else {
        for (int i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

TreeNode *newFuncNode(FuncKind kind)
{
 //   printf("build func node\n");
    TreeNode* t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) 
        raiseException("Out of memory", lineno);
    else {
        for (int i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = FuncK;
        t->kind.func = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

TreeNode *newTurpleNode()
{
    TreeNode* t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) 
        raiseException("Out of memory", lineno);
    else {
        for (int i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = TurpleK;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

char* StrClone(const char* s) {
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

void Resetindent() {
    indentno = 0;
}

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
                printf("If\n");
                break;
                case WhileK:
                printf("While\n");
                break;
                case AssignK:
                printf("Assign to: %s\n",tree->attr.name);
                break;
                default:
                printf("Unknown ExpNode kind\n");
                break;
            }
        }
        else if (tree->nodekind==ExpK) { 
            switch (tree->kind.exp) {
                case OpK:
                printf("Op: ");
                printToken(tree->attr.op);
                printf("\n");
                break;
                case ConstK:
                printf("Const: %d\n",tree->attr.val);
                break;
                case IdK:
                printf("Id: %s\n",tree->attr.name);
                break;
                case CallK:
                printf("Call: %s\n", tree->attr.name);
                break;
                default:
                printf("Unknown ExpNode kind\n");
                break;
            }
        }
        else if (tree->nodekind==TurpleK) {
            printf("Turple\n");
        } else if (tree->nodekind==FuncK) {
            printf("Function: %s\n", tree->attr.name);
        } else 
            printf("Unknown node kind\n");
        for (int i = 0; i < MAXCHILDREN; i++)
            printTree(tree->child[i]);
 //       tree = tree->sibling;
        tree = NULL;
    }
    indentno -= 2;
}   

int RegisterSymbol(char* name, int size) {
}

int IDhash(const char* ID) {
    int total = 0;
    for (int i = 0; ID[i] != '\0'; i++) 
        total = (total * 52 +
            ID[i] - (ID[i] >= 'a'? 'a' - 26 : 'A')) % 9999997;
    return total;
}
