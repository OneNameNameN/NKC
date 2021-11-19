#include "DefineVarNode.h"
DefineVarNode::DefineVarNode(string varType,AbstractNode* defineListNode){
    classType = DefineVarNodeClass;
    this->value = "DefineVarNode";
    this->varType = varType;
    this->defineListNode = defineListNode;
}

void DefineVarNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    for(int i = 0; i < deep+1; i++)
        printf("| ");
    cout<<this->varType;
    defineListNode->printInfo(deep+1);
}