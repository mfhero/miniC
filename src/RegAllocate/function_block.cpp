/***************************************************************
	> File Name: function_block.cpp
	> Author: gaoyu
	> Mail: gaoyu14@pku.edu.cn
	> Created Time: 2017年11月29日 星期三 21时06分33秒
 **************************************************************/

#include <unordered_map>
#include <queue>
#include <map>
#include <algorithm>
#include <cstring>
#include <iostream>

#include "RegAllocate/function_block.hpp"

using namespace std;
using namespace rall;

extern map<string, int> symbol_table;
extern map<string, int> global_symbol;

void function_block::build_label(int label) {
    int top = length();
    statements.push_back(new LabelStatement(label, top));
}

void function_block::jump_label(int label) {
    int top = length();
    statements.push_back(new JumpStatement(label, top));
}

void function_block::jump_label(int label, factor a, op o, factor b) {
    int top = length();
    statements.push_back(new JumpStatement(label, top));
    static_cast<JumpStatement*>(statements[top])->set_exp(
            new op_expression(o, a, b));
}

void function_block::setup() {
    // allocate id
    stack_size = 0;
    used_global.clear();
    unordered_map<factor, int> symbol_map;
    int cnt = 1; //0 for globals
    for (auto p : statements) 
        for (auto s : p->all_symbol()) 
            if (check_var(s->name) == normalV ||
                    check_var(s->name) == functionV) {
                auto it = symbol_map.find(s->name);
                if (it == symbol_map.end()) {
                    auto it2 = global_symbol.find(s->name);
                    if (it2 == global_symbol.end()) {
                        symbol_map[s->name] = cnt;
                        s->id = cnt++;
                    } else {
                        s->id = 0;//symbol_map[s->name] = cnt;
                        if (used_global.find(s->name) == used_global.end()) {
                            int x = (stack_size < MAXALLOCATOR) ?
                                stack_size++ : -it2->second - 1;
                            used_global[s->name] = x;
                        }
                        s->allocator = used_global[s->name];
                        cerr << s->name << " get allocator " <<
                            s->id << endl;
                    }
                } else {
                    s->id = it->second;
                }
            } else {
                s->id = 0;
            }
    symbol_num = cnt;

    //match jump label 
    unordered_map<int, LabelStatement*> label_map;
    for (auto p : statements) 
        if (p->this_type() == labelS) {
            auto p_ = static_cast<LabelStatement*>(p);
            label_map[p_->label] = p_;
        }
    for (auto p : statements) 
        if (p->this_type() == jumpS) {
            auto p_ = static_cast<JumpStatement*>(p);
            (*p_) >> label_map[p_->label];
        }
}

//for bfs
static vector<varset> live;
static vector<bool> inqueue;
static queue<int> q;

static void update_queue(int index) {
    if (!inqueue[index]) {
        inqueue[index] = true;
        q.push(index);
    }
}

static bool get_queue(int &index) {
    if (q.empty())
        return false;
    index = q.front();
    inqueue[index] = false;
    q.pop();
    return true;
}
//end bfs

vector<live_interval> function_block::active_analysis() {
    //bfs
    live = vector<varset>(length());
    inqueue = vector<bool>(symbol_num, false);
    while (!q.empty()) q.pop();

    for (int i = 0; i < length(); i++) {
        //live[i] |= statements[i]->used_set();
        live[i] = 0;
        if (statements[i]->used_set().count() > 0) 
            if (i > 0)
                update_queue(i);
    }
    int u;
    while (get_queue(u)) {
        Statement* st = statements[u];
        varset new_live;

        //next statement
        if (st->this_type() == jumpS) {
            JumpStatement* st_ = static_cast<JumpStatement*>(st);
            new_live |= live[st_->opp->lineno]; 
            if (st_->has_condition() && u + 1 < length())
                new_live |= live[u + 1];
        } else {
            if (u + 1 < length())
                new_live |= live[u + 1];
        }

        new_live &= (~st->assign_set());
        new_live |= (st->used_set());

        //update state
        if (live[u] == new_live)
            continue;
        live[u] = new_live;

        //previous statement 
        if (st->this_type() == labelS) {
            LabelStatement* st_ = static_cast<LabelStatement*>(st);
            update_queue(st_->opp->lineno);
        } 
        if (u > 0 &&
            ((!(statements[u - 1]->this_type() == jumpS)) ||
            static_cast<JumpStatement*>(statements[u - 1])->has_condition())) 
            update_queue(u - 1);
    }

    //gen live interval
    vector<live_interval> live_intervals(symbol_num);
    for (live_interval& p : live_intervals) {
        p.live_range = make_pair(length() - 1, 0);
        p.refer_symbol.clear();
    }
    for (int j = 0; j < symbol_num; j++) {
        live_interval& p = live_intervals[j];
        for (int i = 0; i < length(); i++) {
           if (live[i][j]) {
               p.live_range.first = min(p.live_range.first, i);
               p.live_range.second = max(p.live_range.second, i);
           }
        }
    }

    for (auto p : statements) {
        for (auto s : p->all_symbol())
            //if (check_var(s->name) == normalV || 
            //    check_var(s->name) == functionV)
            live_intervals[s->id].refer_symbol.push_back(
                        make_pair(0, s));
    }

    //remove symbol of global, id = 0, mean global
    live_intervals.erase(live_intervals.begin());
    return live_intervals;
}

void function_block::reg_allocate(vector<live_interval> live_intervals) {
    priority_queue<pair<int, live_interval*> > active_intervals;
    while (!active_intervals.empty()) 
        active_intervals.pop();

    sort(live_intervals.begin(), live_intervals.end());   
    for (auto p : live_intervals) {
        cerr << p.live_range.first << ',' << p.live_range.second << "\n";
        if (!p.refer_symbol.empty())
            cerr << p.refer_symbol[0].second->name << "\n";
    }   
    
    bool flag[MAXALLOCATOR + MAXSTACK];
    memset(flag, 0, sizeof(flag));
    used_param.clear();
    //stack_size = 1;
    for (int i = 0; i < stack_size; i++)
        flag[i] = true;
    for (live_interval& p : live_intervals) {
        if (p.refer_symbol.empty() || p.live_range.first > p.live_range.second)
            continue;
        while (!active_intervals.empty() 
                && -active_intervals.top().first < p.live_range.first) {
            live_interval* p = active_intervals.top().second;
            flag[p->allocator] = false;
     //       cout << p->allocator << " free\n";
            active_intervals.pop();
        }
        int min_flag;
        for (min_flag = 0; flag[min_flag]; min_flag++);
        flag[min_flag] = true;
     //   cout << min_flag << "use\n";

        stack_size = max(stack_size, min_flag + 1);
        for (auto s : p.refer_symbol) 
            s.second->allocator = min_flag;

        p.allocator = min_flag;
        //give px a register 
        if (p.live_range.first == 0 
                && check_var(p.refer_symbol[0].second->name) == functionV)
            used_param[p.refer_symbol[0].second->name] = min_flag;

        active_intervals.push(make_pair(-p.live_range.second, &p));
    }
}

string function_block::tiger_string() {
    if (name == "f_main") 
        stack_size = 0;
    string s = name + " [" + to_string(params_num) + "] [" + 
                to_string(max(0, stack_size)) + "]\n";
    //save allocators
    for (int i = 0; i < min(stack_size, MAXALLOCATOR); i++) 
        s += "store s" + to_string(i) + " " + to_string(i) + "\n";
    //get global var 
    for (auto i : used_global) {
        if (i.second < 0)
            continue;
        s += "load";
        if (INMAP(i.first, symbol_table)) 
            s += "addr"; 
        s += " " + VREG(global_symbol[i.first]) + " ";
      //  if (i.second >= 0) 
        s += SREG(i.second) + "\n";
      /*  else {
            s += TREG(0) + "\n";
            s += "store " + TREG(0) + " " + to_string(i.second) + "\n";
        } */
    }
    //get params var 
    for (auto i : used_param) 
        if (i.second < MAXALLOCATOR)
            s += SREG(i.second) + " = " + 
                 AREG(realnum(i.first)) +"\n";
        else 
            s += "store " + AREG(realnum(i.first)) +
                 to_string(i.second);

    //statements 
    for (auto p : statements) 
        s = s + p->tiger_string();

    //set global var 
    for (auto i : used_global) 
        if (!INMAP(i.first, symbol_table) && i.second >= 0) {
            string global_s = VREG(global_symbol[i.first]);
            string tmp_s = TREG(0);
            s += "loadaddr " + global_s + " " + tmp_s + "\n";
            //if (i.second < MAXALLOCATOR)
            s += tmp_s + " [ 0 ] = " + SREG(i.second) + "\n";
           /* else {
                s += "load " + to_string(i.second) + TREG(1) +"\n";
                s += tmp_s + " [ 0 ] = " + TREG(1) + "\n";
            } */
        }
    //load allocators 
    for (int i = 0; i < min(stack_size, MAXALLOCATOR); i++) 
        s += "load " + to_string(i) + " s" + to_string(i) + "\n";
    s = s + "return\nend " + name + "\n";
    return s;
}

bool operator < (const live_interval& a, const live_interval& b) {
    return a.live_range < b.live_range;
}

