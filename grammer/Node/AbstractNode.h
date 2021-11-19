#ifndef PROJECT_ABSTRACTNODE_H
#define PROJECT_ABSTRACTNODE_H
#include <iostream>
#include <string>
#include <map>
using namespace std;
class AbstractNode{
public:
    enum ClassType{
        BaseNodeClass=0,
        DefineListNodeClass=1,
        DefineVarNodeClass=2,
        ExpressionNodeClass=3,
        StatementBlockNodeClass=4,
        StatementsNodeClass=5,
        WhileNodeClass=6,
        };
    char nodeType;
    int classType;
    string type;
    string value;
    AbstractNode *cousin= nullptr, *parent= nullptr, *son= nullptr;
    void addNode(AbstractNode *son);
    void print(int deep);
    virtual void printInfo(int deep);
};

#endif
