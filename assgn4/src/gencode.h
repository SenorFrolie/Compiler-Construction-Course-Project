#ifndef GENCODE_H
#define GENCODE_H

typedef struct activationRecord{
    char* name; 
    int numFunctionArguments; 
    int numLocalVariables; 
    int functionArguments[50];
    int localVariables[50];
    struct activationRecord* next;
} activationRecord;

typedef struct globalVar{
    tree* node; 
    struct globalVar* next; 
} globalVar;

void gencode(tree* node);
int expr(tree* node);
int assgn(tree* node);
int cond(tree* node);
int iter(tree* node);
int func(tree* node);

int nextReg();
char* op(tree* node);
void gencodeAllChildren(tree* node);
tree* getCondNode(tree* node);
tree* getThenNode(tree* node);
tree* getElseNode(tree* node);
char* newLabel(char* type);
void incrementSp(int bytes);
void decrementSp(int bytes);
int calculateNeededBytes();
activationRecord* CreateNewActivationRecord(char* name);
void EnterNewActivationRecord(char* name);
void AddFunctionArgumentToActivationRecord(tree* node);
void AddLocalVariableToActivationRecord(tree* node);
void SaveRegistersToActivationRecord();
void LoadRegistersFromActivationRecord();
void addGlobalVariable(tree* node);
void EmitDataSection();
void EmitLabel(char* label);
void EmitJump(char* instr, char* label);
void EmitBranch(char* instr, int reg1, int reg2, char* label);
void Emit(char* instruction, int reg1, int reg2, int reg3);
void EmitComment(char* comment);
void EmitString(char* string);
#endif