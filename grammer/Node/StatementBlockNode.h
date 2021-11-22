#ifndef GRAMMER_STATEMENTBLOCKNODE_H
#define GRAMMER_STATEMENTBLOCKNODE_H
#include "AbstractNode.h"
#include "StatementsNode.h"
class StatementBlockNode:public AbstractNode{
public:
    AbstractNode* statementsNode;
    StatementBlockNode(AbstractNode* statementsNode);
    void printInfo(int deep) override;
    void createSymbolTable() override;
};

#endif