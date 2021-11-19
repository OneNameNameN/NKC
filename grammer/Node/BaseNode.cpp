#include "BaseNode.h"

BaseNode::BaseNode()
{
    nodeType = 'N';
    type = "";
    value = "";
}
BaseNode::BaseNode(string value)
{
    nodeType = 'N';
    type = "";
    this->value = value;
}
BaseNode::BaseNode(string type,string value)
{
    nodeType = 'T';
    this->type = type;
    this->value = value;
}

void BaseNode::printInfo(int deep){
    AbstractNode::printInfo(deep);
}
