//
// Created by xtrit on 31/07/17.
//

#ifndef LANGD_ANALYSER_HPP
#define LANGD_ANALYSER_HPP

#include <parser/ast.hpp>
#include <map>
#include "Expression.hpp"
#include "SymbolTable.hpp"

namespace langd {
    namespace semantic {
        class Analyser : public parser::ExpressionVisitor {
        public:
            Analyser();
            Block *analyse(parser::Block *block);

            void visit(parser::Block *block) override;

            void visit(parser::Assignment *assignment) override;

            void visit(parser::TypeAssignment *typeAssignment) override;

            void visit(parser::PlusOp *plusOp) override;

            void visit(parser::MinusOp *minusOp) override;

            void visit(parser::TimesOp *timesOp) override;

            void visit(parser::Negation *negation) override;

            void visit(parser::StringValue *negation) override;

            void visit(parser::IntValue *intValue) override;

            void visit(parser::IdReference *idReference) override;

            void visit(parser::Tuple *construct) override;

            void visit(parser::MemberSelection *memberSelection) override;

            void visit(parser::FunctionDefinition *functionDefinition) override;

            void visit(parser::FunctionCall *functionCall) override;

            void visit(parser::InfixFunctionCall *infixFunctionCall) override;

        private:
            TupleType *mapTuple(parser::TupleType *tupleType);
            Type *mapType(parser::Type* type);

            Expression* lastExpression;
            SymbolTable symbolTable;

        };

    }
}


#endif //LANGD_ANALYSER_HPP
