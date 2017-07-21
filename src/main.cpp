#include "ast.hpp"
#include "parser.hpp"
#include "printer.hpp"

extern Program* parsedProgram;

int main() {
    int result = yyparse();
    if(result == 0) {
        Printer* printer = new Printer();
        printer->print(parsedProgram);
    }
    return result;
}