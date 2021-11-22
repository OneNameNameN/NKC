#ifndef GRAMMER_STATEMENTSNODE_H
#define GRAMMER_STATEMENTSNODE_H

#include "AbstractNode.h"
class StatementsNode:public AbstractNode{
public:
    AbstractNode* statementNode;
    AbstractNode* statementsNode;
    StatementsNode();
    StatementsNode(AbstractNode* node);
    StatementsNode(AbstractNode* node,AbstractNode* statementsNode);
    void printInfo(int deep) override;
    void createSymbolTable() override;
};

#endif
