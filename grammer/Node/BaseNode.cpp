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
BaseNode::BaseNode(string type,string value,int n){
    nodeType = 'T';
    this->type = type;
    this->value = value;
    this->num = n;
}
BaseNode::BaseNode(string type,string value,bool isPtr){
    nodeType = 'T';
    this->type = type;
    this->value = value;
    this->isPointer = isPtr;
}
void BaseNode::printInfo(int deep){
    for(int i = 0; i < deep; i++)
        printf("| ");
    if(isPointer){
        cout<<"*";
    }
    cout<<this->value;
    if(num){
        cout<<"["<<num<<"]";
    }
    cout<<endl;
}
