%{
    #include "y.tab.h"
    #include <stdlib.h>
    #include <string.h>
%}

%%
"for"                   { return FOR; }
"++"                    { return INCOP; }
"--"                    { return DECOP; }
"<="|">="|"=="|"!="|"<"|">" { return RELOP; }
"="                     { return ASSIGN; }
";"                     { return SEMICOLON; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
[0-9]+                  { return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*  { return ID; }
[ \t\n]+                ;   /* skip whitespace */
.                       ;   /* ignore invalid characters */
%%

int yywrap(void) {
    return 1;
}

