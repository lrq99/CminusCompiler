/*
 * @name driver.cpp
 * @description A driver for parse program
 * @date 03/31/2020
 * @author Lrq
 */

#include <iostream>
#include <unistd.h>
#include "bison.tab.hxx"
void usage();
extern FILE* yyin;
using namespace std;
void preOrder(Expression* root){
	if(root == NULL) return;
	std::cout << root->place << ' ';
	preOrder(root->left);
	preOrder(root->right);
}

int main(int argc, char** argv)
{	
    /* Get command from shell */
	int eval = 0;
	int postfix = 0;
	char opt;

	/*while((opt = getopt(argc, argv, "hep")) != -1){
		switch (opt){
			case 'h':
				usage();
			break;
			case 'e':
				eval = 1;
			break;
			case 'p':
				postfix = 1;
			break;
		default:
				usage();
		}
	}
	*/
	/* No argument */
	//if(eval == 0 && postfix == 0) usage();
	
	/* Parse */
	if(argc > 1)
		if(!(yyin = fopen(argv[1], "r"))){
			std::cerr << "Read file error." << std::endl;
			return -1;
		}
	Expression *root;
	Statement* stmt;
	yy::parser parser(&root, &stmt);
	parser.parse();
	
	//cout << ((If*)stmt)->s2->type << endl;
	cout << ((Identifier*)(((If*)stmt)->s1))->name << ' ' << ((Identifier*)(stmt->e->right))->name << endl;
	//preOrder(root);
	//std::cout << root->type << ' ' << root->left->type << ' ' << root->right->left->type << std::endl;
	/*if(root == NULL){
		std::cerr << "error!!" << std::endl;
		exit(0);
	}*/
	
	/* Analyse Expression */
	/*if(eval == 1){
		EvalVisitor* ev = new EvalVisitor();
		double v = root->accept(ev);
		std::cout << "By Visitor Pattern: Value=" << v << std::endl;
	}
	if(postfix == 1){
		PostfixVisitor* pv = new PostfixVisitor();
		std::cout << "Postfix Expression:";
		root->accept(pv);
		std::cout << std::endl;	
	}*/

	return 0;
}

void usage(){
	printf("Usage: target [-hei]\n");
	printf("   -h   print this message\n");
	printf("   -e   print evaluation of target expression\n");
	printf("   -p   print postfix of target expression\n");
	exit(1);
}
