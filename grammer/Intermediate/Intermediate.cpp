#include "./Intermediate.h"
#include <typeinfo>
#include <cstdio>

varStruct *Intermediate::generateExp(ExpressionNode *exp)
{
    Quaternion *quaTmp;
    AbstractNode *arg1_node, *arg2_node;
    switch (exp->expressionType)
    {
    case ExpressionNode::ExpressionType::NumberOrID:
    {
        break;
    }
    case ExpressionNode::ExpressionType::MonOpr:
    {
        break;
    }
    case ExpressionNode::ExpressionType::BinOpr:
    {
        varStruct *result = new varStruct("Temp" + to_string(Quaternion::tempVars->size()), "Integer", 0, 0, 0, 4);
        arg1_node = exp->node;
        arg2_node = exp->expressionNode;
        Quaternion::tempVars->push_back(result);
        result = Quaternion::tempVars->back();
        if (exp->oprStr == "+")
        {
            quaTmp = Intermediate::calculateOperator(IM::PLUS, (ExpressionNode *)arg1_node, (ExpressionNode *)arg2_node, result);
        }
        else if (exp->oprStr == "-")
        {
            quaTmp = Intermediate::calculateOperator(IM::MINUS, (ExpressionNode *)arg1_node, (ExpressionNode *)arg2_node, result);
        }
        else if (exp->oprStr == "*")
        {
            quaTmp = Intermediate::calculateOperator(IM::TIMES, (ExpressionNode *)arg1_node, (ExpressionNode *)arg2_node, result);
        }
        else if (exp->oprStr == "/")
        {
            quaTmp = Intermediate::calculateOperator(IM::DIV, (ExpressionNode *)arg1_node, (ExpressionNode *)arg2_node, result);
        }
        else if (exp->oprStr == "%")
        {
            quaTmp = Intermediate::calculateOperator(IM::MOD, (ExpressionNode *)arg1_node, (ExpressionNode *)arg2_node, result);
        }
        else if (exp->oprStr == "^")
        {
            quaTmp = Intermediate::calculateOperator(IM::POWER, (ExpressionNode *)arg1_node, (ExpressionNode *)arg2_node, result);
        }
        else if (exp->oprStr == "&&")
        {
            quaTmp = Intermediate::calculateOperator(IM::LOGIC_AND, (ExpressionNode *)arg1_node, (ExpressionNode *)arg2_node, result);
        }
        else if (exp->oprStr == "||")
        {
            quaTmp = Intermediate::calculateOperator(IM::LOGIC_OR, (ExpressionNode *)arg1_node, (ExpressionNode *)arg2_node, result);
        }
        Quaternion::quads->push_back(*quaTmp);
        return result;
        break;
    }
    case ExpressionNode::ExpressionType::Address:
    {
        varStruct *tmp = new varStruct("Temp" + std::to_string(Quaternion::tempVars->size()), "ADDRESS", 0, 0, 0, 4);
        Quaternion::tempVars->push_back(tmp);
        quaTmp = new Quaternion(IM::GET_ADDRESS, SymbolTable::currentTable->get(exp->node->value), tmp);
        Quaternion::quads->push_back(*quaTmp);
        quaTmp = new Quaternion(IM::PARAM, tmp, (varStruct *)NULL);
        Quaternion::quads->push_back(*quaTmp);
        break;
    }
    default:
        break;
    }
};

Quaternion *Intermediate::calculateOperator(OperatorCode op, ExpressionNode *arg1_node, ExpressionNode *arg2_node, varStruct *result)
{
    Quaternion *temp;
    if (arg1_node->node->type == "ID" && arg2_node->node->type == "ID")
    {
        varStruct *arg1 = SymbolTable::currentTable->get(arg1_node->node->value);
        varStruct *arg2 = SymbolTable::currentTable->get(arg2_node->node->value);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->node->type == "ID" && arg2_node->expressionType != ExpressionNode::NumberOrID)
    {
        varStruct *arg1 = SymbolTable::currentTable->get(arg1_node->node->value);
        varStruct *arg2 = generateExp((ExpressionNode *)arg2_node);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->node->type == "ID" && arg2_node->node->type == "NUMBER")
    {
        varStruct *arg1 = SymbolTable::currentTable->get(arg1_node->node->value);
        int arg2 = std::stoi(arg2_node->node->value);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->expressionType != ExpressionNode::NumberOrID && arg2_node->node->type == "ID")
    {
        varStruct *arg1 = generateExp((ExpressionNode *)arg1_node);
        varStruct *arg2 = SymbolTable::currentTable->get(arg2_node->node->value);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->expressionType != ExpressionNode::NumberOrID && arg2_node->expressionType != ExpressionNode::NumberOrID)
    {
        varStruct *arg1 = generateExp((ExpressionNode *)arg1_node);
        varStruct *arg2 = generateExp((ExpressionNode *)arg2_node);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->expressionType != ExpressionNode::NumberOrID && arg2_node->node->type == "NUMBER")
    {
        varStruct *arg1 = generateExp((ExpressionNode *)arg1_node);
        int arg2 = std::stoi(arg2_node->node->value);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->node->type == "NUMBER" && arg2_node->node->type == "ID")
    {
        int arg1 = std::stoi(arg1_node->node->value);
        varStruct *arg2 = SymbolTable::currentTable->get(arg2_node->node->value);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->node->type == "NUMBER" && arg2_node->expressionType != ExpressionNode::NumberOrID)
    {
        int arg1 = std::stoi(arg1_node->node->value);
        varStruct *arg2 = generateExp((ExpressionNode *)arg2_node);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->node->type == "NUMBER" && arg2_node->node->type == "NUMBER")
    {
        int arg1 = std::stoi(arg1_node->node->value);
        int arg2 = std::stoi(arg2_node->node->value);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else
    {
        cout << "Fuck: calculateOperator" << std::endl;
        exit(1);
    }
    return temp;
}
