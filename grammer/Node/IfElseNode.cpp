#include "IfElseNode.h"
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
    int start = Intermediate::quads->size();
    list<int> Judge_true = Intermediate::trueList->top();
    Intermediate::trueList->pop();
    list<int> Judge_false = Intermediate::falseList->top();
    Intermediate::falseList->pop();
    Intermediate::backPatch(&Judge_true,start);

    SymbolTable::rootTable->startSpace();
    if(ifStatementBlock)ifStatementBlock->createSymbolTable(false);
    SymbolTable::rootTable->endSpace();
    SymbolTable::rootTable->startSpace();
    if(elseStatementBlock)
    {
        Quaternion *temp = new Quaternion(IM::JUMP, (int)NULL);
        Intermediate::quads->push_back(*temp);
        int tempIndex = Intermediate::quads->size() - 1;
        int else_start = Intermediate::quads->size();

        elseStatementBlock->createSymbolTable(false);

        Intermediate::backPatch(&Judge_false, else_start);
        int end = Intermediate::quads->size();
        (*Intermediate::quads)[tempIndex].backPatch(end);
    }
    else
    {
        int end = Intermediate::quads->size();
        Intermediate::backPatch(&Judge_false, end);
    }
    SymbolTable::rootTable->endSpace();

    if(son != nullptr) son->createSymbolTable(true);
}