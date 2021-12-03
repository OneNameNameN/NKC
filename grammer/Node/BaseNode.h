#ifndef GRAMMER_BASENODE_H
#define GRAMMER_BASENODE_H

#include "AbstractNode.h"
class BaseNode: public AbstractNode{
public:
    int num = 0;
    bool isPointer = false;
    BaseNode();
    BaseNode(string value);
    BaseNode(string type,string value);
    BaseNode(string type,string value,int n);
    BaseNode(string type,string value,bool isPtr);
    void printInfo(int deep) override;
};


#endif