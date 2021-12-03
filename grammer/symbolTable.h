#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#include <iostream>
#include <vector>
#include <map>
using namespace std;
class varStruct{
public:
    string name;
    string type;
    int isPointer;
    int describe;
    int address;
    int length;
    varStruct(string n,string type,int m,int d,int adr,int len);
};

class SymbolTable{
public:
    static int address;
    static SymbolTable* rootTable;
    static SymbolTable* currentTable;
    vector<SymbolTable*> tableList;
    SymbolTable* beforeTable = nullptr;
    map<string,varStruct*> varTable;
    bool insert(string name,string type,int modifier,int describe);
    varStruct* get(string name);
    bool startSpace();
    bool endSpace();
    void print(int deep);
};

#endif