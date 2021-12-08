#ifndef GRAMMER_ASMGENERATOR_H
#define GRAMMER_ASMGENERATOR_H

#include "symbolTable.h"
#include "Intermediate/Intermediate.h"
class AsmGenerator{
public:
    varStruct* reg[4];
    vector<Quaternion> *quads; //保存四元式
    void out(string name);
    string getReg(varStruct* target);
    string getVarName(varStruct* var,bool address);
    void generate();
    void generateData();
    void generateText();
    void generateAssign(Quaternion* quad);
    void generateCalculate(Quaternion* quad);
    void generateJump(Quaternion* quad);
    void generateParam(Quaternion* quad);
    void generateCall(Quaternion* quad);
    AsmGenerator(vector<Quaternion>* quads);
};

#endif
