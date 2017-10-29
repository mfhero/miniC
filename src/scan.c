/*******************************************************
    > File Name: scan.c
    > Author: gaoyu
    > Mail: 1400012705@pku.edu.cn
    > Created Time: 2017年10月29日 星期日 16时57分59秒
*******************************************************/

#include <assert.h>

#include "scan.h"

static TreeNode* nodes[MAXDEPTH];
static int node_cnt; 
static int availabel_tmp, availabel_label, availabel_var;

void dfs_goal(TreeNode* t) {
    availabel_label = availabel_tmp = availabel_var = node_cnt = 0;
    dfs_definitions(t->child[0]);
    dfs_mainfunc(t->child[1]);
}

void dfs_definitions(TreeNode *t) { }

void dfs_definition(TreeNode *t) { }
   
void dfs_functiondefition(TreeNode *t) { 
    assert(t->nodekind == FuncK);
    assert(t->kind.func == DefK);
    printf("//function %s:\n", t->attr.name);
    node_cnt = 0;
    dfs_vardefinitions(t->child[0]);
    printf("f_%s [%d]\n", t->attr.name, node_cnt);
    dfs_stmts(t->child[1]);
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
    const char * flag = dfs_expression(t->child[0]);
    int l1 = availabel_label++;
    printf("//if\n");
    printf("if %s < 1 goto l%d\n", flag, l1);
    dfs_stmt(t->child[1]);
    if (t->child[2] == NULL) {
        printf("//endif\n\nl%d:", l1);
    } else {
        int l2 = availabel_label++;
        printf("goto l%d\n//else\nl%d:", l2, l1);
        dfs_stmt(t->child[2]);
        printf("//endif\n\nl%d:", l2);
    }
}

void dfs_while_stmt(TreeNode *t) {
    int l1 = availabel_label++;
    printf("//while\nl%d:\n", l1);
    const char * flag = dfs_expression(t->child[0]);
    int l2 = availabel_label++;
    printf("if %s < 1 goto l%d\n", flag, l2); 
    dfs_stmt(t->child[1]);
    printf("goto l%d\n\n l%d:", l1, l2);
}

void dfs_assign_stmt(TreeNode *t) {
    const char * val = dfs_expression(t->child[0]);
    printf("T%d", IDhash(t->attr.name));
    if (t->child[1]) {
        const char * index = dfs_expression(t->child[1]); 
        printf(" [%s]", index);
    }
    printf(" = %s\n", val);
}

const char * dfs_expression(TreeNode *t) {
}


