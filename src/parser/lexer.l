%{
    #include <vector>
    #include <string>
    #include <iostream>
    #include "parser/ast.hpp"
    #include "parser.hpp"

    using namespace std;
    void yyerror(char const *);
%}
%%
"def"                       {   return DEF;         }
"let"                       {   return LET;         }
"type"                      {   return TYPE;        }
[1-9][0-9]*|0               {
                                yylval.integer = stoi(yytext);
                                return INT;
                            }
"-"                         {   return MINUS;       }
"+"                         {   return PLUS;        }
"*"                         {   return TIMES;       }
"("                         {   return LPARENT;     }
")"                         {   return RPARENT;     }
"="                         {   return EQUALS;      }
";"                         {   return SEMICOLON;   }
":"                         {   return COLON;       }
"=>"                        {   return ARROW;       }
"."                         {   return DOT;         }
","                         {   return COMMA;       }
\"([^\"\\]|\\\"|\\\\)*\"    {
                                yylval.string = new string(yytext);
                                return STRING;
                            }
[_a-zA-Z][_a-zA-Z0-9]*     {
                                yylval.string = new string(yytext);
                                return ID;
                            }
[ \n\t]+                    ;
.                           yyerror(("Unknown token " + string(yytext)).c_str());
%%

int yywrap(void) {
    return 1;
}
