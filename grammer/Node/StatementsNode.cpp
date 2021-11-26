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
    if(statementsNode!= nullptr){
        this->statementsNode->print(deep);
    }
    if(statementNode!= nullptr){
        this->statementNode->print(deep);
    }
}

void StatementsNode::createSymbolTable(bool needNewSpace) {
    if(cousin != nullptr) cousin->createSymbolTable(true);
    if(statementNode)statementNode->createSymbolTable(true);
    if(statementsNode)statementsNode->createSymbolTable(true);
    if(son != nullptr) son->createSymbolTable(true);
}