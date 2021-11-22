#include "StatementBlockNode.h"
StatementBlockNode::StatementBlockNode(AbstractNode* statementsNode){
    this->value = "StatementBlockNode";
    this->statementsNode = statementsNode;
}

void StatementBlockNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    statementsNode->print(deep+1);
}

void StatementBlockNode::createSymbolTable(){
    if(cousin != nullptr) cousin->createSymbolTable();
    if(statementsNode)statementsNode->createSymbolTable();
    if(son != nullptr) son->createSymbolTable();
}
