%{
	#include <stdio.h>
%}

%s Clasa

function_name ([a-zA-Z0-9])*
delimiter [ \t\n]*

arg_even {delimiter}([a-z0-9][a-z0-9])*{delimiter}
arg_odd {delimiter}([a-z0-9][a-z0-9])*[a-z0-9]{delimiter}

function_arg_even [ ]*\({arg_even}(,{arg_even})*\)
function_arg_odd [ ]*\({arg_odd},{arg_odd},{arg_odd}\)

int_arg_odd {arg_odd}(,{arg_odd},{arg_odd})*;
float_id {arg_even}(,[ ]*{arg_even})*;

%%
class[ ]*\{ { BEGIN Clasa; }
<Clasa>\} { BEGIN 0; }
<Clasa>"public int "{int_arg_odd} { printf ("%s\n", yytext); }
<Clasa>"private float "{float_id} { printf("%s \n", yytext); }
<Clasa>"protected float "{float_id} { printf("%s \n", yytext); }
.|\n ;  
%%

int main(int argc, char** argv) {
	FILE *g;
	if(argc>0)
		yyin = fopen(argv[1],"r");

	yylex();
}
