%{
	#include <vector>
	#include <string>
	#define YYSTYPE std::vector<std::string>*
	#include "parser.hpp"

	using namespace std;
	void yyerror(char *);
%}
%%
[1-9][0-9]* 	{ 
						auto val = new vector<string>();
						val->push_back(yytext);
						yylval = val;
						return INT;
				}

"-"				{	return MINUS;	}
"+"				{	return PLUS;	}
"*"				{	return TIMES;	}
"("				{	return LPARENT; }
")"				{	return RPARENT; }
.
%%

int yywrap(void) {
	return 1;
}
