#include "StatementsNode.h"
#include "ExpressionNode.h"
#include "../Intermediate/Intermediate.h"


StatementsNode::StatementsNode() {
    this->value = "StatementsNode";
    this->statementNode = nullptr;
    this->statementsNode = nullptr;
}
StatementsNode::StatementsNode(AbstractNode *node) {
    this->value = "StatementsNode";
    this->statementNode = node;
    this->statementsNode = nullptr;
}
StatementsNode::StatementsNode(AbstractNode *node, AbstractNode *statementsNode) {
    this->value = "StatementsNode";
    this->statementNode = node;
    this->statementsNode = statementsNode;
}
void StatementsNode::printInfo(int deep) {
    if(statementsNode!= nullptr){
        this->statementsNode->print(deep);
    }
    if(statementNode!= nullptr){
        this->statementNode->print(deep);
    }
}

void StatementsNode::createSymbolTable(bool needNewSpace) {
    if(cousin != nullptr) cousin->createSymbolTable(true);
    if(statementsNode)statementsNode->createSymbolTable(true);
    if (statementNode)
    {
        statementNode->createSymbolTable(true);
        //处理Return语句
        if (statementNode->type == "RETURN")
        {
            ExpressionNode* exp = (ExpressionNode *)statementNode->son;
            Quaternion* quaTmp;
            if (exp->expressionType == ExpressionNode::ExpressionType::NumberOrID)
            {
                if (exp->node->type == "NUMBER")
                {
                    int arg1 = std::stoi(exp->node->value);
                    quaTmp = new IM::Quaternion(IM::RET, arg1, (varStruct*)NULL);
                }
                else if (exp->node->type == "ID")
                {
                    varStruct *arg1 = SymbolTable::currentTable->get(exp->node->value);
                    quaTmp = new IM::Quaternion(IM::RET, arg1, (varStruct*)NULL);
                }
            }
            else
            {
                varStruct *arg1 = Intermediate::generateExp(exp);
                quaTmp = new IM::Quaternion(IM::RET, arg1, (varStruct*)NULL);
            }
            Intermediate::quads->push_back(*quaTmp);
        }
        
    }
    if(son != nullptr) son->createSymbolTable(true);
}