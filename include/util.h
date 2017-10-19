/*************************************************************************
	> File Name: util.h
	> Author: gaoyu
	> Mail: 1400012705@pku.edu.cn
	> Created Time: 2017年10月19日 星期四 16时47分29秒
 ************************************************************************/

#ifndef _UTIL_H
#define _UTIL_H

TreeNode * newStmtNode(StmtKind);
TreeNode * newStmtNode(ExpKind);

char* strclone(char *);
void printTree(TreeNode *);

#endif
