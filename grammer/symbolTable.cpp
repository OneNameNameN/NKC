#include "symbolTable.h"

bool SymbolTable::insert(string name,string type,int modifier,int describe){
    cout<<"insert"<<name<<endl;
    if(varTable[name] == nullptr){
        int length;
        length = 4;
        varTable[name] = new varStruct(name,type,modifier,describe,address,length);
        address+=length;
        return true;
    } else{
        return false;
    }
}
varStruct* SymbolTable::get(string name){
    varStruct* result = nullptr;
    SymbolTable* now = this;
    while(now!= nullptr){
        if(now->varTable[name]!= nullptr){
            result = now->varTable[name];
            return result;
        }
        now = now->beforeTable;
    }
    return result;
}

bool SymbolTable::startSpace() {
    SymbolTable* newTable = new SymbolTable();
    newTable->beforeTable = currentTable;
    currentTable->tableList.push_back(newTable);
    currentTable = newTable;
    return true;
}
bool SymbolTable::endSpace(){
    if(currentTable->beforeTable){
        currentTable = currentTable->beforeTable;
        return true;
    }
    return false;
}

void SymbolTable::print(int deep){
    map<string,varStruct*>::iterator iter;
    iter = varTable.begin();
    while(iter != varTable.end()){
        if (iter->second != nullptr)
        {
            for (int i = 0; i < deep; i++)
            {
                cout << "|";
            }
            cout << iter->first << " " << iter->second << "\n";
        }
        iter++;
    }
    for(SymbolTable* table:tableList){
        table->print(deep+1);
    }
}

varStruct::varStruct(string n,string type,int m,int d,int adr,int len){
    this->name = n;
    this->type = type;
    this->modifier = m;
    this->describe = d;
    this->address = adr;
    this->length = len;
}

int SymbolTable::address = 0;
SymbolTable* SymbolTable::rootTable = new SymbolTable();
SymbolTable* SymbolTable::currentTable = rootTable;