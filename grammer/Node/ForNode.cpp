#include "ForNode.h"
#include "ExpressionNode.h"
#include "../Intermediate/Intermediate.h"

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

    int start = Intermediate::quads->size(); 
    list<int> Judge_true,Judge_false;
    if (expression!=nullptr)
    {
        Intermediate::generateExp((ExpressionNode *)expression);
        Judge_true = Intermediate::trueList->top();
        Intermediate::trueList->pop();
        Judge_false = Intermediate::falseList->top();
        Intermediate::falseList->pop();
        Intermediate::backPatch(&Judge_true, Judge_true.back() + 2); //跳过false时jump语句,所以加2
    }
    if (statementBlock)
    {
        if (statementBlock->type == "EXPRESSION")
        {
            Intermediate::generateExp((ExpressionNode *)statementBlock);
        }
        else
        {
            statementBlock->createSymbolTable(false);
        }
    }
    if (action != nullptr)
    {
        Intermediate::generateExp((ExpressionNode*)action);
    }

    Quaternion *temp = new Quaternion(IM::JUMP, start);
    Intermediate::quads->push_back(*temp);
    int end = Intermediate::quads->size();
    if (expression!=nullptr)
    {
        Intermediate::backPatch(&Judge_false, end);
    }

    SymbolTable::rootTable->endSpace();
    if(son != nullptr) son->createSymbolTable(true);
}