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

            void registerVariable(Variable *variable) override {
                throw logic_error("You can not register a variable");
            }

            Type *getType(string name) override {
                throw TypeNotFoundException(name);
            }

            void registerType(string name, Type *type) override {
                throw logic_error("You can not register a type");
            }

            Closure* getClosure() override {
                throw logic_error("Null scope has no closure");
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
                    auto var = parent->getVariable(name);
                    closure->addVariable(var);
                    return var;
                }
                return variable->second;
            }

            void registerVariable(Variable *variable) override {
                if(hasVariable(variable->getName())) {
                    throw VariableAlreadyDefined(variable->getName());
                }
                variables[variable->getName()] = variable;
            }

            Type *getType(string name) override {
                auto type = types.find(name);
                if (type == types.end()) {
                    return parent->getType(name);
                }
                return type->second;
            }

            void registerType(string name, Type *type) override {
                if(hasType(name)) {
                    throw TypeAlreadyDefined(name);
                }
                types[name] = type;
            }

            Closure* getClosure() override {
                return closure;
            }


            Scope *getParent() override {
                return parent;
            }
        private:
            map<string, Variable *> variables;
            map<string, Type *> types;

            Scope *parent;
            Closure *closure = new Closure();

            bool hasVariable(string name) {
                return variables.count(name) != 0;
            }

            bool hasType(string name) {
                return types.count(name) != 0;
            }
        };

        SymbolTable::SymbolTable(): innerScope(new DefaultScope(new NullScope())) {
            registerType("String", &STRING);
            registerType("Int", &INTEGER);
            registerType("Void", &VOID);
        }

        Variable *SymbolTable::getVariable(string name) {
            return innerScope->getVariable(name);
        }

        void SymbolTable::registerVariable(Variable *variable) {
            innerScope->registerVariable(variable);
        }

        Closure *SymbolTable::getClosure() {
            return innerScope->getClosure();
        }

        Type *SymbolTable::getType(string name) {
            return innerScope->getType(name);
        }

        void SymbolTable::registerType(string name, Type *type) {
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