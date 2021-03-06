/*
* @name ast.h
* @description Cminus Language AST(Abstract Syntax tree) Node 
* @date 05/26/2020 
* @author Rongqing Li
*/
#ifndef _AST_H
#define _AST_H
#include <iostream>
#include <vector>
#include "visitor.h"
using namespace std;
class AstNode{
public:
	AstNode(){ }
	virtual float accept( Visitor* v ) = 0;
};

class Statement : public AstNode{
public:
	string type;
	string next;
	string code;
	Statement(string type):
		type(type) {}
	Statement(string type, string code):
		type(type), code(code) {}
};

class IfStmt : public Statement{
public:
	Statement* s1;
	Statement* s2;
	Expression* e;
	IfStmt(string type, Expression* e, Statement* s1, Statement* s2 = nullptr):
		Statement(type), s1(s1), s2(s2), e(e) {}

	virtual float accept( Visitor* v) {return v->visit(this);}
};

class WhileStmt : public Statement{
public:
	Statement* s;
	Expression* e;
	WhileStmt(string type, Expression* e, Statement* s):
		Statement(type), s(s), e(e) {}
	
	virtual float accept( Visitor* v) {return v->visit(this);}
};

class ReturnStmt : public Statement{
public:
	Expression* e;
	ReturnStmt(Expression* e, string type="return_stmt"):
		Statement(type), e(e){}
		
	virtual float accept( Visitor* v) {return v->visit(this);}
};

class ExpStmt : public Statement{
public:
	Expression* e;
	ExpStmt(string type, Expression* e):
		Statement(type), e(e) {
			/* Pass code */
			
		}
	virtual float accept( Visitor* v) {return v->visit(this);}
};

class BlockStmt : public Statement{
public:
	vector<Statement*>* stmt_list;
	vector<Statement*>* local_decl_list;
	vector<ReturnStmt*>* ret_decl_list;
	BlockStmt(string type, vector<Statement*>* local_decl_list, vector<Statement*>* stmt_list):
		Statement(type), stmt_list(stmt_list), local_decl_list(local_decl_list) {
			ret_decl_list = new vector<ReturnStmt*>();
			/* Merge(pass) code */
			for(Statement* stmt : *stmt_list){
				code += stmt->code;
				if(ReturnStmt* ret_stmt = dynamic_cast<ReturnStmt*>(stmt)){
					ret_decl_list->push_back(ret_stmt);
				}
			}
		}
	virtual float accept( Visitor* v) {return v->visit(this);}
};

class EmptyStmt : public Statement{
public:
	EmptyStmt(string type):
		Statement(type){ }
	virtual float accept( Visitor* v) {return v->visit(this);}
};

class Expression : public AstNode{
public:
	string type;
	string code;
	string place;
	string True;
	string False;
	union{
		int number;
		float f_number;
	}num;
	string number_type;

	Expression* left;
	Expression* right;
	Expression(){ }
	Expression(string type): type(type) { }
	Expression(string type, Expression* left, Expression* right) : 
					type(type), left(left), right(right) { }
};

class BinOp : public Expression{
public:
	virtual float accept( Visitor* v) {return v->visit(this);}
	
	BinOp( string type, Expression* left, Expression* right ) : 
				Expression(type, left, right){ }
	BinOp( string type) : 
				Expression(type){ }
};

class UnaryOp : public Expression{
	UnaryOp(string type, Expression* left):
			Expression(type, left, NULL){}
	UnaryOp(string type):
			Expression(type){}
	virtual float accept( Visitor* v) {return v->visit(this);}

};

class Int10 : public Expression{
public:		
	int val;
	virtual float accept( Visitor* v){ return v->visit(this); }
	Int10( float val ): Expression("Int10"), val(val){}	
};

class Assign : public Expression{
public:
	virtual float accept(Visitor * v){return v->visit(this);}
	Assign( string type, Expression* left, Expression* right ) : 
			Expression(type, left, right){ }
	Assign(string type):
		Expression(type){

		}
};

class Identifier : public Expression{
public:
	string name;
	virtual float accept( Visitor* v){ return v->visit(this); }
	Identifier(string name,  string type="id") :
			Expression(type), name(name) { }
};

class PrimitiveType : AstNode{
public:
	string type;
	PrimitiveType(string type) : type(type){}
	virtual float accept( Visitor* v) {return v->visit(this);}
};

class Variable : public Statement{
public:
	PrimitiveType* v_type;
	Identifier* id;
	Variable(PrimitiveType* v_type, Identifier* id, string type="id_var"):
				Statement(type), v_type(v_type), id(id){}
	virtual float accept( Visitor* v) {return v->visit(this);}
};

class ArrayVariable : public Variable{
public:
	Int10* size;
	ArrayVariable(PrimitiveType* v_type, Identifier* id, string type="arr_var", Int10* size = NULL):
				Variable(v_type, id, type),  size(size){ }
	virtual float accept( Visitor* v) {return v->visit(this);}
};

class FunctionInvocation : public Expression{
public:
	Identifier* id;
	vector<Expression*>* arg_list;
	FunctionInvocation(Identifier* id, vector<Expression*>* arg_list, string type="func_invoke"):
			Expression(type), id(id), arg_list(arg_list){}
	virtual float accept( Visitor* v){return v->visit(this); }
};

class Function : public Statement{
public:
	PrimitiveType* f_type;
	Identifier* id;
	vector<Variable*>* param_list;
	BlockStmt* block;
	Function(PrimitiveType* f_type, Identifier* id, vector<Variable*>* param_list, BlockStmt* block, string type="func"):
			Statement(type), f_type(f_type), id(id), param_list(param_list), block(block){ }
	virtual float accept( Visitor* v) {return v->visit(this);}
};

class Real10 : public Expression{
public:
	float val;
	virtual float accept(Visitor* v){return v->visit(this);}
	Real10(float val): Expression("Real10"), val(val){}
};

class AccessVar : public AstNode{
public:
	string type;
	Identifier* id;
	Expression* index;
	AccessVar(){}
	AccessVar(Identifier* id, Expression* index = NULL, string type="access_var"):
		id(id), index(index){ }
	virtual float accept(Visitor *v){return v->visit(this);}
};

class SyntaxRoot : public AstNode{
public:
	string type;
	vector<Statement*>* stmt_list;
	SyntaxRoot();
	SyntaxRoot(vector<Statement*>* stmt_list, string type="syntax_root"):
		stmt_list(stmt_list), type(type) {}
	virtual float accept(Visitor* v){return v->visit(this);}
};
#endif
