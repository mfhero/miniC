/*******************************************************
    > File Name: scan.c
    > Author: gaoyu
    > Mail: 1400012705@pku.edu.cn
    > Created Time: 2017年10月29日 星期日 16时57分59秒
*******************************************************/

#include <assert.h>

#include "scan.h"

#define abs(x) ((x)>0?(x):-(x))

static TreeNode* nodes[MAXSYMBOLNUM];
static int node_cnt; 
static int available_tmp, available_label, available_var, available_param;

static Symbol* symbol_table = NULL;
static int depth;

static const char* int2str(int i) {
    return "0";
    char* s = malloc(25);
    sprintf(s, "%d", i);
    return s;
}

static void PrintSpaceBaseDepth() {
    for (int i = 0; i < depth; i++)
        printf("    ");
}

static void FlushTable() {
    Symbol *p = symbol_table;
    while (p && abs(p->depth) > abs(depth)) { 
        Symbol* q = p;
        p = p->next;       
        PrintSpaceBaseDepth();
        if (q->id > 0)
            printf("//Remove symbol %s(T%d)\n", q->name, q->id); 
        free(q);
    }
    symbol_table = p;
}
   

static int GetSymbolFromTable(const char* name) {
    FlushTable();
    for (Symbol* p = symbol_table; p; p = p->next) {
        if (strcmp(p->name, name) == 0) 
            return p->id;
    }
    printf("//Unknown Symbol\n");
    return 99999;
}

static int RegisterSymbolOnTable(const char* name, int len) {
    FlushTable();
    Symbol *p = (Symbol*)malloc(sizeof(Symbol));
    p->name = name;
    p->depth = depth;
    if (depth >= 0) 
        p->id = available_var++;
    else
        p->id = -available_param++;
    p->next = symbol_table;
    symbol_table = p;
    PrintSpaceBaseDepth();
    if (depth >= 0) {
        printf("//register %s as T%d\n", name, p->id);
        PrintSpaceBaseDepth();
        if (len > 0)
            printf("var %d T%d\n", len * 4, p->id);
        else if (len == 0)
            printf("var T%d\n", p->id);
    } else {
        printf("//register %s as p%d\n", name, -p->id - 1);
    }
}

static int get_tmp() {
    PrintSpaceBaseDepth();
    printf("var t%d\n", available_tmp);
    return available_tmp++;
}

void dfs_goal(TreeNode* t) {
    available_label = available_tmp = available_var = node_cnt = 0;
    dfs_definitions(t->child[0]);
    dfs_mainfunc(t->child[1]);
}

void dfs_definitions(TreeNode *t) { 
    if (t == NULL)
        return ;
    if (t->nodekind == TurpleK) {
        dfs_definitions(t->child[0]);
        dfs_definitions(t->child[1]);
    } else {
        dfs_definition(t);
    }
}

void dfs_definition(TreeNode *t) { 
    if (t->nodekind == FuncK) {
        if (t->kind.func == DefK)
            dfs_functiondefition(t);
    } else {
        dfs_vardefinitions(t);
    }
}
   
void dfs_functiondefition(TreeNode *t) { 
    assert(t->nodekind == FuncK);
    assert(t->kind.func == DefK);
    PrintSpaceBaseDepth();
    printf("//function %s:\n", t->attr.name);
    depth++;
    available_param = 1;
    node_cnt = 0;
    depth = -depth;
    dfs_vardefinitions(t->child[0]);
    depth = -depth;
    depth--; PrintSpaceBaseDepth(); depth++;
    printf("f_%s [%d]\n", t->attr.name, node_cnt);
    dfs_stmts(t->child[1]);
    depth--; FlushTable(); PrintSpaceBaseDepth();
    printf("end f_%s\n\n", t->attr.name);
}

void dfs_functiondeclare(TreeNode *t) {
}

void dfs_vardefinitions(TreeNode *t) { 
    if (t == NULL)
        return ;
    if (t->nodekind == TurpleK) {
        dfs_vardefinitions(t->child[0]);
        dfs_vardefinitions(t->child[1]);
    } else {
        nodes[node_cnt++] = t;
        RegisterSymbolOnTable(t->attr.name, t->attr.val);
    }
}
 
void dfs_stmts(TreeNode *t) {
    if (t == NULL)
        return ;
    if (t->nodekind == TurpleK) {
        dfs_stmts(t->child[0]);
        dfs_stmts(t->child[1]);
    } else {
        dfs_stmt(t);
    }
}
void dfs_stmt(TreeNode *t) {
    if (t == NULL) {
        printf("//Pass\n");
        return ;
    }
    if (t->nodekind == TurpleK) {
        dfs_stmts(t);
        return ;
    }
    if (t->nodekind == ExpK) {
        if (t->kind.exp == IdK) 
            RegisterSymbolOnTable(t->attr.name, t->attr.val);
        if (t->kind.exp == CallK) {
            dfs_expression(t);
           // PrintSpaceBaseDepth();
           // printf("%s\n", s);
        }
        return ;
    }
    switch (t->kind.stmt) {
        case IfK: dfs_if_stmt(t); break;
        case WhileK: dfs_while_stmt(t); break;
        case AssignK: dfs_assign_stmt(t); break;
        default: printf("//unuse statement!\n");
    }
}

void dfs_mainfunc(TreeNode *t) {
    dfs_functiondefition(t);
}

void dfs_if_stmt(TreeNode *t) {
    PrintSpaceBaseDepth();
    printf("//if\n");
    depth++;
    const char * flag = dfs_expression(t->child[0]);
    depth--; FlushTable();
    int l1 = available_label++;
    PrintSpaceBaseDepth(); 
    printf("if %s == 0 goto l%d\n", flag, l1);
    depth++;
    dfs_stmt(t->child[1]);
    depth--; FlushTable();
    if (t->child[2] == NULL) {
        PrintSpaceBaseDepth();
        printf("//endif\n");
        //PrintSpaceBaseDepth();
        printf("l%d:\n\n", l1);
    } else {
        int l2 = available_label++;
        depth++; PrintSpaceBaseDepth();
        printf("goto l%d\n", l2);
        depth--; FlushTable(); PrintSpaceBaseDepth();
        printf("//else\n");
        printf("l%d:\n", l1);
        depth++; 
        dfs_stmt(t->child[2]);
        depth--; FlushTable(); //PrintSpaceBaseDepth();
        printf("l%d:\n", l2);
        PrintSpaceBaseDepth();
        printf("//endif\n\n");
    }
}

void dfs_while_stmt(TreeNode *t) {
    int l1 = available_label++;
    PrintSpaceBaseDepth();
    printf("//while\n");
    //PrintSpaceBaseDepth();
    printf("l%d:\n", l1);
    depth++;
    const char * flag = dfs_expression(t->child[0]);
    int l2 = available_label++;
    depth--; FlushTable(); PrintSpaceBaseDepth();
    printf("if %s == 0 goto l%d\n", flag, l2); 
    depth++;
    dfs_stmt(t->child[1]);
    depth--; FlushTable(); PrintSpaceBaseDepth();
    printf("goto l%d\n", l1);
    PrintSpaceBaseDepth();
    printf("//end_while\n\n");
    //PrintSpaceBaseDepth();
    printf("l%d:\n", l2);
}

void dfs_assign_stmt(TreeNode *t) {
    assert(t->child[0]); 
    char * val = StrClone(dfs_expression(t->child[0]));
    if (strcmp(t->attr.name, "return") == 0) { 
        PrintSpaceBaseDepth();
        printf("return %s\n", val);
        return ;
    }
    int vals = GetSymbolFromTable(t->attr.name);
    char* index;
    int realindex;
    if (t->child[1]) {
        index = StrClone(dfs_expression(t->child[1])); 
        realindex = get_tmp();
        PrintSpaceBaseDepth();
        printf("t%d = %s * 4\n", realindex, index);
    }
    PrintSpaceBaseDepth();
    if (vals >= 0)
        printf("T%d", vals);
    else 
        printf("p%d", -vals - 1);
    if (t->child[1]) 
        printf(" [t%d]", realindex);
    printf(" = %s\n", val);
}

const char * dfs_expression(TreeNode *t) {
    assert(t);
    assert(t->nodekind == ExpK);
    static char res[25] = "";
    switch (t->kind.exp) {
        case OpK: {
            char* a, *b;
            a = StrClone(dfs_expression(t->child[0]));
            if (t->child[1])
                b = StrClone(dfs_expression(t->child[1]));
            int val = get_tmp();
            PrintSpaceBaseDepth();
            printf("t%d = ", val);
            if (t->child[1]) {
                printf("%s ", a);
                printToken(t->attr.op);
                printf(" %s\n", b);
            } else { 
                printToken(t->attr.op);
                printf(" %s\n", a);
            }
            sprintf(res, "t%d", val);
            return res;
        }
        case CallK:
            dfs_args(t->child[0]);
            int tmp0 = get_tmp();
            PrintSpaceBaseDepth();
            printf("t%d = call f_%s\n", tmp0, t->attr.name);
            sprintf(res, "t%d", tmp0);
            return res;
        case ConstK:
            return t->attr.name;
        case IdK: {
            int val = GetSymbolFromTable(t->attr.name);
            if (val >= 0)
                sprintf(res, "T%d", val);
            else 
                sprintf(res, "p%d", -val - 1);
            if (t->child[0]) {
                char* arr = StrClone(res);
                char* val2 = 
                    StrClone(dfs_expression(t->child[0]));
                int val3 = get_tmp();
                PrintSpaceBaseDepth();
                printf("t%d = %s * 4\n", val3, val2);
                int tmp0 = get_tmp();
                PrintSpaceBaseDepth();
                printf("t%d = %s [t%d]\n", tmp0, arr, val3);
                sprintf(res, "t%d", tmp0);
            }
            return res;
        }
        default:
            PrintSpaceBaseDepth();
            printf("Unknown Expression type.\n");
            return "Error";
    }
    //return "building..";
}

void dfs_args(TreeNode* t) {
    if (t == NULL)
        return ;
    if (t->nodekind == TurpleK) { 
        dfs_args(t->child[0]);
        dfs_args(t->child[1]);
    } else {
        const char* val = dfs_expression(t);
        PrintSpaceBaseDepth();
        printf("param %s\n", val);
    }
}


