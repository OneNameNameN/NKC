#include "WhileNode.h"

WhileNode::WhileNode(ExpressionNode *expressionNode, AbstractNode* statementBlockNode) {
    classType = WhileNodeClass;
    this->value = "WhileNode";
    this->expressionNode = expressionNode;
    this->statementBlockNode = statementBlockNode;
}

void WhileNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    this->expressionNode->printInfo(deep+1);
    this->statementBlockNode->printInfo(deep+1);
}