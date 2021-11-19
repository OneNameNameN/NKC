#include "StatementBlockNode.h"
StatementBlockNode::StatementBlockNode(AbstractNode* statementsNode){
    this->value = "StatementBlockNode";
    this->statementsNode = statementsNode;
}

void StatementBlockNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    statementsNode->printInfo(deep+1);
}