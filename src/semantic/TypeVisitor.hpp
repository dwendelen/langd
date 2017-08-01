//
// Created by xtrit on 1/08/17.
//

#ifndef LANGD_TYPEVISITOR_HPP
#define LANGD_TYPEVISITOR_HPP

namespace langd {
    namespace semantic {
        class Type;

        class VoidType;

        class StringType;

        class IntegerType;

        class TupleType;

        class TupleTypeMember;

        class FunctionType;

        class TypeVisitor {
        public:
            virtual void visit(VoidType *type) = 0;

            virtual void visit(StringType *type) = 0;

            virtual void visit(IntegerType *type) = 0;

            virtual void visit(TupleType *type) = 0;

            virtual void visit(FunctionType *type) = 0;
        };
    }
}




#endif //LANGD_TYPEVISITOR_HPP
