//
// Created by xtrit on 1/08/17.
//

#ifndef LANGD_EXPRESSIONVISITOR_HPP
#define LANGD_EXPRESSIONVISITOR_HPP

#include "Expression.hpp"

namespace langd {
    namespace semantic {
        class ExpressionVisitor {
            virtual void accept(Block *expression) = 0;

            virtual void accept(Assignment *expression) = 0;

            virtual void accept(VariableReference *expression) = 0;

            virtual void accept(BinaryOperation *expression) = 0;

            virtual void accept(PlusOperation *expression) = 0;

            virtual void accept(MinusOperation *expression) = 0;

            virtual void accept(TimesOperation *expression) = 0;

            virtual void accept(Concatenation *expression) = 0;

            virtual void accept(Negation *expression) = 0;

            virtual void accept(StringConstant *expression) = 0;

            virtual void accept(IntConstant *expression) = 0;

            virtual void accept(Tuple *expression) = 0;

            virtual void accept(TupleElement *expression) = 0;

            virtual void accept(MemberSelection *expression) = 0;

            virtual void accept(FunctionCall *expression) = 0;

            virtual void accept(FunctionDefinition *expression) = 0;
        };
    }
}

#endif //LANGD_EXPRESSIONVISITOR_HPP