#include "WhileNode.h"

WhileNode::WhileNode(AbstractNode *expressionNode, AbstractNode* statementBlockNode) {
    this->value = "WhileNode";
    this->expressionNode = expressionNode;
    this->statementBlockNode = statementBlockNode;
}

void WhileNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    this->expressionNode->printInfo(deep+1);
    this->statementBlockNode->printInfo(deep+1);
}

void WhileNode::createSymbolTable() {
    if(cousin != nullptr) cousin->createSymbolTable();
    SymbolTable::rootTable->startSpace();
    statementBlockNode->createSymbolTable();
    SymbolTable::rootTable->endSpace();
    if(son != nullptr) son->createSymbolTable();
}