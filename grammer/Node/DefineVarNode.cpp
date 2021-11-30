#include "DefineVarNode.h"
#include "ExpressionNode.h"
#include "../Intermediate/Intermediate.h"
DefineVarNode::DefineVarNode(string varType, AbstractNode *defineListNode)
{
    this->value = "DefineVarNode";
    this->varType = varType;
    this->defineListNode = defineListNode;
}

void DefineVarNode::printInfo(int deep)
{
    AbstractNode::printInfo(deep);
    for (int i = 0; i < deep + 1; i++)
        printf("| ");
    cout << this->varType << endl;
    defineListNode->print(deep + 1);
}

void DefineVarNode::createSymbolTable(bool needNewSpace)
{
    if (cousin != nullptr)
        cousin->createSymbolTable(true);
    while (defineListNode != nullptr)
    {
        if (!SymbolTable::currentTable->insert(((DefineListNode *)defineListNode)->define->son->value, this->varType, 0, 0))
        {
            cout << "Insert Error: " << ((DefineListNode *)defineListNode)->define->son->value << endl;
            exit(0);
        }
        //IM
        varStruct *varTmp = SymbolTable::currentTable->get(((DefineListNode *)defineListNode)->define->son->value);
        IM::Quaternion *quaTmp;
        // 暂时 定义变量的右值只有Expression
        ExpressionNode *exp = (ExpressionNode *)(((DefineListNode *)defineListNode)->define->son->cousin);
        if (exp == NULL)
        {
            //处理仅定义变量,未给初始值情况
            quaTmp = new IM::Quaternion(IM::DEFINE, varTmp, (varStruct*)NULL);
        }
        else if (exp->expressionType == ExpressionNode::ExpressionType::NumberOrID)
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
        Intermediate::quads->push_back(*quaTmp);
        defineListNode = ((DefineListNode *)defineListNode)->defineListNode;
    }
    if (son != nullptr)
        son->createSymbolTable(true);
}