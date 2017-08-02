#include "parser/ast.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include <iostream>
#include <semantic/Analyser.hpp>
#include <java/JavaPrinter.hpp>


using namespace std;
using namespace langd::java;
using namespace langd;

extern Block* parsedProgram;

int main() {
    int result = yyparse();
    if(result == 0) {
        cout << "/*" << endl;
        Printer* printer = new Printer();
        printer->print(parsedProgram);
        cout << "*/" << endl;

        semantic::Analyser* analyser = new semantic::Analyser();
        auto analysedBlock = analyser->analyse(parsedProgram);
        
        JavaPrinter* javaPrinter = new JavaPrinter();
        javaPrinter->print(analysedBlock);
    }
    return result;
}
