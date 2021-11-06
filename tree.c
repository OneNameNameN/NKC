#ifndef TREE
#define TREE
#include<stdio.h>
#include<stdlib.h>
struct Node{
    char* type;
    char* value;
    int childrenNumber;
    struct Node** children;
};
#define YYSTYPE struct Node*
struct Node* createNode(char type[],int childrenNumber,struct Node** x,char value[]){
    struct Node * node = malloc(sizeof(struct Node));
    node->type = type;
    node->value = value;
    node->childrenNumber = childrenNumber;
    node->children = x;
    return node;
}
struct Node* createNewNode(char type[],int childrenNumber,struct Node** x){
    char s[]= "";
    return createNode(type,childrenNumber,x,s);
}
struct Node* createNewEndNode(char type[],char value[]){
    return createNode(type,0,NULL,value);
}
void printTree(struct Node * x,int depth){
    int i=0;
    for(i=0;i<depth;i++){
        printf("--- ");    
    }
    printf("%s %s\n",x->type,x->value);
    for(i=0;i<x->childrenNumber;i++){
        printTree(x->children[i],depth+1);
    }
}
#endif