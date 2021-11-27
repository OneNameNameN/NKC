#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include "./Quaternion.h"
#include "../Node/Node.h"
#include <stack>
#include <list>
using namespace IM;

class Intermediate
{
public:
    static varStruct *generateExp(ExpressionNode *);
    static Quaternion *calculateOperator(OperatorCode, ExpressionNode *, ExpressionNode *, varStruct *);

    // void generate(AbstractNode *node, SymbolTable*symbol_table);
    // Create a child symbol table to generate follow code.
    // SymbolTable *generateStatement(AST::StatementNode*, SymbolTable*);
    // SymbolTable *generateReturn(AST::StatementNode*, SymbolTable*);

    // Quaternion *caculateOperator(OperatorCode, AbstractNode*, AbstractNode*, varStruct*, SymbolTable*);
    // void relopOperator(Quaternion*, Quaternion*, OperatorCode, AbstractNode*, AbstractNode*, SymbolTable*);
    // void print();
};

#endif