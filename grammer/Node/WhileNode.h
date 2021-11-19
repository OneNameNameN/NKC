#ifndef PROJECT_WHILENODE_H
#define PROJECT_WHILENODE_H

#include "AbstractNode.h"
#include "ExpressionNode.h"
#include "StatementBlockNode.h"
class WhileNode:public AbstractNode{
public:
    ExpressionNode* expressionNode;
    AbstractNode* statementBlockNode;
    WhileNode(ExpressionNode* expressionNode,AbstractNode* statementBlockNode);
    void printInfo(int deep);
};

#endif