#ifndef LANGD_PRINTER_H
#define LANGD_PRINTER_H

#include "ast.hpp"

class Printer: public ExpressionVisitor {
private:
    string beforeRootPrefix = "";
    string afterRootPrefix = "";
    string rootPrefix = "";

    void printElement(string s);
    template <typename T> void printList(string op, vector<T> list);
    void printItem(Expression* expression);
    void printItem(TypedId typedId);
    void printUnary(string op, Expression* param);
    void printBinary(string op, Expression* lhs, Expression* rhs);
public:
    void print(Block* block);

    virtual void visit(Block* block);
    virtual void visit(Assignment* assignment);

    virtual void visit(PlusOp* plusOp);
    virtual void visit(MinusOp* minusOp);
    virtual void visit(TimesOp* timesOp);
    virtual void visit(Negation* negation);
    virtual void visit(StringValue* negation);
    virtual void visit(IntValue* intValue);

    virtual void visit(Tuple* construct);
    virtual void visit(MemberSelection* memberSelection);

    virtual void visit(FunctionDeclaration* functionDeclaration);
    virtual void visit(FunctionCall* functionCall);
    virtual void visit(InfixFunctionCall* infixFunctionCall);

    virtual void visit(IdReference* idReference);
    virtual void visit(TupleType* complexType);

    virtual void visit(IdFunctionType* IdFunctionType);
    virtual void visit(TupleFunctionType* parameterFunctionType);
};

#endif
