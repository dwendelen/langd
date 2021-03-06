//
// Created by xtrit on 31/07/17.
//

#ifndef LANGD_SEMANTICEXCEPTION_HPP
#define LANGD_SEMANTICEXCEPTION_HPP

#include <exception>
#include <string>

namespace langd {
    namespace semantic {
        class SemanticException : public std::exception {
        public:
            explicit SemanticException(std::string message)
                    : message(message) {}

            std::string getMessage() {
                return message;
            }

        private:
            std::string message;
        };

        class SymbolNotFoundException : public SemanticException {
        public:
            explicit SymbolNotFoundException(std::string symbol)
                    : SemanticException("Could not find " + symbol),
                      symbol(symbol) {}

            std::string getSymbol() {
                return symbol;
            }

        private:
            std::string symbol;
        };

        class VariableAlreadyDefined : public SemanticException {
        public:
            explicit VariableAlreadyDefined(std::string variable)
                    : SemanticException(variable + " is already defined."),
                      variable(variable) {}

            std::string getVariable() {
                return variable;
            }

        private:
            std::string variable;
        };

        class TypeNotFoundException : public SemanticException {
        public:
            explicit TypeNotFoundException(std::string typeName)
                    : SemanticException("Could not find " + typeName),
                      typeName(typeName) {}

            std::string getTypeName() {
                return typeName;
            }

        private:
            std::string typeName;
        };

        class TypeAlreadyDefined : public SemanticException {
        public:
            explicit TypeAlreadyDefined(std::string typeName)
                    : SemanticException(typeName + " is already defined."),
                      typeName(typeName) {}

            std::string getTypeName() {
                return typeName;
            }

        private:
            std::string typeName;
        };
    }
}


#endif //LANGD_SEMANTICEXCEPTION_HPP
