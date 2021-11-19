#include "DefineListNode.h"
DefineListNode::DefineListNode(AbstractNode* define){
    this->value = "DefineListNode";
    classType = DefineListNodeClass;
    this->define = define;
    this->defineListNode = nullptr;
}
DefineListNode::DefineListNode(AbstractNode* define, AbstractNode* defineListNode){
    classType = DefineListNodeClass;
    this->value = "DefineListNode";
    this->define = define;
    this->defineListNode = defineListNode;
}

void DefineListNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    this->define->print(deep+1);
    if(this->defineListNode!= nullptr){
        this->defineListNode->printInfo(deep+1);
    }
}