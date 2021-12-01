#ifndef GRAMMER_EXPRESSIONNODE_H
#define GRAMMER_EXPRESSIONNODE_H
#include "AbstractNode.h"
class ExpressionNode:public AbstractNode{
public:
    enum ExpressionType {
        NumberOrID = 0,
        MonOpr = 1, //单目运算
        BinOpr = 2, //双目运算(不包含比较运算)
        Address = 3, //取地址
        Relop=4 //比较运算符
    };
    ExpressionNode(AbstractNode* node);
    ExpressionNode(AbstractNode* node,int expressionType,AbstractNode* expressionNode,string oprStr);
    int expressionType;
    AbstractNode* node = nullptr;
    AbstractNode* expressionNode = nullptr;
    string oprStr="";
    void printInfo(int deep) override;
    void createSymbolTable(bool needNewSpace) override;
};

#endif