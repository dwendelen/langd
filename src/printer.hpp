#ifndef LANGD_PRINTER_H
#define LANGD_PRINTER_H

#include "ast.hpp"

class Printer: public ExpressionVisitor, public TypeVisitor {
private:
    string beforeRootPrefix = "";
    string afterRootPrefix = "";
    string rootPrefix = "";

    void printElement(string s);
    template <typename T> void printList(string op, vector<T> list);
    void printItem(Expression* expression);
    void printItem(TypedId typedId);
    template <class T> void printUnary(string op, T* param);
    template <class L, class R> void printBinary(string op, L* lhs, R* rhs);
public:
    void print(Block* block);

    virtual void visit(Block* block);
    virtual void visit(Assignment* assignment);
    virtual void visit(TypeAssignment* typeAssignment);


    virtual void visit(PlusOp* plusOp);
    virtual void visit(MinusOp* minusOp);
    virtual void visit(TimesOp* timesOp);
    virtual void visit(Negation* negation);
    virtual void visit(StringValue* negation);
    virtual void visit(IntValue* intValue);

    virtual void visit(Tuple* construct);
    virtual void visit(MemberSelection* memberSelection);

    virtual void visit(FunctionDefinition* functionDefinition);
    virtual void visit(FunctionCall* functionCall);
    virtual void visit(InfixFunctionCall* infixFunctionCall);

    virtual void visit(IdReference* idReference);
    virtual void visit(TupleType* complexType);
    virtual void visit(FunctionType* functionType);
};

#endif
