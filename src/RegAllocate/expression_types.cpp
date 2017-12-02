/***************************************************************
	> File Name: expression_types.cpp
	> Author: gaoyu
	> Mail: gaoyu14@pku.edu.cn
	> Created Time: 2017年11月27日 星期一 22时55分23秒
 **************************************************************/

#include <iostream>

#include "RegAllocate/expression_types.hpp"

using namespace std;
using namespace rall;

static void set_factor_to_symbol(
        factor name, 
        symbol& s,
        int& v) {
    if (issymbol(name)) {
        s.name = name;
        v = 0;
    } else {
        s.name = NONE;
        v = stoi(name);
    }
}

symbol_expression::symbol_expression(rall::factor name) {
    set_factor_to_symbol(name, s, v);
    s.allowNum = true;
}

vector<symbol*> symbol_expression::used_symbol() {
    vector<symbol*> use;
    if (Valid(s))
        use.push_back(&s);
    return use;
}

string symbol_expression::tiger_string() {
    if (Valid(s))
        return s.name;
    else
        return to_string(v);
}

call_expression::call_expression(factor name_, int param_cnt) {
    name = name_;
    params_num = param_cnt;
    param_symbol.clear();
    for (int i = 0; i < param_cnt; i++) {
        param_symbol.push_back(new symbol(as_param(i)));
    }
}

string call_expression::tiger_string() {
    cerr << "call tiger_string in call_expression.\n";
    exit(1);
}

op_expression::op_expression(op op_, factor f1, factor f2) {
    o = op_;
    set_factor_to_symbol(f1, a, va);
    set_factor_to_symbol(f2, b, vb);
    if (o == "+" || o == "<")
        b.allowNum = true;
}

vector<symbol*> op_expression::used_symbol() {
    vector<symbol*> use;
    if (Valid(a))
        use.push_back(&a);
    if (Valid(b))
        use.push_back(&b); 
    return use;
}

string op_expression::tiger_string() {
    std::string f1 = Valid(a)? a.name : to_string(va);
    std::string f2 = Valid(b)? b.name : to_string(vb);
    if (f2 == "OneOp")
        return o + f1;
    return f1 + o + f2;
}

array_expression::array_expression(factor f1, factor f2) {
    set_factor_to_symbol(f1, a, vb);
    set_factor_to_symbol(f2, b, vb);
    b.allowNum = true;
}

vector<symbol*> array_expression::used_symbol() {
    vector<symbol*> use;
    if (Valid(a))
        use.push_back(&a);
    if (Valid(b))
        use.push_back(&b); 
    return use;
}

string array_expression::tiger_string() {
    std::string f2 = Valid(b)? b.name : to_string(vb);
    return a.name + "["+ f2 + "]";
}

Statement::Statement(string s, base_expression* e, factor f) 
    : a(s), exp(e) {
    a = s;
    exp = e;
    set_factor_to_symbol(f, index, offset);
    index.allowNum = true;
    static_cast<symbol_expression*>(e)->get_symbol().allowNum = false;
    a.mode = rall::writeMode;
}

vector<symbol*> Statement::all_symbol() {
    vector<symbol*> use = exp->used_symbol();
    use.push_back(&a);
    if (Valid(index))
        use.push_back(&index);
    return use;
}

const varset& Statement::used_set() {
    if (!__used_set) {
        __used_set = new rall::varset();
        *__used_set |= exp->used_set();
        if (Valid(index))
            (*__used_set)[index.id] = 1;
    }
    return *__used_set;
}

const varset& Statement::assign_set() {
    if (!__assign_set) {
        __assign_set = new rall::varset();
        if (Valid(a)) 
            (*__assign_set)[a.id] = 1;
    }
    return *__assign_set;
}

string symbol_expression::build(string& pre, string& next) {
    return allocator_string(s, v, pre, next);
}

string call_expression::build(string& pre, string& next) {
    //pre = "s0 = a0\ncall " + name + "\n" + pre;
    //next = next + "a0 = s0\n";
    pre += "call " + name  + "\n";
    return "a0";
}

string op_expression::build(string& pre, string& next) {
    if (b.name == "OneOp") {
        return o + allocator_string(a, va, pre, next);
    } else {
        return allocator_string(a, va, pre, next) 
                + " " + o + " " + 
               allocator_string(b, vb, pre, next);
    }
}

string array_expression::build(string& pre, string& next) {
    string sa = allocator_string(a, 0, pre, next);
    string sb = allocator_string(b, vb, pre, next);
    pre += "t4 = " + sa + " + " + sb + "\n"; 
    return "t4 [ 0 ]";
}

string Statement::tiger_string() { 
    string pre = "", next = "";
    string s;
    s = allocator_string(a, 0, pre, next);
    if (Valid(index)) {
        string index_s = allocator_string(index, offset, pre, next);
        pre += "t4 = " + s + " + " + index_s + "\n";
        s = "t4[ 0 ]";
    } else if (offset > 0) {
        s += " [ " + to_string(offset) + " ]";
    }
    s += " = " + exp->build(pre, next);
    return pre + s + "\n" + next;
}

string LabelStatement::tiger_string() {
    return "l" + to_string(label) + ":\n";
}

string JumpStatement::tiger_string() {
    string s = "", pre = "", next = "";
    if (has_condition())
        s += "if " + exp->build(pre, next) + " ";
    return pre + s + "goto l" + to_string(label) + "\n" + next;
}
