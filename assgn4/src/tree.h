#ifndef TREE_H
#define TREE_H

#define MAXCHILDREN 100

typedef struct treenode tree;

extern char* nodeNames[70];
extern char* ops[10];

/* tree node - you may want to add more fields */
struct treenode {
      int nodeKind;
      int numChildren;
      int val;
      int scope; // Used for var/id. Index of the scope. This works b/c only global and local.
      int childNumber; //indicates which child (of the parent scope) this node is
      int data_type;
      int sym_type; // Only used by var to distinguish SCALAR vs ARRAY
      int arrayBound; 
      tree *parent;
      tree *children[MAXCHILDREN];
};

tree *ast; /* pointer to AST root */

/* builds sub tree with zeor children  */
tree *MakeTree(int kind);

/* builds sub tree with leaf node */
tree *MakeTreeWithVal(int kind, int val);

void AddChild(tree *parent, tree *child);

void PrintAST(tree *root, int nestLevel);

/* Adds all children of sublist to list */
void FlattenList(tree *list, tree *subList);

int CheckTypeMismatch(tree* node, int desiredDataType);

tree* leftChild(tree* node);
tree* rightChild(tree* node);

/* tree manipulation macros */
/* if you are writing your compiler in C, you would want to have a large collection of these */

#define nextAvailChild(node) node->children[node->numChildren]
#define getChild(node, index) node->children[index]

#endif
