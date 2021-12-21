%option noyywrap nodefault yylineno
%x COMMENT
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../grammer/Node/Node.h"
    #include "nkc.tab.h"
%}

delim      [ \t\n]
ws         {delim}+
digit      [0-9]
letter     [A-Za-z_]
id         {letter}({letter}|{digit})*
int        ([1-9]|[1-9]([0-9])+|0)
str        \"[^"]*\"

%%

"/*"                                { BEGIN(COMMENT); }
<COMMENT>"*/"                       { BEGIN(INITIAL); }
<COMMENT>([^*]|\n)+|.
"//".*                

"int"                               {yylval.s=strdup(yytext);return(INT);}
"main"                              {yylval.s=strdup(yytext);return(ID);}
"void"                              {yylval.s=strdup(yytext);return(VOID);}
"if"                                {return(IF);}
"else"                              {return(ELSE);}
"for"                               {return(FOR);}
"while"                             {return(WHILE);}
"return"                            {return(RETURN);}
"struct"                            {return(STRUCT);}
"."                                 {return(GETMEMBER);}
(>=)|(<=)|(==)|(!=)|<|>             {yylval.s=strdup(yytext);return(RELOP);}
"&"                                 {return('&'); }
"&&"                                {return(AND);}
"||"                                {return(OR);}
"="                                 {return('=');}
"!"                                 {return('!');} 
"+"                                 {return('+');}
"++"                                {return(ACC);}
"-"                                 {return('-');}
"--"                                {return(DEC);}
"*"                                 {return('*');}
"/"                                 {return('/');}
"%"                                 {return('%');}
"^"                                 {return('^');}
","                                 {return(',');}
";"                                 {return(';');}
"("                                 {return('(');}
")"                                 {return(')');}
"["                                 {return('[');}
"]"                                 {return(']');}
"{"                                 {return('{');}
"}"                                 {return('}');}
"+="                                 {return(ADD_E);}
"-="                                 {return(SUB_E);}
"*="                                 {return(MUL_E);}
"/="                                 {return(DIV_E);}
"%="                                 {return(MOD_E);}

[ \t\v\f\r\n]                       { }
{int}                               {yylval.s=strdup(yytext);return(NUMBER);}
{id}                                {yylval.s=strdup(yytext);return(ID);}
{str}                               {yylval.s=strdup(yytext);return(STR);}
{digit}{id}                         {return(ERRID);}
.                                   {return(ERRORCHAR);}

%%