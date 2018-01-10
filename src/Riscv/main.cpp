/***************************************************************
	> File Name: src/Riscv/main.cpp
	> Author: gaoyu
	> Mail: gaoyu14@pku.edu.cn
	> Created Time: 2018-01-08 15:21
 **************************************************************/

#include <cstdio>
#include <cstring>

#include "Riscv/method.hpp"

#include "tigger.yacc.cpp"
#include "tigger.lex.cpp"

int main() 
{
    yyparse();
    return 0;
}
