#ifndef GRAMMER_EXPRESSIONLISTNODE_H
#define GRAMMER_EXPRESSIONLISTNODE_H

#include "AbstractNode.h"
class ExpressionListNode:public AbstractNode{
public:
    AbstractNode* expression;
    AbstractNode* expressionList;
    ExpressionListNode(AbstractNode* expression,AbstractNode* expressionList);
    void printInfo(int deep) override;
};

#endif
