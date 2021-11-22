#ifndef GRAMMER_DEFINEVARNODE_H
#define GRAMMER_DEFINEVARNODE_H
#include "AbstractNode.h"
#include "DefineListNode.h"
class DefineVarNode:public AbstractNode{
public:
    string varType;
    AbstractNode* defineListNode;
    DefineVarNode(string varType,AbstractNode* defineListNode);
    void printInfo(int deep) override;
    void createSymbolTable() override;
};

#endif
