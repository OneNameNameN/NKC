#include "IfElseNode.h"
#include "../Intermediate/Quaternion.h"
#include "../Intermediate/Intermediate.h"

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
void IfElseNode::createSymbolTable(bool needNewSpace) {
    if(cousin != nullptr) cousin->createSymbolTable(true);

    Intermediate::generateExp((ExpressionNode*)expressionNode);
    int start = Quaternion::quads->size();
    list<int> Judge_true = Intermediate::trueList->top();
    list<int> Judge_false = Intermediate::falseList->top();
    Intermediate::trueList->pop();
    Intermediate::falseList->pop();
    Intermediate::backPatch(&Judge_true,start);

    SymbolTable::rootTable->startSpace();
    if(ifStatementBlock)ifStatementBlock->createSymbolTable(false);
    SymbolTable::rootTable->endSpace();
    SymbolTable::rootTable->startSpace();
    if(elseStatementBlock)
    {
        Quaternion *temp = new Quaternion(IM::JUMP, (int)NULL);
        Quaternion::quads->push_back(*temp);
        int tempPos = Quaternion::quads->size() - 1;
        int else_start = Quaternion::quads->size();

        elseStatementBlock->createSymbolTable(false);

        Intermediate::backPatch(&Judge_false, else_start);
        int end = Quaternion::quads->size();
        (*IM::Quaternion::quads)[tempPos].backPatch(end);
    }
    else
    {
        int end = Quaternion::quads->size();
        Intermediate::backPatch(&Judge_false, end);
    }
    SymbolTable::rootTable->endSpace();

    if(son != nullptr) son->createSymbolTable(true);
}