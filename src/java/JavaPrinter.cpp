//
// Created by xtrit on 1/08/17.
//

#include <iostream>
#include <sstream>
#include "JavaPrinter.hpp"

using namespace std;

namespace langd {
    namespace java {
        JavaPrinter::JavaPrinter() : typeMapper(new TypeMapper) {

        }

        void JavaPrinter::print(semantic::Block *block) {
            cout << "class LangD {" << endl;
            cout << "    public static void main(String[] args) {" << endl;
            block->accept(this);
            cout << "    }" << endl;

            prefix = "            ";
            printFunctions();

            printTupleTypes();
            printFunctionTypes();

            cout << "}" << endl;
        }

        void JavaPrinter::visit(langd::semantic::Block *block) {
            for (auto statement: block->getExpressions()) {
                statement->accept(this);
            }
        }

        void JavaPrinter::visit(langd::semantic::Assignment *assignment) {
            assignment->getExpression()->accept(this);

            cout << prefix << mapType(assignment->getType()) << " " << assignment->getName() << " = ";
            cout << lastValue << ";" << endl;
        }

        void JavaPrinter::visit(langd::semantic::VariableReference *variableReference) {
            print(variableReference->getType(), "id", variableReference->getName());
        }

        void JavaPrinter::visit(langd::semantic::PlusOperation *expression) {
            expression->getLhs()->accept(this);
            string lhs = lastValue;
            expression->getRhs()->accept(this);
            string rhs = lastValue;

            print(expression->getType(), "plus", lhs, " + ", rhs);
        }

        void JavaPrinter::visit(langd::semantic::MinusOperation *expression) {
            expression->getLhs()->accept(this);
            string lhs = lastValue;
            expression->getRhs()->accept(this);
            string rhs = lastValue;

            print(expression->getType(), "minus", lhs, " - ", rhs);
        }

        void JavaPrinter::visit(langd::semantic::TimesOperation *expression) {
            expression->getLhs()->accept(this);
            string lhs = lastValue;
            expression->getRhs()->accept(this);
            string rhs = lastValue;

            print(expression->getType(), "times", lhs, " * ", rhs);
        }

        void JavaPrinter::visit(langd::semantic::Concatenation *expression) {
            expression->getLhs()->accept(this);
            string lhs = lastValue;
            expression->getRhs()->accept(this);
            string rhs = lastValue;

            print(expression->getType(), "concat", lhs, " + ", rhs);
        }

        void JavaPrinter::visit(langd::semantic::Negation *expression) {
            expression->getExpression()->accept(this);

            print(expression->getType(), "neg", " - ", lastValue);
        }

        void JavaPrinter::visit(langd::semantic::StringConstant *expression) {
            print(expression->getType(), "string", expression->getValue());
        }

        void JavaPrinter::visit(langd::semantic::IntConstant *expression) {
            print(expression->getType(), "int", to_string(expression->getValue()));
        }

        void JavaPrinter::visit(langd::semantic::Tuple *expression) {
            auto javaType = mapType(expression->getType());
            auto javaName = resolveName("tuple");

            cout << prefix << javaType << " " << javaName << " = new " << javaType << "();" << endl;
            for (int i = 0; i < expression->getElements().size(); i++) {
                auto element = expression->getElements()[i];

                element.getExpression()->accept(this);
                cout << prefix << javaName << ".e" << i << " = " << lastValue << ";" << endl;
            }

            lastValue = javaName;
        }

        void JavaPrinter::visit(langd::semantic::MemberSelection *expression) {
            expression->getExpression()->accept(this);
            print(expression->getType(), "select", lastValue, ".", expression->getElement().getName());
        }

        void JavaPrinter::visit(langd::semantic::FunctionCall *expression) {
            expression->getInput()->accept(this);
            print(expression->getType(), "result", expression->getFunction(), ".apply(", lastValue, ")");
        }

        void JavaPrinter::visit(langd::semantic::FunctionDefinition *expression) {
            auto functionJavaName = resolveName("Function");

            functions.emplace_back(functionJavaName, expression);

            auto funcName = resolveName("func");
            cout << prefix << functionJavaName << " " << funcName
                 << " = new " << functionJavaName << "();" << endl;

            for (auto variable: expression->getClosure()->getVariables()) {
                cout << prefix << funcName << "." << variable->getName() << " = " << variable->getName() << ";" << endl;
            }

            lastValue = funcName;
        }

        void
        JavaPrinter::print(semantic::Type *type, string name, string code, string code2, string code3, string code4) {
            lastValue = resolveName(name);
            cout << prefix << mapType(type) << " " << lastValue << " = ";
            cout << code << code2 << code3 << code4 << ";" << endl;
        }

        std::string JavaPrinter::mapType(semantic::Type *type) {
            return typeMapper->map(type);
        }

        string JavaPrinter::resolveName(string name) {
            for (int i = 0; true; i++) {
                auto newName = name + "_" + to_string(i);
                if (isNewName(newName)) {
                    names.push_back(newName);
                    return newName;
                }
            }
        }

        bool JavaPrinter::isNewName(std::string newName) {
            for (string name : names) {
                if (name == newName) {
                    return false;
                }
            }
            return true;
        }

        void JavaPrinter::printFunctions() {
            for (auto function: functions) {
                auto javaName = function.first;
                auto definition = function.second;
                auto type = definition->getType();

                cout << "    private static class " << javaName;
                cout << " implements " << typeMapper->map(definition->getType()) << " {" << endl;

                for (auto variable: definition->getClosure()->getVariables()) {
                    cout << "        public " << typeMapper->map(variable->getType()) << " "
                         << variable->getName() << ";" << endl;
                }

                cout << endl;

                cout << "        public " << typeMapper->map(type->getOutputType());
                cout << " apply(";
                cout << typeMapper->map(type->getInputType());
                cout << " _input) {" << endl;

                auto members = type->getInputType()->getMembers();
                for (int i = 0; i < members.size(); i++) {
                    auto member = members[i];

                    cout << "            " << typeMapper->map(member.getType()) << " " << member.getName()
                         << " = _input.e" << to_string(i) << ";" << endl;
                }

                definition->getBody()->accept(this);

                cout << "            return " << lastValue << ";" << endl;
                cout << "        }" << endl;

                cout << "    }" << endl;
            }
        }

        void JavaPrinter::printTupleTypes() {
            for (auto tuple: typeMapper->getTupleTypes()) {
                cout << "    private static class " << tuple.first << " {" << endl;
                auto members = tuple.second->getMembers();
                for (int i = 0; i < members.size(); i++) {
                    cout << "        public " << typeMapper->map(members[i].getType()) << " e" << i << ";" << endl;
                }
                cout << "    }" << endl;
            }
        }

        void JavaPrinter::printFunctionTypes() {
            for (auto function: typeMapper->getFunctionTypes()) {
                auto type = function.second;

                cout << "    private static interface " << function.first << " {" << endl;

                cout << "        public " << typeMapper->map(type->getOutputType());
                cout << " apply(";
                cout << typeMapper->map(type->getInputType());
                cout << " _input);" << endl;

                cout << "    }" << endl;
            }
        }

        TypeMapper::TypeMapper() : compositeTypeMapper(new CompositeTypeMapper) {}

        string TypeMapper::map(semantic::Type *type) {
            type->accept(this);
            return javaType;
        }

        void TypeMapper::visit(semantic::VoidType *type) {
            javaType = "void";
        }

        void TypeMapper::visit(semantic::StringType *type) {
            javaType = "String";
        }

        void TypeMapper::visit(semantic::IntegerType *type) {
            javaType = "int";
        }

        void TypeMapper::visit(semantic::TupleType *type) {
            javaType = compositeTypeMapper->map(type);
        }

        void TypeMapper::visit(semantic::FunctionType *type) {
            javaType = compositeTypeMapper->map(type);
        }


        std::string CompositeTypeMapper::map(semantic::Type *type) {
            javaName = "";
            type->accept(this);
            return javaName;
        }

        void CompositeTypeMapper::visit(semantic::VoidType *type) {
            javaName += "V";
        }

        void CompositeTypeMapper::visit(semantic::StringType *type) {
            javaName += "S";
        }

        void CompositeTypeMapper::visit(semantic::IntegerType *type) {
            javaName += "I";
        }

        void CompositeTypeMapper::visit(semantic::TupleType *type) {
            auto javaNameBeforeThisTuple = javaName;
            javaName = "T" + to_string(type->getMembers().size());
            for (auto member: type->getMembers()) {
                javaName += "_";
                member.getType()->accept(this);
            }

            auto javaNameOfThisTuple = javaName;
            if (tuples.count(javaNameOfThisTuple) == 0) {
                tuples[javaNameOfThisTuple] = type;
            }

            javaName = javaNameBeforeThisTuple + javaNameOfThisTuple;
        }

        void CompositeTypeMapper::visit(semantic::FunctionType *type) {
            auto javaNameBeforeThisFunction = javaName;
            javaName = "F_";
            type->getInputType()->accept(this);
            javaName += "_";
            type->getOutputType()->accept(this);

            auto javaNameOfThisFunction = javaName;
            if (functions.count(javaNameOfThisFunction) == 0) {
                functions[javaNameOfThisFunction] = type;
            }

            javaName = javaNameBeforeThisFunction + javaNameOfThisFunction;
        }
    }
}