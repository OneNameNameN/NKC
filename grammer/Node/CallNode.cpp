#include "CallNode.h"
#include "ExpressionListNode.h"
#include "ExpressionNode.h"
#include "../Intermediate/Intermediate.h"
CallNode::CallNode(string name, AbstractNode *expressionList)
{
    this->value = "CallNode";
    this->callName = name;
    this->expressionList = expressionList;
}

void CallNode::printInfo(int deep)
{
    AbstractNode::printInfo(deep);
    for (int i = 0; i < deep + 1; i++)
        printf("| ");
    cout << this->callName << "\n";
    this->expressionList->print(deep);
}

void CallNode::createSymbolTable(bool needNewSpace)
{
    if (cousin != nullptr)
        cousin->createSymbolTable(true);
    // IM
    int count = 0;
    ExpressionListNode *expList = (ExpressionListNode *)expressionList;
    ExpressionNode *exp;
    while (expList != NULL)
    {
        count = count + 1;
        exp = (ExpressionNode *)expList->expression;
        Quaternion *quaTmp;
        if (exp == NULL)
        {
            printf("exp NULL ERROR!");
            exit(1);
        }
        if (exp->expressionType==ExpressionNode::ExpressionType::NumberOrID)
        {
            if (exp->node->type == "NUMBER")
            {
                int arg1 = std::stoi(exp->node->value);
                quaTmp = new IM::Quaternion(IM::PARAM, arg1, (varStruct *)NULL);
            }
            else if (exp->node->type == "ID")
            {
                varStruct *arg1 = SymbolTable::currentTable->get(exp->node->value);
                BaseNode* nodeTmp = (BaseNode*)exp->node;
                if (nodeTmp->num)
                {
                    int indexInt = -1;
                    varStruct *indexVar = NULL;
                    ExpressionNode *expIndex = (ExpressionNode *)(nodeTmp->num);
                    if (expIndex->node->type == "NUMBER")
                    {
                        indexInt = stoi(expIndex->node->value);
                    }
                    else if (expIndex->node->type == "ID")
                    {
                        indexVar = SymbolTable::currentTable->get(expIndex->node->value);
                    }
                    else if (expIndex->expressionType != ExpressionNode::ExpressionType::NumberOrID)
                    {
                        indexVar = Intermediate::generateExp(expIndex);
                    }
                    if (indexInt == -1)
                    {
                        quaTmp = new IM::Quaternion(IM::PARAM, arg1, indexVar, (varStruct *)NULL);
                    }
                    else
                    {
                        quaTmp = new IM::Quaternion(IM::ASSIGN_ARRAY, arg1, indexInt, (varStruct *)NULL);
                    }
                }
                else
                {
                    quaTmp = new IM::Quaternion(IM::PARAM, arg1, (varStruct *)NULL);
                } 
            }
            else if (exp->node->type == "STRING")
            {
                //针对调用函数中的string的权宜之策
                // varStruct *tmp = new varStruct("Temp" + std::to_string(Intermediate::tempVars->size()), "STRING", 0, 0, 0, 2 * exp->node->value.size());
                // Intermediate::tempVars->push_back(tmp);
                varStruct *str = new varStruct(exp->node->value, "STRING", 0, 0, 0, 0);
                // quaTmp = new Quaternion(IM::ASSIGN, str, tmp);
                // Intermediate::quads->push_back(*quaTmp);
                quaTmp = new Quaternion(IM::PARAM, str, (varStruct *)NULL);
            }
            Intermediate::quads->push_back(*quaTmp);
        }
        else
        {
            varStruct *arg1 = Intermediate::generateExp((ExpressionNode *)exp);
            quaTmp = new Quaternion(IM::PARAM, arg1, (varStruct *)NULL);
            Intermediate::quads->push_back(*quaTmp);            
        }
        expList = (ExpressionListNode *)expList->expressionList;
    }
    varStruct *varTmp = new varStruct(this->callName, "FUNCTION", 0, 0, 0, 0);
    Quaternion *quaTmp = new Quaternion(IM::CALL, varTmp, count, (varStruct *)NULL);
    Intermediate::quads->push_back(*quaTmp);

    if (son != nullptr)
        son->createSymbolTable(true);
}