#include "StatementsNode.h"

StatementsNode::StatementsNode() {
    classType = StatementsNodeClass;
    this->value = "StatementsNode";
    this->statementNode = nullptr;
    this->statementsNode = nullptr;
}
StatementsNode::StatementsNode(AbstractNode *node) {
    classType = StatementsNodeClass;
    this->value = "StatementsNode";
    this->statementNode = node;
    this->statementsNode = nullptr;
}
StatementsNode::StatementsNode(AbstractNode *node, AbstractNode *statementsNode) {
    classType = StatementsNodeClass;
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