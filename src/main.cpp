#include "parser/ast.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include "java/java_printer.hpp"
#include <iostream>


using namespace std;
using namespace langd::java;

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
