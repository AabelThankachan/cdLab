%{
	#include <stdio.h>
%}
%%
.*sanj.* { printf("Rejected: contains 'sanj'\n"); }
.* { printf("Accepted: %s\n", yytext); }
%%
int yywrap() {
	return 1;
}
int main() {
	yylex();
	return 0;
}
