//
// Created by xtrit on 31/07/17.
//

#include "Analyser.hpp"
#include "Expression.hpp"

namespace langd {
    namespace semantic {

        Block *Analyser::analyse(parser::Block *block) {
            block->accept(this);
            return dynamic_cast<Block *>(lastExpression);
        }

        void Analyser::visit(parser::Block *block) {
            vector<Expression *> expressions;

            for (auto expression: block->expressions) {
                expression->accept(this);
                expressions.push_back(lastExpression);
            }

            lastExpression = new Block(expressions);
        }

        void Analyser::visit(parser::Assignment *assignment) {
            assignment->expression->accept(this);
            symbolTable.registerVariable(new Variable(assignment->id, lastExpression->getType()));
            lastExpression = new Assignment(assignment->id, lastExpression);
        }

        void Analyser::visit(parser::TypeAssignment *typeAssignment) {
            symbolTable.registerType(typeAssignment->id, mapType(typeAssignment->type));
        }

        void Analyser::visit(parser::PlusOp *plusOp) {
            //TODO CHECK TYPES
            plusOp->lhs->accept(this);
            auto lhs = lastExpression;
            plusOp->rhs->accept(this);
            auto rhs = lastExpression;

            lastExpression = new PlusOperation(lhs, rhs);
        }

        void Analyser::visit(parser::MinusOp *minusOp) {
            //TODO CHECK TYPES
            minusOp->lhs->accept(this);
            auto lhs = lastExpression;
            minusOp->rhs->accept(this);
            auto rhs = lastExpression;

            lastExpression = new MinusOperation(lhs, rhs);
        }

        void Analyser::visit(parser::TimesOp *timesOp) {
            //TODO CHECK TYPES
            timesOp->lhs->accept(this);
            auto lhs = lastExpression;
            timesOp->rhs->accept(this);
            auto rhs = lastExpression;

            lastExpression = new TimesOperation(lhs, rhs);
        }

        void Analyser::visit(parser::Negation *negation) {
            //TODO CHECK TYPES
            negation->accept(this);

            lastExpression = new Negation(lastExpression);
        }

        void Analyser::visit(parser::StringValue *stringValue) {
            lastExpression = new StringConstant(stringValue->value);
        }

        void Analyser::visit(parser::IntValue *intValue) {
            lastExpression = new IntConstant(intValue->value);
        }

        void Analyser::visit(parser::IdReference *idReference) {
            auto variable = symbolTable.getVariable(idReference->id);
            lastExpression = new VariableReference(idReference->id, variable->getType());
        }

        void Analyser::visit(parser::Tuple *construct) {
            vector<TupleElement> elements;

            for(auto assignment: construct->assignments) {
                assignment->accept(this);
                elements.emplace_back(assignment->id, lastExpression);
            }

            lastExpression = new Tuple(elements);
        }

        void Analyser::visit(parser::MemberSelection *memberSelection) {
            memberSelection->previousExpression->accept(this);
            auto tupleType = dynamic_cast<TupleType*>( lastExpression->getType());
            if(!tupleType) {
                //TODO EXCEPTION
            }
            for(auto member: tupleType->getMembers() ) {
                if(member.getName() == memberSelection->id) {
                    lastExpression = new MemberSelection(lastExpression, member);
                    return;
                }
            }
            //TODO EXCEPTION
        }

        Block* asBlock(Expression * expression) {
            if(auto block = dynamic_cast<Block*>(expression)) {
                return block;
            }

            return new Block({expression});
        }

        void Analyser::visit(parser::FunctionDefinition *functionDefinition) {
            symbolTable.pushScope();
            TupleType* input = mapTuple(functionDefinition->inputType);

            functionDefinition->body->accept(this);
            auto body = lastExpression;
            symbolTable.popScope();

            lastExpression = new FunctionDefinition(new FunctionType(input, lastExpression->getType()),symbolTable.getClosure(),asBlock(body));
        }

        void Analyser::visit(parser::FunctionCall *functionCall) {
            auto function = symbolTable.getVariable(functionCall->id);
            auto functionType = dynamic_cast<FunctionType*>(function->getType());
            if(!functionType) {
                //TODO exception
            }

            functionCall->parameter->accept(this);
            auto parameterExpression = lastExpression;

            //TODO CHECK TYPE
            lastExpression = new FunctionCall(functionCall->id, parameterExpression, functionType);
        }

        void Analyser::visit(parser::InfixFunctionCall *infixFunctionCall) {

        }

        TupleType* Analyser::mapTuple(parser::TupleType *tupleType) {
            vector<TupleTypeMember> members;
            for(auto member: tupleType->members) {
                members.push_back(TupleTypeMember(member.id, mapType(member.type)));
            }
            return new TupleType(members);
        }

        Type *Analyser::mapType(parser::Type *type) {
            if(auto referencedId = dynamic_cast<parser::IdReference*>(type)) {
                return symbolTable.getType(referencedId->id);
            }

            if(auto tupleType = dynamic_cast<parser::TupleType*>(type)) {
               return mapTuple(tupleType);
            }

            if(auto functionType = dynamic_cast<parser::FunctionType*>(type)) {
                auto inputType = mapType(functionType->inputType);
                auto tupleInputType = dynamic_cast<TupleType*>(inputType);
                if(!tupleInputType) {
                    //TODO THROW EXCEPTION
                }

                return new FunctionType(tupleInputType, mapType(functionType->outputType));
            }

            throw logic_error("Unknown type");
        }
    }
}