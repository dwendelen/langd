//
// Created by xtrit on 2/08/17.
//

#ifndef LANGD_CLOSURE_HPP
#define LANGD_CLOSURE_HPP

#include <vector>

namespace langd {
    namespace semantic {
        class Variable;

        class Closure {
        public:
            std::vector<Variable *> getVariables() {
                return variables;
            }

            void addVariable(Variable *variable) {
                variables.push_back(variable);
            }

        private:
            std::vector<Variable *> variables;
        };
    }
}
#endif //LANGD_CLOSURE_HPP
