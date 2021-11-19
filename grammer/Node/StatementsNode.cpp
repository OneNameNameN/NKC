#include "StatementsNode.h"

StatementsNode::StatementsNode() {
    this->value = "StatementsNode";
    this->statementNode = nullptr;
    this->statementsNode = nullptr;
}
StatementsNode::StatementsNode(AbstractNode *node) {
    this->value = "StatementsNode";
    this->statementNode = node;
    this->statementsNode = nullptr;
}
StatementsNode::StatementsNode(AbstractNode *node, AbstractNode *statementsNode) {
    this->value = "StatementsNode";
    this->statementNode = node;
    this->statementsNode = statementsNode;
}
void StatementsNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    if(statementsNode!= nullptr){
        this->statementsNode->printInfo(deep+1);
    }
    if(statementNode!= nullptr){
        this->statementNode->print(deep+1);
    }
}

void StatementsNode::createSymbolTable() {
    if(cousin != nullptr) cousin->createSymbolTable();
    if(statementNode)statementNode->createSymbolTable();
    if(statementsNode)statementsNode->createSymbolTable();
    if(son != nullptr) son->createSymbolTable();   
}