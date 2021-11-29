#include "./Intermediate.h"
#include <typeinfo>
#include <cstdio>

varStruct *Intermediate::generateExp(ExpressionNode *exp)
{
    Quaternion *quaTmp=NULL;
    ExpressionNode *arg1_node, *arg2_node;
    switch (exp->expressionType)
    {
    case ExpressionNode::ExpressionType::NumberOrID:
    {
        break;
    }
    case ExpressionNode::ExpressionType::MonOpr:
    {
        varStruct *result = new varStruct("Temp" + to_string(Quaternion::tempVars->size()), "Integer", 0, 0, 0, 4);
        arg1_node = (ExpressionNode *)exp->node;
        Quaternion::tempVars->push_back(result);
        result = Quaternion::tempVars->back();
        if (exp->oprStr == "!")
        {
            if (arg1_node->expressionType == ExpressionNode::Relop)
            {
                generateExp((ExpressionNode *)arg1_node);
                list<int> trueL, falseL;
                trueL = trueList->top();
                trueList->pop();
                falseL = falseList->top();
                falseList->pop();
                trueList->push(falseL);
                falseList->push(trueL);
            }
            else
            {
                varStruct *result = new varStruct("Temp" + to_string(Quaternion::tempVars->size()), "Integer", 0, 0, 0, 4);
                Quaternion::tempVars->push_back(result);
                result = Quaternion::tempVars->back();
                quaTmp = Intermediate::calculateOperator(IM::LOGIC_NOT, arg1_node, (ExpressionNode *)NULL, result);
                if (quaTmp != NULL)
                {
                    Quaternion::quads->push_back(*quaTmp);
                }
                return result;
            }
        }
        break;
    }
    case ExpressionNode::ExpressionType::BinOpr:
    {
        varStruct *result = new varStruct("Temp" + to_string(Quaternion::tempVars->size()), "Integer", 0, 0, 0, 4);
        arg1_node = (ExpressionNode *)exp->node;
        arg2_node = (ExpressionNode *)exp->expressionNode;
        Quaternion::tempVars->push_back(result);
        result = Quaternion::tempVars->back();
        if (exp->oprStr == "+")
        {
            quaTmp = Intermediate::calculateOperator(IM::PLUS, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "-")
        {
            quaTmp = Intermediate::calculateOperator(IM::MINUS, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "*")
        {
            quaTmp = Intermediate::calculateOperator(IM::TIMES, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "/")
        {
            quaTmp = Intermediate::calculateOperator(IM::DIV, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "%")
        {
            quaTmp = Intermediate::calculateOperator(IM::MOD, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "^")
        {
            quaTmp = Intermediate::calculateOperator(IM::POWER, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "&&")
        {
            if (exp->node != nullptr)
            {
                exp->node->createSymbolTable(true);
                generateExp((ExpressionNode *)exp->node);
                signal->push(Quaternion::quads->size());
            }
            if (exp->expressionNode != nullptr)
            {
                exp->expressionNode->createSymbolTable(true);
                generateExp((ExpressionNode *)exp->expressionNode);
            }
            list<int> left_true, right_true, left_false, right_false;
            right_true = trueList->top();
            trueList->pop();
            left_true = trueList->top();
            trueList->pop();
            right_false = falseList->top();
            falseList->pop();
            left_false = falseList->top();
            falseList->pop();
            left_false.merge(right_false);
            falseList->push(left_false);
            trueList->push(right_true);
            backPatch(&left_true, signal->top());
            signal->pop();
        }
        else if (exp->oprStr == "||")
        {
            if (exp->node != nullptr)
            {
                exp->node->createSymbolTable(true);
                generateExp((ExpressionNode *)exp->node);
                signal->push(Quaternion::quads->size());
            }
            if (exp->expressionNode != nullptr)
            {
                exp->expressionNode->createSymbolTable(true);
                generateExp((ExpressionNode *)exp->expressionNode);
            }
            list<int> left_true, right_true, left_false, right_false;
            right_true = trueList->top();
            trueList->pop();
            left_true = trueList->top();
            trueList->pop();
            right_false = falseList->top();
            falseList->pop();
            left_false = falseList->top();
            falseList->pop();
            left_true.merge(right_true);
            trueList->push(left_true);
            falseList->push(right_false);
            backPatch(&left_false, signal->top());
            signal->pop();
        }
        else
        {
            cout<<"BinOpr Error"<<endl;
            exit(1);
        }
        if (quaTmp!=NULL)
        {
            Quaternion::quads->push_back(*quaTmp);
        }
        return result;
        break;
    }
    case ExpressionNode::ExpressionType::Address:
    {
        varStruct *tmp = new varStruct("Temp" + to_string(Quaternion::tempVars->size()), "ADDRESS", 0, 0, 0, 4);
        Quaternion::tempVars->push_back(tmp);
        quaTmp = new Quaternion(IM::GET_ADDRESS, SymbolTable::currentTable->get(exp->node->value), tmp);
        Quaternion::quads->push_back(*quaTmp);
        quaTmp = new Quaternion(IM::PARAM, tmp, (varStruct *)NULL);
        Quaternion::quads->push_back(*quaTmp);
        break;
    }
    case ExpressionNode::ExpressionType::Relop:
    {
        arg1_node = (ExpressionNode *)exp->node;
        arg2_node = (ExpressionNode *)exp->expressionNode;
        Quaternion *temp_true, *temp_false;
        if (exp->oprStr == ">")
        {
            relopOperator(temp_true, temp_false, IM::JUMP_GREAT, arg1_node, arg2_node);
        }
        else if (exp->oprStr == ">=")
        {
            relopOperator(temp_true, temp_false, IM::JUMP_EQ_GREAT, arg1_node, arg2_node);
        }
        else if (exp->oprStr == "<")
        {
            relopOperator(temp_true, temp_false, IM::JUMP_SMALL, arg1_node, arg2_node);
        }
        else if (exp->oprStr == "<=")
        {
            relopOperator(temp_true, temp_false, IM::JUMP_EQ_SMALL, arg1_node, arg2_node);
        }
        else if (exp->oprStr == "!=")
        {
            relopOperator(temp_true, temp_false, IM::JUMP_NOT_EQUAL, arg1_node, arg2_node);
        }
        else if (exp->oprStr == "==")
        {
            relopOperator(temp_true, temp_false, IM::JUMP_EQUAL, arg1_node, arg2_node);
        }
        else
        {
            cout << "RELOP ERROR\n";
            exit(1);
        }
        break;
    }
    default:
        break;
    }
}

list<int> *Intermediate::makeList(int index)
{
    list<int> *jump_list = new list<int>();
    jump_list->push_back(index);
    return jump_list;
}
list<int> *Intermediate::merge(list<int> *list1, list<int> *list2)
{
    list1->merge(*list2);
    return list1;
}
void Intermediate::backPatch(list<int> *backList, int target)
{
    list<int>::iterator it;
    for (it = backList->begin(); it != backList->end(); it++)
    {
        (*IM::Quaternion::quads)[*it].backPatch(target);
    }
    return;
}

Quaternion *Intermediate::calculateOperator(OperatorCode op, ExpressionNode *arg1_node, ExpressionNode *arg2_node, varStruct *result)
{
    Quaternion *temp;
    if (arg2_node == NULL)
    {
        if (arg1_node->node->type == "ID")
        {
            varStruct *arg1 = SymbolTable::currentTable->get(arg1_node->node->value);
            temp = new Quaternion(op, arg1, result);
        }
        else if (arg1_node->node->type == "NUMBER")
        {
            int arg1 = stoi(arg1_node->node->value);
            temp = new Quaternion(op, arg1, result);
        }
        else if (arg1_node->expressionType != ExpressionNode::NumberOrID)
        {
            varStruct *arg1 = generateExp((ExpressionNode *)arg1_node);
            temp = new Quaternion(op, arg1, result);
        }
    }
    else if (arg1_node->node->type == "ID" && arg2_node->node->type == "ID")
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
        int arg2 = stoi(arg2_node->node->value);
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
        int arg2 = stoi(arg2_node->node->value);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->node->type == "NUMBER" && arg2_node->node->type == "ID")
    {
        int arg1 = stoi(arg1_node->node->value);
        varStruct *arg2 = SymbolTable::currentTable->get(arg2_node->node->value);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->node->type == "NUMBER" && arg2_node->expressionType != ExpressionNode::NumberOrID)
    {
        int arg1 = stoi(arg1_node->node->value);
        varStruct *arg2 = generateExp((ExpressionNode *)arg2_node);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->node->type == "NUMBER" && arg2_node->node->type == "NUMBER")
    {
        int arg1 = stoi(arg1_node->node->value);
        int arg2 = stoi(arg2_node->node->value);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else
    {
        cout << "Error: calculateOperator" << endl;
        exit(1);
    }
    return temp;
}

void Intermediate::relopOperator(Quaternion *true_quad, Quaternion *false_quad, OperatorCode op, ExpressionNode *arg1_node, ExpressionNode *arg2_node)
{
    if (arg1_node->node->type == "ID" && arg2_node->node->type == "ID")
    {
        true_quad = new Quaternion(op, SymbolTable::currentTable->get(arg1_node->node->value), SymbolTable::currentTable->get(arg2_node->node->value), (int)NULL);
    }
    else if (arg1_node->node->type == "ID" && arg2_node->node->type == "NUMBER")
    {
        true_quad = new Quaternion(op, SymbolTable::currentTable->get(arg1_node->node->value), stoi(arg2_node->node->value), (int)NULL);
    }
    else if (arg1_node->node->type == "NUMBER" && arg2_node->node->type == "ID")
    {
        true_quad = new Quaternion(op, stoi(arg1_node->node->value), SymbolTable::currentTable->get(arg2_node->node->value), (int)NULL);
    }
    else if (arg1_node->node->type == "NUMBER" && arg2_node->node->type == "NUMBER")
    {
        true_quad = new Quaternion(op, stoi(arg1_node->node->value), stoi(arg2_node->node->value), (int)NULL);
    }
    else if (arg1_node->node->type == "ID" && arg2_node->expressionType != ExpressionNode::NumberOrID)
    {
        true_quad = new Quaternion(op, SymbolTable::currentTable->get(arg1_node->node->value), generateExp(arg2_node), (int)NULL);
    }
    else if (arg1_node->expressionType != ExpressionNode::NumberOrID && arg2_node->node->type == "ID")
    {
        true_quad = new Quaternion(op,generateExp(arg1_node), SymbolTable::currentTable->get(arg2_node->node->value),  (int)NULL);
    }
    else if (arg1_node->expressionType != ExpressionNode::NumberOrID && arg2_node->expressionType != ExpressionNode::NumberOrID)
    {
        true_quad = new Quaternion(op,generateExp(arg1_node), generateExp(arg2_node),  (int)NULL);
    }
    else if (arg1_node->node->type == "NUMBER" && arg2_node->expressionType != ExpressionNode::NumberOrID)
    {
        true_quad = new Quaternion(op, stoi(arg1_node->node->value), generateExp(arg2_node), (int)NULL);
    }
    else if (arg1_node->expressionType != ExpressionNode::NumberOrID && arg2_node->node->type == "NUMBER")
    {
        true_quad = new Quaternion(op,generateExp(arg1_node), stoi(arg2_node->node->value),  (int)NULL);
    }
    false_quad = new Quaternion(IM::JUMP, (int)NULL);
    list<int> trueL;
    trueL.push_back(Quaternion::quads->size());
    Quaternion::quads->push_back(*true_quad);
    list<int> falseL;
    falseL.push_back(Quaternion::quads->size());
    Quaternion::quads->push_back(*false_quad);
    trueList->push(trueL);
    falseList->push(falseL);
    return;
}

stack<list<int> >* Intermediate::falseList=new stack<list<int> >();
stack<list<int> >* Intermediate::trueList=new stack<list<int> >();
stack<int>* Intermediate::signal=new stack<int>();