#include "tree.h"
#include "strtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* string values for ast node types, makes tree output more readable */
char *nodeNames[70] = {"program", "declList", "decl", "varDecl", "varDeclNT", "typeSpecifier", "funDecl",
                       "funcTypeName", "formalDeclList", "formalDeclListNT", "formalDecl", "formalDeclNT",
                       "funBody", "localDeclList", "statementList", "statement", "compoundStmt",
                       "assignStmt", "condStmt", "condStmtNT", "loopStmt", "returnStmt", "returnStmtNT",
                       "expression", "relop", "addExpr", "addop", "term", "mulop", "factor", "funcCallExpr",
                       "funcCallExprNT", "argList", "int", "identifier", "var", "varP", "arrayDecl", "char"};

char *typeNames[3] = {"int", "char", "void"};
char *ops[10] = {"+", "-", "*", "/", "<", "<=", "==", ">=", ">", "!="};

/* MakeTree will make a new tree* node and initialize its nodeKind
 * PARAMETER 1: nodeKind to be added
 * RETURNS: pointer to new tree* node
 */
tree *MakeTree(int kind)
{
    tree *this = (tree *)malloc(sizeof(struct treenode));
    this->nodeKind = kind;
    this->numChildren = 0;
    return this;
}

/* MakeTreeWithVal will make a new tree* node and initialize its nodeKind and value
 * PARAMETER 1: nodeKind to be added
 * PARAMETER 2: value to be added
 * RETURNS: pointer to new tree* node
 */
tree *MakeTreeWithVal(int kind, int val)
{
    tree *this = (tree *)malloc(sizeof(tree));
    this->nodeKind = kind;
    this->numChildren = 0;
    this->val = val;
    return this;
}

/* AddChild will add a child to the parent node
 * PARAMETER 1: parent tree* node
 * PARAMETER 2: child tree* node
 */
void AddChild(tree *parent, tree *child)
{
    if (parent->numChildren == MAXCHILDREN)
    {
        printf("Cannot add child to parent node\n");
        exit(1);
    }
    if (child != NULL)
    {
        child->parent = parent;
        parent->children[parent->numChildren] = child;
        parent->numChildren++;
    }
}

/* CheckTypeMismatch will recursively check if there is a type
 * mismatch between LHS and RHS in an assigment statement
 *
 * PARAMETER 1: current tree* node
 * PARAMETER 2: desired data type
 * RETURNS: 0 if there is NOT a type mismatch, 1 if there IS a type mismatch
 */
int CheckTypeMismatch(tree *node, int desiredDataType)
{
    if (!strcmp(nodeNames[node->nodeKind], "factor"))
    {
        if (!strcmp(nodeNames[node->children[0]->nodeKind], "var"))
        {
            tree *temp = node;
            while (temp->numChildren)
            {
                temp = temp->children[0];
            }
            if (desiredDataType == INT_TYPE)
            { // if LHS is INT_TYPE
                if (temp->data_type == VOID_TYPE)
                { // if RHS expression has a VOID_TYPE
                    return 1;
                }
            }
            else if (desiredDataType == CHAR_TYPE)
            { // if RHS is CHAR_TYPE
                if (temp->data_type != CHAR_TYPE)
                { // if RHS expression has INT_TYPE or VOID_TYPE
                    return 1;
                }
            }
            return 0;
        }
        else
        {
            if (!strcmp(nodeNames[node->children[0]->nodeKind], "int"))
            {
                if (desiredDataType == CHAR_TYPE)
                {
                    return 1;
                }
                return 0;
            }
            else if (!strcmp(nodeNames[node->children[0]->nodeKind], "char"))
            {
                return 0;
            }
        }
    }

    for (int i = 0; i < node->numChildren; i++)
    {
        if (CheckTypeMismatch(node->children[i], desiredDataType) == 1)
        {
            return 1;
        }
    }

    return 0;
}

/* PrintAST will print the AST
 */
void PrintAST(tree *node, int nestLevel)
{
    char *nodeName = nodeNames[node->nodeKind];
    if (strcmp(nodeName, "identifier") == 0)
    {
        if (node->val == -1)
            printf("%s,%s\n", nodeName, "undeclared variable");
        else
            printf("%s,%s\n", nodeName, GetSymbolID(node->val, node->scope, node->childNumber));
    }
    else if (strcmp(nodeName, "int") == 0)
    {
        printf("%s,%d\n", nodeName, node->val);
    }
    else if (strcmp(nodeName, "char") == 0)
    {
        printf("%s,%c\n", nodeName, node->val);
    }
    else if (strcmp(nodeName, "typeSpecifier") == 0)
    {
        printf("%s,%s\n", nodeName, typeNames[node->val]);
    }
    else if (strcmp(nodeName, "relop") == 0 || strcmp(nodeName, "mulop") == 0 || strcmp(nodeName, "addop") == 0)
    {
        printf("%s,%s\n", nodeName, ops[node->val]);
    }
    else
    {
        printf("%s\n", nodeName);
    }

    int i, j;

    for (i = 0; i < node->numChildren; i++)
    {
        for (j = 0; j < nestLevel; j++)
            printf("   ");
        PrintAST(getChild(node, i), nestLevel + 1);
    }
}

/* honestly, no clue what this function is supposed to be doing
 * ¯\_(ツ)_/¯
 */
void FlattenList(tree *list, tree *subList)
{
    for (int i = 0; i < subList->numChildren; i++)
    {
        AddChild(list, getChild(subList, i));
    }
}

tree *leftChild(tree *node)
{
    return getChild(node, 0);
}

tree *rightChild(tree *node)
{
    return getChild(node, 1);
}