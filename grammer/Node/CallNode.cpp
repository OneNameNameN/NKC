#include "CallNode.h"
CallNode::CallNode(string name,AbstractNode* expressionList){
    this->value = "CallNode";
    this->callName = name;
    this->expressionList = expressionList;
}

void CallNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    for(int i = 0; i < deep+1; i++)
        printf("| ");
    cout<< this->callName<<"\n";
    this->expressionList->print(deep);
}