//
// Created by xtrit on 31/07/17.
//

#ifndef LANGD_EXPRESSION_HPP
#define LANGD_EXPRESSION_HPP

#include <string>
#include <utility>
#include <vector>
#include "Type.hpp"

namespace langd {
    namespace semantic {

        class Closure;

        class Expression;

        class Block;

        class Assignment;

        class VariableReference;

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


        class Expression {
        public:
            virtual Type *getType() = 0;
        };

        class Block : public Expression {

        public:
            explicit Block(std::vector<Expression *> expressions) : expressions(expressions) {}

            Type *getType() override {
                return expressions.back()->getType();
            }

        private:
            std::vector<Expression *> expressions;
        };

        class Assignment : public Expression {
        public:
            Assignment(std::string name, Expression *expression) : name(name), expression(expression) {}

            std::string getName() {
                return name;
            }

            Expression *getExpression() const {
                return expression;
            }

            Type *getType() override {
                return expression->getType();
            }

        private:
            std::string name;
            Expression *expression;
        };

        class VariableReference : public Expression {
        public:
            VariableReference(std::string name, Type *type) : name(name), type(type) {}

            std::string getName() {
                return name;
            }

            Type *getType() override {
                return type;
            }

        private:
            std::string name;
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
                return &INTEGER;
            }
        };

        class MinusOperation : public BinaryOperation {
        public:
            MinusOperation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {

            }

            IntegerType *getType() override {
                return &INTEGER;
            }
        };

        class TimesOperation : public BinaryOperation {
        public:
            TimesOperation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {

            }

            IntegerType *getType() override {
                return &INTEGER;
            }
        };

        class Concatenation : public BinaryOperation {
        public:
            Concatenation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {

            }

            StringType *getType() override {
                return &STRING;
            }
        };

        class Negation : public Expression {
        public:
            explicit Negation(Expression *expression) : expression(expression) {

            }

            IntegerType *getType() override {
                return &INTEGER;
            }

        private:
            Expression *expression;
        };

        class StringConstant : public Expression {

        public:
            explicit StringConstant(std::string value) : value(value) {

            }

            std::string getValue() {
                return value;
            }

            StringType *getType() override {
                return &STRING;
            };
        private:
            std::string value;
        };

        class IntConstant : public Expression {

        public:
            explicit IntConstant(int value) : value(value) {}

            IntegerType *getType() override {
                return &INTEGER;
            }

        private:
            int value;
        };

        class Tuple : public Expression {
        public:
            Tuple(std::vector<TupleElement> elements) : elements(elements) {}

            std::vector<TupleElement> getElements() {
                return elements;
            }

            TupleType *getType() override {
                return nullptr;
            }

        private:
            std::vector<TupleElement> elements;
        };

        class TupleElement {
        public:
            TupleElement(std::string name, Expression *expression) {}
        };

        class MemberSelection : public Expression {
        public:
            MemberSelection(Expression *expression, TupleTypeMember element)
                    : element(element) {}

            Type *getType() override {
                return element.getType();
            }

        private:
            Tuple *tuple;
            TupleTypeMember element;
        };

        class Closure {
        private:

        };

        class FunctionCall : public Expression {
        public:
            FunctionCall(std::string function, Expression *input, FunctionType *type)
                    : function(function), input(input), type(type) {}

            FunctionType *getType() override {
                return type;
            }

        private:
            std::string function;
            FunctionType *type;
            Expression *input;
        };

        class FunctionDefinition : public Expression {
        public:
            FunctionDefinition(FunctionType *type, Closure *closure, Block *body) : type(type) {}

            FunctionType *getType() override {
                return type;
            }

        private:
            FunctionType *type;
        };
    }
}

#endif //LANGD_EXPRESSION_HPP
