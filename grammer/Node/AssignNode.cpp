#include "AssignNode.h"

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
