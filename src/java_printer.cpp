#include "java_printer.hpp"
#include <iostream>
#include "printer.hpp"

using namespace std;

void JavaPrinter::print(Block* block) {
    vector<TypedId> in = {TypedId("msg", new ReferenceId("String"))};
    auto printInType = new ComplexType(in);
    auto type = new ParameterFunctionType(printInType, new ReferenceId("Void"));

    Symbol printSymbol;
    printSymbol.javaName = "print";
    printSymbol.type = type;
    scope.symbols["print"] = printSymbol;

    StructEntry printStruct;
    printStruct.type = printInType;
    printStruct.javaName = "PrintStruct";
    structs.push_back(printStruct);

    cout << "class LangD {" << endl;
    cout << "    public static void main(String[] args) {" << endl;
    block->accept(this);
    cout << "    }" << endl;

    for(StructEntry entry: structs) {
        cout << "    private static class " << entry.javaName << " {" << endl;
        for(TypedId member: entry.type->members) {
            cout << "        public " << toJavaType(member.type) << " " << member.id << ";" << endl;
        }
        cout << "    }" << endl;
    }

    cout << "    private static void print(PrintStruct toPrint) {" << endl;
    cout << "        System.out.print(toPrint.msg);" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
}

void JavaPrinter::visit(Block* block) {
    for(auto statement: block->statements) {
        statement->accept(this);
    }
}

string JavaPrinter::getNextId() {
    return "id" + to_string(lastId++);
}

bool isInt(Type* type) {
    ReferenceId* simple = dynamic_cast<ReferenceId*>(type);
    if(!simple) {
        return false;
    }

    return simple->id == "Int";
}

bool isVoid(Type* type) {
    ReferenceId* simple = dynamic_cast<ReferenceId*>(type);
    if(!simple) {
        return false;
    }

    return simple->id == "Void";
}

bool isString(Type* type) {
    ReferenceId* simple = dynamic_cast<ReferenceId*>(type);
    if(!simple) {
        return false;
    }

    return simple->id == "String";
}

string JavaPrinter::toJavaType(Type* type) {
    ReferenceId* simple = dynamic_cast<ReferenceId*>(type);
    if(simple) {
       if(simple->id == "String") {
        return "String";
        }

        if(simple->id == "Int") {
            return "int";
        }

        return "unknown";
    }

    ComplexType* complexType = dynamic_cast<ComplexType*>(type);
    if(complexType) {
        return getOrCreateStruct(complexType);
    }

    return "unknown";
}

string JavaPrinter::getOrCreateStruct(ComplexType* type) {
    for(StructEntry entry: structs) {
        if(*entry.type == *type) {
            return entry.javaName;
        }
    }

    string newName = "Struct" + to_string(lastStructId++);
    StructEntry e;
    e.javaName = newName;
    e.type = type;
    structs.push_back(e);

    return newName;
}

void JavaPrinter::endOperation(Type* outputType) {
    symbolLastExpression.javaName = getNextId();
    symbolLastExpression.type = outputType;
    if(isVoid(outputType)) {
        cout << "        ";
    } else {
        cout << "        " << toJavaType(symbolLastExpression.type) << " " << symbolLastExpression.javaName << " = ";
    }
}

void JavaPrinter::error() {
    symbolLastExpression.javaName = "unkown";
    symbolLastExpression.type = new ReferenceId("unknown");
    cerr << "//Error: ";
}

void JavaPrinter::visit(PlusOp* plusOp) {
    plusOp->lhs->accept(this);
    Symbol lhs = symbolLastExpression;

    plusOp->rhs->accept(this);
    Symbol rhs = symbolLastExpression;


    bool canDoMath =
        (isString(rhs.type) && isString(rhs.type)) ||
        (isInt(lhs.type) && isInt(lhs.type));

    if(!canDoMath) {
        error();
        cerr <<  "Can not add " << lhs.javaName << " and " << rhs.javaName << ". Types are not compatible." << endl;
        return;
    }

    endOperation(lhs.type);
    cout << lhs.javaName << " + " << rhs.javaName << ";" << endl;
}

void JavaPrinter::visit(MinusOp* minusOp) {
    minusOp->lhs->accept(this);
    Symbol lhs = symbolLastExpression;

    minusOp->rhs->accept(this);
    Symbol rhs = symbolLastExpression;


    bool canDoMath =
        (isInt(lhs.type) && isInt(rhs.type));

    if(!canDoMath) {
        error();
        cerr << "Can not subtract from " << lhs.javaName << " the value of " << rhs.javaName << ". Types are not compatible." << endl;
        return;
    }

    endOperation(lhs.type);
    cout << lhs.javaName << " - " << rhs.javaName << ";" << endl;
}

void JavaPrinter::visit(TimesOp* timesOp) {
    timesOp->lhs->accept(this);
    Symbol lhs = symbolLastExpression;

    timesOp->rhs->accept(this);
    Symbol rhs = symbolLastExpression;


    bool canDoMath =
        (isInt(lhs.type) && isInt(rhs.type));

    if(!canDoMath) {
        error();
        cerr << "Can not multiply " << lhs.javaName << " with " << rhs.javaName << ". Types are not compatible." << endl;
        return;
    }

    endOperation(lhs.type);
    cout << lhs.javaName << " * " << rhs.javaName << ";" << endl;
}

void JavaPrinter::visit(Negation* negation) {
    negation->expression->accept(this);
    Symbol expr = symbolLastExpression;

    if(!isInt(symbolLastExpression.type)) {
        error();
        cerr << "Can not negate " << expr.javaName << ". Type is not compatible." << endl;
        return;
    }

    endOperation(expr.type);
    cout << " -" << expr.javaName << ";" << endl;
}

void JavaPrinter::visit(StringValue* negation) {
    endOperation(new ReferenceId("String"));
    cout << negation->value << ";" << endl;
}

void JavaPrinter::visit(IntValue* intValue) {
    endOperation(new ReferenceId("Int"));
    cout << intValue->value << ";" << endl;
}

void JavaPrinter::visit(ReferenceId* evalId) {
    auto symbol = scope.symbols.find(evalId->id);
    if(symbol == scope.symbols.end()) {
        error();
        cerr << evalId->id << " is unknown";
        return;
    }

    symbolLastExpression = symbol->second;
}

void JavaPrinter::visit(ParameterLessFunctionCall* functionCall) {
    auto symbol = scope.symbols.find(functionCall->id);
    if(symbol == scope.symbols.end()) {
        error();
        cerr << functionCall->id << " is unknown";
        return;
    }

    Symbol functionSym = symbol->second;

    ParameterLessFunctionType* functionType = dynamic_cast<ParameterLessFunctionType*>(functionSym.type);
    if(functionType == nullptr) {
        error();
        cerr << functionCall->id << " is not a parameterless function";
        return;
    }

    endOperation(functionType->outputType);
    cout << functionSym.javaName << "();" << endl;
}

void JavaPrinter::visit(ParametrisedFunctionCall* functionCall) {
    auto symbol = scope.symbols.find(functionCall->id);
    if(symbol == scope.symbols.end()) {
        error();
        cerr << functionCall->id << " is unknown";
        return;
    }

    Symbol functionSym = symbol->second;

    ParameterFunctionType* functionType = dynamic_cast<ParameterFunctionType*>(functionSym.type);
    if(functionType == nullptr) {
        error();
        cerr << functionCall->id << " is not a parameterless function";
        return;
    }

    functionCall->parameter->accept(this);
    Symbol input = symbolLastExpression;

    if(!(*input.type == *functionType->inputType)) {
        error();
        cerr << "type of " << input.javaName << " does not match the type of function " << functionCall->id;
        return;
    }

    endOperation(functionType->outputType);
    cout << functionSym.javaName << "(" << input.javaName << ");" << endl;
}

void JavaPrinter::visit(Construct* construct) {
    vector<string> javaNames;
    vector<TypedId> typedIds;

    for(Assignment* assignment: construct->assignments) {
        assignment->expression->accept(this);
        typedIds.push_back(TypedId(assignment->id, symbolLastExpression.type));
        javaNames.push_back(symbolLastExpression.javaName);
    }

    auto complexType = new ComplexType(typedIds);

    string className = getOrCreateStruct(complexType);
    string javaName = getNextId();

    cout << "        " << className << " " << javaName << " = " << " new " << className << "();" << endl;
    for(int i = 0; i < typedIds.size(); i++) {
        cout << "        " << javaName << "." << typedIds[i].id << " = " << javaNames[i] << ";" << endl;
    }

    symbolLastExpression.javaName = javaName;
    symbolLastExpression.type = complexType;
}

void JavaPrinter::visit(Assignment* assignment) {
    if(scope.symbols.count(assignment->id) != 0) {
        error();
        cerr << assignment->id << " is already defined" << endl;
        return;
    }
    assignment->expression->accept(this);
    scope.symbols[assignment->id] = symbolLastExpression;

    symbolLastExpression.javaName = "unknown";
    symbolLastExpression.type = new ReferenceId("unknown");
}

void JavaPrinter::visit(FunctionDeclaration* functionDeclaration) {

}

void JavaPrinter::visit(MemberSelection* memberSelection) {
    memberSelection->previousExpression->accept(this);
    ComplexType* complexType = dynamic_cast<ComplexType*>(symbolLastExpression.type);

    if(complexType == nullptr) {
        error();
        cerr << "The expression does not have a complex type";
        return;
    }

    for(TypedId member: complexType->members) {
        if(member.id == memberSelection->id) {
            string javaName = symbolLastExpression.javaName;
            endOperation(member.type);
            cout << javaName << "." << memberSelection->id << ";" << endl;
            return;
        }
    }

    error();
    cerr << "Complex type has no member " + memberSelection->id;
}

void JavaPrinter::visit(InfixFunctionCall* InfixFunctionCall) {

}
void JavaPrinter::visit(ComplexType* complexType){}
void JavaPrinter::visit(IdFunctionType* IdFunctionType){}
void JavaPrinter::visit(ParameterFunctionType* parameterFunctionType){}
void JavaPrinter::visit(ParameterLessFunctionType* ParameterLessFunctionType){}
