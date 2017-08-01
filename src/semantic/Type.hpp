//
// Created by xtrit on 1/08/17.
//

#ifndef LANGD_TYPE_HPP
#define LANGD_TYPE_HPP

#include <string>
#include <vector>

namespace langd {
    namespace semantic {
        class Type;

        class VoidType;

        class StringType;

        class IntegerType;

        class TupleType;

        class TupleTypeMember;

        class FunctionType;


        class Type {
        public:
            virtual bool isAssignableFrom(Type * other) = 0;
            virtual ~Type() = default;
        };


        class VoidType : public Type {
        public:
            bool isAssignableFrom(Type *other) override;
        };


        class StringType : public Type {
        public:
            bool isAssignableFrom(Type *other) override;
        };


        class IntegerType : public Type {
        public:
            bool isAssignableFrom(Type *other) override;
        };


        class TupleType : public Type {
        public:
            TupleType(std::vector<TupleTypeMember> members) : members(members) {}

            std::vector<TupleTypeMember> getMembers() {
                return members;
            }

            bool isAssignableFrom(Type *other) override;

        private:
            std::vector<TupleTypeMember> members;
        };


        class TupleTypeMember {
        public:
            TupleTypeMember(std::string name, Type *type) : name(name), type(type) {}

            std::string getName() {
                return name;
            }

            Type *getType() {
                return type;
            }

            bool isAssignableFrom(TupleTypeMember other);

        private:
            std::string name;
            Type *type;
        };


        class FunctionType : public Type {
        public:
            FunctionType(TupleType *inputType, Type *outputType)
                    : inputType(inputType), outputType(outputType) {}

            bool isAssignableFrom(Type *other) override;

        private:
            TupleType *inputType;
            Type *outputType;
        };

        extern StringType STRING;
        extern VoidType VOID;
        extern IntegerType INTEGER;
    }
}

#endif //LANGD_TYPE_HPP
