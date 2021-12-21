#include "WhileNode.h"
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

    int start = Intermediate::quads->size();
    Intermediate::generateExp((ExpressionNode *)expressionNode);
    list<int> Judge_true = Intermediate::trueList->top();
    Intermediate::trueList->pop();
    list<int> Judge_false = Intermediate::falseList->top();
    Intermediate::falseList->pop();
    Intermediate::backPatch(&Judge_true, Judge_true.back() + 2); //跳过false jump

    SymbolTable::rootTable->startSpace();
    if (statementBlockNode->type == "EXPRESSION")
    {
        Intermediate::generateExp((ExpressionNode *)statementBlockNode);
    }
    else
    {
        statementBlockNode->createSymbolTable(false);
    }

    SymbolTable::rootTable->endSpace();

    Quaternion *temp = new Quaternion(IM::JUMP, start);
    Intermediate::quads->push_back(*temp);
    int end = Intermediate::quads->size();
    Intermediate::backPatch(&Judge_false, end);

    if (son != nullptr)
        son->createSymbolTable(true);
}