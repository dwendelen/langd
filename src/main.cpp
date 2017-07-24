#include "ast.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include "java_printer.hpp"
#include <iostream>

using namespace std;

extern Block* parsedProgram;

int main() {
    int result = yyparse();
    if(result == 0) {
        cout << "/*" << endl;
        Printer* printer = new Printer();
        printer->print(parsedProgram);
        cout << "*/" << endl;

        JavaPrinter* javaPrinter = new JavaPrinter();
        javaPrinter->print(parsedProgram);
    }
    return result;
}
