#include "WhileNode.h"
#include "../Intermediate/Quaternion.h"
#include "../Intermediate/Intermediate.h"

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

void WhileNode::createSymbolTable(bool needNewSpace)
{
    if (cousin != nullptr)
        cousin->createSymbolTable(true);

    int start = Quaternion::quads->size();
    Intermediate::generateExp((ExpressionNode *)expressionNode);
    list<int> Judge_true = Intermediate::trueList->top();
    list<int> Judge_false = Intermediate::falseList->top();
    Intermediate::trueList->pop();
    Intermediate::falseList->pop();
    Intermediate::backPatch(&Judge_true, Judge_true.back() + 2);

    SymbolTable::rootTable->startSpace();
    statementBlockNode->createSymbolTable(false);
    SymbolTable::rootTable->endSpace();

    Quaternion *temp = new Quaternion(IM::JUMP, start);
    Quaternion::quads->push_back(*temp);
    int end = Quaternion::quads->size();
    Intermediate::backPatch(&Judge_false, end);

    if (son != nullptr)
        son->createSymbolTable(true);
}