#include "java_printer.hpp"
#include <iostream>

using namespace std;

void JavaPrinter::print(Program* program) {
    printLine("class LangD");
    printLine("{");

    indent = "    ";
    printLine("public static void main(String[] args) {");
    indented(program);
    printLine("}");

    indent = "";
    printLine("}");
}

void JavaPrinter::printLine(string s) {
    cout << indent << s << endl;
}

void JavaPrinter::indented(Visitable* toVisit) {
    string oldIndent = indent;
    indent = "    " + indent;
    toVisit->accept(this);
    indent = oldIndent;
}

void JavaPrinter::visit(Program* program) {
    for (Statement* stmt: program->getStatements()) {
        stmt->accept(this);
    }
}

void JavaPrinter::visit(PlusOp* plusOp) {

    visitBinaryOp("+", plusOp);
}

void JavaPrinter::visit(MinusOp* minusOp) {
    visitBinaryOp("-", minusOp);
}

void JavaPrinter::visit(TimesOp* timesOp) {
    visitBinaryOp("*", timesOp);
}

void JavaPrinter::visit(Negation* neg) {
    printLine("- (");
    indented(neg->getExpression());
    printLine(")");
}

void JavaPrinter::visit(StringValue* stringValue) {
    printLine(stringValue->getValue());
}

void JavaPrinter::visit(IntValue* intValue) {
    printLine(to_string(intValue->getValue()));
}

void JavaPrinter::visit(EvalId* evalId) {
    printLine("id(" + evalId->getId() + ")");
}

void JavaPrinter::visit(ExpressionAssignment* expressionAssignment) {
    printLine(expressionAssignment->getId() + " =");
    indented(expressionAssignment->getExpression());
}

void JavaPrinter::visit(TypeAssignment* typeAssignment) {
    printLine(typeAssignment->getId() + " = " + typeAssignment->getTypeDeclaration()->text);
}
