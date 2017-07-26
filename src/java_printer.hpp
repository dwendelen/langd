#ifndef LANGD_JAVA_PRINTER_H
#define LANGD_JAVA_PRINTER_H

#include "ast.hpp"
#include <map>
#include <exception>
#include <list>

using namespace std;

class UnknownSymbol: public exception {
};

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
    TupleType* type;
    string javaName;
};

class FunctionEntry {
public:
    FunctionDeclaration* declaration;
    string javaName;
};


class JavaPrinter: public ExpressionVisitor {
private:
    IdReference* STRING = new IdReference("String");
    IdReference* INT = new IdReference("Int");
    IdReference* VOID = new IdReference("Void");


    Symbol symbolLastExpression;
    int lastId = 0;
    Scope* scope = new Scope;

    Symbol getSymbol(string name);
    bool hasSymbol(string name);

    list<StructEntry> structs;
    string getOrCreateStruct(TupleType* type);

    list<FunctionDeclaration*> functions;

    bool isInt(Type* type);
    bool isVoid(Type* type);
    bool isString(Type* type);
    string toJavaType(Type* type);

    string getNextId(string prefix);
    void endOperation(Type* outputType);

    void error();

    bool printFunctionArguments(FunctionType* type);
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
