#include<tree.h>
#include<stdio.h>
#include<stdlib.h>

/* string values for ast node types, makes tree output more readable */
char *nodeNames[8] = {"program", "vardecl", "typeSpecfier", "assignStmt", "ifStmt", "exp", "integer", "identifier"};


tree *maketree(int kind) {
  tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->numChildren = 0;
  return this;

}

tree *maketreeWithVal(int kind, int val) {
  tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->numChildren = 0;
  this->val = val;
  return this;

}

void addChild(tree *parent, tree *child) {
  if (parent->numChildren == MAXCHILDREN) {
    printf("Cannot add child to parent node\n");
    exit(1);
  }
  nextAvailChild(parent) = child;
  parent->numChildren++;
}

void printAst(tree *node, int nestLevel) {

  printf("%s\n", nodeNames[node->nodeKind]);

  int i, j;

  for (i = 0; i < node->numChildren; i++)  {
    for (j = 0; j < nestLevel; j++) 
      printf("\t");
    printAst(getChild(node, i), nestLevel + 1);
  }
}



