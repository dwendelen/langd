#include "java_printer.hpp"
#include <iostream>
#include "printer.hpp"

using namespace std;

namespace langd {
    namespace java {


        using namespace parser;

        bool isVoid(Type *type);

        void JavaPrinter::print(Block *block) {
            auto printInType = new TupleType({TypedId("msg", STRING)});
            auto type = new FunctionType(printInType, VOID);

            Symbol printSymbol;
            printSymbol.javaName = "print";
            printSymbol.type = type;
            scope->symbols["print"] = printSymbol;

            StructEntry printStruct;
            printStruct.type = printInType;
            printStruct.javaName = "PrintStruct";
            structs.push_back(printStruct);

            *output << "class LangD {" << endl;
            *output << "    public static void main(String[] args) {" << endl;
            block->accept(this);
            *output << "    }" << endl;


            for (FunctionEntry func: functions) {
                string type = toJavaType(func.outputType);
                *output << "    private static " << type << " " << func.javaName << "(";
                *output << toJavaType(func.functionDefinition->inputType) << " input";
                *output << ") {" << endl;
                *output << func.javaBody->str();
                *output << "    }" << endl;
            }

            for (StructEntry entry: structs) {
                *output << "    private static class " << entry.javaName << " {" << endl;
                for (TypedId member: entry.type->members) {
                    *output << "        public " << toJavaType(member.type) << " " << member.id << ";" << endl;
                }
                *output << "    }" << endl;
            }

            *output << "    private static void print(PrintStruct toPrint) {" << endl;
            *output << "        System.out.print(toPrint.msg);" << endl;
            *output << "    }" << endl;
            *output << "}" << endl;
        }

        Symbol getSymbol(Scope *scope, string name) {
            auto symbol = scope->symbols.find(name);
            if (symbol == scope->symbols.end()) {
                if (scope->parent == nullptr) {
                    throw UnknownSymbol();
                }
                return getSymbol(scope->parent, name);
            }

            return symbol->second;
        }

        Symbol JavaPrinter::getSymbol(string name) {
            return java::getSymbol(scope, name);
        }

        bool hasSymbol(Scope *scope, string name) {
            int cnt = scope->symbols.count(name);
            if (cnt != 0) {
                return true;
            }

            if (scope->parent == nullptr) {
                return false;
            }

            return hasSymbol(scope->parent, name);
        }

        bool JavaPrinter::hasSymbol(string name) {
            return java::hasSymbol(scope, name);
        }

        string JavaPrinter::getOrCreateStruct(TupleType *type) {
            for (StructEntry entry: structs) {
                if (*entry.type == *type) {
                    return entry.javaName;
                }
            }

            string newName = "Struct" + to_string(lastId++);
            StructEntry e;
            e.javaName = newName;
            e.type = type;
            structs.push_back(e);

            return newName;
        }

        bool JavaPrinter::isInt(Type *type) {
            return *type == *INT;
        }

        bool JavaPrinter::isVoid(Type *type) {
            return *type == *VOID;
        }

        bool JavaPrinter::isString(Type *type) {
            return *type == *STRING;
        }

        string JavaPrinter::toJavaType(Type *type) {
            IdReference *simple = dynamic_cast<IdReference *>(type);
            if (simple) {
                if (simple->id == "String") {
                    return "String";
                }

                if (simple->id == "Int") {
                    return "int";
                }

                return "unknown";
            }

            TupleType *complexType = dynamic_cast<TupleType *>(type);
            if (complexType) {
                return getOrCreateStruct(complexType);
            }

            return "unknown";
        }

        string JavaPrinter::getNextId(string prefix) {
            return prefix + to_string(lastId++);
        }

        void JavaPrinter::endOperation(Type *outputType) {
            symbolLastExpression.javaName = getNextId("id");
            symbolLastExpression.type = outputType;
            if (isVoid(outputType)) {
                *output << "        ";
            } else {
                *output << "        " << toJavaType(symbolLastExpression.type) << " " << symbolLastExpression.javaName
                        << " = ";
            }
        }

        void JavaPrinter::error() {
            symbolLastExpression.javaName = "unkown";
            symbolLastExpression.type = new IdReference("unknown");
            cerr << "//Error: ";
        }

        bool JavaPrinter::printFunctionArguments(TupleType *type) {
            *output << toJavaType(type) << " " << "input";
            //*output << toJavaType(member.type) << " " << member.id;

            return true;
        }


        void JavaPrinter::visit(Block *block) {
            for (auto statement: block->expressions) {
                statement->accept(this);
            }
        }

        void JavaPrinter::visit(Assignment *assignment) {
            if (hasSymbol(assignment->id)) {
                error();
                cerr << assignment->id << " is already defined" << endl;
                return;
            }

            assignment->expression->accept(this);
            scope->symbols[assignment->id] = symbolLastExpression;
        }

        void JavaPrinter::visit(TypeAssignment *assignment) {
            if (hasSymbol(assignment->id)) {
                error();
                cerr << assignment->id << " is already defined" << endl;
                return;
            }

            //assignment->expression->accept(this);
            scope->symbols[assignment->id] = symbolLastExpression;
        }


        void JavaPrinter::visit(PlusOp *plusOp) {
            plusOp->lhs->accept(this);
            Symbol lhs = symbolLastExpression;

            plusOp->rhs->accept(this);
            Symbol rhs = symbolLastExpression;

            bool canDoMath =
                    (isString(rhs.type) && isString(rhs.type)) ||
                    (isInt(lhs.type) && isInt(lhs.type));

            if (!canDoMath) {
                error();
                cerr << "Can not add " << lhs.javaName << " and " << rhs.javaName << ". Types are not compatible."
                     << endl;
                return;
            }

            endOperation(lhs.type);
            *output << lhs.javaName << " + " << rhs.javaName << ";" << endl;
        }

        void JavaPrinter::visit(MinusOp *minusOp) {
            minusOp->lhs->accept(this);
            Symbol lhs = symbolLastExpression;

            minusOp->rhs->accept(this);
            Symbol rhs = symbolLastExpression;

            bool canDoMath =
                    (isInt(lhs.type) && isInt(rhs.type));

            if (!canDoMath) {
                error();
                cerr << "Can not subtract from " << lhs.javaName << " the value of " << rhs.javaName
                     << ". Types are not compatible." << endl;
                return;
            }

            endOperation(lhs.type);
            *output << lhs.javaName << " - " << rhs.javaName << ";" << endl;
        }

        void JavaPrinter::visit(TimesOp *timesOp) {
            timesOp->lhs->accept(this);
            Symbol lhs = symbolLastExpression;

            timesOp->rhs->accept(this);
            Symbol rhs = symbolLastExpression;

            bool canDoMath =
                    (isInt(lhs.type) && isInt(rhs.type));

            if (!canDoMath) {
                error();
                cerr << "Can not multiply " << lhs.javaName << " with " << rhs.javaName << ". Types are not compatible."
                     << endl;
                return;
            }

            endOperation(lhs.type);
            *output << lhs.javaName << " * " << rhs.javaName << ";" << endl;
        }

        void JavaPrinter::visit(Negation *negation) {
            negation->expression->accept(this);
            Symbol expr = symbolLastExpression;

            if (!isInt(symbolLastExpression.type)) {
                error();
                cerr << "Can not negate " << expr.javaName << ". Type is not compatible." << endl;
                return;
            }

            endOperation(expr.type);
            *output << " -" << expr.javaName << ";" << endl;
        }

        void JavaPrinter::visit(StringValue *negation) {
            endOperation(STRING);
            *output << negation->value << ";" << endl;
        }

        void JavaPrinter::visit(IntValue *intValue) {
            endOperation(INT);
            *output << intValue->value << ";" << endl;
        }

        void JavaPrinter::visit(Tuple *tuple) {
            vector<string> javaNames;
            vector<TypedId> typedIds;

            for (Assignment *assignment: tuple->assignments) {
                assignment->expression->accept(this);
                typedIds.push_back(TypedId(assignment->id, symbolLastExpression.type));
                javaNames.push_back(symbolLastExpression.javaName);
            }

            auto tupleType = new TupleType(typedIds);

            string className = getOrCreateStruct(tupleType);
            string javaName = getNextId("id");

            *output << "        " << className << " " << javaName << " = " << " new " << className << "();" << endl;
            for (int i = 0; i < typedIds.size(); i++) {
                *output << "        " << javaName << "." << typedIds[i].id << " = " << javaNames[i] << ";" << endl;
            }

            symbolLastExpression.javaName = javaName;
            symbolLastExpression.type = tupleType;
        }

        void JavaPrinter::visit(MemberSelection *memberSelection) {
            memberSelection->previousExpression->accept(this);
            TupleType *complexType = dynamic_cast<TupleType *>(symbolLastExpression.type);

            if (complexType == nullptr) {
                error();
                cerr << "The expression does not have a complex type";
                return;
            }

            for (TypedId member: complexType->members) {
                if (member.id == memberSelection->id) {
                    string javaName = symbolLastExpression.javaName;
                    endOperation(member.type);
                    *output << javaName << "." << memberSelection->id << ";" << endl;
                    return;
                }
            }

            error();
            cerr << "Complex type has no member " + memberSelection->id;
        }

        void JavaPrinter::visit(FunctionDefinition *functionDefinition) {
            FunctionEntry entry;
            entry.functionDefinition = functionDefinition;
            entry.javaBody = new stringbuf();
            entry.javaName = getNextId("func");


            Scope *outerScope = scope;
            scope = new Scope;
            scope->parent = outerScope;

            ostream *oldOutput = output;
            output = new ostream(entry.javaBody);

            scope->symbols["input"].javaName = "input";
            scope->symbols["input"].type = functionDefinition->inputType;

            for (TypedId member: functionDefinition->inputType->members) {
                this->visit(new Assignment(member.id, new MemberSelection(new IdReference("input"), member.id)));
            }

            functionDefinition->body->accept(this);

            if (!isVoid(symbolLastExpression.type)) {
                *output << "        return " << symbolLastExpression.javaName << ";" << endl;
            }

            Type *outputType = symbolLastExpression.type;
            entry.outputType = outputType;
            functions.push_back(entry);

            scope = outerScope;
            output = oldOutput;

            symbolLastExpression.javaName = entry.javaName;
            symbolLastExpression.type = new FunctionType(functionDefinition->inputType, entry.outputType);
        }

        void JavaPrinter::visit(FunctionCall *functionCall) {
            try {
                Symbol functionSym = getSymbol(functionCall->id);

                FunctionType *functionType = dynamic_cast<FunctionType *>(functionSym.type);
                if (functionType == nullptr) {
                    error();
                    cerr << functionCall->id << " is not a function";
                    return;
                }

                functionCall->parameter->accept(this);
                Symbol input = symbolLastExpression;

                if (!(*input.type == *functionType->inputType)) {
                    error();
                    cerr << "type of " << input.javaName << " does not match the type of function " << functionCall->id
                         << endl;
                    return;
                }

                endOperation(functionType->outputType);
                *output << functionSym.javaName << "(" << input.javaName << ");" << endl;
            } catch (UnknownSymbol e) {
                error();
                cerr << functionCall->id << " is unknown";
                return;
            }
        }

        void JavaPrinter::visit(InfixFunctionCall *infixFunctionCall) {
            infixFunctionCall->precedingExpression->accept(this);
            infixFunctionCall->parameter->accept(this);

            *output << "//Here comes the infix" << endl;
        }

        void JavaPrinter::visit(IdReference *idReference) {
            try {
                symbolLastExpression = getSymbol(idReference->id);
            } catch (UnknownSymbol e) {
                error();
                cerr << idReference->id << " is unknown";
            }
        }
    }
}