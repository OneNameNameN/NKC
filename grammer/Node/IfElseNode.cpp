#include "IfElseNode.h"
IfElseNode::IfElseNode(AbstractNode* expression,AbstractNode* ifStatementBlock){
    value = "ExpressionNode";
    if(expression){
        expression->value = "IfExpressionNode";
    }
    if(ifStatementBlock){
        ifStatementBlock->value = "IfStatementBlock";
    }
    this->expressionNode = expression;
    this->ifStatementBlock = ifStatementBlock;
    this->elseStatementBlock = nullptr;
}
IfElseNode::IfElseNode(AbstractNode* expression,AbstractNode* ifStatementBlock,AbstractNode* elseStatementBlock){
    value = "ExpressionNode";
    if(expression){
        expression->value = "IfExpressionNode";
    }
    if(ifStatementBlock){
        ifStatementBlock->value = "IfStatementBlock";
    }
    if(elseStatementBlock){
        elseStatementBlock->value = "ElseStatementBlock";
    }
    this->expressionNode = expression;
    this->ifStatementBlock = ifStatementBlock;
    this->elseStatementBlock = elseStatementBlock;
}

void IfElseNode::printInfo(int deep){
    AbstractNode::printInfo(deep);
    if(expressionNode)this->expressionNode->print(deep+1);
    if(ifStatementBlock)ifStatementBlock->print(deep+1);
    if(elseStatementBlock)elseStatementBlock->print(deep+1);
}
void IfElseNode::createSymbolTable() {
    if(cousin != nullptr) cousin->createSymbolTable();

    SymbolTable::rootTable->startSpace();
    if(ifStatementBlock)ifStatementBlock->createSymbolTable();
    SymbolTable::rootTable->endSpace();
    SymbolTable::rootTable->startSpace();
    if(elseStatementBlock)elseStatementBlock->createSymbolTable();
    SymbolTable::rootTable->endSpace();

    if(son != nullptr) son->createSymbolTable();
}