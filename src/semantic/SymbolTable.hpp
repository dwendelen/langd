//
// Created by xtrit on 31/07/17.
//

#ifndef LANGD_SYMBOLTABLE_HPP
#define LANGD_SYMBOLTABLE_HPP

#include <string>
#include <map>
#include "Expression.hpp"

namespace langd {
    namespace semantic {
        class SymbolTable;

        class Scope;

        class Variable;

        class Variable {
        public:
            Variable(string name, Type* type): name(name), type(type) {}

            string getName() {
                return name;
            }

            Type *getType() {
                return type;
            }

        private:
            string name;
            Type* type;
        };

        class Scope {
        public:
            virtual Variable* getVariable(string name) = 0;
            virtual bool hasVariable(string name) = 0;
            virtual void registerVariable(Variable *variable) = 0;

            virtual Type* getType(string name) = 0;
            virtual bool hasType(string name) = 0;
            virtual void registerType(string name, Type* type) = 0;

            virtual Scope *getParent() = 0;
        };

        class SymbolTable {
        public:
            SymbolTable();

            Variable* getVariable(string name);
            bool hasVariable(string name);
            void registerVariable(Variable *variable);

            Type* getType(string name);
            bool hasType(string name);
            void registerType(string name, Type* type);

            void pushScope();
            void popScope();

            Closure *getClosure();

        private:
            Scope* innerScope;
        };



    } // namespace semantic
} // namespace langd

#endif //LANGD_SYMBOLTABLE_HPP
