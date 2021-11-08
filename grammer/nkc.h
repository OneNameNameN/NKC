#include <stdio.h>
#include <stdlib.h>

extern int yylineno;

/* 抽象语法树 */
typedef struct ast {
    int nodetype;  
    char *val;
    struct ast *son;
    struct ast *cousin;
}ast;

ast* create(int nodetype, char *val);
void add(ast *father, ast *son);
void print(ast *u, int deep);
