#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include "./Quaternion.h"
#include "../Node/Node.h"
#include <stack>
#include <list>
using namespace IM;

class Intermediate
{
public:
    static varStruct *generateExp(ExpressionNode *); //处理ExpressionNode
    static Quaternion *calculateOperator(OperatorCode, ExpressionNode *, ExpressionNode *, varStruct *); //计算运算符
    static void relopOperator(Quaternion *, Quaternion *, OperatorCode, ExpressionNode *, ExpressionNode *); //比较运算符
    static varStruct *idExp(AbstractNode*); //处理整数型和数组型ID
    static list<int> *merge(list<int> *list1, list<int> *list2); //用于回填
    static void backPatch(list<int> *backList, int target); //回填
    static vector<Quaternion> *quads; //保存四元式
    static vector<varStruct *> *tempVars; //保存临时变量
    static stack<list<int> >* falseList; //用于判断语句
    static stack<list<int> >* trueList; //用于判断语句
    static stack<int>* signal; //用于逻辑连接符
};

#endif