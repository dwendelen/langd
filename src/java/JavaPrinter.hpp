//
// Created by xtrit on 1/08/17.
//

#ifndef LANGD_JAVAPRINTER_HPP
#define LANGD_JAVAPRINTER_HPP

#include <map>
#include <list>
#include <sstream>

#include <semantic/ExpressionVisitor.hpp>
#include <semantic/Expression.hpp>

namespace langd {
    namespace java {
        class TypeMapper;
        class CompositeTypeMapper;

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

            std::list<std::pair<std::string, semantic::FunctionDefinition*>> functions;

            std::string prefix = "        ";

            void print(semantic::Type *type, std::string name, std::string code) {
                print(type, name, code, "");
            }

            void print(semantic::Type *type, std::string name, std::string code, std::string code2) {
                print(type, name, code, code2, "");
            }

            void print(semantic::Type *type, std::string name, std::string code, std::string code2, std::string code3) {
                print(type, name, code, code2, code3, "");
            }

            void print(semantic::Type *type, std::string name, std::string code, std::string code2, std::string code3, std::string code4);

            std::string mapType(semantic::Type *type);

            std::string resolveName(std::string name);

            bool isNewName(std::string name);

            void printTupleTypes();

            void printFunctionTypes();

            void printFunctions();
        };

        class CompositeTypeMapper : public semantic::TypeVisitor {
        public:
            std::string map(semantic::Type *type);

            void visit(semantic::VoidType *type) override;

            void visit(semantic::StringType *type) override;

            void visit(semantic::IntegerType *type) override;

            void visit(semantic::TupleType *type) override;

            void visit(semantic::FunctionType *type) override;

            std::map<std::string, semantic::TupleType *> getTuples() {
                return tuples;
            }

            std::map<std::string, semantic::FunctionType *> getFunctions() {
                return functions;
            }

        private:
            std::string javaName;
            std::map<std::string, semantic::TupleType*> tuples;
            std::map<std::string, semantic::FunctionType*> functions;
        };

        class TypeMapper : public semantic::TypeVisitor {
        public:
            TypeMapper();

            std::string map(semantic::Type *type);

            void visit(semantic::VoidType *type) override;

            void visit(semantic::StringType *type) override;

            void visit(semantic::IntegerType *type) override;

            void visit(semantic::TupleType *type) override;

            void visit(semantic::FunctionType *type) override;

            std::map<std::string, semantic::TupleType *> getTupleTypes() {
                return compositeTypeMapper->getTuples();
            }

            std::map<std::string, semantic::FunctionType *> getFunctionTypes() {
                return compositeTypeMapper->getFunctions();
            }
        private:
            std::string javaType;
            CompositeTypeMapper* compositeTypeMapper;
        };


    }
}


#endif //LANGD_JAVAPRINTER_HPP
