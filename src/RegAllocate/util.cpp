/***************************************************************
	> File Name: util.cpp
	> Author: gaoyu
	> Mail: gaoyu14@pku.edu.cn
	> Created Time: 2017年11月29日 星期三 22时26分43秒
 **************************************************************/

#include <cassert>
#include <algorithm>

#include "RegAllocate/util.hpp"

using namespace std;

extern map<string, int> symbol_table;

namespace rall {

int realnum(const std::string& symbol) {
    int i = 0;
    while (symbol[i] < '0' || symbol[i] > '9') i++;
    return std::stoi(symbol.substr(i));
}

bool issymbol(const factor& factor) {
    return factor[0] < '0' || factor[0] > '9';
}

bool isparam(const factor& factor) {
    return factor[0] == 'a';
}

std::string as_param(const int i) {
    return "a" + std::to_string(i); 
}

var_type check_var(const factor& var) {
    assert(issymbol(var));
    if (var[0] == 'p')
        return functionV;
    if (isparam(var))
        return paramV;
    return normalV;
}

std::string allocator_string(const symbol& s, int v, 
        std::string& pre_string,
        std::string& next_string) {
//int used_stack = count(pre_string.begin(), pre_string.end(), '\n');
    int used_tmp = count(pre_string.begin(), pre_string.end(), '\n');
    string tmp_s = TREG(used_tmp);
    if (Valid(s)) {
        if (check_var(s.name) == paramV) 
            return AREG(realnum(s.name));
       /* if (check_var(s.name) == functionV) {
            pre_string += tmp_s + " = " + AREG(realnum(s.name)) + "\n";
            next_string += AREG(realnum(s.name)) + " = " + tmp_s + "\n";
            return "a" + to_string(realnum(s.name));
        } */
        if (check_var(s.name) == normalV || check_var(s.name) == functionV) {
            //useless var
            if (s.allocator == NULLA) {
                return "t5";
            }

            //global var without register 
            if (s.allocator < 0) {
                string global_s = VREG(-s.allocator - 1);
                if (s.mode == readMode ||
                        INMAP(s.name, symbol_table)) {
                    pre_string += INMAP(s.name, symbol_table) ? 
                        "loadaddr" : "load";
                    pre_string += " " + global_s + " " + tmp_s + "\n";
                } else {
                    pre_string += "\n\n";
                    next_string += "loadaddr " + global_s + " " + 
                        TREG(used_tmp + 1) + "\n";
                    next_string += TREG(used_tmp + 1) + 
                        " [ 0 ] = " + tmp_s;
                }   
                return tmp_s;
            }
         
            //local var without register 
            if (s.allocator >= MAXALLOCATOR) {
                if (s.mode == readMode) 
                    pre_string += "load " + to_string(s.allocator) + " " + tmp_s + "\n";
                else {
                    pre_string += "\n";
                    next_string += "store " + tmp_s + " " + to_string(s.allocator) + "\n";
                }
                return tmp_s;
            }
           return SREG(s.allocator);
        }
    }
    //const 
    if (s.allowNum)
        return to_string(v);
    pre_string += tmp_s + " = " + to_string(v) + "\n";
    return tmp_s;
}

}
