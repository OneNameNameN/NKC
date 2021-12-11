#include "AsmGenerator.h"
#include <fstream>
#include <cstdint>
AsmGenerator::AsmGenerator(vector<Quaternion>* quads){
    this->quads = quads;
    for(int i=0;i<4;i++)
        reg[i] = nullptr;
}

void AsmGenerator::out(string message) {
    ofstream s;
    s.open("target.asm",ios::app);
    s<<message;
    s.close();
}

string AsmGenerator::getReg(varStruct* target) {
    string name[4]={"eax","ebx","ecx","edx"};
    for(int i=0;i<4;i++){
        if(reg[i]== target){
            return name[i];
        }
    }
    return "";
}
string AsmGenerator::getVarName(varStruct* var,bool address){
    string ret;
    map<string,int> index = {
            {"eax",0},
            {"ebx",1},
            {"ecx",2},
            {"edx",3}
    };
    if(var->isTemp){
        if((ret=getReg(var))==""){
            ret = getReg(nullptr);
            reg[index[ret]]=var;
        } else{
            reg[index[ret]]= nullptr;
        }
        return ret;
    }
    else {
        if(address)return var->name+"_"+to_string(var->address);
        else return "["+var->name+"_"+to_string(var->address)+"]";
    }
}
void AsmGenerator::generate() {
    ofstream s;
    s.open("target.asm",ios::out);
    s.close();
    out("%include \"io.inc\"\n");
    generateData();
    generateText();
    system("nasm target.asm -f win32 && gcc target.obj -o program -m32");
}

void AsmGenerator::generateData() {
    out("section .data\n");
    SymbolTable* now;
    stack<SymbolTable*> tableStack;
    tableStack.push(SymbolTable::rootTable);
    while(!tableStack.empty()){
        now = tableStack.top();
        tableStack.pop();
        map<string,varStruct*>::iterator iter;
        iter = now->varTable.begin();
        while(iter != now->varTable.end()){
            if (iter->second != nullptr)
            {
                string describe="";
                if(iter->second->describe){
                    describe = " times ";
                    describe += to_string(iter->second->describe);
                }
                out("\t");
                out(iter->second->name+"_"+to_string((iter->second->address))+describe+" dd 0");
                out("\n");
                iter->second->isTemp = false;
            }
            iter++;
        }
        for(SymbolTable* table:now->tableList){
            tableStack.push(table);
        }
    }
    out("\ttemp times 128 dd 0\n");
}

void AsmGenerator::generateText() {
    out("section .text\n"
        "\tglobal CMAIN\n"
        "; <_Quad#i> corresponds to line(i) in InterMediate Code(Quaternion)\n"
        "CMAIN:\n");
    int i;
    for(i=0;i<quads->size();i++){
        out("_Quad#"+to_string(i)+":\n");
        switch (quads->at(i).op) {
            case IM::DEFINE:break;
            case IM::ASSIGN:
            case IM::ASSIGN_ARRAY:
            case IM::ASSIGN_POINTER:
            case IM::ASSIGN_VALUE:
                generateAssign(&quads->at(i));
                break;
            case IM::JUMP:
            case IM::JUMP_EQUAL:
            case IM::JUMP_EQ_GREAT:
            case IM::JUMP_EQ_SMALL:
            case IM::JUMP_GREAT:
            case IM::JUMP_SMALL:
            case IM::JUMP_NOT_EQUAL:
                generateJump(&quads->at(i));
                break;
            case IM::GET_ADDRESS:
                out("\tmov "+getVarName(quads->at(i).args[2].var, false)+","+ getVarName(quads->at(i).args[0].var,true)+"\n");
                break;
            case IM::PARAM:
                generateParam(&quads->at(i));
                break;
            case IM::CALL:
                generateCall(&quads->at(i));
                break;
            case IM::PLUS:
            case IM::MINUS:
            case IM::TIMES:
            case IM::DIV:
            case IM::MOD:
            case IM::LOGIC_NOT:
            case IM::LOGIC_AND:
            case IM::LOGIC_OR:
                generateCalculate(&quads->at(i));
                break;
            case IM::GET_VALUE:
            {
                string name = getVarName(quads->at(i).args[2].var, false);
                out("\tmov "+name+","+ getVarName(quads->at(i).args[0].var,false)+"\n");
                out("\tmov "+name+",["+name+"]\n");
                break;
            }
            case IM::RET:
                out("\tret\n");
                break;
            default:break;
        }
    }
    out("_Quad#"+ to_string(i)+":\n");
}

void AsmGenerator::generateAssign(Quaternion *quad) {
    if(quad->op==IM::ASSIGN_ARRAY){
        string index;
        if(quad->args[1].isVar){
            if(quad->args[1].var->type == "Literal"){
                index = quad->args[1].var->name;
            }else{
                if(quad->args[1].var->isTemp){
                    index = getVarName(quad->args[1].var, false);
                } else{
                    index = getReg(nullptr);
                    out("\tmov "+index+","+ getVarName(quad->args[1].var, false)+"\n");
                }
            }
        } else{
            index = to_string(quad->args[1].literal);
        }
        string value = "edx";
        if(quad->args[0].isVar){
            if(quad->args[0].var->type == "Literal"){
                out("\tmov "+value+","+quad->args[0].var->name+"\n");
            }else{
                if(quad->args[0].var->isTemp){
                    value = getVarName(quad->args[0].var, false);
                } else{
                    out("\tmov "+value+","+ getVarName(quad->args[0].var, false)+"\n");
                }
            }
        } else{
            out("\tmov "+value+","+to_string(quad->args[0].literal)+"\n");
        }
        out("\tmov ["+quad->args[2].var->name+"_"+to_string(quad->args[2].var->address)+"+4*"+index+"],"+value+"\n");
        return;
    }

    if(quad->op==IM::ASSIGN_VALUE){
        string name = getReg(nullptr);
        string target = "edx";
        out("\tmov "+name+","+ getVarName(quad->args[2].var, false)+"\n");
        if(quad->args[0].isVar){
            if(quad->args[0].var->type=="Literal"){
                out("\tmov "+target+","+quad->args[0].var->name+"\n");
            }else{
                if(quad->args[0].var->isTemp){
                    target = getVarName(quad->args[0].var, false);
                } else{
                    out("\tmov "+target+","+ getVarName(quad->args[0].var, false)+"\n");
                }
            }
        }else{
            out("\tmov "+target+","+to_string(quad->args[0].literal)+"\n");
        }
        out("\tmov ["+name+"],"+target+"\n");
        return;
    }


    if(quad->args[0].isVar&&quad->args[0].var->isTemp){
        if(quad->args[0].var->type=="Literal"){
            string name = getReg(nullptr);
            out("\tmov "+name+","+ quad->args[0].var->name+"\n");
            out("\tmov "+ getVarName(quad->args[2].var, false)+","+ name+"\n");
        } else{
            out("\tmov "+getVarName(quad->args[2].var, false)+","+ getVarName(quad->args[0].var, false)+"\n");
        }
        return;
    }

    string name = getReg(nullptr);
    if(quad->args[0].isVar){
        if(quad->args[1].isVar&&!quad->args[1].var){
            out("\tmov "+name+",");
            out(getVarName(quad->args[0].var, false)+"\n");
        }else{
            // 确定是数组赋值
            string index;
            if(quad->args[1].isVar){
                if(quad->args[1].var->type == "Literal"){
                    index = quad->args[1].var->name;
                }else{
                    if(quad->args[1].var->isTemp){
                        index = getVarName(quad->args[1].var, false);
                    } else{
                        index = "edx";
                        out("\tmov edx,"+ getVarName(quad->args[1].var, false)+"\n");
                    }
                }
            } else{
                index = to_string(quad->args[1].literal);
            }
            out("\tmov "+name+",["+quad->args[0].var->name+"_"+to_string(quad->args[0].var->address)+"+4*"+index+"]\n");
        }
    } else{
        out("\tmov "+name+",");
        out(to_string(quad->args[0].literal)+"\n");
    }
    out("\tmov ");
    out(getVarName(quad->args[2].var, false)+",");
    out(name+"\n");
}

void AsmGenerator::generateCalculate(Quaternion *quad) {
    if(quad->op==IM::PLUS||quad->op==IM::MINUS){
        string name;
        if(quad->args[0].isVar){
            if(quad->args[0].var->type=="Literal"){
                name = getReg(nullptr);
                out("\tmov "+name+","+ quad->args[0].var->name+"\n");
            } else{
                if(quad->args[0].var->isTemp){
                    name = getVarName(quad->args[0].var, false);
                } else{
                    name = getReg(nullptr);
                    out("\tmov "+name+","+ getVarName(quad->args[0].var, false)+"\n");
                }
            }
        } else{
            name = getReg(nullptr);
            out("\tmov "+name+","+to_string(quad->args[0].literal)+"\n");
        }
        string op;
        if(quad->op==IM::MINUS){op="\tsub";}
        else{op="\tadd";}
        if(quad->args[1].isVar){
            if(quad->args[1].var->type == "Literal"){
                out(op+" "+name+","+ quad->args[1].var->name+"\n");
            }else{
                out(op+" "+name+","+ getVarName(quad->args[1].var, false)+"\n");
            }
        } else{
            out(op+" "+name+","+to_string(quad->args[1].literal)+"\n");
        }
        out("\tmov "+ getVarName(quad->args[2].var, false)+","+name+"\n");
        return;
    }

    map<string,int> index={
            {"eax",0},
            {"ebx",1},
            {"ecx",2},
            {"edx",3},
    };
    if(quad->op==IM::MOD||quad->op==IM::DIV){
        string name;
        if(quad->args[0].isVar){
            if(quad->args[0].var->type=="Literal"){
                name = getReg(nullptr);
                if(index[name]!=0&&reg[0]!= nullptr){
                    reg[index[name]]=reg[0];
                    reg[0]= nullptr;
                    out("\tmov "+name+",eax\n");
                }
                out("\tmov eax,"+ quad->args[0].var->name+"\n");
            } else{
                if(quad->args[0].var->isTemp){
                    name = getVarName(quad->args[0].var, false);
                    if(index[name]!=0&&reg[0]!= nullptr){
                        reg[index[name]]=reg[0];
                        reg[0]= nullptr;
                        out("\tmov [temp],eax\n");
                        out("\tmov eax,"+name+"\n");
                        out("\tmov "+name+",[temp]\n");
                    }
                } else{
                    name = getReg(nullptr);
                    if(index[name]!=0&&reg[0]!= nullptr){
                        reg[index[name]]=reg[0];
                        reg[0]= nullptr;
                        out("\tmov "+name+",eax\n");
                    }
                    out("\tmov eax,"+ getVarName(quad->args[0].var, false)+"\n");
                }
            }
        } else{
            name = getReg(nullptr);
            if(index[name]!=0&&reg[0]!= nullptr){
                reg[index[name]]=reg[0];
                reg[0]= nullptr;
                out("\tmov "+name+",eax\n");
            }
            out("\tmov eax,"+to_string(quad->args[0].literal)+"\n");
        }
        if(quad->args[1].isVar){
            if(quad->args[1].var->type == "Literal"){
                out("\tmov dl,"+quad->args[1].var->name+"\n");
            }else{
                if(quad->args[1].var->isTemp){
                    out("\tmov [temp],"+ getVarName(quad->args[1].var, false)+"\n");
                    out("\tmov dl,[temp]\n");
                }else{
                    out("\tmov dl,"+ getVarName(quad->args[1].var, false)+"\n");
                }
            }
        } else{
            out("\tmov dl,"+to_string(quad->args[1].literal)+"\n");
        }
        out("\tcmp dl,0x00\n");
        out("\tjge _NEG#"+to_string(intptr_t(quad))+"\n");
        out("\tneg dl\n");
        out("\tneg eax\n");
        out("_NEG#"+to_string(intptr_t(quad))+":\n");
        out("\tidiv dl\n");
        if(quad->op==IM::MOD){out("\tshr eax,8\n");}
        else{out("\tshr ah,8\n");}
        out("\tmov "+ getVarName(quad->args[2].var, false)+",eax\n");
        return;
    }

    if(quad->op==IM::TIMES){
        string name;
        if(quad->args[0].isVar){
            if(quad->args[0].var->type=="Literal"){
                name = getReg(nullptr);
                if(index[name]!=0&&reg[0]!= nullptr){
                    reg[index[name]]=reg[0];
                    reg[0]= nullptr;
                    out("\tmov "+name+",eax\n");
                }
                out("\tmov eax,"+ quad->args[0].var->name+"\n");
            } else{
                if(quad->args[0].var->isTemp){
                    name = getVarName(quad->args[0].var, false);
                    if(index[name]!=0&&reg[0]!= nullptr){
                        reg[index[name]]=reg[0];
                        reg[0]= nullptr;
                        out("\tmov [temp],eax\n");
                        out("\tmov eax,"+name+"\n");
                        out("\tmov "+name+",[temp]\n");
                    }
                } else{
                    name = getReg(nullptr);
                    if(index[name]!=0&&reg[0]!= nullptr){
                        reg[index[name]]=reg[0];
                        reg[0]= nullptr;
                        out("\tmov "+name+",eax\n");
                    }
                    out("\tmov eax,"+ getVarName(quad->args[0].var, false)+"\n");
                }
            }
        } else{
            name = getReg(nullptr);
            if(index[name]!=0&&reg[0]!= nullptr){
                reg[index[name]]=reg[0];
                reg[0]= nullptr;
                out("\tmov "+name+",eax\n");
            }
            out("\tmov eax,"+to_string(quad->args[0].literal)+"\n");
        }
        if(quad->args[1].isVar){
            if(quad->args[1].var->type == "Literal"){
                out("\tmov dl,"+quad->args[1].var->name+"\n");
            }else{
                if(quad->args[1].var->isTemp){
                    out("\tmov [temp],"+ getVarName(quad->args[1].var, false)+"\n");
                    out("\tmov dl,[temp]\n");
                }else{
                    out("\tmov dl,"+ getVarName(quad->args[1].var, false)+"\n");
                }
            }
        } else{
            out("\tmov dl,"+to_string(quad->args[1].literal)+"\n");
        }
        out("\tcmp dl,0x00\n");
        out("\tjge _NEG#"+to_string(intptr_t(quad))+"\n");
        out("\tneg dl\n");
        out("\tneg eax\n");
        out("_NEG#"+to_string(intptr_t(quad))+":\n");
        out("\timul dl\n");
        out("\tmov "+ getVarName(quad->args[2].var, false)+",eax\n");
        return;
    }

    if(quad->op==IM::LOGIC_NOT){
        string name = getReg(nullptr);
        if(index[name]!=0){
            reg[index[name]]=reg[0];
            reg[0] = nullptr;
            out("\tmov "+name+",eax\n");
        }
        if(quad->args[0].isVar){
            if(quad->args[0].var->type=="Literal") {
                out("\tmov eax,"+quad->args[0].var->name+"\n");
            }else{
                out("\tmov eax,"+ getVarName(quad->args[0].var, false)+"\n");
            }
        }else{
            out("\tmov eax,"+to_string(quad->args[0].literal)+"\n");
        }
        out("\tcmp eax,0x00000000\n");
        out("\tmov eax,0x00000000\n");
        out("\tmov [temp],eax\n");
        out("\tsete al\n");
        out("\tmov [temp],al\n");
        out("\tmov eax,[temp]\n");
        reg[0]=quad->args[2].var;
        return;
    }

    if(quad->op==IM::LOGIC_AND||quad->op==IM::LOGIC_OR){
        string name = getReg(nullptr);
        if(index[name]!=0){
            reg[index[name]]=reg[0];
            reg[0] = nullptr;
            out("\tmov "+name+",eax\n");
        }
        if(quad->args[0].isVar){
            if(quad->args[0].var->type=="Literal") {
                out("\tmov ax,"+quad->args[0].var->name+"\n");
            }else{
                if(quad->args[0].var->isTemp){
                    out("\tmov [temp],"+ getVarName(quad->args[0].var, false)+"\n");
                    out("\tmov ax,[temp]\n");
                }else {
                    out("\tmov ax,"+ getVarName(quad->args[0].var, false)+"\n");
                }
            }
        }else{
            out("\tmov ax,"+to_string(quad->args[0].literal)+"\n");
        }
        out("\tcmp ax,0x00000000\n"
            "\tsetne al\n"
            "\tshl eax,16\n");
        if(quad->args[1].isVar){
            if(quad->args[1].var->type=="Literal") {
                out("\tmov ax,"+quad->args[1].var->name+"\n");
            }else{
                if(quad->args[1].var->isTemp){
                    out("\tmov [temp],"+ getVarName(quad->args[1].var, false)+"\n");
                    out("\tmov ax,[temp]\n");
                }else {
                    out("\tmov ax,"+ getVarName(quad->args[1].var, false)+"\n");
                }
            }
        }else{
            out("\tmov ax,"+to_string(quad->args[1].literal)+"\n");
        }
        out("\tcmp ax,0x00000000\n"
            "\tsetne al\n");

        if(quad->op==IM::LOGIC_AND){
            out("\tand eax 0x00010001\n");
        }else{
            out("\tor eax 0x00000000\n");
        }

        out("\tcmp eax,0x00000000\n");
        out("\tmov eax,0x00000000\n");
        out("\tmov [temp],eax\n");
        out("\tsetne al\n");
        out("\tmov [temp],al\n");
        out("\tmov eax,[temp]\n");
        reg[0] = quad->args[2].var;
        return;
    }
}

void AsmGenerator::generateJump(Quaternion *quad) {
    if(quad->op==IM::JUMP){
        out("\tjmp "+("_Quad#"+to_string(quad->args[2].literal))+"\n");
        return;
    }
    string name = getReg(nullptr);
    if(quad->args[0].isVar){
        if(quad->args[0].var->type!="Literal"){
            out("\tmov "+ name+","+ getVarName(quad->args[0].var, false)+"\n");
        }else{
            out("\tmov "+ name+","+ quad->args[0].var->name+"\n");
        }
    }else{
        out("\tmov "+ name+","+to_string(quad->args[0].literal)+"\n");
    }
    if(quad->args[1].isVar){
        if(quad->args[0].var->type!="Literal"){
            out("\tmov edx,"+ getVarName(quad->args[1].var, false)+"\n");
        }else{
            out("\tmov edx,"+quad->args[1].var->name+"\n");
        }
    } else{
        out("\tmov edx,"+to_string(quad->args[1].literal)+"\n");
    }
    out("\tcmp "+ name+",edx\n");
    switch (quad->op) {
        case IM::JUMP_EQUAL:
            out("\tje "+("_Quad#"+to_string(quad->args[2].literal))+"\n");
            break;
        case IM::JUMP_NOT_EQUAL:
            out("\tjne "+("_Quad#"+to_string(quad->args[2].literal))+"\n");
            break;
        case IM::JUMP_GREAT:
            out("\tjg "+("_Quad#"+to_string(quad->args[2].literal))+"\n");
            break;
        case IM::JUMP_SMALL:
            out("\tjl "+("_Quad#"+to_string(quad->args[2].literal))+"\n");
            break;
        case IM::JUMP_EQ_GREAT:
            out("\tjge "+("_Quad#"+to_string(quad->args[2].literal))+"\n");
            break;
        case IM::JUMP_EQ_SMALL:
            out("\tjle "+("_Quad#"+to_string(quad->args[2].literal))+"\n");
            break;
        default:out("\tERROR");break;
    }
}
void AsmGenerator::generateParam(Quaternion *quad) {
    if(quad->args[0].isVar){
        if(quad->args[0].var->isTemp){
            if(quad->args[0].var->type == "STRING"){
                string str = quad->args[0].var->name;
                str = str.substr(1,str.length()-2);
                int number=0;
                int sum=0;
                string regName = getReg(nullptr);
                string temp="";
                for(int i=0;i<str.length();i++){
                    if(str[i]=='\\'){
                        if(i+1<str.length()&&str[i+1]=='n'){
                            out("\tmov "+regName+",\""+temp+"\"\n");
                            out("\tmov [temp+"+to_string(sum)+"],"+regName+"\n");
                            sum+=number;
                            temp="";
                            number=0;

                            i++;
                            out("\tmov "+regName+",10\n");
                            out("\tmov [temp+"+to_string(sum)+"],"+regName+"\n");
                            sum+=1;
                            continue;
                        }
                    }
                    number++;
                    temp+=str[i];
                    if(number==4||i==str.length()-1){
                        out("\tmov "+regName+",\""+temp+"\"\n");
                        out("\tmov [temp+"+to_string(sum)+"],"+regName+"\n");
                        sum+=number;
                        temp="";
                        number=0;
                    }
                }
                out("\tmov "+regName+",0\n");
                out("\tmov [temp+"+to_string(sum)+"],"+regName+"\n");
                out("\tpush temp\n");
            }else{
                out("\tpush "+ getVarName(quad->args[0].var, false)+"\n");
            }
        } else{
            if(quad->args[1].isVar&&!quad->args[1].var){
                out("\tmov "+getReg(nullptr)+","+ getVarName(quad->args[0].var, false)+"\n");
                out("\tpush "+getReg(nullptr)+"\n");
            }else{
                // 找出index
                string index;
                if(quad->args[1].isVar){
                    if(quad->args[1].var->type=="Literal"){
                        index = quad->args[1].var->name;
                    } else{
                        if(quad->args[1].var->isTemp){
                            index = getVarName(quad->args[1].var, false);
                        } else{
                            index = getReg(nullptr);
                            out("\tmov "+index+","+ getVarName(quad->args[1].var, false)+"\n");
                        }
                    }
                } else{
                    index = to_string(quad->args[1].literal);
                }
                out("\tmov "+getReg(nullptr)+",["+quad->args[0].var->name+"_"+to_string(quad->args[0].var->address)+"+4*"+index+"]"+"\n");
                out("\tpush "+getReg(nullptr)+"\n");
            }
        }
    }else{
        out("\tpush "+ to_string(quad->args[0].literal)+"\n");
    }
}

void AsmGenerator::generateCall(Quaternion *quad) {
    out("\tcall "+quad->args[0].var->name+"\n");
    out("\tadd esp,"+to_string(quad->args[1].literal*4)+"\n");
}
