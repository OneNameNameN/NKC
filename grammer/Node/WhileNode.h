#ifndef PROJECT_WHILENODE_H
#define PROJECT_WHILENODE_H

#include "AbstractNode.h"
#include "ExpressionNode.h"
#include "StatementBlockNode.h"
class WhileNode:public AbstractNode{
public:
    AbstractNode* expressionNode;
    AbstractNode* statementBlockNode;
    WhileNode(AbstractNode* expressionNode,AbstractNode* statementBlockNode);
    void printInfo(int deep);
    void createSymbolTable();
};

#endif