%{
	#include <vector>
	#include <string>
    	#include <iostream>
	#define YYSTYPE std::vector<std::string>*

	using namespace std;
	void yyerror(char const *);
%}
%%
[1-9][0-9]* 	            { 
						        auto val = new vector<string>();
						     val->push_back(yytext);
						     yylval = val;
						        return INT;
				            }
"-"				            {	return MINUS;	    }
"+"				            {	return PLUS;	    }
"*"				            {   return TIMES;	    }
"("				            {   return LPARENT;     }
")"				            {	return RPARENT;     }
"="                         {   return EQUALS;      }
";"                         {   return SEMICOLON;   }
":"                         {   return COLON;       }
"=>"                        {   return ARROW;       }
"."                         {   return DOT;         }
","                         {   return COMMA;       }
\"([^\"\\]|\\\"|\\\\)*\"	{ 	
                                auto val = new vector<string>();
						        val->push_back(yytext);
						        yylval = val;
					            return STRING;	
				            }
[_a-zA-Z][-_a-zA-Z0-9]*     {
                                auto val = new vector<string>();
						        val->push_back(yytext);
						        yylval = val;
					            return ID;
                            }
[ \n\t]+                    ;
.                           yyerror(("Unknown token " + string(yytext)).c_str());          
%%

int yywrap(void) {
	return 1;
}
