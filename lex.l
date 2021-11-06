%{  
	#include <stdio.h>

	#include "y.tab.h"
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
KEYWORD         if|else|do|while|for|scanf|printf|sqrt|abs|main
RETURN         return
TYPEIDENTIFY    %d|%c|%s|%f|&{ID}
SinglePS        \/\/.*
DoublePS        \/\*(.|\n)*\*\/ 

%%
\n   {}
{TYPE}                          {yylval = createNewEndNode("TYPE",strdup(yytext));printf("TYPE");}return(TYPE);
{KEYWORD}                       {yylval = createNewEndNode("KEYWORD",strdup(yytext));}return(KEYWORD);
{RETURN}                       {yylval = createNewEndNode("RETURN",strdup(yytext));}return(RETURN);
{ID}                            {yylval = createNewEndNode("ID",strdup(yytext));}return(ID);
{LP}                            {yylval = createNewEndNode("LP",strdup(yytext));}return(LP);
{RP}                            {yylval = createNewEndNode("RP",strdup(yytext));}return(RP);
{LBRACKET}                      {yylval = createNewEndNode("LBRACKET",strdup(yytext));}return(LBRACKET);
{RBRACKET}                      {yylval = createNewEndNode("RBRACKET",strdup(yytext));}return(RBRACKET);
{LBRACE}                        {yylval = createNewEndNode("LBRACE",strdup(yytext));}return(LBRACE);
{RBRACE}                        {yylval = createNewEndNode("RBRACE",strdup(yytext));}return(RBRACE);
{SEMICOLON}                     {yylval = createNewEndNode("SEMICOLON",strdup(yytext));}return(SEMICOLON);
{COMMA}                         {yylval = createNewEndNode("COMMA",strdup(yytext));}return(COMMA);
{SINGLEQUOTES}                  {yylval = createNewEndNode("SINGLEQUOTES",strdup(yytext));}return(SINGLEQUOTES);
{DOUBLEQUOTES}                  {yylval = createNewEndNode("DOUBLEQUOTES",strdup(yytext));}return(DOUBLEQUOTES);
{WELLSIGN}                      {yylval = createNewEndNode("WELLSIGN",strdup(yytext));}return(WELLSIGN);
{OPT}                           {yylval = createNewEndNode("OPT",strdup(yytext));}return(OPT);
({LT}|{LE}|{EQ}|{NE}|{GT}|{GE}) {yylval = createNewEndNode("COMPARISION",strdup(yytext));}return(COMPARISION);
{ASSIGN}                        {yylval = createNewEndNode("ASSIGN",strdup(yytext));}return(ASSIGN);
{LNOT}                          {yylval = createNewEndNode("LNOT",strdup(yytext));}return(LNOT);
{INTEGER}                       {yylval = createNewEndNode("INTEGER",strdup(yytext));}return(INTEGER);
{DECIMAL}                       {yylval = createNewEndNode("DECIMAL",strdup(yytext));}return(DECIMAL);
{FLOAT}                         {yylval = createNewEndNode("FLOAT",strdup(yytext));}return(FLOAT);
{TYPEIDENTIFY}                  {yylval = createNewEndNode("TYPEIDENTIFY",strdup(yytext));}return(TYPEIDENTIFY);
{ERRORFLOAT}                    {yylval = createNewEndNode("ERRORFLOAT",strdup(yytext));}return(ERRORFLOAT);
({SinglePS}|{DoublePS})         {yylval = createNewEndNode("PS",strdup(yytext));}return(PS);
.       {}
[ \t]+  {}

%%
