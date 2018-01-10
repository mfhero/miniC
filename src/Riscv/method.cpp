/***************************************************************
	> File Name: method.cpp
	> Author: gaoyu
	> Mail: gaoyu14@pku.edu.cn
	> Created Time: 2018-01-08 16:13
 **************************************************************/

#include "Riscv/method.hpp"

using namespace std;

static int stk;

template <typename ...Args>
void printline(string op, Args... args) {
    cout << "\t" << op << "\t";
    printargs(args...);
}

void printargs(string arg) {
    cout << arg << endl;
}

template <typename ...Args>
void printargs(string arg, Args... args) {
    cout << arg << ", ";
    printargs(args...);
}

void printline2(string op, string a, string b, string c) {
    printline(op, a, b + "(" + c + ")");
}

void PrintFunctionHead(string fname, string arg1, string arg2) {
    fname = fname.substr(2);
    stk = (stoi(arg2) / 4 + 1) * 16;
    cout << "\t.text\n\t.align\t2\n\t.global\t" << fname 
         << "\n\t.type\t" << fname << ", @function\n" << fname 
         << ":\n\tadd\tsp,sp,-" << stk 
         << "\n\tsw\tra," << stk - 4 
         << "(sp)\n";
}

void PrintFunctionTail(string fname) {
    fname = fname.substr(2);
    cout << "\t.size\t" << fname << ",.-" << fname << "\n";
}

void PrintGlobalArray(string reg, string len) {
    printline(".comm", reg, to_string(stoi(len) * 4), "4");
}

void PrintGlobal(string reg, string val) {
    printline(".global", reg);
    printline(".section", ".sdata");
    printline(".align", "2");
    printline(".type", reg, "@object");
    printline(".size", reg, "4");
    cout << reg << ":\n";
    printline(".word", val);
}

void PrintOp(string op, string a, string b, string c) {
    string opname = "", op2 = "";
    if (op == "+") opname = "add";
    if (op == "-") opname = "sub";
    if (op == "*") opname = "mul";
    if (op == "/") opname = "div";
    if (op == "%") opname = "rem";
    if (op == "<") 
        if (c[0] >= '0' && c[0] <= '9')
            opname = "slti";
        else
            opname = "slt";
    if (op == ">") opname = "sgt";
    if (opname != "") {
        printline(opname, a, b, c);
        return ;
    }
    if (op == "&&") {
        printline("snez", b, b);
        printline("snez", c, c);
        printline("and", a, b, c);
    }
    if (op == "||") {
        printline("or", a, b, c);
        printline("snez", a, a);
    }
    if (op == "!=") {
        printline("xor", a, b, c);
        printline("snez", a, a);
    }
    if (op == "==") {
        printline("xor", a, b, c);
        printline("seqz", a, a);
    }
}

void Print1Op(string op, string a, string b) {
    if (op == "-") 
        printline("neg", a, b);
    if (op == "!")
        printline("seqz", a, b);
}

void PrintConditionJump(string op, string a, string b, string p) {
    p = "." + p;
    string opname;
    if (op == ">=") {
        printline("ble", b, a, p);
        return ;
    }
    if (op == "<") opname = "blt";
    if (op == ">") opname = "bgt";
    if (op == "!=") opname = "bne";
    if (op == "==") opname = "beq";
    if (op == "<=") opname = "ble";
    printline(opname, a, b, p);
}

void PrintLoadGlobal(string var, string reg) {
    printline2("lui", reg, "\%hi", var);
    printline2("lw", reg, "\%lo", var + ")(" + reg);
}

void PrintLoadGlobalAddr(string var, string reg) {
    printline2("lui", reg, "\%hi", var);
    printline("add", reg, reg, "\%lo(" + var + ")");
}

void PrintReturn() {
    printline2("lw", "ra", to_string(stk - 4), "sp");
    printline("add", "sp", "sp", to_string(stk));
    printline("jr", "ra");
}


