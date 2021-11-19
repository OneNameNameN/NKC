#include "ExpressionNode.h"

ExpressionNode::ExpressionNode(AbstractNode* node){
    value = "ExpressionNode";
    classType = ExpressionNodeClass;
    expressionType = NumberOrID;
    this->node = node;
}
ExpressionNode::ExpressionNode(AbstractNode* node,int expressionType,AbstractNode* expressionNode,string oprStr){
    value = "ExpressionNode";
    classType = ExpressionNodeClass;
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
            for(int i = 0; i < deep+1; i++)
                printf("| ");
            cout<<this->oprStr<<"\n";
            break;
        default:break;
    }
    if(this->expressionNode){
        this->expressionNode->printInfo(deep+1);
    }
}