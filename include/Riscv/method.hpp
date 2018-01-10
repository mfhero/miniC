/***************************************************************
	> File Name: method.hpp
	> Author: gaoyu
	> Mail: gaoyu14@pku.edu.cn
	> Created Time: 2018-01-08 15:24
 **************************************************************/

#ifndef METHOD_HPP
#define METHOD_HPP

#include <iostream>
#include <cstring>

template <typename ...Args>
void printline(std::string op, Args... args);

void printargs(std::string);
template <typename ...Args>
void printargs(std::string, Args... args);

void printline2(std::string, std::string, std::string, std::string);


void PrintFunctionHead(std::string fname, std::string arg1, std::string arg2);

void PrintFunctionTail(std::string fname);

void PrintGlobalArray(std::string reg, std::string len);
void PrintGlobal(std::string reg, std::string val);

void PrintOp(std::string, std::string, std::string, std::string);
void Print1Op(std::string, std::string, std::string);

void PrintConditionJump(std::string, std::string, std::string, std::string);

void PrintLoadGlobal(std::string, std::string);
void PrintLoadGlobalAddr(std::string, std::string);

void PrintReturn();

#endif
