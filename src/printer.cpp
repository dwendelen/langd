#include "printer.hpp"
#include <iostream>

using namespace std;

void Printer::print(Block* block) {
    visit(block);
}

void Printer::printElement(string element) {
    cout << rootPrefix << element << endl;
}

template<class T> void Printer::printUnary(string op, T* param) {
    string oldBeforeRootPrefix = beforeRootPrefix;
    string oldRootPrefix = rootPrefix;
    string oldAfterRootPrefix = afterRootPrefix;

    int lengthOpInTabs = ((op.size() - 1) / 4) + 1;
    int lengthOpInTabSpaces = lengthOpInTabs * 4;
    int lengthPadding = lengthOpInTabSpaces - op.size();

    string padding;
    if(lengthPadding == 0) {
        padding = "";
    } else {
        padding = string(lengthPadding - 1, '-') + " ";
    }

    beforeRootPrefix = oldBeforeRootPrefix + string(lengthOpInTabSpaces, ' ');
    rootPrefix = rootPrefix + op + padding;
    afterRootPrefix = oldAfterRootPrefix + string(lengthOpInTabSpaces, ' ');

    param->accept(this);

    beforeRootPrefix = oldBeforeRootPrefix;
    rootPrefix = oldRootPrefix;
    afterRootPrefix = oldAfterRootPrefix;
}

template<class L, class R> void Printer::printBinary(string op, L* lhs, R* rhs) {
    string oldBeforeRootPrefix = beforeRootPrefix;
    string oldRootPrefix = rootPrefix;
    string oldAfterRootPrefix = afterRootPrefix;

    beforeRootPrefix = oldBeforeRootPrefix + "    ";
    rootPrefix = oldBeforeRootPrefix + "/-- ";
    afterRootPrefix = oldBeforeRootPrefix + "|   ";
    lhs->accept(this);

    cout << oldRootPrefix << op << endl;

    beforeRootPrefix = oldAfterRootPrefix + "|   ";
    rootPrefix = oldAfterRootPrefix + "\\-- ";
    afterRootPrefix = oldAfterRootPrefix + "    ";
    rhs->accept(this);

    beforeRootPrefix = oldBeforeRootPrefix;
    rootPrefix = oldRootPrefix;
    afterRootPrefix = oldAfterRootPrefix;
}

template <typename T> void Printer::printList(string op, vector<T> list) {
    string oldBeforeRootPrefix = beforeRootPrefix;
    string oldRootPrefix = rootPrefix;
    string oldAfterRootPrefix = afterRootPrefix;

    beforeRootPrefix = oldAfterRootPrefix + "|   ";
    rootPrefix = oldAfterRootPrefix + "|-- ";
    afterRootPrefix = oldAfterRootPrefix + "|   ";

    cout << oldRootPrefix << op << endl;

    for(T item: list) {
        printItem(item);
    }

    beforeRootPrefix = oldBeforeRootPrefix;
    rootPrefix = oldRootPrefix;
    afterRootPrefix = oldAfterRootPrefix;
}

void Printer::printItem(Expression* expression) {
    expression->accept(this);
}

void Printer::printItem(TypedId typedId) {
    printUnary(typedId.id + ": ", typedId.type);
}

void Printer::visit(Block* block) {
    printList("block", block->expressions);
}

void Printer::visit(Assignment* assignment) {
    printUnary(assignment->id + " = ", assignment->expression);
}

void Printer::visit(TypeAssignment* assignment) {
    printUnary(assignment->id + " = ", assignment->type);
}


void Printer::visit(PlusOp* plusOp) {
    printBinary("+", plusOp->lhs, plusOp->rhs);
}

void Printer::visit(MinusOp* minusOp) {
    printBinary("-", minusOp->lhs, minusOp->rhs);
}

void Printer::visit(TimesOp* timesOp) {
    printBinary("*", timesOp->lhs, timesOp->rhs);
}

void Printer::visit(Negation* neg) {
    printUnary("-", neg->expression);
}

void Printer::visit(StringValue* stringValue) {
    printElement("string(" + stringValue->value + ")");
}

void Printer::visit(IntValue* intValue) {
    printElement("int(" + to_string(intValue->value) + ")");
}

void Printer::visit(Tuple* tuple) {
    printList("tuple", tuple->assignments);
}

void Printer::visit(MemberSelection* memberSelection) {
    printUnary("select(" + memberSelection->id + ") ", memberSelection->previousExpression);
}

void Printer::visit(FunctionDefinition* functionDefinition) {
    printBinary("=> ", functionDefinition->inputType, functionDefinition->body);
}

void Printer::visit(FunctionCall* functionCall) {
    printUnary("call(" + functionCall->id + ")", functionCall->parameter);
}

void Printer::visit(InfixFunctionCall* infixFunctionCall) {
    printBinary("infixCall(" + infixFunctionCall->id + ")", infixFunctionCall->precedingExpression, infixFunctionCall->parameter);
}

void Printer::visit(IdReference* idRef) {
    printElement("id(" + idRef->id + ")");
}

void Printer::visit(TupleType* complexType) {
    printList("tuple", complexType->members);
}

void Printer::visit(FunctionType* functionType) {
    printBinary("=> ", functionType->inputType, functionType->outputType);
}
