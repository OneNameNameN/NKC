#include "./Quaternion.h"

string IM::Quaternion::argToStr(Arg a)
{
    if (a.isVar)
    {
        if (a.var == NULL)
        {
            return "_";
        }
        else
        {
            return a.var->name;
        }
    }
    else
    {
        return to_string(a.literal);
    }
}

void IM::Quaternion::print()
{
    cout << "(" << get_op_str() << ", " << argToStr(args[0]) << ", " << argToStr(args[1]) << ", " << argToStr(args[2]) << ")" << endl;
}

vector<IM::Quaternion>* IM::Quaternion::quads = new vector<IM::Quaternion>();
vector<varStruct*>* IM::Quaternion::tempVars = new vector<varStruct*>();