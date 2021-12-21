%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../grammer/Node/Node.h"
    #include "../grammer/Intermediate/Intermediate.h"
    #include "../grammer/AsmGenerator.h"
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
$$ = new BaseNode("ID",$1);
}
| ID '[' expression ']' {
$$ = new BaseNode("ID",$1,$3);
}
| '*' ID {
$$ = new BaseNode("ID",$2,true);
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
| direct_declarator '=' expression {
$$ = new ExpressionNode(new AssignNode($1,$3));
((ExpressionNode*)$$) -> expressionType = ExpressionNode::ExpressionType::Assign;
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
    yyin = fopen(argv[1], "r");
    if(yyin == NULL)
        yyin = fopen("../test/base.cpp", "r");
    yyparse();
    fclose(yyin);

    cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Abstract Syntex Tree:" << endl;
    root->print(0);
    cout << endl << endl;

    
    cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Symbol Table:" << endl;
    root->createSymbolTable(true);
    SymbolTable::rootTable->print(0);

    cout << endl << endl;
    cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Intermediate Code:" << endl;
    for(int i=0;i<Intermediate::quads->size();i++)
    {
        printf("%d ",i);
        (*Intermediate::quads)[i].print();
    }
    printf("\n");
    AsmGenerator* asmGenerator = new AsmGenerator(Intermediate::quads);
    asmGenerator->generate();
    cout << "Complete!" << endl;
}
int yyerror(char *s)
{
    fprintf(stderr, "error: %s\n", s);
    exit(1);
}