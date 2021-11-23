#include "WhileNode.h"

WhileNode::WhileNode(AbstractNode *expressionNode, AbstractNode* statementBlockNode) {
    this->value = "WhileNode";
    this->expressionNode = expressionNode;
    this->statementBlockNode = statementBlockNode;
}

void WhileNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    this->expressionNode->print(deep+1);
    this->statementBlockNode->print(deep+1);
}

void WhileNode::createSymbolTable(bool needNewSpace) {
    if(cousin != nullptr) cousin->createSymbolTable(true);
    if(needNewSpace)SymbolTable::rootTable->startSpace();
    statementBlockNode->createSymbolTable(true);
    if(needNewSpace)SymbolTable::rootTable->endSpace();
    if(son != nullptr) son->createSymbolTable(true);
}