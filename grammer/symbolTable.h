#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#include <iostream>
#include <vector>
#include <map>
using namespace std;
class varStruct{
public:
    string name;
    int type;
    int modifier;
    int describe;
    int address;
    int length;
    varStruct(string n,int type,int m,int d,int adr,int len);
};

class SymbolTable{
    static int address;
    static SymbolTable* rootTable;
    static SymbolTable* currentTable;
    vector<SymbolTable*> tableList;
    SymbolTable* beforeTable = nullptr;
    map<string,varStruct*> varTable;
    bool insert(string name,int type,int modifier,int describe);
    varStruct* get(string name);
    bool startSpace();
    bool endSpace();
};

#endif