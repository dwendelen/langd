//
// Created by xtrit on 31/07/17.
//

#include "SymbolTable.hpp"
#include "SemanticException.hpp"

using namespace std;

namespace langd {
    namespace semantic {
        class NullScope : public Scope {
        public:
            Variable *getVariable(string name) override {
                throw SymbolNotFoundException(name);
            }

            bool hasVariable(string name) override {
                return false;
            }

            void registerVariable(Variable *variable) override {
                //TODO THROW EXCEPTION
            }

            Type *getType(string name) override {
                //TODO THROW EXCEPTION
                return nullptr;
            }

            bool hasType(string name) override {
                return false;
            }

            void registerType(string name, Type *type) override {
                //TODO THROW EXCEPTION
            }

            Scope *getParent() override {
                return this;
            }
        };

        class DefaultScope : public Scope {
        public:
            explicit DefaultScope(Scope *parent) : parent(parent) {}

            Variable *getVariable(string name) override {
                auto variable = variables.find(name);
                if (variable == variables.end()) {
                    return parent->getVariable(name);
                }
                return variable->second;
            }

            bool hasVariable(string name) override {
                unsigned long cnt = variables.count(name);
                if (cnt == 0) {
                    return parent->hasVariable(name);
                }
                return true;
            }

            void registerVariable(Variable *variable) override {
                variables[variable->getName()] = variable;
            }

            Type *getType(string name) override {
                auto type = types.find(name);
                if (type == types.end()) {
                    return parent->getType(name);
                }
                return type->second;
            }

            bool hasType(string name) override {
                unsigned long cnt = types.count(name);
                if (cnt == 0) {
                    return parent->hasType(name);
                }
                return true;
            }

            void registerType(string name, Type *type) override {
                types[name] = type;
            }

            Scope *getParent() override {
                return parent;
            }
        private:
            map<string, Variable *> variables;
            map<string, Type *> types;

            Scope *parent;
        };

        SymbolTable::SymbolTable(): innerScope(new DefaultScope(new NullScope())) {
            registerType("String", new StringType());
            registerType("Int", new IntegerType());
            registerType("Void", new VoidType());
        }

        Variable *SymbolTable::getVariable(string name) {
            return innerScope->getVariable(name);
        }

        bool SymbolTable::hasVariable(string name) {
            return innerScope->hasVariable(name);
        }

        void SymbolTable::registerVariable(Variable *variable) {
            if(hasVariable(variable->getName())) {
                throw VariableAlreadyDefined(variable->getName());
            }

            innerScope->registerVariable(variable);
        }

        Closure *SymbolTable::getClosure() {
            return new Closure();
        }

        Type *SymbolTable::getType(string name) {
            return innerScope->getType(name);
        }

        bool SymbolTable::hasType(string name) {
            return innerScope->hasType(name);
        }

        void SymbolTable::registerType(string name, Type *type) {
            if(hasType(name)) {
                //TODO THROW EXCEPTION
            }
            innerScope->registerType(name, type);
        }

        void SymbolTable::pushScope() {
            innerScope = new DefaultScope(innerScope);
        }

        void SymbolTable::popScope() {
            innerScope = innerScope->getParent();
        }
    }
}