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
BaseNode::BaseNode(string type,string value,int num)
{
    this->num = num;
    nodeType = 'T';
    this->type = type;
    this->value = value;
}

void BaseNode::printInfo(int deep){
    if(!num){
        AbstractNode::printInfo(deep);
    }
    else{
        for(int i = 0; i < deep; i++)
            printf("| ");
        cout<< this->value<<"["<<num<<"]"<<"\n";
    }
}
