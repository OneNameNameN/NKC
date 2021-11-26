#include "ExpressionListNode.h"
ExpressionListNode::ExpressionListNode(AbstractNode* expression,AbstractNode* expressionList){
    value = "ExpressionListNode";
    this->expression = expression;
    this->expressionList = expressionList;
}
void ExpressionListNode::printInfo(int deep){
    AbstractNode::printInfo(deep);
    this->expression->print(deep+1);
    if(this->expressionList)this->expressionList->print(deep+1);
}
