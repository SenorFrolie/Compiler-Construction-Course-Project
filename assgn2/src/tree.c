#include<tree.h>
#include<strtab.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* string values for ast node types, makes tree output more readable */
char* nodeNames[70] = {"program", "declList", "decl", "varDecl", "varDeclNT", "typeSpecifier", "funDecl", 
					   "funcTypeName", "funDeclNT",  "formalDeclList", "fDeclListNT", "formalDecl", "fDeclNT", 
					   "funBody", "localDeclList", "statementList", "statement", "compoundStmt", 
					   "assignStmt", "condStmt", "condStmtNT", "loopStmt", "returnStmt", "retStmtNT", 
				 	   "expression", "relop", "addExpr", "addop", "term", "mulop", "factor", "funcCallExpr", 
						"funcCallNT", "argList", "int", "identifier","var","varNT", "arrayDecl", "char"
						};
enum nodeTypes {PROGRAM, DECLLIST, DECL, VARDECL, VARDECLNT, TYPESPEC, FUNDECL, FUNCTYPENAME, 
				FUNDECLNT, FORMALDECLLIST, FDECLLISTNT, FORMALDECL, FDECLNT,
				FUNBODY, LOCALDECLLIST, STATEMENTLIST, STATEMENT, COMPOUNDSTMT, ASSIGNSTMT, 
				CONDSTMT, CONDSTMTNT, LOOPSTMT, RETURNSTMT, RETSTMTNT,  EXPRESSION, RELOP, 
				ADDEXPR, ADDOP, TERM, MULOP, FACTOR, FUNCCALLEXPR, FUNCCALLNT,  
				ARGLIST, INTEGER, IDENTIFIER, VAR, VARNT,  ARRAYDECL, CHAR, 
				};

enum opType {ADD, SUB, MUL, DIV, LT, LTE, EQ, GTE, GT, NEQ};


tree *maketree(int kind){
	tree *newTreeNode = (tree *) malloc(sizeof(struct treenode));
	newTreeNode->nodeKind = kind;
	newTreeNode->numChildren = 0;
	return newTreeNode;
}

tree *maketreeWithVal(int kind, int val){
	tree *newTreeNode = (tree *) malloc(sizeof(struct treenode));
	newTreeNode->nodeKind = kind;
	newTreeNode->numChildren = 0;
	newTreeNode->val = val;
	return newTreeNode;
}

void addChild(tree *parent, tree *child){
//	if (parent->numChildren == MAXCHILDREN){
//		printf("Cannot add child to parent node\n");
//		exit(1);
//	}
	if (child != NULL){
		child->parent = parent;
		parent->children[parent->numChildren] = child;
		parent->numChildren++;
	}
}

void printAst(tree *node, int nestLevel){
	switch (node->nodeKind){
		case INTEGER: 
			printf("integer, %d\n", node->val);
			break;
		case IDENTIFIER:
			if(node->val == -1){
				printf("<Undeclared var/fun>\n");
			} else {
				printf("identifier, %s\n", strTable[node->val].id);
			}
			break;
		case ADDOP:
			if(node->val == ADD)
				printf("addop, +\n");
			else if(node->val == SUB)
				printf("addop, -\n");
			break;
		case MULOP:
			if(node->val == MUL)
				printf("mulop, *\n");
			else if(node->val == DIV)
				printf("mulop, /\n");
			break;
		case TYPESPEC:
			if(node->val == INT_TYPE)
				printf("typeSpecifier, int\n");
			else if (node->val == CHAR_TYPE)
				printf("typeSpecifier, char\n");
			else if (node->val == VOID_TYPE)
				printf("typeSpecifier, void\n");
			break;
		default:
				printf("%s\n", nodeNames[node->nodeKind]);
		break;
	}
	int i, j;
	for (int i=0; i< node->numChildren; i++){
		for(j=0; j<nestLevel;j++)
			printf("  ");
	printAst(getChild(node,i),nestLevel+1);
	}
}
