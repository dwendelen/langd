#ifndef LANGD_PRINTER_H
#define LANGD_PRINTER_H

#include "ast.hpp"

class Printer: public Visitor {
private:
    string beforeRootPrefix = "";
    string afterRootPrefix = "";
    string rootPrefix = "";
public:
    void print(Block* block);

    void printElement(string s);
    template <typename T> void printList(string op, vector<T> list);
    void printItem(Visitable* visitable);
    void printItem(TypedId typedId);
    void printUnary(string op, Visitable* param);
    void printBinary(string op, Visitable* lhs, Visitable* rhs);

    virtual void visit(Block* block);
    virtual void visit(PlusOp* plusOp);
    virtual void visit(MinusOp* minusOp);
    virtual void visit(TimesOp* timesOp);
    virtual void visit(Negation* negation);
    virtual void visit(StringValue* negation);
    virtual void visit(IntValue* intValue);
    virtual void visit(ReferenceId* evalId);

    virtual void visit(ParameterLessFunctionCall* functionCall);
    virtual void visit(ParametrisedFunctionCall* functionCall);
    virtual void visit(Construct* construct);
    virtual void visit(Assignment* assignment);
    virtual void visit(FunctionDeclaration* functionDeclaration);
    virtual void visit(MemberSelection* memberSelection);

    virtual void visit(InfixFunctionCall* InfixFunctionCall);
    virtual void visit(ComplexType* complexType);
    virtual void visit(IdFunctionType* IdFunctionType);
    virtual void visit(ParameterFunctionType* parameterFunctionType);
    virtual void visit(ParameterLessFunctionType* ParameterLessFunctionType);
};

#endif
