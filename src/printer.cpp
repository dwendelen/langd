#include "printer.hpp"
#include <iostream>

using namespace std;

void Printer::print(Program* program) {
    visit(program);
}

void Printer::printLine(string s) {
    cout << indent << s << endl;
}

void Printer::indented(Visitable* toVisit) {
    string oldIndent = indent;
    indent = "    " + indent;
    toVisit->accept(this);
    indent = oldIndent;
}

void Printer::visitBinaryOp(string op, BinaryOp* binaryOp) {
    printLine("(");
    indented(binaryOp->getLhs());
    printLine(op);
    indented(binaryOp->getRhs());
    printLine(")");
}

void Printer::visit(Program* program) {
    for (Statement* stmt: program->getStatements()) {
        stmt->accept(this);
    }
}

void Printer::visit(PlusOp* plusOp) {
    visitBinaryOp("+", plusOp);
}

void Printer::visit(MinusOp* minusOp) {
    visitBinaryOp("-", minusOp);
}

void Printer::visit(TimesOp* timesOp) {
    visitBinaryOp("*", timesOp);
}

void Printer::visit(Negation* neg) {
    printLine("- (");
    indented(neg->getExpression());
    printLine(")");
}

void Printer::visit(StringValue* stringValue) {
    printLine(stringValue->getValue());
}

void Printer::visit(IntValue* intValue) {
    printLine(to_string(intValue->getValue()));
}

void Printer::visit(EvalId* evalId) {
    printLine("id(" + evalId->getId() + ")");
}

void Printer::visit(ExpressionAssignment* expressionAssignment) {
    printLine(expressionAssignment->getId() + " =");
    indented(expressionAssignment->getExpression());
}

void Printer::visit(TypeAssignment* typeAssignment) {
    printLine(typeAssignment->getId() + " = " + typeAssignment->getTypeDeclaration()->text);
}
