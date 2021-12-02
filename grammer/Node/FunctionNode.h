#ifndef GRAMMER_FUNCTIONNODE_H
#define GRAMMER_FUNCTIONNODE_H

#include "AbstractNode.h"
class FunctionNode:public AbstractNode{
public:
    string returnType;
    string funName;
    vector<AbstractNode*> defArgs;
    AbstractNode* statementBlock;
    FunctionNode(string type,string name,AbstractNode* statms);
    FunctionNode(string type,string name,vector<AbstractNode*> defArgs,AbstractNode* statms);
    void printInfo(int deep) override;
    void createSymbolTable(bool needNewSpace) override;
};

#endif