/***************************************************************
	> File Name: include/RegAllocate/util.hpp
	> Author: gaoyu
	> Mail: gaoyu14@pku.edu.cn
	> Created Time: 2017年11月27日 星期一 21时32分41秒
 **************************************************************/

#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <cstring>
#include <bitset>
#include <map>

#define MAXVARS 2000

#define MAXALLOCATOR 11
#define MAXSTACK 1000

#define NONE ("NONE")
#define Valid(A) ((A).name!=NONE&&(A).name!="OneOp")
#define NULLA (-99999) 

#define INMAP(s, M) ((M).find(s) != (M).end())

#define SREG(i) ("s" + std::to_string(i))
#define TREG(i) ("t" + std::to_string(i))
#define AREG(i) ("a" + std::to_string(i))
#define PREG(i) ("p" + std::to_string(i))
#define VREG(i) ("v" + std::to_string(i))

//function, class
namespace rall {

typedef std::bitset<MAXVARS> varset;
typedef std::string factor;
typedef std::string op;

typedef enum { functionV, paramV, normalV } var_type;
int realnum(const std::string& symbol);
bool issymbol(const factor& factor);
bool isparam(const factor& factor);
std::string as_param(const int i);
var_type check_var(const factor& var);

typedef enum { readMode, writeMode, bothMode } modeType;

class symbol { 
public:
    symbol() : allocator(NULLA) {}
    symbol(factor name_) : allocator(NULLA), name(name_) {};
    int id;
    int allocator;
    std::string name;
    modeType mode = readMode;    
    bool allowNum = false;
private:
};


std::string allocator_string(const symbol& s, int v, 
        std::string& pre_string,
        std::string& next_string);

} //namespace rall


#endif
