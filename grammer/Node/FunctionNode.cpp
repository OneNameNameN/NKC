#include "FunctionNode.h"
FunctionNode::FunctionNode(string type,string name,AbstractNode* statms){
    value = "FunctionNode";
    this->returnType = type;
    this->funName = name;
    this->statementBlock = statms;
}
FunctionNode::FunctionNode(string type,string name,vector<AbstractNode*> defArgs,AbstractNode* statms){
    value = "FunctionNode";
    this->returnType = type;
    this->funName = name;
    this->defArgs = defArgs;
    this->statementBlock = statms;
}
void FunctionNode::printInfo(int deep){
    AbstractNode::printInfo(deep);
    for(int i = 0; i < deep+1; i++)
        printf("| ");
    cout<< this->returnType<<"  "<< funName <<"\n";
    for(int k=0;k<defArgs.size();k++){
        this->defArgs[k]->print(deep+1);
    }
    statementBlock->print(deep+1);
}

void FunctionNode::createSymbolTable(bool needNewSpace) {
    if(cousin != nullptr) cousin->createSymbolTable(true);
    SymbolTable::currentTable->insert(this->funName, "Function", 0, 0);
    SymbolTable::rootTable->startSpace();
    for(int i=0;i<defArgs.size();i++){
        defArgs[i]->createSymbolTable(true);
    }
    statementBlock->createSymbolTable(false);
    SymbolTable::rootTable->endSpace();
    if(son != nullptr) son->createSymbolTable(true);
}