//
// Created by xtrit on 31/07/17.
//

#ifndef LANGD_SYMBOLTABLE_HPP
#define LANGD_SYMBOLTABLE_HPP

#include <string>
#include <map>
#include "Expression.hpp"
#include "Closure.hpp"

namespace langd {
    namespace semantic {
        class SymbolTable;

        class Scope;

        class Variable;

        class Variable {
        public:
            Variable(std::string name, Type* type): name(name), type(type) {}

            std::string getName() {
                return name;
            }

            Type *getType() {
                return type;
            }

        private:
            std::string name;
            Type* type;
        };

        class Scope {
        public:
            virtual Variable* getVariable(std::string name) = 0;
            virtual void registerVariable(Variable *variable) = 0;

            virtual Type* getType(std::string name) = 0;
            virtual void registerType(std::string name, Type* type) = 0;

            virtual Closure* getClosure() = 0;
            virtual Scope *getParent() = 0;
        };

        class SymbolTable {
        public:
            SymbolTable();

            Variable* getVariable(std::string name);
            void registerVariable(Variable *variable);

            Type* getType(std::string name);
            void registerType(std::string name, Type* type);

            void pushScope();
            void popScope();

            Closure *getClosure();

        private:
            Scope* innerScope;
        };



    } // namespace semantic
} // namespace langd

#endif //LANGD_SYMBOLTABLE_HPP
