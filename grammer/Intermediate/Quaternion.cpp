#include "./Quaternion.h"

string IM::Quaternion::argToStr(Arg a)
{
    if (a.isVar)
    {
        if (a.var == NULL)
        {
            return "-";
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
    cout << get_op_str() << "\t" << argToStr(args[0]) << "\t" << argToStr(args[1]) << "\t" << argToStr(args[2]) << endl;
}

vector<IM::Quaternion>* IM::Quaternion::quads = new vector<IM::Quaternion>();
vector<varStruct*>* IM::Quaternion::tempVars = new vector<varStruct*>();