/***************************************************************
	> File Name: src/RegAllocate/main.cpp
	> Author: gaoyu
	> Mail: gaoyu14@pku.edu.cn
	> Created Time: 2017-11-30 16:29
**************************************************************/
 
#include <iostream>
#include <vector>
#include <map>

#include "RegAllocate/function_block.hpp"
#include "Eeyore.yacc.cpp"
#include "Eeyore.lex.cpp"

using namespace std;

vector<function_block*> functions;
map<string, int> symbol_table, global_symbol;
int lineno = 1;

int main() {
    yyparse();
    for (auto p : global_symbol) {
        string name = p.first;
        int id = p.second;
        auto it = symbol_table.find(name);
        if (it != symbol_table.end()) {
            int len = it->second;
            cout << "v" << id << " = malloc " << len << "\n";
        } else {
            cout << "v" << id << " = 0\n";
        }
    }
    for (auto f : functions) {
        f->setup();
        auto live_intervals = f->active_analysis();
        f->reg_allocate(live_intervals);
        string s = f->tiger_string();
        cout << s;
    }
    return 0;
}
