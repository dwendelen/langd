#ifndef LANGD_AST_H
#define LANGD_AST_H

#include <vector>
#include <string>

using namespace std;

class Block;
class Statement;
class PlusOp;
class MinusOp;
class TimesOp;
class Negation;
class StringValue;
class IntValue;
class ReferenceId;
class Assignment;
class Type;
class SimpleType;
class ComplexType;
class TypedId;
class FunctionType;
class IdFunctionType;
class ParameterFunctionType;
class ParameterLessFunctionType;
class FunctionCall;
class ParameterLessFunctionCall;
class ParametrisedFunctionCall;
class Construct;
class InfixFunctionCall;

class Visitor {
public:
    virtual void visit(Block* block) = 0;
    virtual void visit(PlusOp* plusOp) = 0;
    virtual void visit(MinusOp* minusOp) = 0;
    virtual void visit(TimesOp* timesOp) = 0;
    virtual void visit(Negation* negation) = 0;
    virtual void visit(StringValue* negation) = 0;
    virtual void visit(IntValue* intValue) = 0;
    virtual void visit(ReferenceId* evalId) = 0;

    virtual void visit(ParameterLessFunctionCall* functionCall) = 0;
    virtual void visit(ParametrisedFunctionCall* functionCall) = 0;
    virtual void visit(Construct* construct) = 0;
    virtual void visit(Assignment* assignment) = 0;

    virtual void visit(InfixFunctionCall* InfixFunctionCall) = 0;
    virtual void visit(ComplexType* complexType) = 0;
    virtual void visit(IdFunctionType* IdFunctionType) = 0;
    virtual void visit(ParameterFunctionType* parameterFunctionType) = 0;
    virtual void visit(ParameterLessFunctionType* ParameterLessFunctionType) = 0;
};

class Visitable {
public:
    virtual void accept(Visitor* visitor) = 0;
};

class Block: public Visitable {
public:
    vector<Statement*> statements;
    Block(vector<Statement*> statements): statements(statements) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class Statement: public Visitable {
};

class Expression: public Statement {
};

class BinaryOp: public Expression {
public:
    Expression* lhs;
    Expression* rhs;

    BinaryOp(Expression* lhs, Expression* rhs): lhs(lhs), rhs(rhs) {}
};

class PlusOp: public BinaryOp {
public:
    PlusOp(Expression* lhs, Expression* rhs): BinaryOp(lhs, rhs) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class MinusOp: public BinaryOp {
public:
    MinusOp(Expression* lhs, Expression* rhs): BinaryOp(lhs, rhs) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class TimesOp: public BinaryOp {
public:
    TimesOp(Expression* lhs, Expression* rhs): BinaryOp(lhs, rhs) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class Negation: public Expression {
public:
    Expression* expression;

    Negation(Expression* expression): expression(expression) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class StringValue: public Expression {
public:
    string value;

    StringValue(string value): value(value) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class IntValue: public Expression {
public:
    int value;
    IntValue(int value): value(value) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class Assignment: public Statement {
public:
    string id;
    Expression* expression;

    Assignment(string id, Expression* expression):
        id(id),
        expression(expression) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class Type: public Expression {
};

class ReferenceId: public Type {
public:
    string id;
    ReferenceId(string id): id(id) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class TypedId {
public:
    string id;
    Type* type;

    TypedId(string id, Type* type): id(id), type(type) {}
};

class ComplexType: public Type {
public:
    vector<TypedId> members;
    ComplexType(vector<TypedId> members): members(members) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class FunctionType: public Type {
public:
    Type* outputType;
    FunctionType(Type* outputType): outputType(outputType) {}
};

class IdFunctionType: public FunctionType {
public:
    string id;
    IdFunctionType(string id, Type* outputType): FunctionType(outputType), id(id) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class ParameterFunctionType: public FunctionType {
public:
    ComplexType* inputType;
    ParameterFunctionType(ComplexType* inputType, Type* outputType):
        FunctionType(outputType),
        inputType(inputType) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class ParameterLessFunctionType: public FunctionType {
public:
    ParameterLessFunctionType(Type* outputType): FunctionType(outputType) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class FunctionCall: public Expression {
public:
    string id;

    FunctionCall(string id): id(id) {}
};

class ParameterLessFunctionCall: public FunctionCall {
public:
    ParameterLessFunctionCall(string id): FunctionCall(id) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class ParametrisedFunctionCall: public FunctionCall {
public:
    Expression* parameter;

    ParametrisedFunctionCall(string id, Expression* parameter): FunctionCall(id), parameter(parameter) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};
class Construct: public Expression {
public:
    vector<Assignment*> assignments;
    Construct(vector<Assignment*> assignments): assignments(assignments) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};
class InfixFunctionCall: public Expression {
public:
    Expression* precedingExpression;
    FunctionCall* functionCall;

    InfixFunctionCall(Expression* precedingExpression, FunctionCall* functionCall):
        precedingExpression(precedingExpression),
        functionCall(functionCall) {}

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};
#endif
