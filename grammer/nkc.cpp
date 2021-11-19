#include <stdio.h>
#include <stdlib.h>
#include "nkc.h"

ast* create(int nodetype, char *val)
{
    ast *u = (ast*)malloc(sizeof(ast));
    u->nodetype = nodetype;
    u->val = val;
    u->son = NULL;
    u->cousin = NULL;
    // printf("create %s\n", val);
    return u;
}

void add(ast *father, ast *son)
{
    if(son == NULL) return;
    if(father->son == NULL)
        father->son = son;
    else
    {
        son->cousin = father->son;
        father->son = son;
    }
}

void print(ast *u, int deep)
{
    if(u == NULL) return;
    if(u->cousin != NULL) print(u->cousin, deep);
    for(int i = 0; i < deep; i++)
        printf("| ");
    printf("%s\n", u->val);
    if(u->son != NULL) print(u->son, deep+1);
}