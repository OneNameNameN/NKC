#include "AbstractNode.h"
void AbstractNode::addNode(AbstractNode *son)
{
    if(son == nullptr) return;
    if(this->son == NULL){
        this->son = son;
        son->parent = this;
    }
    else{
        son->cousin = this->son;
        this->son = son;
        son->parent = this;
    }
}

void AbstractNode::print(int deep)
{
    if(cousin != nullptr) cousin->print(deep);
    this->printInfo(deep);
    if(son != nullptr) son->print(deep+1);
}
void AbstractNode::printInfo(int deep){
    for(int i = 0; i < deep; i++)
        printf("| ");
    cout<< this->value<<"\n";
}
void AbstractNode::createSymbolTable() {
    if(cousin != nullptr) cousin->createSymbolTable();
    // solve symbol table
    if(son != nullptr) son->createSymbolTable();
}