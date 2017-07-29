#ifndef LANGD_AST_H
#define LANGD_AST_H

#include <vector>
#include <string>

using namespace std;

class Expression;
class Block;
class Assignment;
class TypeAssignment;

class PlusOp;
class MinusOp;
class TimesOp;
class Negation;
class StringValue;
class IntValue;

class Tuple;
class TypedId;
class MemberSelection;

class FunctionDefinition;
class FunctionCall;
class InfixFunctionCall;

class Type;
class IdReference;
class TupleType;
class FunctionType;


class ExpressionVisitor;
class TypeVisitor;
class FunctionTypeVisitor;


class TypeVisitor {
public:
    virtual void visit(IdReference* idReference) = 0;
    virtual void visit(TupleType* tupleType) = 0;
    virtual void visit(FunctionType* functionType) = 0;
};

class ExpressionVisitor {
public:
    virtual void visit(Block* block) = 0;
    virtual void visit(Assignment* assignment) = 0;
    virtual void visit(TypeAssignment* typeAssignment) = 0;

    virtual void visit(PlusOp* plusOp) = 0;
    virtual void visit(MinusOp* minusOp) = 0;
    virtual void visit(TimesOp* timesOp) = 0;
    virtual void visit(Negation* negation) = 0;
    virtual void visit(StringValue* negation) = 0;
    virtual void visit(IntValue* intValue) = 0;

    virtual void visit(IdReference* idReference) = 0;
    virtual void visit(Tuple* construct) = 0;
    virtual void visit(MemberSelection* memberSelection) = 0;

    virtual void visit(FunctionDefinition* functionDefinition) = 0;
    virtual void visit(FunctionCall* functionCall) = 0;
    virtual void visit(InfixFunctionCall* infixFunctionCall) = 0;
};

class Expression {
public:
    virtual void accept(ExpressionVisitor* visitor) = 0;
};

class Block: public Expression {
public:
    vector<Expression*> expressions;
    Block(vector<Expression*> expressions): expressions(expressions) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class Assignment: public Expression {
public:
    string id;
    Expression* expression;

    Assignment(string id, Expression* expression):
        id(id),
        expression(expression) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class TypeAssignment: public Expression {
public:
    string id;
    Type* type;

    TypeAssignment(string id, Type* type):
        id(id),
        type(type) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
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

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class MinusOp: public BinaryOp {
public:
    MinusOp(Expression* lhs, Expression* rhs): BinaryOp(lhs, rhs) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class TimesOp: public BinaryOp {
public:
    TimesOp(Expression* lhs, Expression* rhs): BinaryOp(lhs, rhs) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class Negation: public Expression {
public:
    Expression* expression;

    Negation(Expression* expression): expression(expression) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class StringValue: public Expression {
public:
    string value;

    StringValue(string value): value(value) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class IntValue: public Expression {
public:
    int value;
    IntValue(int value): value(value) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};


class Tuple: public Expression {
public:
    vector<Assignment*> assignments;
    Tuple(vector<Assignment*> assignments): assignments(assignments) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class MemberSelection: public Expression {
public:
    Expression* previousExpression;
    string id;

    MemberSelection(Expression* previousExpression, string id):
        previousExpression(previousExpression),
        id(id) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class FunctionDefinition: public Expression {
public:
    TupleType* inputType;
    Expression* body;

    FunctionDefinition(TupleType* inputType, Expression* body):
    inputType(inputType), body(body) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class FunctionCall: public Expression {
public:
    string id;
    Expression* parameter;

    FunctionCall(string id, Expression* parameter): id(id), parameter(parameter) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class InfixFunctionCall: public FunctionCall {
public:
    Expression* precedingExpression;

    InfixFunctionCall(Expression* precedingExpression, string id, Expression* parameter):
        FunctionCall(id, parameter),
        precedingExpression(precedingExpression) {}

    void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
};

class Type {
public:
    virtual void accept(TypeVisitor* visitor) = 0;

    bool virtual operator ==(Type& other) = 0;
};

class IdReference: public Type, public Expression {
public:
    string id;
    IdReference(string id): id(id) {}

    bool virtual operator ==(Type& other) {
        IdReference* idRef = dynamic_cast<IdReference*>(&other);
        if(idRef == nullptr) {
            return false;
        }

        return id == idRef->id;
    }

    virtual void accept(ExpressionVisitor* visitor) {
        visitor->visit(this);
    }
    virtual void accept(TypeVisitor* visitor) {
        visitor->visit(this);
    }
};

class TypedId {
public:
    string id;
    Type* type;

    bool virtual operator ==(TypedId& other) {
        return id == other.id &&
            *type == *other.type;
    }

    TypedId(string id, Type* type): id(id), type(type) {}
};

class TupleType: public Type {
public:
    vector<TypedId> members;
    TupleType(vector<TypedId> members): members(members) {}

    bool virtual operator ==(Type& other) {
        TupleType* tupleType = dynamic_cast<TupleType*>(&other);
        if(tupleType == nullptr) {
            return false;
        }

        if(members.size() != tupleType->members.size()) {
            return false;
        }

        for(int i = 0; i < members.size(); i++) {
            if(!(members[i] == tupleType->members[i])) {
                return false;
            }
        }

        return true;
    }

    virtual void accept(TypeVisitor* visitor) {
        visitor->visit(this);
    }
};

class FunctionType: public Type {
public:
    Type* inputType;
    Type* outputType;
    FunctionType(Type* inputType, Type* outputType): inputType(inputType), outputType(outputType) {}

    virtual void accept(TypeVisitor* visitor) {
        visitor->visit(this);
    }

    bool virtual operator ==(Type& other) {
        FunctionType* functionType = dynamic_cast<FunctionType*>(&other);
        if(functionType == nullptr) {
            return false;
        }

        return
            *inputType == *functionType->inputType &&
            *outputType == *functionType->outputType;
    }

};

#endif
