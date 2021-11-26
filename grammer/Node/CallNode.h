#ifndef GRAMMER_CALLNODE_H
#define GRAMMER_CALLNODE_H

#include "AbstractNode.h"
class CallNode:public AbstractNode{
public:
    string callName;
    AbstractNode* expressionList;
    CallNode(string name,AbstractNode* expressionList);
    void printInfo(int deep) override;
};

#endif
