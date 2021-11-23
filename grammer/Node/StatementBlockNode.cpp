#include "StatementBlockNode.h"
StatementBlockNode::StatementBlockNode(AbstractNode* statementsNode){
    this->value = "StatementBlockNode";
    this->statementsNode = statementsNode;
}

void StatementBlockNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    statementsNode->print(deep+1);
}

void StatementBlockNode::createSymbolTable(bool needNewSpace){
    if(cousin != nullptr) cousin->createSymbolTable(true);
    if(needNewSpace)SymbolTable::rootTable->startSpace();
    if(statementsNode)statementsNode->createSymbolTable(true);
    if(needNewSpace)SymbolTable::rootTable->endSpace();
    if(son != nullptr) son->createSymbolTable(true);
}
