//
// Created by xtrit on 1/08/17.
//

#ifndef LANGD_EXPRESSIONVISITOR_HPP
#define LANGD_EXPRESSIONVISITOR_HPP

namespace langd {
    namespace semantic {
        class Block;

        class Assignment;

        class VariableReference;

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

        class ExpressionVisitor {
        public:
            virtual void visit(Block *expression) = 0;

            virtual void visit(Assignment *expression) = 0;

            virtual void visit(VariableReference *expression) = 0;

            virtual void visit(PlusOperation *expression) = 0;

            virtual void visit(MinusOperation *expression) = 0;

            virtual void visit(TimesOperation *expression) = 0;

            virtual void visit(Concatenation *expression) = 0;

            virtual void visit(Negation *expression) = 0;

            virtual void visit(StringConstant *expression) = 0;

            virtual void visit(IntConstant *expression) = 0;

            virtual void visit(Tuple *expression) = 0;
            
            virtual void visit(MemberSelection *expression) = 0;

            virtual void visit(FunctionCall *expression) = 0;

            virtual void visit(FunctionDefinition *expression) = 0;
        };
    }
}

#endif //LANGD_EXPRESSIONVISITOR_HPP