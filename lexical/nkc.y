%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../grammer/Node/Node.h"
    #include "../grammer/Intermediate/Quaternion.h"
    int yylex();
    int yyerror(char *);
    // struct ast* root;
    AbstractNode* root;
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


%type <a> program
%type <a> specifier
%type <a> direct_declarator
%type <a> statement_block statements declaration_for
%type <a> expression statement argument_expression_list defination declaration_list declaration

%%

program:specifier ID '(' ')'statement{
root = new BaseNode("program");
root->addNode($5);
}
;

statement_block: '{' statements '}'{
$$ = new StatementBlockNode($2);
}
| '{' '}' {
$$ = nullptr;
}
;

statements: statement {
$$ = new StatementsNode($1);
}
|statements statement {
$$ = new StatementsNode($2,$1);
}
;

defination: specifier declaration_list  {
$$ = new DefineVarNode($1->value,$2);
}
;

declaration_list: declaration {
$$ = new DefineListNode($1);
}
| declaration ',' declaration_list {
$$ = new DefineListNode($1,$3);
}
;

declaration: direct_declarator {
$$ = new BaseNode("DefineNode");
$$->addNode($1);
}
| direct_declarator '=' expression {
$$ = new BaseNode("DefineNode");
$$->addNode($3);
$$->addNode($1);
}
;

direct_declarator: ID {
$$ = new BaseNode("ID",$1)
}
| ID '[' INT ']' {
$$ = new BaseNode("ID",$1)
}
;

declaration_for: defination {
$$ = $1;
}
| expression {
$$ = $1;
}
;

statement: defination';'{
$$ = $1;
}
| direct_declarator '=' expression ';'{
$$ = new AssignNode($1,$3);
}
| expression ';'{
$$ = $1;
}
| statement_block {
$$ = $1;
}
| IF '(' expression ')' statement {
$$ = new IfElseNode($3,$5);
}
| IF '(' expression ')' statement ELSE statement %prec LOWER_THAN_ELSE{
$$ = new IfElseNode($3,$5,$7);
}
| WHILE '(' expression ')' statement{
$$ = new WhileNode($3,$5);
}
| FOR '(' ';' ';' ')' statement{
$$ = new ForNode(nullptr,nullptr,nullptr,$6);
}
| FOR '(' declaration_for ';' ';' ')' statement{
$$ = new ForNode($3,nullptr,nullptr,$7);
}
| FOR '(' ';' expression ';' ')' statement{ 
$$ = new ForNode(nullptr,$4,nullptr,$7);
}
| FOR '(' ';' ';' expression ')' statement{
$$ = new ForNode(nullptr,nullptr,$5,$7);
}
| FOR '(' declaration_for ';' expression ';' expression ')' statement {
$$ = new ForNode($3,$5,$7,$9);
}
| FOR '(' declaration_for ';' expression ';' ')' statement  {
$$ = new ForNode($3,$5,nullptr,$8);
}
| FOR '(' declaration_for ';' ';' expression ')' statement  {
$$ = new ForNode($3,nullptr,$6,$8);
}
| FOR '(' ';' expression ';' expression ')' statement { 
$$ = new ForNode(nullptr,$4,$6,$8);
}
| RETURN expression ';' {
$$ = new BaseNode("RETURN","RETURN");
$$ -> addNode($2);
}
| RETURN ';' {
$$ = new BaseNode($1,$1);
}
| ';' {
$$ = nullptr;
}
;

specifier: INT {
$$ = new BaseNode("specifier",$1);
}
| VOID {
$$ = new BaseNode("specifier",$1);
}
|INT '*' {
$$ = new BaseNode("specifier","int*");
}
|VOID '*' {
$$ = new BaseNode("specifier","void*");
}
;

expression: expression RELOP expression{
$$ = new ExpressionNode($1,4,$3,$2);
}
| expression '+' expression{
$$ = new ExpressionNode($1,2,$3,"+");
}
| expression '-' expression{
$$ = new ExpressionNode($1,2,$3,"-");
}
| expression '*' expression{
$$ = new ExpressionNode($1,2,$3,"*");
}
| expression '/' expression{
$$ = new ExpressionNode($1,2,$3,"/");
}
| expression '%' expression{
$$ = new ExpressionNode($1,2,$3,"%");
}
| expression '^' expression{
$$ = new ExpressionNode($1,2,$3,"^");
}
| expression AND expression{
$$ = new ExpressionNode($1,2,$3,"&&");
}
| expression OR expression{
$$ = new ExpressionNode($1,2,$3,"||");
}
| '-' expression{
$$ = new ExpressionNode($2,1,nullptr,"-");
}
| '!' expression{
$$ = new ExpressionNode($2,1,nullptr,"!");
}
| expression ACC {
$$ = new ExpressionNode($1,1,nullptr,"++");
}
| expression DEC {
$$ = new ExpressionNode($1,1,nullptr,"--");
}
| '&' direct_declarator {
$$ = new ExpressionNode($2,3,nullptr,"&");
}
| '(' expression ')' {
$$ = $2;
}
| NUMBER{
$$ = new ExpressionNode(new BaseNode("NUMBER",$1));
}
| STR {
$$ = new ExpressionNode(new BaseNode("STRING",$1));
}
| ID '(' argument_expression_list ')' {
$$ =new ExpressionNode(new CallNode($1,$3));
}
| direct_declarator{
$$ = new ExpressionNode($1);
}
;

argument_expression_list: expression{
	$$ = new ExpressionListNode($1,nullptr);
}
| argument_expression_list ',' expression{
	$$ = new ExpressionListNode($3,$1);
}
;

%%

int main(int argc, char **argv)
{
    yyin = fopen("../test/base.cpp", "r");
    yyparse();
    fclose(yyin);
    root->print(0);
    root->createSymbolTable(true);
    cout<<endl<<endl;
    SymbolTable::rootTable->print(0);
    for(int i=0;i<IM::Quaternion::quads->size();i++)
    {
        printf("%d ",i);
        (*IM::Quaternion::quads)[i].print();
    }
    /* Intermediate *im = new IM::Intermediate(root, struct_table);
    im->generate(root, im->getTable());
    im->print(); */
}
int yyerror(char *s)
{
    fprintf(stderr, "error: %s\n", s);
    return 1;
}