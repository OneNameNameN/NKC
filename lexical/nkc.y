%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../grammer/nkc.h"
    #include "../grammer/symbolTable.h"

    int yylex();
    int yyerror(char *);
    struct ast* root;
    extern FILE *yyin;
%}

%union {
    struct ast *a;
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
%token <s> ID 
%token <s> INT VOID STR
%token <s> ',' ';'
%token <s> STRUCT
%token <s> IF ELSE WHILE FOR CONTINUE RETURN ERRORCHAR GETMEMBER
%token <s> '{' '}'


%type <a> program translation_unit external_declaration external_declaration_list
%type <a> specifier
%type <a> struct_specifier struct_declaration_list struct_declaration
%type <a> direct_declarator func_declarator parameter_list parameter_declaration
%type <a> compound_statement block_item_list declaration_for 
%type <a> expression argument_expression_list statement defination declaration_list declaration
%type <a> init_declarator_list init_declarator

%%

program: translation_unit {
    $$ = create('N', "program");
    add($$, $1);
    root = $$;
    }
    ;

translation_unit: external_declaration { 
        $$ = create('N', "translation_unit");
        add($$, $1);
    }
    | translation_unit external_declaration {
        $$ = create('N', "translation_unit");
        add($$, $1);
        add($$, $2);
    }
    ;

external_declaration: specifier external_declaration_list ';' {
        $$ = create('N', "external_declaration");
        add($$, $1);
        add($$, $2);
        add($$, create('T', ";"));
    }
    | specifier ';' {
        $$ = create('N', "external_declaration");
        add($$, $1);
        add($$, create('T', ";"));
    }
    | specifier func_declarator compound_statement {
        $$ = create('N', "external_declaration");
        add($$, $1);
        add($$, $2);
        add($$, $3);
    }
    | specifier func_declarator ';'  {
        $$ = create('N', "external_declaration");
        add($$, $1);
        add($$, $2);
        add($$, create('T', ";"));
    }
    | struct_specifier ';' { 
        $$ = create('N', "external_declaration");
        add($$, $1);
        add($$, create('T', ";"));
    }
    | error ';' { yyerrok; $$ = NULL; }
    ;

external_declaration_list: init_declarator_list { 
        $$ = create('N', "external_declaration_list");
        add($$, $1);
    }
    | external_declaration_list ',' init_declarator_list {
        $$ = create('N', "external_declaration_list");
        add($$, $1);
        add($$, create('T', ","));
        add($$, $3);
    }
    ;

init_declarator_list: init_declarator {
        $$ = create('N', "init_declarator_list");
        add($$, $1);
    }
    | init_declarator_list ',' init_declarator {
        $$ = create('N', "init_declarator_list");
        add($$, $1);
        add($$, create('T', ","));
        add($$, $3);
    }
    ;

init_declarator: direct_declarator {
        $$ = create('N', "init_declarator");
        add($$, $1); 
    }
    | direct_declarator '=' INT {
        $$ = create('N', "init_declarator");
        add($$, $1); 
        add($$, create('T', "="));
        add($$, create('T', $3)); 
    }
    ;

/* specifiers 说明符*/
specifier: INT { $$ = create('N', "specifier"); add($$, create('T', $1)); }
    | VOID { $$ = create('N', "specifier"); add($$, create('T', $1)); }
    | INT '*' { $$ = create('N', "specifier"); add($$, create('T', $1)); add($$, create('T', "*")); }
    | VOID '*' { $$ = create('N', "specifier"); add($$, create('T', $1)); add($$, create('T', "*")); }
    ;

struct_specifier: STRUCT ID '{' struct_declaration_list '}' { 
        $$ = create('N', "struct_specifier");
        add($$, create('T', "struct"));
        add($$, create('T', $2));
        add($$, create('T', "{"));
        add($$, $4);
        add($$, create('T', "}"));
    }
    ;

struct_declaration_list: struct_declaration { 
        $$ = create('N', "struct_declaration_list");
        add($$, $1);
    }
    | struct_declaration_list struct_declaration{
        $$ = create('N', "struct_declaration_list");
        add($$, $1);
        add($$, $2);
    }
    ;

struct_declaration: specifier ID ';' { 
        $$ = create('N', "struct_declaration");
        add($$, $1);
        add($$, create('T', $2));
        add($$, create('T', ";"));
    }
    ;

direct_declarator: ID {
        $$ = create('N', "direct_declarator");
        add($$, create('T', $1));
    }
    | ID '[' INT ']' {
        $$ = create('N', "direct_declarator");
        add($$, create('T', $1));
        add($$, create('T', "["));
        add($$, create('T', $3));
        add($$, create('T', "]"));
    }
    ;

func_declarator: ID '(' parameter_list ')' { 
        $$ = create('N', "func_declarator");
        add($$, create('T', $1));
        add($$, create('T', "("));
        add($$, $3);
        add($$, create('T', ")"));
    }
    | ID '(' ')' {
        $$ = create('N', "func_declarator");
        add($$, create('T', $1));
        add($$, create('T', "("));
        add($$, create('T', ")"));
    }
    ;

parameter_list: parameter_list ',' parameter_declaration {
        $$ = create('N', "parameter_list");
        add($$, $1);
        add($$, create('T', ","));
        add($$, $3);
    }
    | parameter_declaration { 
        $$ = create('N', "parameter_list");
        add($$, $1);
    }
    ;

parameter_declaration: specifier ID {
        $$ = create('N', "parameter_declaration");
        add($$, $1);
        add($$, create('T', $2));
    }
    | specifier {
        $$ = create('N', "parameter_declaration");
        add($$, $1);
    }
    ;


/* Statement 声明*/
compound_statement: '{' block_item_list '}' {
        $$ = create('N', "compound_statement");
        add($$, create('T', "{"));
        add($$, $2);
        add($$, create('T', "}"));
    }
    | error '}' { yyerrok; }
    ;

block_item_list: statement block_item_list {
        $$ = create('N', "block_item_list");
        add($$, $1);
        add($$, $2);
    }
    | { $$ = NULL; }
    ;

declaration_for: defination { $$ = create('N', "declaration_for"); add($$, $1); }
    | expression { $$ = create('N', "declaration_for"); add($$, $1); }
    ;

statement: expression ';' { 
        $$ = create('N', "statement");
        add($$, $1);
        add($$, create('T', ";"));
    }
    | defination ';' { 
        $$ = create('N', "statement");
        add($$, $1);
        add($$, create('T', ";"));
    }
    | STRUCT ID ID ';' { 
        $$ = create('N', "statement");
        add($$, create('T', "struct"));
        add($$, create('T', $2));
        add($$, create('T', $3));
        add($$, create('T', ";"));
    }
    | compound_statement { $$ = create('N', "statement"); add($$, $1); }
    | RETURN expression ';' {
        $$ = create('N', "statement");
        add($$, create('T', "return"));
        add($$, $2);
        add($$, create('T', ";"));
    }
    | RETURN ';' {
        $$ = create('N', "statement");
        add($$, create('T', "return"));
        add($$, create('T', ";"));
    }
    | IF '(' expression ')' statement {  // ok
        $$ = create('N', "statement");
        add($$, create('T', "if"));
        add($$, create('T', "("));
        add($$, $3);
        add($$, create('T', ")"));
        add($$, $5);
    }
    | IF '(' expression ')' statement ELSE statement %prec LOWER_THAN_ELSE{
        $$ = create('N', "statement");
        add($$, create('T', "if"));
        add($$, create('T', "("));
        add($$, $3);
        add($$, create('T', ")"));
        add($$, $5);
        add($$, create('T', "else"));
        add($$, $7);
    }
    | WHILE '(' expression ')' statement {
        $$ = create('N', "statement");
        add($$, create('T', "while"));
        add($$, create('T', "("));
        add($$, $3);
        add($$, create('T', ")"));
        add($$, $5);
    }
    | FOR '(' ';' ';' ')' statement{
        $$ = create('N', "statement");
        add($$, create('T', "for"));
        add($$, create('T', "("));
        add($$, create('T', ";"));
        add($$, create('T', ";"));
        add($$, create('T', ")"));
        add($$, $6);
    }
    | FOR '(' declaration_for ';' ';' ')' statement{
        $$ = create('N', "statement");
        add($$, create('T', "for"));
        add($$, create('T', "("));
        add($$, $3);
        add($$, create('T', ";"));
        add($$, create('T', ";"));
        add($$, create('T', ")"));
        add($$, $7);
    }
    | FOR '(' ';' expression ';' ')' statement{ 
        $$ = create('N', "statement");
        add($$, create('T', "for"));
        add($$, create('T', "("));
        add($$, create('T', ";"));
        add($$, $4);
        add($$, create('T', ";"));
        add($$, create('T', ")"));
        add($$, $7);
    }
    | FOR '(' ';' ';' expression ')' statement{
        $$ = create('N', "statement");
        add($$, create('T', "for"));
        add($$, create('T', "("));
        add($$, create('T', ";"));
        add($$, create('T', ";"));
        add($$, $5);
        add($$, create('T', ")"));
        add($$, $7);
    }
    | FOR '(' declaration_for ';' expression ';' expression ')' statement {
        $$ = create('N', "statement");
        add($$, create('T', "for"));
        add($$, create('T', "("));
        add($$, $3);
        add($$, create('T', ";"));
        add($$, $5);
        add($$, create('T', ";"));
        add($$, $7);
        add($$, create('T', ")"));
        add($$, $9);
    }
    | FOR '(' declaration_for ';' expression ';' ')' statement  {
        $$ = create('N', "statement");
        add($$, create('T', "for"));
        add($$, create('T', "("));
        add($$, $3);
        add($$, create('T', ";"));
        add($$, $5);
        add($$, create('T', ";"));
        add($$, create('T', ")"));
        add($$, $8);
    }
    | FOR '(' declaration_for ';' ';' expression ')' statement  {
        $$ = create('N', "statement");
        add($$, create('T', "for"));
        add($$, create('T', "("));
        add($$, $3);
        add($$, create('T', ";"));
        add($$, create('T', ";"));
        add($$, $6);
        add($$, create('T', ")"));
        add($$, $8);
    }
    | FOR '(' ';' expression ';' expression ')' statement { 
        $$ = create('N', "statement");
        add($$, create('T', "for"));
        add($$, create('T', "("));
        add($$, create('T', ";"));
        add($$, $4);
        add($$, create('T', ";"));
        add($$, $6);
        add($$, create('T', ")"));
        add($$, $8);
    }
    | error ';' { yyerrok; } 
    ;


/* Local Definitions */
defination: specifier declaration_list  {
        $$ = create('N', "defination");
        add($$, $1);
        add($$, $2);
    }
    | error ';' { yyerrok; }
    ;

declaration_list: declaration { $$ = create('N', "declaration_list"); add($$, $1); }
    | declaration ',' declaration_list {
        $$ = create('N', "declaration_list");
        add($$, $1);
        add($$, create('T', ","));
        add($$, $3);
    }
    ;

declaration: direct_declarator { $$ = create('N', "declaration"); add($$, $1); }
    | direct_declarator '=' expression {
        $$ = create('N', "declaration");
        add($$, $1);
        add($$, create('T', "="));
        add($$, $3);
    }
;


/* expressionression */
expression: expression '=' expression {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "="));
            add($$, $3);
        }
        | expression AND expression {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "and"));
            add($$, $3);
        }
        | expression OR expression {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "or"));
            add($$, $3);
        }
        | expression RELOP expression {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', $2));
            add($$, $3);
        }
        | expression '+' expression {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "+"));
            add($$, $3);
        }
        | expression '-' expression {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "-"));
            add($$, $3);
        }
        | expression '*' expression {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "*"));
            add($$, $3);
        }
        | expression '/' expression {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "/"));
            add($$, $3);
        }
        | expression '%' expression {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "%"));
            add($$, $3);
        }
        | expression '^' expression {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "^"));
            add($$, $3);
        }
        | '(' expression ')' {
            $$ = create('N', "expression");
            add($$, create('T', "("));
            add($$, $2);
            add($$, create('T', ")"));
        }
        | '-' expression {
            $$ = create('N', "expression");
            add($$, create('T', "-"));
            add($$, $2);
        }
        | '!' expression {
            $$ = create('N', "expression");
            add($$, create('T', "!"));
            add($$, $2);
        }
        | expression ACC {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "++"));
        }
        | expression DEC {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "--"));
        }
        | '&' ID {
            $$ = create('N', "expression");
            add($$, create('T', "&"));
            add($$, create('T', $2));
        }
        | ID '(' argument_expression_list ')' {
            $$ = create('N', "expression");
            add($$, create('T', $1));
            add($$, create('T', "("));
            add($$, $3);
            add($$, create('T', ")"));
        }
        | ID '(' ')' {
            $$ = create('N', "expression");
            add($$, create('T', $1));
            add($$, create('T', "("));
            add($$, create('T', ")"));
        }
        | expression '[' expression ']' {
            $$ = create('N', "expression");
            add($$, $1);
            add($$, create('T', "["));
            add($$, $3);
            add($$, create('T', "]"));
        }
        | ID {
            $$ = create('N', "expression");
            add($$, create('T', $1));
        }
        | STR {
            $$ = create('N', "expression");
            add($$, create('T', $1));
        }
        | ID '[' expression ']' {
            $$ = create('N', "expression");
            add($$, create('T', $1));
            add($$, create('T', "["));
            add($$, $3);
            add($$, create('T', "]"));
        }
        | ID GETMEMBER ID {
            $$ = create('N', "expression");
            add($$, create('T', $1));
            add($$, create('T', "."));
            add($$, create('T', $3));
        }
        | INT {
            $$ = create('N', "expression");
            add($$, create('T', $1));
        }
        | '*' ID {
            $$ = create('N', "expression");
            add($$, create('T', "*"));
            add($$, create('T', $2));
        }
        | error ')' {yyerrok;} 
        ;

argument_expression_list: expression {
            $$ = create('N', "argument_expression_list");
            add($$, $1);
        }
        | argument_expression_list ',' expression {
            $$ = create('N', "argument_expression_list");
            add($$, $1);
            add($$, create('T', ","));
            add($$, $3);
        }
        ;

%%

int main(int argc, char **argv)
{
    yyin = fopen("../test/base.cpp", "r");
    yyparse();
    fclose(yyin);
    print(root, 0);
}
int yyerror(char *s)
{
    fprintf(stderr, "error: %s\n", s);
    return 1;
}