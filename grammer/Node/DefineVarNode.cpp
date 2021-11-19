#include "DefineVarNode.h"
DefineVarNode::DefineVarNode(string varType,AbstractNode* defineListNode){
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

void DefineVarNode::createSymbolTable() {
    if(cousin != nullptr) cousin->createSymbolTable();
    while(defineListNode!= nullptr){
        SymbolTable::currentTable->insert(((DefineListNode*)defineListNode)->define->value,this->varType,0,0);
        defineListNode = ((DefineListNode*)defineListNode)->defineListNode;
    }
    if(son != nullptr) son->createSymbolTable();
}