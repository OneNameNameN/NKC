#include "ForNode.h"
ForNode::ForNode(AbstractNode* init,AbstractNode* expression,AbstractNode* action,AbstractNode* statementBlock){
    value = "ForNode";
    if(init){
        init->value = "ForInitNode";
    }
    if(expression){
        expression->value = "ForExpressionNode";
    }
    if(action){
        action->value = "ForActionNode";
    }
    if(statementBlock){
        statementBlock->value = "ForStatementNode";
    }
    this->init = init;
    this->expression = expression;
    this->action = action;
    this->statementBlock = statementBlock;
}
void ForNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    if(init)init->print(deep+1);
    if(expression)expression->print(deep+1);
    if(action)action->print(deep+1);
    if(statementBlock)statementBlock->print(deep+1);
}
void ForNode::createSymbolTable(bool needNewSpace){
    if(cousin != nullptr) cousin->createSymbolTable(true);
    SymbolTable::rootTable->startSpace();
    if(init)init->createSymbolTable(false);
    if(statementBlock)statementBlock->createSymbolTable(false);
    SymbolTable::rootTable->endSpace();
    if(son != nullptr) son->createSymbolTable(true);
}