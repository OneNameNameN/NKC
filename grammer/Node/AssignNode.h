#ifndef GRAMMER_ASSIGNNODE_H
#define GRAMMER_ASSIGNNODE_H

#include "AbstractNode.h"
class AssignNode:public AbstractNode{
public:
    AbstractNode* node;
    AbstractNode* expression;
    AssignNode(AbstractNode* node, AbstractNode* expression);
    void printInfo(int deep) override;
};

#endif
