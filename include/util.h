/*************************************************************************
	> File Name: util.h
	> Author: gaoyu
	> Mail: 1400012705@pku.edu.cn
	> Created Time: 2017年10月19日 星期四 16时47分29秒
 ************************************************************************/

#ifndef _UTIL_H
#define _UTIL_H

TreeNode * newStmtNode(StmtKind);
TreeNode * newExpNode(ExpKind);
TreeNode * newTurpleNode();
TreeNode * newFuncNode();

char* StrClone(char *);
void printTree(TreeNode *);
int RegisterSymbol(char* name, int size);
void raiseException(const char* message, int lineno);

int IDhash(const char* );

#endif
