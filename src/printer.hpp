#include "ast.hpp"

class Printer: public Visitor {
private:
    string indent = "";
public:
    void print(Program* program);
    void printLine(string s);
    void indented(Visitable* toVisit);

    void visitBinaryOp(string op, BinaryOp* binaryOp);

    virtual void visit(Program* program);
    virtual void visit(PlusOp* plusOp);
    virtual void visit(MinusOp* minusOp);
    virtual void visit(TimesOp* timesOp);
    virtual void visit(Negation* negation);
    virtual void visit(StringValue* negation);
    virtual void visit(IntValue* intValue);
    virtual void visit(EvalId* evalId);

    virtual void visit(ExpressionAssignment* expressionAssignment);
    virtual void visit(TypeAssignment* typeAssignment);
};
