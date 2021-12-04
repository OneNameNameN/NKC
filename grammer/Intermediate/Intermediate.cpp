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
        //对于NumberOrID,在各Node内自行处理
        break;
    }
    case ExpressionNode::ExpressionType::MonOpr:
    {
        arg1_node = (ExpressionNode *)exp->node;
        if (exp->oprStr == "!")
        {
            if (arg1_node->expressionType == ExpressionNode::Relop)
            {
                // !后为逻辑运算符,交换trueList,falseList顶层元素
                generateExp((ExpressionNode *)arg1_node);
                list<int> trueTmp = trueList->top();
                list<int> falseTmp = falseList->top();
                trueList->pop();
                falseList->pop();
                trueList->push(trueTmp);
                falseList->push(falseTmp);
            }
            else
            {
                varStruct *result = new varStruct("Temp" + to_string(tempVars->size()), "Integer", 0, 0, 0, 4);
                tempVars->push_back(result);
                result = tempVars->back();
                quaTmp =calculateOperator(IM::LOGIC_NOT, arg1_node, (ExpressionNode *)NULL, result);
                if (quaTmp != NULL)
                {
                    quads->push_back(*quaTmp);
                }
                return result;
            }
        }
        else if (exp->oprStr == "++" || exp->oprStr == "--")
        {
            varStruct *arg1 = SymbolTable::currentTable->get(arg1_node->node->value);
            BaseNode *nodeTmp = (BaseNode *)arg1_node->node;
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

                varStruct *tmp = new varStruct("Temp" + to_string(tempVars->size()), "Integer", 0, 0, 0, 4);
                tempVars->push_back(tmp);
                tmp = tempVars->back();
                if (indexInt == -1)
                {
                    quaTmp = new IM::Quaternion(IM::ASSIGN, arg1, indexVar, tmp);
                    quads->push_back(*quaTmp);
                    if (exp->oprStr == "++")
                    {
                        quaTmp = new IM::Quaternion(IM::PLUS, tmp, 1, tmp);
                    }
                    else
                    {
                        quaTmp = new IM::Quaternion(IM::MINUS, tmp, 1, tmp);
                    }
                    quads->push_back(*quaTmp);
                    quaTmp = new IM::Quaternion(IM::ASSIGN_ARRAY, tmp, indexVar, arg1);
                }
                else
                {
                    quaTmp = new IM::Quaternion(IM::ASSIGN, arg1, indexInt, tmp);
                    quads->push_back(*quaTmp);
                    if (exp->oprStr == "++")
                    {
                        quaTmp = new IM::Quaternion(IM::PLUS, tmp, 1, tmp);
                    }
                    else
                    {
                        quaTmp = new IM::Quaternion(IM::MINUS, tmp, 1, tmp);
                    }

                    quads->push_back(*quaTmp);
                    quaTmp = new IM::Quaternion(IM::ASSIGN_ARRAY, tmp, indexInt, arg1);
                }
            }
            else
            {
                if (exp->oprStr == "++")
                {
                    quaTmp = new Quaternion(IM::PLUS, arg1, 1, arg1);
                }
                else
                {
                    quaTmp = new Quaternion(IM::MINUS, arg1, 1, arg1);
                }
            }
            quads->push_back(*quaTmp);
        }
        break;
    }
    case ExpressionNode::ExpressionType::BinOpr:
    {
        varStruct *result = new varStruct("Temp" + to_string(tempVars->size()), "Integer", 0, 0, 0, 4);
        arg1_node = (ExpressionNode *)exp->node;
        arg2_node = (ExpressionNode *)exp->expressionNode;
        tempVars->push_back(result);
        result = tempVars->back();
        if (exp->oprStr == "+")
        {
            quaTmp = calculateOperator(IM::PLUS, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "-")
        {
            quaTmp = calculateOperator(IM::MINUS, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "*")
        {
            quaTmp = calculateOperator(IM::TIMES, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "/")
        {
            quaTmp = calculateOperator(IM::DIV, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "%")
        {
            quaTmp = calculateOperator(IM::MOD, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "^")
        {
            quaTmp = calculateOperator(IM::POWER, arg1_node, arg2_node, result);
        }
        else if (exp->oprStr == "&&" || exp->oprStr == "||")
        {
            if (exp->node != nullptr)
            {
                exp->node->createSymbolTable(true);
                generateExp((ExpressionNode *)exp->node);
                signal->push(quads->size());
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
            if (exp->oprStr == "&&")
            {
                left_false.merge(right_false);
                falseList->push(left_false);
                trueList->push(right_true);
                backPatch(&left_true, signal->top());
            }
            else
            {
                left_true.merge(right_true);
                trueList->push(left_true);
                falseList->push(right_false);
                backPatch(&left_false, signal->top());
            }
            signal->pop();
        }
        else
        {
            cout<<"BinOpr Error"<<endl;
            exit(1);
        }
        if (quaTmp!=NULL)
        {
            if (quaTmp->op==IM::NUM)
            {
                result = new varStruct(quaTmp->argToStr(quaTmp->args[2]), "Literal", 0, 0, 0, 4);
            }
            else
            {
                quads->push_back(*quaTmp);
            }
        }
        return result;
        break;
    }
    case ExpressionNode::ExpressionType::Address:
    {
        varStruct *result = new varStruct("Temp" + to_string(tempVars->size()), "ADDRESS", 0, 0, 0, 4);
        tempVars->push_back(result);
        quaTmp = new Quaternion(IM::GET_ADDRESS, SymbolTable::currentTable->get(exp->node->value), result);
        quads->push_back(*quaTmp);
        return result;
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
            cout << "RELOP ERROR" << endl;
            exit(1);
        }
        break;
    }
    case ExpressionNode::ExpressionType::Assign:
    {
        AssignNode* assignNode = (AssignNode*)exp->node;
        AbstractNode* node = assignNode->node;
        AbstractNode* expression = assignNode->expression;
        varStruct *varTmp = SymbolTable::currentTable->get(node->value);
        if (varTmp == NULL)
        {
            cout << "Error: use of undeclared identifier \"" << node->value << "\"" << endl;
            exit(1);
        }
        IM::Quaternion *quaTmp = NULL;
        ExpressionNode *exp = (ExpressionNode *)expression;
        if (exp == NULL)
        {
            printf("exp NULL!");
            exit(1);
        }
        BaseNode *nodeTmp = (BaseNode *)node;
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

            if (exp->expressionType == ExpressionNode::ExpressionType::NumberOrID)
            {
                if (exp->node->type == "NUMBER")
                {
                    int arg1 = std::stoi(exp->node->value);
                    if (indexInt == -1)
                    {
                        quaTmp = new IM::Quaternion(IM::ASSIGN_ARRAY, arg1, indexVar, varTmp);
                    }
                    else
                    {
                        quaTmp = new IM::Quaternion(IM::ASSIGN_ARRAY, arg1, indexInt, varTmp);
                    }
                }
                else if (exp->node->type == "ID")
                {
                    varStruct *arg1 = SymbolTable::currentTable->get(exp->node->value);
                    if (indexInt == -1)
                    {
                        quaTmp = new IM::Quaternion(IM::ASSIGN_ARRAY, arg1, indexVar, varTmp);
                    }
                    else
                    {
                        quaTmp = new IM::Quaternion(IM::ASSIGN_ARRAY, arg1, indexInt, varTmp);
                    }
                }
            }
            else
            {
                varStruct *arg1 = Intermediate::generateExp(exp);
                if (indexInt == -1)
                {
                    quaTmp = new IM::Quaternion(IM::ASSIGN_ARRAY, arg1, indexVar, varTmp);
                }
                else
                {
                    quaTmp = new IM::Quaternion(IM::ASSIGN_ARRAY, arg1, indexInt, varTmp);
                }
            }
        }
        else if (nodeTmp->isPointer)
        {
            /* code */
        }
        else
        {
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
        }

        if (quaTmp != NULL)
        {
            Intermediate::quads->push_back(*quaTmp);
        }
    }
    default:
        break;
    }
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
        (*quads)[*it].backPatch(target);
    }
    return;
}

Quaternion *Intermediate::calculateOperator(OperatorCode op, ExpressionNode *arg1_node, ExpressionNode *arg2_node, varStruct *result)
{
    Quaternion *temp;
    if (arg2_node == NULL)
    {
        //单目运算
        if (arg1_node->node->type == "ID")
        {
            varStruct *arg1 = idExp(arg1_node->node);
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
        varStruct *arg1 = idExp(arg1_node->node);
        varStruct *arg2 = idExp(arg2_node->node);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->node->type == "ID" && arg2_node->expressionType != ExpressionNode::NumberOrID)
    {
        varStruct *arg1 = idExp(arg1_node->node);
        varStruct *arg2 = generateExp((ExpressionNode *)arg2_node);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->node->type == "ID" && arg2_node->node->type == "NUMBER")
    {
        varStruct *arg1 = idExp(arg1_node->node);
        int arg2 = stoi(arg2_node->node->value);
        temp = new Quaternion(op, arg1, arg2, result);
    }
    else if (arg1_node->expressionType != ExpressionNode::NumberOrID && arg2_node->node->type == "ID")
    {
        varStruct *arg1 = generateExp((ExpressionNode *)arg1_node);
        varStruct *arg2 = idExp(arg2_node->node);
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
        varStruct *arg2 = idExp(arg2_node->node);
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
        int literal;
        if (op == PLUS)
        {
            literal = arg1 + arg2;
        }
        else if (op == MINUS)
        {
            literal = arg1 - arg2;
        }
        else if (op == TIMES)
        {
            literal = arg1 * arg2;
        }
        else if (op == DIV)
        {
            literal = arg1 / arg2;
        }
        else if (op == MOD)
        {
            literal = arg1 % arg2;
        }
        else if (op == POWER)
        {
            literal = arg1 ^ arg2;
        }
        temp = new Quaternion(IM::NUM, literal);
        tempVars->pop_back();
    }
    else
    {
        if (arg1_node->node->type == "STRING" || arg2_node->node->type == "STRING")
        {
            cout << "Error: char* can't be operated." << endl;
        }
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

    list<int> trueTmp;
    trueTmp.push_back(quads->size());
    quads->push_back(*true_quad);
    trueList->push(trueTmp);
    list<int> falseTmp;
    falseTmp.push_back(quads->size());
    quads->push_back(*false_quad);
    falseList->push(falseTmp);
    return;
}

varStruct *Intermediate::idExp(AbstractNode *node)
{
    BaseNode *nodeTmp = (BaseNode *)node;
    varStruct *arg1 = SymbolTable::currentTable->get(nodeTmp->value);
    if (nodeTmp->num)
    {
        Quaternion* quaTmp;
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
        varStruct *tmp = new varStruct("Temp" + to_string(tempVars->size()), "Integer", 0, 0, 0, 4);
        tempVars->push_back(tmp);
        tmp = tempVars->back();
        if (indexInt == -1)
        {
            quaTmp = new IM::Quaternion(IM::ASSIGN, arg1, indexVar, tmp);
        }
        else
        {
            quaTmp = new IM::Quaternion(IM::ASSIGN, arg1, indexInt, tmp);
        }
        quads->push_back(*quaTmp);
        return tmp;
    }
    else
    {
        return arg1;
    }
}
//静态成员变量类外初始化
vector<IM::Quaternion>* Intermediate::quads = new vector<IM::Quaternion>();
vector<varStruct*>* Intermediate::tempVars = new vector<varStruct*>();
stack<list<int> >* Intermediate::falseList=new stack<list<int> >();
stack<list<int> >* Intermediate::trueList=new stack<list<int> >();
stack<int>* Intermediate::signal=new stack<int>();