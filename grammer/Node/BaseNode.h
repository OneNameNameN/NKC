#ifndef GRAMMER_BASENODE_H
#define GRAMMER_BASENODE_H

#include "AbstractNode.h"
class BaseNode: public AbstractNode{
public:
    BaseNode();
    BaseNode(string value);
    BaseNode(string type,string value);
    void printInfo(int deep) override;
};


#endif