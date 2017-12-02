/***************************************************************
	> File Name: expression_types.hpp
	> Author: gaoyu
	> Mail: gaoyu14@pku.edu.cn
	> Created Time: 2017年11月27日 星期一 21时45分22秒
 **************************************************************/

#ifndef _EXPRESSION_TYPES_HPP
#define _EXPRESSION_TYPES_HPP

#include <cstring>
#include <vector>
#include <bitset>

#include "RegAllocate/util.hpp"

typedef enum{symbolE, callE, opE, arrayE} expression_type;

class base_expression 
{
public:
    base_expression() {}
    virtual expression_type this_type() = 0; 
    virtual std::string tiger_string() = 0;
    virtual std::vector<rall::symbol*> used_symbol() {
        std::vector<rall::symbol*> use;
        return use;
    }
    virtual const rall::varset& used_set() {
        if (!__used_set) {
            __used_set = new rall::varset;
            for (auto i : used_symbol()) 
                (*__used_set)[i->id] = 1;
        } 
        return *__used_set;
    }
    virtual std::string build(std::string&, std::string&) = 0;
protected:
    rall::varset* __used_set = NULL; 
};

class symbol_expression : public base_expression 
{
public:
    virtual expression_type this_type() { return symbolE; } 
    symbol_expression(rall::factor name);
    rall::symbol& get_symbol() { return s; }
    virtual std::vector<rall::symbol*> used_symbol();
    virtual std::string tiger_string();
    virtual std::string build(std::string&, std::string&);
private:
    rall::symbol s;
    int v;
};

class call_expression : public base_expression 
{
public:
    virtual expression_type this_type() { return callE; } 
    call_expression(rall::factor name_, int param_cnt);
  //  virtual std::vector<rall::symbol*> used_symbol();
    virtual std::string tiger_string();
    virtual std::string build(std::string&, std::string&);
private:
    std::string name;
    int params_num; 
    std::vector<rall::symbol*> param_symbol;
};

class op_expression : public base_expression 
{
public:
    virtual expression_type this_type() { return opE; } 
    op_expression(rall::op op_, rall::factor f1, rall::factor f2 = "OneOp");
    virtual std::vector<rall::symbol*> used_symbol();
    virtual std::string tiger_string();
    virtual std::string build(std::string&, std::string&);
private:
    rall::op o;
    rall::symbol a, b;
    int va, vb;
};

class array_expression : public base_expression 
{
public:
    virtual expression_type this_type() { return arrayE; } 
    array_expression(rall::factor array, rall::factor index);
    virtual std::vector<rall::symbol*> used_symbol();
    virtual std::string tiger_string();
    virtual std::string build(std::string&, std::string&);
private:
    rall::symbol a, b;
    int vb;
};

typedef enum { assignS, labelS, jumpS } statement_type;

class Statement 
{
public:
    Statement() {}
    Statement(std::string s, base_expression* e)
        : a(s), exp(e), offset(0), index(NONE) { 
            a.mode = rall::writeMode; }
    Statement(std::string s, base_expression* e, rall::factor f);

    virtual statement_type this_type() { return assignS; }
    virtual std::vector<rall::symbol*> all_symbol();
    virtual const rall::varset& used_set();
    virtual const rall::varset& assign_set();
    virtual std::string tiger_string();
protected:
    rall::symbol a;
    base_expression* exp;
    int offset; 
    rall::symbol index;

    rall::varset *__used_set = NULL, *__assign_set = NULL; 
};

class JumpStatement; 

class LabelStatement : public Statement
{
public:
    LabelStatement(int label_, int lineno_) :
                Statement(NONE, new symbol_expression("0")),
                label(label_), lineno(lineno_) {}
    virtual statement_type this_type() { return labelS; }
    virtual std::string tiger_string();
    int label, lineno;
    JumpStatement* opp;
};

class JumpStatement : public Statement 
{
public:
    JumpStatement(int label_, int lineno_) :
                Statement(NONE, new symbol_expression("0")),
                label(label_), lineno(lineno_) {}
    virtual statement_type this_type() { return jumpS; }
    void set_exp(base_expression* exp_) { exp = exp_; }
    void operator >> (LabelStatement* opp_) {
        opp = opp_;
        opp->opp = this;
    }
    bool has_condition() {
        return exp->this_type() == opE;
    }
    virtual std::string tiger_string();
    int label, lineno;
    LabelStatement* opp;
};

#endif
