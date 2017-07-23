#include "ast.hpp"
#include "parser.hpp"
#include "printer.hpp"

extern Block* parsedProgram;

int main() {
yydebug = 1;

    int result = yyparse();
    if(result == 0) {
        Printer* printer = new Printer();
        printer->print(parsedProgram);
    }
    return result;
}
