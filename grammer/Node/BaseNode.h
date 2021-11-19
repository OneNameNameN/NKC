#ifndef PROJECT_BASENODE_H
#define PROJECT_BASENODE_H

#include "AbstractNode.h"
class BaseNode: public AbstractNode{
public:
    BaseNode();
    BaseNode(string value);
    BaseNode(string type,string value);
    void printInfo(int deep);
};


#endif