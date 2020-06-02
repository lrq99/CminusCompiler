#ifndef _VISITOR_H
#define _VISITOR_H
#include <string>
using namespace std;
enum Error{
	FUNC_REDECL,
	ID_REDECL,
	ARR_REDECL,
	ARR_INDEX_OUT_OF_BOUND,
	ARR_INDEX_ACCESS_TYPE_IMCOMPATIBLE,
	RET_TYPE_IMCOMPATIBLE
};

class AstNode;
class BinOp;
class Expression;
class Int10;

class Compose;
class SymbolTable;
class Variable;
class ArrayVariable;
class Function;
class AccessVar;

class Visitor{
public:
	SymbolTable* sym_table;
	virtual float  visit (Expression* n) = 0;
	virtual float  visit (Int10* n) = 0;
	virtual int    visit (Variable*) = 0;
	virtual int    visit (ArrayVariable*) = 0;
	virtual int    visit (Function*) = 0;
	virtual int    visit (AccessVar*) = 0;
	Visitor(SymbolTable* sym_table):
		sym_table(sym_table) {}
};

class SemanticCheckVisitor : public Visitor{	
public:
	virtual float  visit (Expression* n);
	virtual float  visit (Int10* n);
	virtual int    visit (Variable*);
	virtual int    visit (ArrayVariable*);
	virtual int    visit (Function*);
	virtual int    visit (AccessVar*);
	SemanticCheckVisitor(SymbolTable* sym_table):
		Visitor(sym_table) {}
};

/*class PostfixVisitor : public Visitor{
public:
	virtual float visit ( Expression* n );
	virtual float visit ( Int10* n );
};	
*/
#endif
