#include "BaseNode.h"

BaseNode::BaseNode()
{
    classType = BaseNodeClass;
    nodeType = 'N';
    type = "";
    value = "";
}
BaseNode::BaseNode(string value)
{
    classType = BaseNodeClass;
    nodeType = 'N';
    type = "";
    this->value = value;
}
BaseNode::BaseNode(string type,string value)
{
    classType = BaseNodeClass;
    nodeType = 'T';
    this->type = type;
    this->value = value;
}

void BaseNode::printInfo(int deep){
    AbstractNode::printInfo(deep);
}