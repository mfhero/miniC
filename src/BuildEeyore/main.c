/*******************************************************
    > File Name: src/main.c
    > Author: gaoyu
    > Mail: 1400012705@pku.edu.cn
    > Created Time: 2017年10月26日 星期四 13时42分44秒
*******************************************************/

#include "globals.h"
#include "util.h"

#include "miniC.lex.c"
#include "miniC.yacc.c"

#include "scan.h"

int lineno;
int Error;

TreeNode * savedTree;

int main(int argv, char * argc[]) {
    lineno = 1;
    yyparse();
    if (argv > 1) {
        freopen("result/gen.ast", "w", stdout);
        printTree(savedTree);
        freopen("result/gen.out.eeyore", "w", stdout);
    }
    dfs_goal(savedTree);
    return 0;
}
