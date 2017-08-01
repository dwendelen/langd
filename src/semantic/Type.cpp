//
// Created by xtrit on 1/08/17.
//

#include "Expression.hpp"
#include "Type.hpp"

namespace langd {
    namespace semantic {
        StringType STRING;
        VoidType VOID;
        IntegerType INTEGER;

        bool VoidType::isAssignableFrom(Type *other) {
            return true;
        }

        bool StringType::isAssignableFrom(Type *other) {
            return dynamic_cast<StringType*> (other) != nullptr;
        }

        bool IntegerType::isAssignableFrom(Type *other) {
            return dynamic_cast<IntegerType*> (other) != nullptr;
        }

        bool TupleType::isAssignableFrom(Type *other) {
            auto otherTuple = dynamic_cast<TupleType*> (other);
            if(otherTuple == nullptr) {
                return false;
            }

            if(members.size() != otherTuple->members.size()) {
                return false;
            }

            for(int i = 0; members.size(); i++) {
                if(!members[i].isAssignableFrom(otherTuple->members[i])) {
                    return false;
                }
            }

            return true;
        }

        bool TupleTypeMember::isAssignableFrom(TupleTypeMember other) {
            if(name != "" && other.name != "") {
                if(name != other.name) {
                    return false;
                }
            }

            return type->isAssignableFrom(other.type);
        }

        bool FunctionType::isAssignableFrom(Type *other) {
            auto otherFunc = dynamic_cast<FunctionType*> (other);
            if(otherFunc == nullptr) {
                return false;
            }
            return
                    inputType->isAssignableFrom(otherFunc->inputType) &&
                    otherFunc->outputType->isAssignableFrom(outputType);
        }


    }
}