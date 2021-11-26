#ifndef GRAMMER_DEFINELISTNODE_H
#define GRAMMER_DEFINELISTNODE_H

#include "AbstractNode.h"
class DefineListNode:public AbstractNode{
public:
    AbstractNode* define;
    AbstractNode* defineListNode;
    DefineListNode(AbstractNode* define);
    DefineListNode(AbstractNode* define, AbstractNode* defineListNode);
    void printInfo(int deep) override;
};

#endif
