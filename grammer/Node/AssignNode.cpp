#include "AssignNode.h"
#include "ExpressionNode.h"
#include "../Intermediate/Intermediate.h"

AssignNode::AssignNode(AbstractNode* node, AbstractNode* expression){
    value = "AssignNode";
    this->node = node;
    this->expression = expression;
}
void AssignNode::printInfo(int deep){
    AbstractNode::printInfo(deep);
    if(this->node)node->print(deep+1);
    if(this->expression)expression->print(deep+1);
}

void AssignNode::createSymbolTable(bool needNewSpace)
{
    if (cousin != nullptr)
        cousin->createSymbolTable(true);

    //IM
    varStruct *varTmp = SymbolTable::currentTable->get(node->value);
    if(varTmp == NULL)
    {
        cout<<"Error: use of undeclared identifier \""<<node->value<<"\""<<endl;
        exit(1);
    }
    IM::Quaternion *quaTmp = NULL;
    // 暂时 定义变量的右值只有Expression
    ExpressionNode *exp = (ExpressionNode *)expression;
    if (exp == NULL)
    {
        printf("exp NULL!");
        exit(1);
    }
    if (exp->expressionType == ExpressionNode::ExpressionType::NumberOrID)
    {
        if (exp->node->type == "NUMBER")
        {
            int arg1 = std::stoi(exp->node->value);
            quaTmp = new IM::Quaternion(IM::ASSIGN, arg1, varTmp);
        }
        else if (exp->node->type == "ID")
        {
            varStruct *arg1 = SymbolTable::currentTable->get(exp->node->value);
            quaTmp = new IM::Quaternion(IM::ASSIGN, arg1, varTmp);
        }
    }
    else
    {
        varStruct *arg1 = Intermediate::generateExp(exp);
        quaTmp = new IM::Quaternion(IM::ASSIGN, arg1, varTmp);
    }
    if (quaTmp != NULL)
    {
        Intermediate::quads->push_back(*quaTmp);
    }

    if (son != nullptr)
        son->createSymbolTable(true);
}
