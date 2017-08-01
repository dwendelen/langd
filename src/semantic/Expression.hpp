//
// Created by xtrit on 31/07/17.
//

#ifndef LANGD_EXPRESSION_HPP
#define LANGD_EXPRESSION_HPP

#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace langd {
    namespace semantic {
        class Type;

        class VoidType;

        class StringType;

        class IntegerType;

        class TupleType;

        class TupleTypeMember;

        class FunctionType;

        class Closure;

        class Expression;

        class Block;

        class Assignment;

        class BinaryOperation;

        class PlusOperation;

        class MinusOperation;

        class TimesOperation;

        class Concatenation;

        class Negation;

        class StringConstant;

        class IntConstant;

        class Tuple;

        class TupleElement;

        class MemberSelection;

        class FunctionCall;

        class FunctionDefinition;

        class Type {
        public:
            virtual ~Type();
        };

        class VoidType : public Type {

        };

        class StringType : public Type {

        };

        class IntegerType : public Type {

        };

        class TupleType : public Type {
        public:
            TupleType(vector<TupleTypeMember> members): members(members) {}

            vector<TupleTypeMember> getMembers() {
                return members;
            }

        private:
            vector<TupleTypeMember> members;
        };

        class TupleTypeMember {
        public:
            TupleTypeMember(string name, Type* type): name(name), type(type) {}
            string getName() {
                return name;
            }
            Type* getType() {
                return type;
            }
        private:
            string name;
            Type *type;
        };



        class FunctionType : public Type {
        public:
            FunctionType(TupleType* inputType, Type* outputType)
                    : inputType(inputType), outputType(outputType) {}
        private:
            TupleType *inputType;
            Type* outputType;
        };




        class Expression {
        public:
            virtual Type *getType() = 0;
        };

        class Block : public Expression {

        public:
            explicit Block(vector<Expression*> expressions) : expressions(expressions) {}

            Type *getType() override {
                return expressions.back()->getType();
            }

        private:
            vector<Expression *> expressions;
        };

        class Assignment : public Expression {
        public:
            Assignment(string name, Expression *expression) : name(name), expression(expression) {}

            string getName() {
                return name;
            }

            Expression * getExpression() const {
                return expression;
            }

            Type *getType() override {
                return expression->getType();
            }
        private:
            string name;
            Expression *expression;
        };

        class VariableReference : public Expression {
        public:
            VariableReference(string name, Type* type): name(name), type(type) {}

            string getName() {
                return name;
            }

            Type *getType() override {
                return type;
            }

        private:
            string name;
            Type *type;
        };

        class BinaryOperation : public Expression {

        public:
            BinaryOperation(Expression *lhs, Expression *rhs) : lhs(lhs), rhs(rhs) {}

            Expression *getLhs() const {
                return lhs;
            }

            Expression *getRhs() const {
                return rhs;
            }
        private:
            Expression *lhs;
            Expression *rhs;
        };

        class PlusOperation : public BinaryOperation {
        public:
            PlusOperation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {

            }

            IntegerType *getType() override {
                return new IntegerType();
            }
        };

        class MinusOperation : public BinaryOperation {
        public:
            MinusOperation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {

            }

            IntegerType *getType() override {
                return new IntegerType();
            }
        };

        class TimesOperation : public BinaryOperation {
        public:
            TimesOperation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {

            }

            IntegerType *getType() override {
                return new IntegerType();
            }
        };

        class Concatenation : public BinaryOperation {
        public:
            Concatenation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {

            }

            StringType *getType() override {
                return new StringType();
            }
        };

        class Negation : public Expression {
        public:
            explicit Negation(Expression *expression) : expression(expression) {

            }

            IntegerType *getType() override {
                return new IntegerType();
            }

        private:
            Expression *expression;
        };

        class StringConstant : public Expression {

        public:
            explicit StringConstant(string value) : value(value) {

            }

            StringType *getType() override {
                return new StringType();
            };
        private:
            string value;
        };

        class IntConstant : public Expression {

        public:
            explicit IntConstant(int value) : value(value) {}

            IntegerType *getType() override {
                return new IntegerType();
            }

        private:
            int value;
        };

        class Tuple : public Expression {
        public:
            Tuple(vector<TupleElement> elements): elements(elements) {}

            TupleType *getType() override {
                return nullptr;
            }

        private:
            vector<TupleElement> elements;
        };

        class TupleElement {
        public:
            TupleElement(string name, Expression* expression) {}
        };

        class MemberSelection : public Expression {
        public:
            MemberSelection(Expression* expression, TupleTypeMember element)
            : element(element){}

            Type *getType() override {
                return element.getType();
            }

        private:
            Tuple *tuple;
            TupleTypeMember  element;
        };

        class Closure {
        private:

        };

        class FunctionCall : public Expression {
        public:
            FunctionCall(string function, Expression* input, FunctionType* type)
                    : function(function), input(input), type(type) {}

            FunctionType *getType() override {
                return type;
            }

        private:
            string function;
            FunctionType* type;
            Expression* input;
        };

        class FunctionDefinition: public Expression {
        public:
            FunctionDefinition(FunctionType* type, Closure* closure, Block* body) :  type(type) {}

            FunctionType *getType() override {
                return type;
            }

        private:
            FunctionType* type;
        };
    }
}

#endif //LANGD_EXPRESSION_HPP
