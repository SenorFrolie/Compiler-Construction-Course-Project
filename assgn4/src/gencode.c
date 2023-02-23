#include "tree.h"
#include "strtab.h"
#include "gencode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *NodeNames[70] = {"program", "declList", "decl", "varDecl", "varDeclNT", "typeSpecifier", "funDecl",
                       "funcTypeName", "formalDeclList", "formalDeclListNT", "formalDecl", "formalDeclNT",
                       "funBody", "localDeclList", "statementList", "statement", "compoundStmt",
                       "assignStmt", "condStmt", "condStmtNT", "loopStmt", "returnStmt", "returnStmtNT",
                       "expression", "relop", "addExpr", "addop", "term", "mulop", "factor", "funcCallExpr",
                       "funcCallExprNT", "argList", "int", "identifier", "var", "varP", "arrayDecl", "char"};

char *Ops[10] = {"+", "-", "*", "/", "<", "<=", "==", ">=", ">", "!="};

int NUM_TEMPORARY_REGISTERS = 8;
int currentRegister = -1;
int numConditionalStatements = 0;
int numLoopStatements = 0;
int numEqualityRelops = 0;

activationRecord *activationRecordHead = NULL;
activationRecord *activationRecordScope = NULL;
globalVar *globalVariableLinkedList = NULL;

FILE *fp;

/* gencode
 * PARAMETER 1: the current tree* node
 * primary function for printing out the assembly code. Recursively checks each tree* node and generates the appropriate assembly instruction based on the node type
 *********************************************************/
void gencode(tree *node)
{
    char *nodeName = NodeNames[node->nodeKind];

    if (!strcmp(nodeName, "program"))
    {
        // open file
        fp = fopen("./out.asm", "w+");
        // print header
        EmitString(".text\n");
        EmitString(".globl main\n");
        // jump to main, then gracefully exit
        EmitString("\tjal main\n");
        EmitString("\tli $v0, 10\n");
        EmitString("\tsyscall\n");
        // gencode for the rest of the input program
        gencodeAllChildren(node);
        // print the .data section
        EmitString(".data\n");
        EmitDataSection();
        // close the file
        fclose(fp);
    }
    else if (!strcmp(nodeName, "expression"))
    {
        expr(node);
    }
    else if (!strcmp(nodeName, "assignStmt"))
    {
        assgn(node);
    }
    else if (!strcmp(nodeName, "condStmt"))
    {
        cond(node);
    }
    else if (!strcmp(nodeName, "loopStmt"))
    {
        iter(node);
    }
    else if (!strcmp(nodeName, "funDecl"))
    {
        func(node);
    }
    else if (!strcmp(nodeName, "formalDecl"))
    {
        func(node);
    }
    else if (!strcmp(nodeName, "funBody"))
    {
        func(node);
    }
    else if (!strcmp(nodeName, "argList"))
    {
        func(node);
    }
    else if (!strcmp(nodeName, "funcCallExpr"))
    {
        expr(node);
    }
    else if (!strcmp(nodeName, "varDecl"))
    {
        tree *IDNode = rightChild(node);
        char *variableName = GetSymbolID(IDNode->val, IDNode->scope, IDNode->childNumber);
        if (activationRecordScope == NULL)
        {
            addGlobalVariable(node);
        }
        else
        {
            fprintf(fp, "\t#loading var %s into activation record\n", variableName);
            AddLocalVariableToActivationRecord(node);
            decrementSp(4);
        }
    }
    else if (!strcmp(nodeName, "returnStmt"))
    {
        func(node);
    }
    else
    {
        gencodeAllChildren(node);
    }
} // end gencode

/* expr
 * PARAMETER 1: the current tree* node
 * RETURNS: the register number to be output in the fprintf
 *********************************************************/
int expr(tree *node)
{
    //   EmitComment("Process Expr++++++++++");
    int result, t1, t2;
    char *nodeName = NodeNames[node->nodeKind];

    if (!strcmp(nodeName, "mulop") || !strcmp(nodeName, "addop"))
    {
        t1 = expr(leftChild(node));
        t2 = expr(rightChild(node));
        result = t1;
        Emit(op(node), t1, t2, t1);
    }
    else if (!strcmp(nodeName, "int"))
    {
        result = nextReg();
        Emit("li", node->val, -1, result);
    }
    else if (!strcmp(nodeName, "identifier"))
    {
        result = nextReg();
        int offset;

        // search for its offset fro $fp - if an argument
        for (int i = 0; i < activationRecordScope->numFunctionArguments; i++)
        {
            if (activationRecordScope->functionArguments[i] == node->val)
            {
                offset = (activationRecordScope->numFunctionArguments * 4) - ((i - 1) * 4);
                fprintf(fp, "\tlw $t%d, %d($fp)\n", result, offset);
                return result;
            }
        }
        // search for its offset from $sp
        for (int i = 0; i < activationRecordScope->numLocalVariables; i++)
        {
            if (activationRecordScope->localVariables[i] == node->val)
            {
                offset = activationRecordScope->numLocalVariables * 4 - ((i + 1) * 4);
                fprintf(fp, "\tlw $t%d, %d($sp)\n", result, offset);
                return result;
            }
        }
        globalVar *temp = globalVariableLinkedList;
        while (temp != NULL)
        {
            if (temp->node->val == node->val)
            {
                char *variableName = GetSymbolID(node->val, node->scope, node->childNumber);
                fprintf(fp, "\tlw $t%d, %s\n", result, variableName);
                return result;
            }
            temp = temp->next;
        }
    }
    else if (!strcmp(nodeName, "funcCallExpr"))
    {
        tree *IDNode = leftChild(node);
        tree *argList = rightChild(node);
        char *funcName = GetSymbolID(IDNode->val, IDNode->scope, IDNode->childNumber);

        // check if function is output()
        if (!strcmp(funcName, "output"))
        {
            EmitComment("output()");
            t1 = expr(getChild(node, 1));
            EmitString("\tli $v0, 1\n");
            fprintf(fp, "\tadd $a0, $t%d, $zero\n", t1);
            EmitString("\tsyscall\n");
        }
        else
        {
            // push arguments into the activationRecord
            EmitComment("put function arguments into activation record");
            if (argList != NULL)
            {
                gencode(argList);
            }
            // allocate space for return address
            EmitComment("putting return address into the activation record");
            decrementSp(4);
            EmitJump("jal", funcName);
            result = nextReg();
            fprintf(fp, "\tlw $t%d, -4($sp)\n", result);
            EmitComment("clearing return address and function arguments from the activation record");
            incrementSp(4); // clear the return address
            if (argList != NULL)
            {                   // clear the function arguments
                incrementSp(4); // max 1 argument
            }
        }
    }
    else
    {
        result = expr(leftChild(node));
    }
    //   EmitComment("End Process Expr----------");
    return result;
} // end expr

/* assgn
 * PARAMETER 1: the current tree* node
 * DESCRIPTION: determines the assembly instruction when it finds an ASSIGNSTMT tree* node
 *********************************************************/
int assgn(tree *node)
{
    //   EmitComment("Process Assgn++++++++++");
    int result, t1, t2;
    char *nodeName = NodeNames[node->nodeKind];
    if (!strcmp(nodeName, "assignStmt"))
    {
        if (node->numChildren == 2)
        {
            tree *variableNode = getChild(node, 0);
            tree *IDNode = getChild(variableNode, 0);
            char *variableName = GetSymbolID(IDNode->val, IDNode->scope, IDNode->childNumber);
            t2 = expr(rightChild(node));
            int offset;

            // check function arguments
            for (int i = 0; i < activationRecordScope->numFunctionArguments; i++)
            {
                if (activationRecordScope->functionArguments[i] == IDNode->val)
                {
                    offset = (activationRecordScope->numFunctionArguments * 4) - ((i - 1) * 4);
                    fprintf(fp, "\tsw $t%d, %d($fp)\n", t2, offset);
                    return -1;
                }
            }
            // find offset from $sp
            for (int i = 0; i < activationRecordScope->numLocalVariables; i++)
            {
                if (activationRecordScope->localVariables[i] == IDNode->val)
                {
                    offset = activationRecordScope->numLocalVariables * 4 - ((i + 1) * 4);
                    fprintf(fp, "\tsw $t%d, %d($sp)\n", t2, offset);
                    return -1;
                }
            }
            // check global variables
            globalVar *temp = globalVariableLinkedList;
            t1 = nextReg();
            while (temp != NULL)
            {
                tree *variableNode = getChild(node, 0);
                tree *IDNode = getChild(variableNode, 0);
                if (temp->node->val == IDNode->val)
                {
                    char *variableName = GetSymbolID(IDNode->val, IDNode->scope, IDNode->childNumber);
                    fprintf(fp, "\tla $t%d, %s\n", t1, variableName);
                    break;
                }
                temp = temp->next;
            }

            fprintf(fp, "\tsw $t%d, 0($t%d)\n", t2, t1);
        }
        else if (node->numChildren == 1)
        {
            expr(leftChild(node));
        }
    }
    //   EmitComment("End Process Assgn----------");
} // end assgn

/* cond
 * PARAMETER 1: the current tree* node
 * DESCRIPTION:
 *********************************************************/
int cond(tree *node)
{
    //    EmitComment("Process Cond++++++++++");
    int result, t1, t2, t3, t4;
    char *nodeName = NodeNames[node->nodeKind];

    if (!strcmp(nodeName, "condStmt"))
    {
        t1 = cond(getCondNode(node));
        t2 = nextReg();
        newLabel("if");
        Emit("li", 1, -1, t2);
        EmitBranch("beq", t2, t1, "if");

        // else statement code
        if (getElseNode(node) != NULL)
        {
            gencode(getElseNode(node));
        }
        EmitJump("j", "skipIf");

        // then statement code
        EmitLabel("if");
        gencode(getThenNode(node));

        // label to skip the then statment
        EmitLabel("skipIf");
    }
    else if (!strcmp(nodeName, "relop"))
    {
        char *nodeOp = Ops[node->val];
        t1 = expr(leftChild(node));  // LHS
        t2 = expr(rightChild(node)); // RHS
        result = nextReg();

        if (!strcmp(nodeOp, "<"))
        {
            Emit("slt", t1, t2, result);
        }
        else if (!strcmp(nodeOp, ">"))
        {
            Emit("slt", t2, t1, result);
        }
        else if (!strcmp(nodeOp, "=="))
        {
            EmitComment("start of equal condition");
            newLabel("eq");
            // calc cond
            Emit("sub", t1, t2, result);
            fprintf(fp, "\tbeq $zero, $t%d, %s%d\n", result, "eq", numConditionalStatements);
            Emit("li", 0, -1, result);
            EmitJump("j", "neq");

            // if equal
            EmitLabel("eq");
            Emit("li", 1, -1, result);

            // skips past equal instr
            EmitLabel("neq");
            EmitComment("end of equal condition");
        }
    }
    //   EmitComment("End Process Cond----------");
    return result;
} // end cond

/* iter
 * PARAMETER 1: the current tree* node
 * DESCRIPTION:
 *********************************************************/
int iter(tree *node)
{
    //    EmitComment("Process Iter++++++++++");
    int result, t1, t2;
    char *nodeName = NodeNames[node->nodeKind];

    if (!strcmp(nodeName, "loopStmt"))
    {
        newLabel("loop");
        // condition eval
        EmitLabel("loop");
        EmitComment("condition statement");
        t1 = cond(getCondNode(node));

        // condition check
        t2 = nextReg();
        EmitComment("evaluate if condition is true. Branch to skipLoop-label if false");
        Emit("li", 1, -1, t2);
        EmitBranch("bne", t2, t1, "skipLoop");

        // loop body
        EmitComment("loop body");
        gencode(getThenNode(node));
        EmitJump("j", "loop");

        // skip loop
        EmitLabel("skipLoop");
    }
    //   EmitComment("End Process Iter----------");
    return result;
} // end iter

/* func
 * PARAMETER 1: the current tree* node
 * DESCRIPTION:
 *********************************************************/
int func(tree *node)
{
    //    EmitComment("Process func++++++++++");
    int result, t1, t2;
    char *nodeName = NodeNames[node->nodeKind];

    if (!strcmp(nodeName, "funDecl"))
    {
        tree *funcTypeNameNode = getChild(node, 0);
        tree *IDNode = getChild(funcTypeNameNode, 1);
        char *funcName = GetSymbolID(IDNode->val, IDNode->scope, IDNode->childNumber);
        // print label and enter new activationRecord
        EmitLabel(funcName);
        EnterNewActivationRecord(funcName);
        // funbody and formalDeclList

        SaveRegistersToActivationRecord();

        for (int i = 1; i < node->numChildren; i++)
        {
            tree *childNode = getChild(node, i);
            gencode(childNode);
        }

        LoadRegistersFromActivationRecord();
    }
    if (!strcmp(nodeName, "formalDecl"))
    {
        AddFunctionArgumentToActivationRecord(node);
    }
    else if (!strcmp(nodeName, "argList"))
    {
        // functionArguments
        for (int i = 0; i < node->numChildren; i++)
        {
            t1 = expr(getChild(node, i));
            decrementSp(4);
            fprintf(fp, "\tsw $t%d, 0($sp)\n", t1);
        }
    }
    else if (!strcmp(nodeName, "formalDecl"))
    {
        gencodeAllChildren(node);
    }
    else if (!strcmp(nodeName, "funBody"))
    {
        gencodeAllChildren(node);
    }
    else if (!strcmp(nodeName, "returnStmt"))
    {
        tree *returnStmtNode = getChild(node, 0);

        if (returnStmtNode != NULL)
        {
            t1 = expr(returnStmtNode);
            fprintf(fp, "\tsw $t%d, 0($fp)\n", t1);
        }
    }
    //   EmitComment("End Process Func----------");
    return result;
} // end func

/* nextReg
 * RETURNS: the next available register
 *********************************************************/
int nextReg()
{
    currentRegister++;
    return currentRegister % NUM_TEMPORARY_REGISTERS;
}

/* op
 * PARAMETER 1: the current tree* node
 * RETURNS: the MIPS operation to be printed
 *********************************************************/
char *op(tree *node)
{
    char *nodeOp = Ops[node->val];
    if (strcmp(nodeOp, "+") == 0)
        return "add";
    else if (strcmp(nodeOp, "-") == 0)
        return "sub";
    else if (strcmp(nodeOp, "*") == 0)
        return "mul";
    else if (strcmp(nodeOp, "/") == 0)
        return "div";
    else if (strcmp(nodeOp, "<") == 0)
        return "slt";
    else if (strcmp(nodeOp, ">") == 0)
        return "slt";
    else if (strcmp(nodeOp, "==") == 0)
        return "beq";
}

/* gencodeAllChildren
 * PARAMETER 1: the current tree* node
 * recursively calls gencode for all children of the current tree* node
 *********************************************************/
void gencodeAllChildren(tree *node)
{
    for (int i = 0; i < node->numChildren; i++)
    {
        tree *childNode = getChild(node, i);
        gencode(childNode);
    }
}

/* getCondNode
 * PARAMETER 1: the current tree* node
 * DESCRIPTION:
 *********************************************************/
tree *getCondNode(tree *node)
{
    return node->children[0];
}

/* getThenNode
 * PARAMETER 1: the current tree* node
 * DESCRIPTION:
 *********************************************************/
tree *getThenNode(tree *node)
{
    return node->children[1];
}

/* getElseNode
 * PARAMETER 1: the current tree* node
 * DESCRIPTION:
 *********************************************************/
tree *getElseNode(tree *node)
{
    if (node->numChildren > 2)
    {
        return node->children[2];
    }
    return NULL;
}

/* newLabel
 * PARAMETER 1: the label type
 * DESCRIPTION:
 *********************************************************/
char *newLabel(char *type)
{
    if (!strcmp(type, "if"))
    {
        numConditionalStatements++;
    }
    else if (!strcmp(type, "eq"))
    {
        numEqualityRelops++;
    }
    else if (!strcmp(type, "loop"))
    {
        numLoopStatements++;
    }
}
/* incrementSp
 * PARAMETER 1: the number of bytes to increment $sp by
 * DESCRIPTION:
 *********************************************************/
void incrementSp(int bytes)
{
    fprintf(fp, "\taddi $sp, $sp, %d\n", bytes);
}
/* decrementSp
 * PARAMETER 1: the number of bytes to decrement $sp by
 * DESCRIPTION:
 *********************************************************/
void decrementSp(int bytes)
{
    fprintf(fp, "\tsubi $sp, $sp, %d\n", bytes);
}

/* calculateNeededBytes
 * DESCRIPTION: Calculates how many bytes are needed for the offset
 * RETURNS: the number of bytes to offest
 *********************************************************/
int calculateNeededBytes()
{
    int bytesPerWord = 4;
    int returnValue = 0;
    returnValue += bytesPerWord;                           // return value
    returnValue += bytesPerWord;                           // function fp
    returnValue += NUM_TEMPORARY_REGISTERS * bytesPerWord; // save registers
    returnValue += bytesPerWord;                           // globals
    return returnValue;
}

/* CreateNewActivationRecord
 * PARAMETER 1: the name of the activationRecord to be created
 * DESCRIPTION: creates a new activationRecord object for function calls
 *********************************************************/
activationRecord *CreateNewActivationRecord(char *name)
{
    activationRecord *newActivationRecord = (activationRecord *)malloc(sizeof(newActivationRecord));
    newActivationRecord->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(newActivationRecord->name, name);
    newActivationRecord->numLocalVariables = 0;
    newActivationRecord->numFunctionArguments = 0;
    newActivationRecord->next = NULL;
    return newActivationRecord;
}

/* EnterNewActivationRecord
 * PARAMETER 1: the name of the activationRecord to be created
 * DESCRIPTION:
 *********************************************************/
void EnterNewActivationRecord(char *name)
{
    activationRecord *newActivationRecord = CreateNewActivationRecord(name);
    if (activationRecordHead == NULL)
    {
        activationRecordHead = newActivationRecord;
    }
    else
    {
        activationRecord *temp = activationRecordHead;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newActivationRecord;
    }
    activationRecordScope = newActivationRecord;
}

/* AddFunctionArgumentToActivationRecord
 * PARAMETER 1: the current tree* node
 * DESCRIPTION: adds function parameters to the activationRecord
 *********************************************************/
void AddFunctionArgumentToActivationRecord(tree *node)
{
    tree *IDNode = getChild(node, 1);
    activationRecordScope->functionArguments[activationRecordScope->numFunctionArguments] = IDNode->val;
    activationRecordScope->numFunctionArguments++;
}

/* AddLocalVariableToActivationRecord
 * PARAMETER 1: the current tree* node
 * DESCRIPTION: adds variables to the activationRecord
 *********************************************************/
void AddLocalVariableToActivationRecord(tree *node)
{
    tree *IDNode = getChild(node, 1);
    // fprintf(fp, "adding hashkey %d to the local vars activationRecord\n", IDNode->val);
    activationRecordScope->localVariables[activationRecordScope->numLocalVariables] = IDNode->val;
    activationRecordScope->numLocalVariables++;
}

/* SaveRegistersToActivationRecord
 * DESCRIPTION: saves the registers before entering a new function body / scope
 *********************************************************/
void SaveRegistersToActivationRecord()
{
    int neededBytes = calculateNeededBytes();
    int fpOffset = neededBytes - 8;

    EmitComment("allocating space on activation record for everything except local variables");
    decrementSp(neededBytes);
    // save $fp
    fprintf(fp, "\tsw $fp, %d($sp)\n", fpOffset); //$fp points to return value
    EmitComment("saving registers $t0-$t7");
    for (int i = 0; i < NUM_TEMPORARY_REGISTERS; i++)
    {
        int offset = (NUM_TEMPORARY_REGISTERS * 4) - (i * 4);
        fprintf(fp, "\tsw $t%d, %d($sp)\n", i, offset);
    }
    EmitComment("make $fp point to return value in activation record");
    fprintf(fp, "\taddi $fp, $sp, %d\n", neededBytes - 4);
    // save $ra
    EmitComment("save return address into activation record");
    EmitString("\tsw $ra, 4($fp)\n\n");

    EmitComment("Begin Function Body");
}

/* LoadRegistersFromActivationRecord
 * DESCRIPTION: loads the registers after exiting a function body / going up in scope
 *********************************************************/
void LoadRegistersFromActivationRecord()
{
    int neededBytes = calculateNeededBytes();
    int fpOffset = neededBytes - 8;
    // restore the caller regs
    EmitComment("loading registers $t0-$t7 back from activation record");
    for (int i = 0; i < NUM_TEMPORARY_REGISTERS; i++)
    {
        int offset = (NUM_TEMPORARY_REGISTERS * 4) - (i * 4);
        fprintf(fp, "\tlw $t%d, %d($sp)\n", i, offset);
    }
    // store return address in reg
    EmitString("\tlw $ra, 4($fp)\n");
    // restore fp
    EmitComment("reset $fp back to where it was before function call");
    fprintf(fp, "\tlw $fp, %d($sp)\n", fpOffset);
    incrementSp(neededBytes);
    EmitString("\tjr $ra\n");
}

/* addGlobalVariable
 * PARAMETER 1: the current tree* node
 * DESCRIPTION: adds global variable to a linked list of tree* nodes to keep track of global variables for the .data section
 *********************************************************/
void addGlobalVariable(tree *node)
{
    globalVar *newGlobalVar = (globalVar *)malloc(sizeof(globalVar));
    newGlobalVar->node = getChild(node, 1);
    newGlobalVar->next = NULL;
    if (globalVariableLinkedList == NULL)
    {
        globalVariableLinkedList = newGlobalVar;
    }
    else
    {
        globalVar *temp = globalVariableLinkedList;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newGlobalVar;
    }
}

/*******BEGIN EMIT FUNCTIONS*******/
/* Emit
 * DESCRIPTION: used to emit either "li" or most others
 *********************************************************/
void Emit(char *instruction, int reg1, int reg2, int reg3)
{
    if (!strcmp(instruction, "li"))
        fprintf(fp, "\t%s $t%d, %d\n", instruction, reg3, reg1);
    else
        fprintf(fp, "\t%s $t%d, $t%d, $t%d\n", instruction, reg3, reg1, reg2);
}
/* EmitString
 * DESCRIPTION: wrapper for fprintf - used to Emit a string
 *********************************************************/
void EmitString(char *string)
{
    fprintf(fp, "%s", string);
}
/* EmitLabel
 * DESCRIPTION: used to emit labels
 *********************************************************/
void EmitLabel(char *label)
{
    if (!strcmp(label, "if") || !strcmp(label, "skipIf"))
    {
        fprintf(fp, "%s%d:\n", label, numConditionalStatements);
    }
    else if (!strcmp(label, "eq") || !strcmp(label, "neq"))
    {
        fprintf(fp, "%s%d:\n", label, numEqualityRelops);
    }
    else if (!strcmp(label, "loop") || !strcmp(label, "skipLoop"))
    {
        fprintf(fp, "%s%d:\n", label, numLoopStatements);
    }
    else
    {
        fprintf(fp, "%s:\n", label);
    }
}
/* EmitJump
 * DESCRIPTION: used to emit the various jump instructions
 *********************************************************/
void EmitJump(char *instr, char *label)
{
    if (!strcmp(label, "if") || !strcmp(label, "skipIf"))
    {
        fprintf(fp, "\t%s %s%d\n", instr, label, numConditionalStatements);
    }
    else if (!strcmp(label, "eq") || !strcmp(label, "neq"))
    {
        fprintf(fp, "\t%s %s%d\n", instr, label, numEqualityRelops);
    }
    else if (!strcmp(label, "loop") || !strcmp(label, "skipLoop"))
    {
        fprintf(fp, "\t%s %s%d\n", instr, label, numLoopStatements);
    }
    else
    {
        // function names
        fprintf(fp, "\t%s %s\n", instr, label);
    }
}
/* EmitBranch
 * DESCRIPTION: used to emit the various branch instructions
 *********************************************************/
void EmitBranch(char *instr, int reg1, int reg2, char *label)
{
    if (!strcmp(label, "if") || !strcmp(label, "skipIf"))
    {
        fprintf(fp, "\t%s $t%d, $t%d, %s%d\n", instr, reg1, reg2, label, numConditionalStatements);
    }
    else if (!strcmp(label, "eq") || !strcmp(label, "neq"))
    {
        fprintf(fp, "\t%s $t%d, $t%d, %s%d\n", instr, reg1, reg2, label, numEqualityRelops);
    }
    else if (!strcmp(label, "loop") || !strcmp(label, "skipLoop"))
    {
        fprintf(fp, "\t%s $t%d, $t%d, %s%d\n", instr, reg1, reg2, label, numLoopStatements);
    }
}
/* EmitComment
 * DESCRIPTION: used to emit assembly comments
 *********************************************************/
void EmitComment(char *comment)
{
    fprintf(fp, "\t#%s\n", comment);
}

/* EmitDataSection
 * DESCRIPTION: emits the .data section
 *********************************************************/
void EmitDataSection()
{
    globalVar *temp = globalVariableLinkedList;

    while (temp != NULL)
    {
        tree *IDNode = temp->node;
        char *variableName = GetSymbolID(IDNode->val, IDNode->scope, IDNode->childNumber);

        fprintf(fp, "%s:\t.word\n", variableName);
        temp = temp->next;
    }
}