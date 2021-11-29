#include "ExpressionNode.h"
#include "../Intermediate/Quaternion.h"
#include "../Intermediate/Intermediate.h"

ExpressionNode::ExpressionNode(AbstractNode* node){
    value = "ExpressionNode";
    expressionType = NumberOrID;
    this->node = node;
}
ExpressionNode::ExpressionNode(AbstractNode* node,int expressionType,AbstractNode* expressionNode,string oprStr){
    value = "ExpressionNode";
    this->expressionType = expressionType;
    this->node = node;
    this->expressionNode = expressionNode;
    this->oprStr = oprStr;
}
void ExpressionNode::printInfo(int deep) {
    AbstractNode::printInfo(deep);
    this->node->print(deep+1);
    switch (expressionType) {
        case NumberOrID:break;
        case MonOpr:
        case BinOpr:
        case Address:
        case Relop:
            for(int i = 0; i < deep+1; i++)
                printf("| ");
            cout<<this->oprStr<<"\n";
            break;
        default:break;
    }
    if(this->expressionNode){
        this->expressionNode->print(deep+1);
    }
}
void ExpressionNode::createSymbolTable(bool needNewSpace)
{
    if (cousin != nullptr)
        cousin->createSymbolTable(true);
    if (node != nullptr)
    {
        node->createSymbolTable(true);
    }
    if (expressionNode != nullptr)
    {
        expressionNode->createSymbolTable(true);
    }
    if (son != nullptr)
        son->createSymbolTable(true);
}