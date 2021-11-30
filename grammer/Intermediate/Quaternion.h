#ifndef QUATERNION_H
#define QUATERNION_H

#include "../symbolTable.h"
#include <string.h>
#include <vector>
namespace IM
{
    enum OperatorCode
    {
        JUMP,
        JUMP_SMALL,
        JUMP_EQ_SMALL,
        JUMP_GREAT,
        JUMP_EQ_GREAT,
        JUMP_EQUAL,
        JUMP_NOT_EQUAL,
        PLUS,
        MINUS,
        TIMES,
        DIV,
        MOD,
        POWER,
        NEGATIVE,
        ASSIGN,
        ASSIGN_ARRAY,
        ASSIGN_STRUCT,
        ASSIGN_POINTER,
        GET_ADDRESS,
        PARAM,
        CALL,
        RET,
        FUNC_DEF,
        END_FUNCTION,
        LABEL,
        GET_VALUE,
        GET_ARRAY,
        GET_STRUCT,
        LOGIC_AND,
        LOGIC_OR,
        LOGIC_NOT,
        NONE
    };

    static std::string operator_string[] = {
        "JUMP",
        "JUMP_SMALL",
        "JUMP_EQ_SMALL",
        "JUMP_GREAT",
        "JUMP_EQ_GREAT",
        "JUMP_EQUAL",
        "JUMP_NOT_EQUAL",
        "PLUS",
        "MINUS",
        "TIMES",
        "DIV",
        "MOD",
        "POWER",
        "NEGATIVE",
        "ASSIGN",
        "ASSIGN_ARRAY",
        "ASSIGN_STRUCT",
        "ASSIGN_POINTER",
        "GET_ADDRESS",
        "PARAM",
        "CALL",
        "RETURN",
        "FUNC_DEF",
        "END_FUNCTION",
        "LABEL",
        "GET_VALUE",
        "GET_ARRAY",
        "GET_STRUCT",
        "LOGIC_AND",
        "LOGIC_OR",
        "LOGIC_NOT",
        "NONE"};

    struct Arg
    {
        varStruct *var = NULL;
        int literal;
        bool isVar = true;
    };

    class Quaternion
    {
    private:
        OperatorCode op;
        Arg args[3]; // 0: arg1, 1: arg2, 2: result
        inline std::string get_op_str() { return operator_string[op]; }
        string argToStr(Arg a);

    public:
        inline Quaternion(OperatorCode op, int arg1, int arg2, int result)
        {
            this->op = op;
            this->args[0].literal = arg1;
            this->args[1].literal = arg2;
            this->args[2].literal = result;
            this->args[0].isVar = false;
            this->args[1].isVar = false;
            this->args[2].isVar = false;
        }
        inline Quaternion(OperatorCode op, varStruct *arg1, int arg2, int result)
        {
            this->op = op;
            this->args[0].var = arg1;
            this->args[1].literal = arg2;
            this->args[2].literal = result;
            this->args[0].isVar = true;
            this->args[1].isVar = false;
            this->args[2].isVar = false;
        }
        inline Quaternion(OperatorCode op, int result)
        {
            this->op = op;
            this->args[2].literal = result;
            this->args[2].isVar = false;
        }
        inline Quaternion(OperatorCode op, varStruct *arg1, varStruct *arg2, int result)
        {
            this->op = op;
            this->args[0].var = arg1;
            this->args[1].var = arg2;
            this->args[2].literal = result;
            this->args[0].isVar = true;
            this->args[1].isVar = true;
            this->args[2].isVar = false;
        }
        inline Quaternion(OperatorCode op, int arg1, int arg2, varStruct *result)
        {
            this->op = op;
            this->args[0].literal = arg1;
            this->args[1].literal = arg2;
            this->args[2].var = result;
            this->args[0].isVar = false;
            this->args[1].isVar = false;
            this->args[2].isVar = true;
        }
        inline Quaternion(OperatorCode op, varStruct *arg1, int arg2, varStruct *result)
        {
            this->op = op;
            this->args[0].var = arg1;
            this->args[1].literal = arg2;
            this->args[2].var = result;
            this->args[0].isVar = true;
            this->args[1].isVar = false;
            this->args[2].isVar = true;
        }
        inline Quaternion(OperatorCode op, int arg1, varStruct *result)
        {
            this->op = op;
            this->args[0].literal = arg1;
            this->args[2].var = result;
            this->args[0].isVar = false;
            this->args[2].isVar = true;
        }
        inline Quaternion(OperatorCode op, int arg1, varStruct *arg2, varStruct *result)
        {
            this->op = op;
            this->args[0].literal = arg1;
            this->args[1].var = arg2;
            this->args[2].var = result;
            this->args[0].isVar = false;
            this->args[1].isVar = true;
            this->args[2].isVar = true;
        }
        inline Quaternion(OperatorCode op, varStruct *arg1, varStruct *result)
        {
            this->op = op;
            this->args[0].var = arg1;
            this->args[2].var = result;
            this->args[0].isVar = true;
            this->args[2].isVar = true;
        }
        inline Quaternion(OperatorCode op, varStruct *arg1, varStruct *arg2, varStruct *result)
        {
            this->op = op;
            this->args[0].var = arg1;
            this->args[1].var = arg2;
            this->args[2].var = result;
            this->args[0].isVar = true;
            this->args[1].isVar = true;
            this->args[2].isVar = true;
        }

        inline void backPatch(int target) { this->args[2].literal = target; } //回填
        void print();
    };
}
#endif