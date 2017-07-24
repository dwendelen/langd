#ifndef LANGD_JAVA_PRINTER_H
#define LANGD_JAVA_PRINTER_H

#include "ast.hpp"
#include <map>

using namespace std;

class Symbol {
public:
    Type* type;
    string javaName;
};

class Scope {
public:
    map<string, Symbol> symbols;
    Scope* parent;
};

class StructEntry {
public:
    ComplexType* type;
    string javaName;
};

class FunctionEntry {
public:
    FunctionDeclaration* declaration;
    string javaName;
};

class JavaPrinter: public Visitor {
private:
    Symbol symbolLastExpression;
    int lastId = 0;
    Scope scope;

    int lastStructId = 0;
    vector<StructEntry> structs;
    string getOrCreateStruct(ComplexType* type);

    string toJavaType(Type* type);

    string getNextId();
    void endOperation(Type* outputType);

    void error();
public:
    void print(Block* block);

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
