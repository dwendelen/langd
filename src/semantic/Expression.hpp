//
// Created by xtrit on 31/07/17.
//

#ifndef LANGD_EXPRESSION_HPP
#define LANGD_EXPRESSION_HPP

#include <string>
#include <utility>
#include <vector>
#include "Type.hpp"
#include "ExpressionVisitor.hpp"

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
            virtual void accept(ExpressionVisitor* visitor) = 0;
        };

        class Block : public Expression {

        public:
            explicit Block(std::vector<Expression *> expressions) : expressions(expressions) {}

            std::vector<Expression *> getExpressions() {
                return expressions;
            }

            Type *getType() override {
                return expressions.back()->getType();
            }

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
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

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
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

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
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

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
            }
        };

        class MinusOperation : public BinaryOperation {
        public:
            MinusOperation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {

            }

            IntegerType *getType() override {
                return &INTEGER;
            }

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
            }
        };

        class TimesOperation : public BinaryOperation {
        public:
            TimesOperation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {

            }

            IntegerType *getType() override {
                return &INTEGER;
            }

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
            }
        };

        class Concatenation : public BinaryOperation {
        public:
            Concatenation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {

            }

            StringType *getType() override {
                return &STRING;
            }

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
            }
        };

        class Negation : public Expression {
        public:
            explicit Negation(Expression *expression) : expression(expression) {

            }

            Expression *getExpression() {
                return expression;
            }

            IntegerType *getType() override {
                return &INTEGER;
            }

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
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

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
            }
        private:
            std::string value;
        };

        class IntConstant : public Expression {

        public:
            explicit IntConstant(int value) : value(value) {}

            int getValue() {
                return value;
            }

            IntegerType *getType() override {
                return &INTEGER;
            }

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
            }

        private:
            int value;
        };

        class TupleElement {
        public:
            TupleElement(std::string name, Expression *expression): name(name), expression(expression) {}
            std::string getName() {
                return name;
            }
            Expression* getExpression() {
                return expression;
            }

        private:
            std::string name;
            Expression *expression;
        };

        class Tuple : public Expression {
        public:
            Tuple(std::vector<TupleElement> elements) : elements(elements) {}

            std::vector<TupleElement> getElements() {
                return elements;
            }

            TupleType *getType() override {
                std::vector<TupleTypeMember> members;
                for (TupleElement element: elements) {
                    members.push_back(TupleTypeMember(element.getName(), element.getExpression()->getType()));
                }
                return new TupleType(members);
            }

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
            }

        private:
            std::vector<TupleElement> elements;
        };

        class MemberSelection : public Expression {
        public:
            MemberSelection(Expression *expression, TupleTypeMember element)
                    : expression(expression), element(element) {}

            Expression *getExpression() {
                return expression;
            }

            TupleTypeMember getElement() {
                return element;
            }

            Type *getType() override {
                return element.getType();
            }

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
            }

        private:
            Expression *expression;
            TupleTypeMember element;
        };

        class Closure {
        private:
        };

        class FunctionCall : public Expression {
        public:
            FunctionCall(std::string function, Expression *input, Type *type)
                    : function(function), input(input), type(type) {}

            Type *getType() override {
                return type;
            }

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
            }

        private:
            std::string function;
            Type *type;
            Expression *input;
        };

        class FunctionDefinition : public Expression {
        public:
            FunctionDefinition(FunctionType *type, Closure *closure, Block *body) : type(type) {}

            FunctionType *getType() override {
                return type;
            }

            void accept(ExpressionVisitor *visitor) override {
                visitor->visit(this);
            }

        private:
            FunctionType *type;
        };
    }
}

#endif //LANGD_EXPRESSION_HPP
