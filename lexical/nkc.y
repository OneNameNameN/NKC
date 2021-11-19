%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../grammer/Node/Node.h"

    int yylex();
    int yyerror(char *);
    struct ast* root;
    extern FILE *yyin;
%}

%union {
    AbstractNode* a;
    char *s;
}

%right <s> '='
%left <s> OR
%left <s> AND
%left <s> '&'
%left <s> RELOP
%left <s> '-' '+'
%left <s> '*' '/' '%'
%left <s> ACC DEC /* 这里仅认为自加自减会出现在变量右侧 */
%left <s> '^'
%right <s> '!'
%left <s> '(' ')' '[' ']'
%nonassoc <s> LOWER_THAN_ELSE
%token ERRID
%token <s> ID INTEGER NUMBER
%token <s> INT VOID STR
%token <s> ',' ';'
%token <s> STRUCT
%token <s> IF ELSE WHILE FOR CONTINUE RETURN ERRORCHAR GETMEMBER
%token <s> '{' '}'


%type <a>  program statements type expression statement

%%

program:statements{
$$ = new BaseNode("program");
$$->addNode($1);
$$->print(0);
}
;

statements: statement {
$$ = new StatementsNode($1);
}
|statements statement {
$$ = new StatementsNode($2,$1);
}
;

statement: type ID ';'{
$$ = new BaseNode("statement");
$$->addNode($1);
$$->addNode(new BaseNode("ID",$2));
}
| expression ';'{
$$ = new BaseNode("statement");
$$->addNode($1);
}
| WHILE '(' expression ')' statement{
$$ = new WhileNode($3,$5);
}
;

expression: ID{
$$ = new ExpressionNode(new BaseNode("ID",$1));
}
| NUMBER{
$$ = new ExpressionNode(new BaseNode("NUMBER",$1));
}
| NUMBER '+' expression{
$$ = new ExpressionNode(new BaseNode("NUMBER",$1),2,$3,"+");
}
| ID '+' expression{
$$ = new ExpressionNode(new BaseNode("ID",$1),2,$3,"+");
}
;

type: INT {
$$ = new BaseNode("type",$1);
}
| VOID {
$$ = new BaseNode("type",$1);
}
|INT '*' {
$$ = new BaseNode("type","int*");
}
|VOID '*' {
$$ = new BaseNode("type","void*");
}
;

%%

int main(int argc, char **argv)
{
    yyin = fopen("../test/base.cpp", "r");
    yyparse();
    fclose(yyin);
}
int yyerror(char *s)
{
    fprintf(stderr, "error: %s\n", s);
    return 1;
}