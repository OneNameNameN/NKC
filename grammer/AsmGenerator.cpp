#include "AsmGenerator.h"
#include <fstream>
#include <cstdint>
AsmGenerator::AsmGenerator(vector<Quaternion>* quads){
    this->quads = quads;
    for(int i=0;i<4;i++)
        reg[i] = nullptr;
}

int* getNext(string P) {
	int i = 0, j = -1, len = P.size();
    int *next = new int[len];
    next[0] = -1;
    while (i < len - 1) {
        if (j == -1 || P[i] == P[j]) {
            next[++i] = ++j;
        } else {
            j = next[j];
        }
    }
	return next;
}
int KMP(string sstr, string pstr) {
    int i = 0, j = 0;
    int* next = getNext(pstr);
	int s_size = sstr.size(), p_size = pstr.size();
	while (i < s_size && j < p_size) {
        if (sstr[i] == pstr[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
        if (j == -1) {
            j++;
            i++;
        }
    }
    return j >= p_size ? i - p_size : -1;
}

bool isLabelStmt(string sstr){
    string LabelStmt[]={
        "%include",
        "section",
        "CMAIN:",
        "_Quad#",
        "_NEG#"
    };
    for(string pstr : LabelStmt)
        if(!KMP(sstr, pstr))
            return true;
    return false;    
}

void AsmGenerator::out(string message) {
    ofstream s;
    s.open("target.asm",ios::app);
    if(!isLabelStmt(message))
        s<<'\t';
    s<<message;
    s<<'\n';
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
    out("%include \"io.inc\"");
    generateData();
    generateText();
    system("nasm target.asm -f win32 && gcc target.obj -o program -m32");
}

void AsmGenerator::generateData() {
    out("section .data");
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
                out(iter->second->name+"_"+to_string((iter->second->address))+describe+" dd 0");
                iter->second->isTemp = false;
            }
            iter++;
        }
        for(SymbolTable* table:now->tableList){
            tableStack.push(table);
        }
    }
    out("temp times 128 dd 0");
}

void AsmGenerator::generateText() {
    out("section .text");
    out("extern printf");
    out("extern scanf");
    out("global CMAIN");
    out("CMAIN:");
    int i;
    for(i=0;i<quads->size();i++){
        out("_Quad#"+to_string(i)+":");
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
                out("mov "+getVarName(quads->at(i).args[2].var, false)+","+ getVarName(quads->at(i).args[0].var,true));
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
                out("mov "+name+","+ getVarName(quads->at(i).args[0].var,false));
                out("mov "+name+",["+name+"]");
                break;
            }
            case IM::RET:
                out("ret");
                break;
            default:break;
        }
    }
    out("_Quad#"+ to_string(i)+":");
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
                    out("mov "+index+","+ getVarName(quad->args[1].var, false));
                }
            }
        } else{
            index = to_string(quad->args[1].literal);
        }
        string value = "edx";
        if(quad->args[0].isVar){
            if(quad->args[0].var->type == "Literal"){
                out("mov "+value+","+quad->args[0].var->name);
            }else{
                if(quad->args[0].var->isTemp){
                    value = getVarName(quad->args[0].var, false);
                } else{
                    out("mov "+value+","+ getVarName(quad->args[0].var, false));
                }
            }
        } else{
            out("mov "+value+","+to_string(quad->args[0].literal));
        }
        out("mov ["+quad->args[2].var->name+"_"+to_string(quad->args[2].var->address)+"+4*"+index+"],"+value);
        return;
    }

    if(quad->op==IM::ASSIGN_VALUE){
        string name = getReg(nullptr);
        string target = "edx";
        out("mov "+name+","+ getVarName(quad->args[2].var, false));
        if(quad->args[0].isVar){
            if(quad->args[0].var->type=="Literal"){
                out("mov "+target+","+quad->args[0].var->name);
            }else{
                if(quad->args[0].var->isTemp){
                    target = getVarName(quad->args[0].var, false);
                } else{
                    out("mov "+target+","+ getVarName(quad->args[0].var, false));
                }
            }
        }else{
            out("mov "+target+","+to_string(quad->args[0].literal));
        }
        out("mov ["+name+"],"+target);
        return;
    }


    if(quad->args[0].isVar&&quad->args[0].var->isTemp){
        if(quad->args[0].var->type=="Literal"){
            string name = getReg(nullptr);
            out("mov "+name+","+ quad->args[0].var->name);
            out("mov "+ getVarName(quad->args[2].var, false)+","+ name);
        } else{
            out("mov "+getVarName(quad->args[2].var, false)+","+ getVarName(quad->args[0].var, false));
        }
        return;
    }

    string name = getReg(nullptr);
    if(quad->args[0].isVar){
        if(quad->args[1].isVar&&!quad->args[1].var){
            out("mov "+name+","+getVarName(quad->args[0].var, false));
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
                        out("mov edx,"+ getVarName(quad->args[1].var, false));
                    }
                }
            } else{
                index = to_string(quad->args[1].literal);
            }
            out("mov "+name+",["+quad->args[0].var->name+"_"+to_string(quad->args[0].var->address)+"+4*"+index+"]");
        }
    } else{
        out("mov "+name+","+to_string(quad->args[0].literal));
    }
    out("mov "+getVarName(quad->args[2].var, false)+","+name);
}

void AsmGenerator::generateCalculate(Quaternion *quad) {
    if(quad->op==IM::PLUS||quad->op==IM::MINUS){
        string name;
        if(quad->args[0].isVar){
            if(quad->args[0].var->type=="Literal"){
                name = getReg(nullptr);
                out("mov "+name+","+ quad->args[0].var->name);
            } else{
                if(quad->args[0].var->isTemp){
                    name = getVarName(quad->args[0].var, false);
                } else{
                    name = getReg(nullptr);
                    out("mov "+name+","+ getVarName(quad->args[0].var, false));
                }
            }
        } else{
            name = getReg(nullptr);
            out("mov "+name+","+to_string(quad->args[0].literal));
        }
        string op;
        if(quad->op==IM::MINUS){op="sub";}
        else{op="add";}
        if(quad->args[1].isVar){
            if(quad->args[1].var->type == "Literal"){
                out(op+" "+name+","+ quad->args[1].var->name);
            }else{
                out(op+" "+name+","+ getVarName(quad->args[1].var, false));
            }
        } else{
            out(op+" "+name+","+to_string(quad->args[1].literal));
        }
        out("mov "+ getVarName(quad->args[2].var, false)+","+name);
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
                    out("mov "+name+",eax");
                }
                out("mov eax,"+ quad->args[0].var->name);
            } else{
                if(quad->args[0].var->isTemp){
                    name = getVarName(quad->args[0].var, false);
                    if(index[name]!=0&&reg[0]!= nullptr){
                        reg[index[name]]=reg[0];
                        reg[0]= nullptr;
                        out("mov [temp],eax");
                        out("mov eax,"+name);
                        out("mov "+name+",[temp]");
                    }
                } else{
                    name = getReg(nullptr);
                    if(index[name]!=0&&reg[0]!= nullptr){
                        reg[index[name]]=reg[0];
                        reg[0]= nullptr;
                        out("mov "+name+",eax");
                    }
                    out("mov eax,"+ getVarName(quad->args[0].var, false));
                }
            }
        } else{
            name = getReg(nullptr);
            if(index[name]!=0&&reg[0]!= nullptr){
                reg[index[name]]=reg[0];
                reg[0]= nullptr;
                out("mov "+name+",eax");
            }
            out("mov eax,"+to_string(quad->args[0].literal));
        }
        if(quad->args[1].isVar){
            if(quad->args[1].var->type == "Literal"){
                out("mov dl,"+quad->args[1].var->name);
            }else{
                if(quad->args[1].var->isTemp){
                    out("mov [temp],"+ getVarName(quad->args[1].var, false));
                    out("mov dl,[temp]");
                }else{
                    out("mov dl,"+ getVarName(quad->args[1].var, false));
                }
            }
        } else{
            out("mov dl,"+to_string(quad->args[1].literal));
        }
        out("cmp dl,0x00");
        out("jge _NEG#"+to_string(intptr_t(quad)));
        out("neg dl");
        out("neg eax");
        out("_NEG#"+to_string(intptr_t(quad))+":");
        out("idiv dl");
        if(quad->op==IM::MOD){out("shr eax,8");}
        else{out("shr ah,8");}
        out("mov "+ getVarName(quad->args[2].var, false)+",eax");
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
                    out("mov "+name+",eax");
                }
                out("mov eax,"+ quad->args[0].var->name);
            } else{
                if(quad->args[0].var->isTemp){
                    name = getVarName(quad->args[0].var, false);
                    if(index[name]!=0&&reg[0]!= nullptr){
                        reg[index[name]]=reg[0];
                        reg[0]= nullptr;
                        out("mov [temp],eax");
                        out("mov eax,"+name);
                        out("mov "+name+",[temp]");
                    }
                } else{
                    name = getReg(nullptr);
                    if(index[name]!=0&&reg[0]!= nullptr){
                        reg[index[name]]=reg[0];
                        reg[0]= nullptr;
                        out("mov "+name+",eax");
                    }
                    out("mov eax,"+ getVarName(quad->args[0].var, false));
                }
            }
        } else{
            name = getReg(nullptr);
            if(index[name]!=0&&reg[0]!= nullptr){
                reg[index[name]]=reg[0];
                reg[0]= nullptr;
                out("mov "+name+",eax");
            }
            out("mov eax,"+to_string(quad->args[0].literal));
        }
        if(quad->args[1].isVar){
            if(quad->args[1].var->type == "Literal"){
                out("mov dl,"+quad->args[1].var->name);
            }else{
                if(quad->args[1].var->isTemp){
                    out("mov [temp],"+ getVarName(quad->args[1].var, false));
                    out("mov dl,[temp]");
                }else{
                    out("mov dl,"+ getVarName(quad->args[1].var, false));
                }
            }
        } else{
            out("mov dl,"+to_string(quad->args[1].literal));
        }
        out("cmp dl,0x00");
        out("jge _NEG#"+to_string(intptr_t(quad)));
        out("neg dl");
        out("neg eax");
        out("_NEG#"+to_string(intptr_t(quad))+":");
        out("imul dl");
        out("mov "+ getVarName(quad->args[2].var, false)+",eax");
        return;
    }

    if(quad->op==IM::LOGIC_NOT){
        string name = getReg(nullptr);
        if(index[name]!=0){
            reg[index[name]]=reg[0];
            reg[0] = nullptr;
            out("mov "+name+",eax");
        }
        if(quad->args[0].isVar){
            if(quad->args[0].var->type=="Literal") {
                out("mov eax,"+quad->args[0].var->name);
            }else{
                out("mov eax,"+ getVarName(quad->args[0].var, false));
            }
        }else{
            out("mov eax,"+to_string(quad->args[0].literal));
        }
        out("cmp eax,0x00000000");
        out("mov eax,0x00000000");
        out("mov [temp],eax");
        out("sete al");
        out("mov [temp],al");
        out("mov eax,[temp]");
        reg[0]=quad->args[2].var;
        return;
    }

    if(quad->op==IM::LOGIC_AND||quad->op==IM::LOGIC_OR){
        string name = getReg(nullptr);
        if(index[name]!=0){
            reg[index[name]]=reg[0];
            reg[0] = nullptr;
            out("mov "+name+",eax");
        }
        if(quad->args[0].isVar){
            if(quad->args[0].var->type=="Literal") {
                out("mov ax,"+quad->args[0].var->name);
            }else{
                if(quad->args[0].var->isTemp){
                    out("mov [temp],"+ getVarName(quad->args[0].var, false));
                    out("mov ax,[temp]");
                }else {
                    out("mov ax,"+ getVarName(quad->args[0].var, false));
                }
            }
        }else{
            out("mov ax,"+to_string(quad->args[0].literal));
        }
        out("cmp ax,0x00000000");
        out("setne al");
        out("shl eax,16");
        if(quad->args[1].isVar){
            if(quad->args[1].var->type=="Literal") {
                out("mov ax,"+quad->args[1].var->name);
            }else{
                if(quad->args[1].var->isTemp){
                    out("mov [temp],"+ getVarName(quad->args[1].var, false));
                    out("mov ax,[temp]");
                }else {
                    out("mov ax,"+ getVarName(quad->args[1].var, false));
                }
            }
        }else{
            out("mov ax,"+to_string(quad->args[1].literal));
        }
        out("cmp ax,0x00000000");
        out("setne al");

        if(quad->op==IM::LOGIC_AND){
            out("and eax 0x00010001");
        }else{
            out("or eax 0x00000000");
        }

        out("cmp eax,0x00000000");
        out("mov eax,0x00000000");
        out("mov [temp],eax");
        out("setne al");
        out("mov [temp],al");
        out("mov eax,[temp]");
        reg[0] = quad->args[2].var;
        return;
    }
}

void AsmGenerator::generateJump(Quaternion *quad) {
    if(quad->op==IM::JUMP){
        out("jmp "+("_Quad#"+to_string(quad->args[2].literal)));
        return;
    }
    string name = getReg(nullptr);
    if(quad->args[0].isVar){
        if(quad->args[0].var->type!="Literal"){
            out("mov "+ name+","+ getVarName(quad->args[0].var, false));
        }else{
            out("mov "+ name+","+ quad->args[0].var->name);
        }
    }else{
        out("mov "+ name+","+to_string(quad->args[0].literal));
    }
    if(quad->args[1].isVar){
        if(quad->args[0].var->type!="Literal"){
            out("mov edx,"+ getVarName(quad->args[1].var, false));
        }else{
            out("mov edx,"+quad->args[1].var->name);
        }
    } else{
        out("mov edx,"+to_string(quad->args[1].literal));
    }
    out("cmp "+ name+",edx");
    switch (quad->op) {
        case IM::JUMP_EQUAL:
            out("je "+("_Quad#"+to_string(quad->args[2].literal)));
            break;
        case IM::JUMP_NOT_EQUAL:
            out("jne "+("_Quad#"+to_string(quad->args[2].literal)));
            break;
        case IM::JUMP_GREAT:
            out("jg "+("_Quad#"+to_string(quad->args[2].literal)));
            break;
        case IM::JUMP_SMALL:
            out("jl "+("_Quad#"+to_string(quad->args[2].literal)));
            break;
        case IM::JUMP_EQ_GREAT:
            out("jge "+("_Quad#"+to_string(quad->args[2].literal)));
            break;
        case IM::JUMP_EQ_SMALL:
            out("jle "+("_Quad#"+to_string(quad->args[2].literal)));
            break;
        default:out("ERROR");break;
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
                            out("mov "+regName+",\""+temp+"\"");
                            out("mov [temp+"+to_string(sum)+"],"+regName);
                            sum+=number;
                            temp="";
                            number=0;

                            i++;
                            out("mov "+regName+",10");
                            out("mov [temp+"+to_string(sum)+"],"+regName);
                            sum+=1;
                            continue;
                        }
                    }
                    number++;
                    temp+=str[i];
                    if(number==4||i==str.length()-1){
                        out("mov "+regName+",\""+temp+"\"");
                        out("mov [temp+"+to_string(sum)+"],"+regName);
                        sum+=number;
                        temp="";
                        number=0;
                    }
                }
                out("mov "+regName+",0");
                out("mov [temp+"+to_string(sum)+"],"+regName);
                out("push temp");
            }else{
                out("push "+ getVarName(quad->args[0].var, false));
            }
        } else{
            if(quad->args[1].isVar&&!quad->args[1].var){
                out("mov "+getReg(nullptr)+","+ getVarName(quad->args[0].var, false));
                out("push "+getReg(nullptr));
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
                            out("mov "+index+","+ getVarName(quad->args[1].var, false));
                        }
                    }
                } else{
                    index = to_string(quad->args[1].literal);
                }
                out("mov "+getReg(nullptr)+",["+quad->args[0].var->name+"_"+to_string(quad->args[0].var->address)+"+4*"+index+"]");
                out("push "+getReg(nullptr));
            }
        }
    }else{
        out("push "+ to_string(quad->args[0].literal));
    }
}

void AsmGenerator::generateCall(Quaternion *quad) {
    out("call "+quad->args[0].var->name);
    out("add esp,"+to_string(quad->args[1].literal*4));
}
