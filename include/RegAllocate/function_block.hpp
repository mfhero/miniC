/***************************************************************
	> File Name: function_block.hpp
	> Author: gaoyu
	> Mail: gaoyu14@pku.edu.cn
	> Created Time: 2017年11月29日 星期三 20时07分35秒
 **************************************************************/

#ifndef _FUNCTION_BLOCK_HPP
#define _FUNCTION_BLOCK_HPP

#include <algorithm>
#include <map>
#include <cassert>

#include "RegAllocate/util.hpp"
#include "RegAllocate/expression_types.hpp"

class live_interval 
{
public:
    std::pair<int, int> live_range;
    std::vector<std::pair<int, rall::symbol*> > refer_symbol;
    int allocator;
};

bool operator < (const live_interval&, const live_interval& );

class function_block 
{
public:
    function_block(const std::string& name_, int params_num_)
        : name(name_), params_num(params_num_) {
            label_map.clear();
            jump_map.clear();
            bjump_map.clear();
        }

    Statement*& operator [](int i) {
        assert(i < statements.size());
        return statements[i];
    }
    int length() {
        return statements.size();
    }

    void build_label(int label);
    void jump_label(int label);
    void jump_label(int label, rall::factor, rall::op, rall::factor);

    void setup();
    std::vector<live_interval> active_analysis();
    void reg_allocate(std::vector<live_interval> live_intervals);

    std::string tiger_string();

    std::vector<Statement*> statements;
    std::string name;
    int params_num, symbol_num;
    int stack_size;
private:
    //my eeyore will make sure label no increase as state_no increase
    std::map<std::string, int> used_global, used_param;
    std::map<int, int> label_map; //map<label_num, statement_no>
    std::vector<std::pair<int, int> > jump_map, bjump_map; 
};

#endif
