#include "AssignNode.h"
#include "ExpressionNode.h"
#include "../Intermediate/Intermediate.h"

AssignNode::AssignNode(AbstractNode* node, AbstractNode* expression){
    value = "AssignNode";
    this->node = node;
    this->expression = expression;
}
void AssignNode::printInfo(int deep){
    AbstractNode::printInfo(deep);
    if(this->node)node->print(deep+1);
    if(this->expression)expression->print(deep+1);
}

void AssignNode::createSymbolTable(bool needNewSpace)
{
    if (cousin != nullptr)
        cousin->createSymbolTable(true);

    Intermediate::generateExp((ExpressionNode*)(this->parent));

    if (son != nullptr)
        son->createSymbolTable(true);
}
