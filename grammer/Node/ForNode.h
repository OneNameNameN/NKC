#ifndef GRAMMER_FORNODE_H
#define GRAMMER_FORNODE_H

#include "AbstractNode.h"
class ForNode:public AbstractNode{
public:
    AbstractNode* init;
    AbstractNode* expression;
    AbstractNode* action;
    AbstractNode* statementBlock;
    ForNode(AbstractNode* init,AbstractNode* expression,AbstractNode* action,AbstractNode* statementBlock);
    void printInfo(int deep) override;
    void createSymbolTable(bool needNewSpace) override;
};

#endif
