#ifndef GRAMMER_BASENODE_H
#define GRAMMER_BASENODE_H

#include "AbstractNode.h"
class BaseNode: public AbstractNode{
public:
    AbstractNode* num = nullptr;
    bool isPointer = false;
    BaseNode();
    BaseNode(string value);
    BaseNode(string type,string value);
    BaseNode(string type,string value,AbstractNode* n);
    BaseNode(string type,string value,bool isPtr);
    void printInfo(int deep) override;
};


#endif