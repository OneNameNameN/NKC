#ifndef GRAMMER_ABSTRACTNODE_H
#define GRAMMER_ABSTRACTNODE_H
#include <iostream>
#include <string>
#include <map>
#include "../symbolTable.h"
#include "../Intermediate/Quaternion.h"

using namespace std;
class AbstractNode{
public:
    char nodeType;
    string type;
    string value;
    AbstractNode *cousin= nullptr, *parent= nullptr, *son= nullptr;
    void addNode(AbstractNode *son);
    void print(int deep);
    virtual void printInfo(int deep);
    virtual void createSymbolTable(bool needNewSpace);
};

#endif
