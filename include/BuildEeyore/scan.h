/*******************************************************
    > File Name: include/scan.h
    > Author: gaoyu
    > Mail: 1400012705@pku.edu.cn
    > Created Time: 2017年10月29日 星期日 16时27分13秒
*******************************************************/

#ifndef _SCAN_H
#define _SCAN_H
#include "globals.h"
#include "util.h"

#define MAXSYMBOLNUM 200000
#define MAXDEPTH 200

void dfs_goal(TreeNode* t);
void dfs_definitions(TreeNode *t);
void dfs_definition(TreeNode *t);
void dfs_vardefinitions(TreeNode *t);
void dfs_functiondefition(TreeNode *t);
void dfs_functiondeclare(TreeNode *t);
void dfs_vardefinitions(TreeNode *t);
void dfs_mainfunc(TreeNode *t);
void dfs_stmts(TreeNode *t);
void dfs_stmt(TreeNode *t);
void dfs_if_stmt(TreeNode *t);
void dfs_while_stmt(TreeNode *t);
void dfs_assign_stmt(TreeNode *t);
void dfs_args(TreeNode *t);
String dfs_expression(TreeNode *t);

#endif
