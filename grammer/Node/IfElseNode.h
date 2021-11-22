#ifndef GRAMMER_IFELSENODE_H
#define GRAMMER_IFELSENODE_H
#include "AbstractNode.h"
class IfElseNode:public AbstractNode{
public:
    AbstractNode* expressionNode;
    AbstractNode* ifStatementBlock;
    AbstractNode* elseStatementBlock;
    IfElseNode(AbstractNode* expression,AbstractNode* ifStatementBlock);
    IfElseNode(AbstractNode* expression,AbstractNode* ifStatementBlock,AbstractNode* elseStatementBlock);
    void printInfo(int deep) override;
    void createSymbolTable() override;
};

#endif
