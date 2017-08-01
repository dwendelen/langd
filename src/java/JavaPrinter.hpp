//
// Created by xtrit on 1/08/17.
//

#ifndef LANGD_JAVAPRINTER_HPP
#define LANGD_JAVAPRINTER_HPP

#include <semantic/ExpressionVisitor.hpp>
#include <semantic/Expression.hpp>

namespace langd {
    namespace java {
        class TypeMapper;

        class JavaPrinter : public semantic::ExpressionVisitor {
        public:
            JavaPrinter();
            void print(semantic::Block *block);

            void visit(semantic::Block *block) override;

            void visit(semantic::Assignment *assignment) override;

            void visit(semantic::VariableReference *variableReference) override;

            void visit(semantic::PlusOperation *expression) override;

            void visit(semantic::MinusOperation *expression) override;

            void visit(semantic::TimesOperation *expression) override;

            void visit(semantic::Concatenation *expression) override;

            void visit(semantic::Negation *expression) override;

            void visit(semantic::StringConstant *expression) override;

            void visit(semantic::IntConstant *expression) override;

            void visit(semantic::Tuple *expression) override;

            void visit(semantic::MemberSelection *expression) override;

            void visit(semantic::FunctionCall *expression) override;

            void visit(semantic::FunctionDefinition *expression) override;

        private:
            TypeMapper *typeMapper;
            std::vector<std::string> names;
            std::string lastValue;

            void print(semantic::Type *type, std::string name, std::string code) {
                print(type, name, code, "");
            }

            void print(semantic::Type *type, std::string name, std::string code, std::string code2) {
                print(type, name, code, code2, "");
            }

            void print(semantic::Type *type, std::string name, std::string code, std::string code2, std::string code3);

            std::string mapType(semantic::Type *type);

            std::string resolveName(std::string name);

            bool isNewName(std::string name);
        };


        class TypeMapper : public semantic::TypeVisitor {
        public:
            std::string map(semantic::Type *type);

            void visit(semantic::VoidType *type) override;

            void visit(semantic::StringType *type) override;

            void visit(semantic::IntegerType *type) override;

            void visit(semantic::TupleType *type) override;

            void visit(semantic::FunctionType *type) override;

        private:
            std::string javaType;
        };
    }
}


#endif //LANGD_JAVAPRINTER_HPP
