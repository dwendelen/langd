#ifndef LANGD_AST_H
#define LANGD_AST_H

#include <vector>
#include <string>

using namespace std;

class Program;
class Statement;
class PlusOp;
class MinusOp;
class TimesOp;
class Negation;
class StringValue;
class IntValue;
class EvalId;
class ExpressionAssignment;
class TypeAssignment;
class Type;
class SimpleType;
class ComplexType;
class TypedId;
class FunctionType;
class IdFunctionType;
class ParameterFunctionType;
class ParameterLessFunctionType;

class Visitor {
public:
    virtual void visit(Program* program) = 0;
    virtual void visit(PlusOp* plusOp) = 0;
    virtual void visit(MinusOp* minusOp) = 0;
    virtual void visit(TimesOp* timesOp) = 0;
    virtual void visit(Negation* negation) = 0;
    virtual void visit(StringValue* negation) = 0;
    virtual void visit(IntValue* intValue) = 0;
    virtual void visit(EvalId* evalId) = 0;

    virtual void visit(ExpressionAssignment* expressionAssignment) = 0;
    virtual void visit(TypeAssignment* typeAssignment) = 0;
};

class Visitable {
public:
    virtual void accept(Visitor* visitor) = 0;
};

class Program: public Visitable {
private:
    vector<Statement*> statements;
public:
    void addStatement(Statement* statement) {
        statements.push_back(statement);
    }
    vector<Statement*> getStatements() {
        vector<Statement*> res = this->statements;
        return res;
    }

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class Statement: public Visitable {
};

class Expression: public Statement {
};

class BinaryOp: public Expression {
private:
    Expression* lhs;
    Expression* rhs;
public:
    BinaryOp(Expression* lhs, Expression* rhs): lhs(lhs), rhs(rhs) {}

    Expression* getRhs() {
        return this->rhs;
    }
    Expression* getLhs() {
        return this->lhs;
    }
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
private:
    Expression* expression;
public:
    Negation(Expression* expression): expression(expression) {}

    Expression* getExpression() {
        return expression;
    }

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class StringValue: public Expression {
private:
    string value;
public:
    StringValue(string value): value(value) {}

    string getValue() {
        return value;
    }

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class IntValue: public Expression {
private:
    int value;
public:
    IntValue(int value): value(value) {}

    int getValue() {
        return value;
    }

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class EvalId: public Expression {
private:
    string id;
public:
    EvalId(string id): id(id) {}

    string getId() {
        return id;
    }

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class Assignment: public Statement {
};

class ExpressionAssignment: public Assignment {
    string id;
    Expression* expression;
public:
    ExpressionAssignment(string id, Expression* expression):
        id(id),
        expression(expression) {}

    string getId() {
        return id;
    }

    Expression* getExpression() {
        return expression;
    }

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class TypeAssignment: public Assignment {
    string id;
    Type* type;
public:
    TypeAssignment(string id, Type* type):
        id(id),
        type(type) {}

    string getId() {
        return id;
    }

    Type* getType() {
        return type;
    }

    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

class Type {
public:
    virtual string asString() = 0;
};

class SimpleType: public Type {
private:
    string type;
public:
    SimpleType(string type): type(type) {}

    virtual string asString() {
        return type;
    }
};

class TypedId {
private:
    string id;
    Type* type;
public:
    TypedId(string id, Type* type): id(id), type(type) {}

    string getId() {
        return id;
    }

    Type* getType() {
        return type;
    }
};

class ComplexType: public Type {
private:
    vector<TypedId> members;
public:
    void addMember(TypedId member) {
        members.push_back(member);
    }
    virtual string asString() {
        string result = "(";
        for(TypedId member: members) {
            result += (member.getId() + ": " + member.getType()->asString() + ", ");
        }
        return result.substr(0, result.size()-2) + ")";
    }
};

class FunctionType: public Type {
protected:
    Type* outputType;
protected:
    FunctionType(Type* outputType): outputType(outputType) {}
};

class IdFunctionType: public FunctionType {
private:
    string id;
public:
    IdFunctionType(string id, Type* outputType): FunctionType(outputType), id(id) {}
    virtual string asString() {
        return "(" + id + " => " + outputType->asString() + ")";
    }
};

class ParameterFunctionType: public FunctionType {
private:
    ComplexType* inputType;
public:
    ParameterFunctionType(ComplexType* inputType, Type* outputType):
        FunctionType(outputType),
        inputType(inputType) {}
    virtual string asString() {
        return inputType->asString() + " => " + outputType->asString();
    }
};

class ParameterLessFunctionType: public FunctionType {
public:
    ParameterLessFunctionType(Type* outputType): FunctionType(outputType) {}
    virtual string asString() {
        return "() => " + outputType->asString();
    }
};

#endif
