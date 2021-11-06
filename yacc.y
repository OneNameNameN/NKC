%token TYPE KEYWORD ID
%token LP RP LBRACKET RBRACKET LBRACE RBRACE SEMICOLON COMMA SINGLEQUOTES DOUBLEQUOTES WELLSIGN RETURN
%token OPT COMPARISION ASSIGN LNOT INTEGER DECIMAL FLOAT TYPEIDENTIFY ERRORFLOAT PS
%{
#include<stdio.h>
#include "tree.c"
extern int yylex();
extern FILE *yyin;
int yyerror(const char *);
%}

%start function
%%

function   : TYPE ID LP parameters RP LBRACE statements RBRACE {
struct Node** children = malloc(8*sizeof(struct Node*));
children[0] = $1;
children[1] = $2;
children[2] = $3;
children[3] = $4;
children[4] = $5;
children[5] = $6;
children[6] = $7;
children[7] = $8;
$$ = createNewNode("function",8,children);
printf("\n");
printTree($$,0);}
       ;

parameters   : parameter parameters {
struct Node** children = malloc(2*sizeof(struct Node*));
children[0] = $1;
children[1] = $2;
$$ = createNewNode("parameters",2,children);
}
       | {
$$ = createNewEndNode("parameters","space");}
       ;

parameter   : TYPE ID {
struct Node** children = malloc(2*sizeof(struct Node*));
children[0] = $1;
children[1] = $2;
$$ = createNewNode("parameter",2,children);
}
       ;

statements   : statement statements {
struct Node** children = malloc(2*sizeof(struct Node*));
children[0] = $1;
children[1] = $2;
$$ = createNewNode("statements",2,children);
}
       | {
$$ = createNewEndNode("statements","space");
}
       ;

statement   : TYPE ID SEMICOLON {
struct Node** children = malloc(3*sizeof(struct Node*));
children[0] = $1;
children[1] = $2;
children[2] = $3;
$$ = createNewNode("statement",3,children);
}
       | RETURN INTEGER SEMICOLON {
struct Node** children = malloc(3*sizeof(struct Node*));
children[0] = $1;
children[1] = $2;
children[2] = $3;
$$ = createNewNode("statement",3,children);
}
       | TYPE ID ASSIGN expression SEMICOLON {
struct Node** children = malloc(5*sizeof(struct Node*));
children[0] = $1;
children[1] = $2;
children[2] = $3;
children[3] = $4;
children[4] = $5;
$$ = createNewNode("statement",5,children);
}
       ;

expression   : INTEGER {
struct Node** children = malloc(1*sizeof(struct Node*));
children[0] = $1;
$$ = createNewNode("expression",1,children);
}
       | INTEGER OPT expression {
struct Node** children = malloc(3*sizeof(struct Node*));
children[0] = $1;
children[1] = $2;
children[2] = $3;
$$ = createNewNode("expression",3,children);
}
       ;

%%
int yyerror(const char *s)
{
    return 0;
}

int yywrap() 
{ 
   return(1); 
} 

int main(int argc,char * argv[])
{
    yyin = fopen(argv[1],"r");
    int x = yyparse();
    printf("%d",x);
    return 0;
}