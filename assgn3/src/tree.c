#include"tree.h"
#include"strtab.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* string values for ast node types, makes tree output more readable */
char* nodeNames[70] = {"program", "declList", "decl", "varDecl", "varDeclP", "typeSpecifier","funDecl",
                      "funcTypeName", "formalDeclList", "formalDeclListP", "formalDecl", "formalDeclP",
                      "funBody", "localDeclList", "statementList", "statement", "compoundStmt", 
                      "assignStmt", "condStmt", "condStmtP","loopStmt", "returnStmt", "returnStmtP", 
                      "expression", "relop", "addExpr", "addop", "term", "mulop", "factor", "funcCallExpr",
                      "funcCallExprP", "argList", "int", "identifier","var","varP", "arrayDecl", "char"
                      };


char *typeNames[3] = {"int", "char", "void"};
char *ops[10] = {"+", "-", "*", "/", "<", "<=", "==", ">=", ">", "!="};

tree *maketree(int kind) {
      tree *this = (tree *) malloc(sizeof(struct treenode));
      this->nodeKind = kind;
      this->numChildren = 0;
      return this;
}

tree *maketreeWithVal(int kind, int val) {
      tree *this = (tree *) malloc(sizeof(tree));
      this->nodeKind = kind;
      this->numChildren = 0;
      this->val = val;
      //printf("val is %d\n", val);

      return this;
}

void addChild(tree *parent, tree *child) {
      if (parent->numChildren == MAXCHILDREN) {
          //printf("Cannot add child to parent node\n");
          exit(1);
      }
    if(child != NULL){
        child->parent = parent; 
        parent->children[parent->numChildren] = child; 
        parent->numChildren++; 
    }
    else{
        //printf("nosdfsjfadsj\n");
    }
}

int isMismatch(tree* node, int desiredDataType){
    if(!strcmp(nodeNames[node->nodeKind], "factor")){
        if(!strcmp(nodeNames[node->children[0]->nodeKind], "var")){
            tree* temp = node; 
            while(temp->numChildren){
                temp = temp->children[0];
            }
            //printf("temp->datatype = %s\n", typeNames[temp->data_type]);
            if (desiredDataType == INT_TYPE){
                if(temp->data_type == VOID_TYPE){
                    return 1; 
                }
            }
            else if(desiredDataType == CHAR_TYPE){
                if(temp->data_type != CHAR_TYPE){
                    return 1; 
                }
            }

            return 0; 
        }
        else{
            if(!strcmp(nodeNames[node->children[0]->nodeKind], "int")){
                //printf("desired: %d constantType: %d\n", desiredDataType, INT_TYPE);
                if(desiredDataType == CHAR_TYPE){
                    return 1; 
                }

                return 0; 
            }
            else if(!strcmp(nodeNames[node->children[0]->nodeKind], "char")){
                //printf("desired: %d constantType: %d\n", desiredDataType, CHAR_TYPE);
                return 0; 
            }
        }
        //printf("\n");
    }

    for(int i = 0; i < node->numChildren; i++){
        if(isMismatch(node->children[i], desiredDataType) == 1){
            return 1; 
        }
    }

    return 0; 
}

void addExtraData(tree* treeNode, int scope, int order, int dataType, int symType){
    treeNode->scope = scope;
    treeNode->order = order; 
    treeNode->data_type = dataType; 
    treeNode->sym_type = symType; 
} 

void printAst(tree *node, int nestLevel) {
      char* nodeName = nodeNames[node->nodeKind];
      if(strcmp(nodeName,"identifier") == 0){
          if(node->val == -1)
              printf("%s,%s\n", nodeName,"undeclared variable");
          else
              printf("%s,%s\n", nodeName,get_symbol_id(node->val, node->scope, node->order));
              //printf("%s,%s\n", nodeName,node->val);
      }
      else if(strcmp(nodeName,"integer") == 0){
          printf("%s,%d\n", nodeName,node->val);
      }
      else if(strcmp(nodeName,"char") == 0){
          printf("%s,%c\n", nodeName,node->val);
      }
      else if(strcmp(nodeName,"typeSpecifier") == 0){
          printf("%s,%s\n", nodeName,typeNames[node->val]);
      }
      else if(strcmp(nodeName,"relop") == 0 || strcmp(nodeName,"mulop") == 0 || strcmp(nodeName,"addop") == 0){
          printf("%s,%s\n", nodeName,ops[node->val]);
      }
      else{
          printf("%s\n", nodeName);
      }

      int i, j;

      for (i = 0; i < node->numChildren; i++)  {
          for (j = 0; j < nestLevel; j++)
              printf("   ");
          printAst(getChild(node, i), nestLevel + 1);
      }

}

void flattenList(tree *list, tree *subList){
    for(int i=0; i < subList->numChildren; i++){
        addChild(list,getChild(subList,i));
    }
}

