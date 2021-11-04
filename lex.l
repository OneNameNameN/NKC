%{  
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	int line=1;
%}
LETTER      [a-zA-Z]
ID          ({LETTER}|_)({LETTER}|_|{DIGIT})* 
OPT         ("+"|"-"|"*"|"/"|"+="|"-="|"*="|"/="|"++"|"--") 
LT          ("<")
LE          ("<=")
EQ          ("==")
NE          ("<>")
GT          (">")
GE          (">=")
ASSIGN      ("=")
LNOT        ("!")
LP          ("(")
RP          (")")
LBRACKET    ("[")
RBRACKET    ("]")
LBRACE      ("{")
RBRACE      ("}")
SEMICOLON   (";")
COMMA       (",")
SINGLEQUOTES    ("\'")
DOUBLEQUOTES    ("\"")
WELLSIGN        ("#")
DIGIT           [0-9]
DIGITS          {DIGIT}+
OINTEGER        0|[1-9]{DIGIT}*
INTEGER         ("+"|"-")?{OINTEGER}
DECIMAL         {INTEGER}(.{DIGITS})
FLOAT           {INTEGER}(.{DIGITS})?[Ee]{INTEGER}
ERRORFLOAT      ([0-9])*+[.](0-9)*+[Ee]
TYPE            void|int|double|char|float
KEYWORD         if|else|do|while|for|scanf|printf|sqrt|abs|main|return
TYPEIDENTIFY    %d|%c|%s|%f|&{ID}
SinglePS        \/\/.*
DoublePS        \/\*(.|\n)*\*\/ 

%%
\n   {++line;}
{TYPE}                          {printf("line%d:(type, %s)\n",                  line,yytext);}
{KEYWORD}                       {printf("line%d:(keyword, %s)\n",               line,yytext);}
{ID}                            {printf("line%d:(identity, %s)\n",              line,yytext);}
{LP}                            {printf("line%d:(left parenthesis, %s)\n",      line,yytext);}
{RP}                            {printf("line%d:(right parenthesis, %s)\n",     line,yytext);}
{LBRACKET}                      {printf("line%d:(left bracket, %s)\n",          line,yytext);}
{RBRACKET}                      {printf("line%d:(right bracket, %s)\n",         line,yytext);}
{LBRACE}                        {printf("line%d:(left brace, %s)\n",            line,yytext);}
{RBRACE}                        {printf("line%d:(right brace, %s)\n",           line,yytext);}
{SEMICOLON}                     {printf("line%d:(semicolon, %s)\n",             line,yytext);}
{COMMA}                         {printf("line%d:(comma, %s)\n",                 line,yytext);}
{SINGLEQUOTES}                  {printf("line%d:(singlequotes, %s)\n",          line,yytext);}
{DOUBLEQUOTES}                  {printf("line%d:(doublequotes, %s)\n",          line,yytext);}
{WELLSIGN}                      {printf("line%d:(wellsign, %s)\n",              line,yytext);}
{OPT}                           {printf("line%d:(operator, %s)\n",              line,yytext);}
({LT}|{LE}|{EQ}|{NE}|{GT}|{GE}) {printf("line%d:(comparison, %s)\n",            line,yytext);}
{ASSIGN}                        {printf("line%d:(assign, %s)\n",                line,yytext);}
{LNOT}                          {printf("line%d:(left not, %s)\n",              line,yytext);}
{INTEGER}                       {printf("line%d:(integer, %s)\n",               line,yytext);}
{DECIMAL}                       {printf("line%d:(decimal, %s)\n",               line,yytext);}
{FLOAT}                         {printf("line%d:(float, %s)\n",                 line,yytext);}
{TYPEIDENTIFY}                  {printf("line%d:(typeidentify, %s)\n",          line,yytext);}
{ERRORFLOAT}                    {printf("ERRORFLOAT\n");}
({SinglePS}|{DoublePS})         {printf("PS\n");}
.       {}
[ \t]+  {}

%%
int main(int argc,char ** argv){
    if(argc>=2){
        if((yyin=fopen(argv[1],"r"))==NULL){
            printf("Can't open file %s\n",argv[1]);
            return 1;
        }
    }else return 0;
    yylex();
    return 0;
}
int yywrap(){
	return 1;
}