#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20130304

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 1 "src/parser.y"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../src/tree.h"
#include"../src/strtab.h"

int yydebug=1; 
extern int yylineno;
/* nodeTypes refer to different types of internal and external nodes that can be part of the abstract syntax tree.*/
enum nodeTypes {PROGRAM, DECLLIST, DECL, VARDECL, VARDECLNT, TYPESPEC, FUNDECL, 
                FUNCTYPENAME, FORMALDECLLIST, FORMALDECLLISTNT, FORMALDECL, FORMALDECLNT, 
                FUNBODY, LOCALDECLLIST, STATEMENTLIST, STATEMENT, COMPOUNDSTMT, ASSIGNSTMT,
                CONDSTMT, CONDSTMTNT, LOOPSTMT, RETURNSTMT, RETURNSTMTNT, EXPRESSION, RELOP,
                ADDEXPR, ADDOP, TERM, MULOP, FACTOR, FUNCCALLEXPR, FUNCCALLEXPRNT,
                ARGLIST, INTEGER, IDENTIFIER, VAR, VARNT, ARRAYDECL, CHAR,
                };
enum opType {ADD, SUB, MUL, DIV, LT, LTE, EQ, GTE, GT, NEQ};

/* NOTE: mC has two kinds of scopes for variables : local and global. Variables declared outside any
function are considered globals, whereas variables (and parameters) declared inside a function foo are local to foo. You should update the scope variable whenever you are inside a production that matches function definition (funDecl production). The rationale is that you are entering that function, so all variables, arrays, and other functions should be within this scope. You should pass this variable whenever you are calling the STInsert or STLookup functions. This variable should be updated to scope = "" to indicate global scope whenever funDecl finishes. Treat these hints as helpful directions only. You may implement all of the functions as you like and not adhere to my instructions. As long as the directory structure is correct and the file names are correct, we are okay with it. */
int scope = 0;
#line 26 "src/parser.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union
{
    int value;
    struct treenode *node;
    char *strval;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 56 "obj/y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define ID 257
#define INTCONST 258
#define CHARCONST 259
#define STRCONST 260
#define KWD_IF 261
#define KWD_ELSE 262
#define KWD_WHILE 263
#define KWD_INT 264
#define KWD_STRING 265
#define KWD_CHAR 266
#define KWD_RETURN 267
#define KWD_VOID 268
#define OPER_ADD 269
#define OPER_SUB 270
#define OPER_MUL 271
#define OPER_DIV 272
#define OPER_LT 273
#define OPER_GT 274
#define OPER_GTE 275
#define OPER_LTE 276
#define OPER_EQ 277
#define OPER_NEQ 278
#define OPER_ASGN 279
#define OPER_AT 280
#define OPER_INC 281
#define OPER_DEC 282
#define OPER_AND 283
#define OPER_OR 284
#define OPER_NOT 285
#define OPER_MOD 286
#define LSQ_BRKT 287
#define RSQ_BRKT 288
#define LCRLY_BRKT 289
#define RCRLY_BRKT 290
#define LPAREN 291
#define RPAREN 292
#define COMMA 293
#define SEMICLN 294
#define ILLEGAL_TOKEN 295
#define ERROR 296
#define funDeclP 297
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
   35,    0,    1,    1,    2,    2,    3,    4,    4,    6,
    6,    6,   36,    5,   37,    5,   11,    7,    8,    8,
    9,   10,   10,   12,   13,   13,   14,   14,   15,   15,
   15,   15,   15,   16,   17,   17,   18,   19,   19,   20,
   21,   22,   22,   23,   24,   24,   25,   25,   26,   26,
   26,   26,   26,   26,   27,   27,   28,   28,   29,   29,
   30,   30,   31,   31,   31,   31,   31,   31,   32,   33,
   33,   34,   34,
};
static const short yylen[] = {                            2,
    0,    2,    1,    2,    1,    1,    3,    4,    1,    1,
    1,    1,    0,    6,    0,    5,    2,    2,    0,    2,
    3,    0,    2,    4,    0,    2,    0,    2,    1,    1,
    1,    1,    1,    3,    4,    2,    6,    0,    2,    5,
    2,    1,    2,    2,    0,    3,    1,    3,    1,    1,
    1,    1,    1,    1,    1,    3,    1,    1,    1,    3,
    1,    1,    3,    1,    1,    1,    1,    1,    3,    2,
    1,    1,    3,
};
static const short yydefred[] = {                         1,
    0,    0,   10,   11,   12,    0,    3,    5,    6,    0,
    0,    4,    0,    0,    0,    9,    7,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   18,    0,
   16,    8,    0,   21,   14,   20,    0,    0,    0,   23,
   26,    0,    0,   66,   67,   68,    0,    0,    0,    0,
    0,    0,    0,   29,   30,   31,   32,   33,    0,    0,
    0,    0,   59,   65,    0,    0,   44,    0,    0,   42,
   41,   64,    0,    0,    0,   24,   28,    0,   50,   51,
   52,   49,   53,   54,   36,    0,   57,   58,    0,   61,
   62,    0,    0,   71,    0,   69,    0,    0,    0,   43,
   34,   63,    0,    0,    0,   60,   46,   70,    0,    0,
    0,   35,    0,    0,   40,    0,   37,   39,
};
static const short yydgoto[] = {                          1,
    6,    7,    8,   17,    9,   10,   22,   29,   23,   34,
   11,   31,   39,   52,   53,   54,   55,   56,  117,   57,
   58,   71,   72,   67,   60,   86,   61,   89,   62,   92,
   63,   64,   96,   97,    2,   18,   19,
};
static const short yysindex[] = {                         0,
    0, -125,    0,    0,    0, -125,    0,    0,    0, -244,
 -274,    0, -285,    0, -239,    0,    0, -125, -260, -248,
 -212, -245, -240, -234, -180, -199, -234, -125,    0, -125,
    0,    0, -170,    0,    0,    0, -125, -138, -174,    0,
    0, -285, -275,    0,    0,    0, -151, -147, -253, -174,
 -231, -145, -174,    0,    0,    0,    0,    0, -119,  -60,
 -206, -203,    0,    0, -231, -235,    0, -231, -231,    0,
    0,    0,  -54, -131, -140,    0,    0, -231,    0,    0,
    0,    0,    0,    0,    0, -231,    0,    0, -231,    0,
    0, -231, -196,    0, -165,    0, -217,  -32,  -25,    0,
    0,    0,  -47, -206, -203,    0,    0,    0, -231, -174,
 -174,    0, -165, -100,    0, -174,    0,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0,    0,  164,    0,    0,    0,    0,
    0,    0, -126, -222,    0,    0,    0,    0,    0,    0,
    0,    0, -123,    0,    0, -214,    0,    0,    0, -224,
    0,    0,    0,    0,    0,    0, -224,    0, -124,    0,
    0,    0, -172,    0,    0,    0,    0,    0,    0, -124,
    0,    0, -124,    0,    0,    0,    0,    0,  -94,    0,
  -88, -146,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -198,    0,    0,    0,    0,    0,
    0,    0,    0,  -82, -120,    0,    0,    0,    0,    0,
    0,    0, -167, -209,    0,    0,    0,    0,
};
static const short yygindex[] = {                         0,
    0,  161,  -22,    0,    0,  133,  143,    0,    0,    0,
    0,  170,  162,    9,  -20,    0,    0,    0,    0,    0,
    0,    0,  -39,    0,  -48,    0,  -55,    0,  109,    0,
  110,    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 267
static const short yytable[] = {                         59,
   73,   15,   75,   43,   44,   45,   46,   37,   16,   93,
   59,   65,   13,   59,   37,   66,   14,   95,   20,   98,
   99,   43,   44,   45,   46,   43,   44,   45,   46,  103,
  104,   24,   25,   25,   25,   25,   25,   51,   25,   25,
   70,   13,   25,   13,   26,   13,   27,   38,   38,   38,
   38,   38,   28,   38,   30,   51,   94,   38,   74,   51,
  113,   77,   87,   88,   25,   25,   25,   90,   91,   15,
   59,   59,   87,   88,  108,  109,   59,   22,   22,   38,
   38,   38,   43,   44,   45,   46,   47,   33,   48,  114,
  115,  107,   49,   72,   72,  118,   45,   45,   45,   45,
   45,   45,   45,   45,   45,   45,   45,   79,   80,   81,
   82,   83,   84,   32,   50,   45,   51,   40,   42,   45,
   45,   45,   55,   55,   73,   73,   55,   55,   55,   55,
   55,   55,   79,   80,   81,   82,   83,   84,    3,   68,
    4,   55,    5,   69,   76,   55,   55,   55,   56,   56,
   21,  102,   56,   56,   56,   56,   56,   56,  101,   78,
   21,  116,   38,    2,   17,   27,   12,   56,   19,   38,
   36,   56,   56,   56,   64,   64,   64,   64,   64,   64,
   64,   64,   64,   64,   47,   47,   47,   47,   47,   47,
   48,   48,   48,   48,   48,   48,   35,  105,   41,   64,
    0,  106,    0,   47,   47,   47,    0,    0,    0,   48,
   48,   48,   79,   80,   81,   82,   83,   84,   79,   80,
   81,   82,   83,   84,    0,   79,   80,   81,   82,   83,
   84,    0,    0,   85,    0,    0,    0,    0,    0,  100,
   79,   80,   81,   82,   83,   84,  112,   79,   80,   81,
   82,   83,   84,    0,    0,    0,    0,    0,    0,  110,
    0,    0,    0,    0,    0,    0,  111,
};
static const short yycheck[] = {                         39,
   49,  287,   51,  257,  258,  259,  260,   30,  294,   65,
   50,  287,  257,   53,   37,  291,  291,   66,  258,   68,
   69,  257,  258,  259,  260,  257,  258,  259,  260,   78,
   86,  292,  257,  258,  259,  260,  261,  291,  263,  288,
  294,  264,  267,  266,  257,  268,  292,  257,  258,  259,
  260,  261,  293,  263,  289,  291,  292,  267,   50,  291,
  109,   53,  269,  270,  289,  290,  291,  271,  272,  292,
  110,  111,  269,  270,  292,  293,  116,  292,  293,  289,
  290,  291,  257,  258,  259,  260,  261,  287,  263,  110,
  111,  288,  267,  292,  293,  116,  269,  270,  271,  272,
  273,  274,  275,  276,  277,  278,  279,  273,  274,  275,
  276,  277,  278,  294,  289,  288,  291,  288,  257,  292,
  293,  294,  269,  270,  292,  293,  273,  274,  275,  276,
  277,  278,  273,  274,  275,  276,  277,  278,  264,  291,
  266,  288,  268,  291,  290,  292,  293,  294,  269,  270,
   18,  292,  273,  274,  275,  276,  277,  278,  290,  279,
   28,  262,   30,    0,  291,  290,    6,  288,  292,   37,
   28,  292,  293,  294,  269,  270,  271,  272,  273,  274,
  275,  276,  277,  278,  273,  274,  275,  276,  277,  278,
  273,  274,  275,  276,  277,  278,   27,   89,   37,  294,
   -1,   92,   -1,  292,  293,  294,   -1,   -1,   -1,  292,
  293,  294,  273,  274,  275,  276,  277,  278,  273,  274,
  275,  276,  277,  278,   -1,  273,  274,  275,  276,  277,
  278,   -1,   -1,  294,   -1,   -1,   -1,   -1,   -1,  294,
  273,  274,  275,  276,  277,  278,  294,  273,  274,  275,
  276,  277,  278,   -1,   -1,   -1,   -1,   -1,   -1,  292,
   -1,   -1,   -1,   -1,   -1,   -1,  292,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 297
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"ID","INTCONST","CHARCONST",
"STRCONST","KWD_IF","KWD_ELSE","KWD_WHILE","KWD_INT","KWD_STRING","KWD_CHAR",
"KWD_RETURN","KWD_VOID","OPER_ADD","OPER_SUB","OPER_MUL","OPER_DIV","OPER_LT",
"OPER_GT","OPER_GTE","OPER_LTE","OPER_EQ","OPER_NEQ","OPER_ASGN","OPER_AT",
"OPER_INC","OPER_DEC","OPER_AND","OPER_OR","OPER_NOT","OPER_MOD","LSQ_BRKT",
"RSQ_BRKT","LCRLY_BRKT","RCRLY_BRKT","LPAREN","RPAREN","COMMA","SEMICLN",
"ILLEGAL_TOKEN","ERROR","funDeclP",
};
static const char *yyrule[] = {
"$accept : program",
"$$1 :",
"program : $$1 declList",
"declList : decl",
"declList : declList decl",
"decl : varDecl",
"decl : funDecl",
"varDecl : typeSpecifier ID varDeclNT",
"varDeclNT : LSQ_BRKT INTCONST RSQ_BRKT SEMICLN",
"varDeclNT : SEMICLN",
"typeSpecifier : KWD_INT",
"typeSpecifier : KWD_CHAR",
"typeSpecifier : KWD_VOID",
"$$2 :",
"funDecl : funcTypeName LPAREN $$2 formalDeclList RPAREN funBody",
"$$3 :",
"funDecl : funcTypeName LPAREN $$3 RPAREN funBody",
"funcTypeName : typeSpecifier ID",
"formalDeclList : formalDecl formalDeclListNT",
"formalDeclListNT :",
"formalDeclListNT : COMMA formalDeclList",
"formalDecl : typeSpecifier ID formalDeclNT",
"formalDeclNT :",
"formalDeclNT : LSQ_BRKT RSQ_BRKT",
"funBody : LCRLY_BRKT localDeclList statementList RCRLY_BRKT",
"localDeclList :",
"localDeclList : varDecl localDeclList",
"statementList :",
"statementList : statement statementList",
"statement : compoundStmt",
"statement : assignStmt",
"statement : condStmt",
"statement : loopStmt",
"statement : returnStmt",
"compoundStmt : LCRLY_BRKT statementList RCRLY_BRKT",
"assignStmt : var OPER_ASGN expression SEMICLN",
"assignStmt : expression SEMICLN",
"condStmt : KWD_IF LPAREN expression RPAREN statement condStmtNT",
"condStmtNT :",
"condStmtNT : KWD_ELSE statement",
"loopStmt : KWD_WHILE LPAREN expression RPAREN statement",
"returnStmt : KWD_RETURN returnStmtNT",
"returnStmtNT : SEMICLN",
"returnStmtNT : expression SEMICLN",
"var : ID varNT",
"varNT :",
"varNT : LSQ_BRKT addExpr RSQ_BRKT",
"expression : addExpr",
"expression : expression relOp addExpr",
"relOp : OPER_LTE",
"relOp : OPER_LT",
"relOp : OPER_GT",
"relOp : OPER_GTE",
"relOp : OPER_EQ",
"relOp : OPER_NEQ",
"addExpr : term",
"addExpr : addExpr addOp term",
"addOp : OPER_ADD",
"addOp : OPER_SUB",
"term : factor",
"term : term mulOp factor",
"mulOp : OPER_MUL",
"mulOp : OPER_DIV",
"factor : LPAREN expression RPAREN",
"factor : var",
"factor : funcCallExpr",
"factor : INTCONST",
"factor : CHARCONST",
"factor : STRCONST",
"funcCallExpr : ID LPAREN funcCallExprNT",
"funcCallExprNT : argList RPAREN",
"funcCallExprNT : RPAREN",
"argList : expression",
"argList : argList COMMA expression",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 756 "src/parser.y"


int yywarning(char * msg){
    printf("warning: line %d: %s\n", yylineno, msg);
    return 0;
}

int yyerror(char * msg){
    printf("error: line %d: %s\n", yylineno, msg);
    return 0;
}
#line 410 "obj/y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 83 "src/parser.y"
	{ NewScope(); scope++; }
break;
case 2:
#line 84 "src/parser.y"
	{
 /*                   printf("top of program\n");*/
                    tree* progNode = MakeTree(PROGRAM);
                    AddChild(progNode, yystack.l_mark[0].node);
                    ast = progNode;
                 }
break;
case 3:
#line 92 "src/parser.y"
	{
                    tree* declListNode = MakeTree(DECLLIST);
                    AddChild(declListNode, yystack.l_mark[0].node);
                    yyval.node = declListNode;
                 }
break;
case 4:
#line 98 "src/parser.y"
	{
                    tree* declListNode = MakeTree(DECLLIST);
                    AddChild(declListNode, yystack.l_mark[-1].node);
                    AddChild(declListNode, yystack.l_mark[0].node);
                    yyval.node = declListNode;
                 }
break;
case 5:
#line 106 "src/parser.y"
	{
                    tree* declNode = MakeTree(DECL);
                    AddChild(declNode, yystack.l_mark[0].node);
                    yyval.node = declNode;
                 }
break;
case 6:
#line 112 "src/parser.y"
	{
                    tree* declNode = MakeTree(DECL);
                    AddChild(declNode, yystack.l_mark[0].node);
                    yyval.node = declNode;
                 }
break;
case 7:
#line 119 "src/parser.y"
	{
                    tree* varDeclNode = MakeTree(VARDECL);
                    AddChild(varDeclNode, yystack.l_mark[-2].node);
                    int exists = -1; 
                    int key; 
                    if(yystack.l_mark[0].node == NULL){
                        key = STInsert(yystack.l_mark[-1].strval, varDeclNode->children[0]->val, SCALAR, &scope, &exists);
                    }
                    else{
                        key = STInsert(yystack.l_mark[-1].strval, varDeclNode->children[0]->val, ARRAY, &scope, &exists);
                        currentScope->strTable[key]->arrayBound = yystack.l_mark[0].node->val; 
                    }
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = MakeTreeWithVal(IDENTIFIER, key);
                    childNode->data_type   = currentScope->strTable[key]->data_type;
                    childNode->sym_type    = currentScope->strTable[key]->symbol_type;
                    childNode->scope       = currentScope->strTable[key]->scope;
                    childNode->childNumber = currentScope->strTable[key]->childNumber;
                    AddChild(varDeclNode, childNode);
                    AddChild(varDeclNode, yystack.l_mark[0].node);
                    yyval.node = varDeclNode;
                 }
break;
case 8:
#line 145 "src/parser.y"
	{
                    if(yystack.l_mark[-2].value == 0){
                        yyerror("Array variable declared with size of zero."); 
                    }
                    yyval.node = MakeTreeWithVal(INTEGER, yystack.l_mark[-2].value);
                 }
break;
case 9:
#line 152 "src/parser.y"
	{
                    yyval.node = NULL; 
                 }
break;
case 10:
#line 157 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(TYPESPEC, INT_TYPE);
                 }
break;
case 11:
#line 161 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(TYPESPEC, CHAR_TYPE);
                 }
break;
case 12:
#line 165 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(TYPESPEC, VOID_TYPE);
                 }
break;
case 13:
#line 169 "src/parser.y"
	{ NewScope(); scope++; }
break;
case 14:
#line 170 "src/parser.y"
	{
                    UpScope();
                    scope--; 

                    ConnectParams(yystack.l_mark[-5].node->children[1]->val); 
                    tree* funDeclNode = MakeTree(FUNDECL);
                    AddChild(funDeclNode, yystack.l_mark[-5].node);
                    AddChild(funDeclNode, yystack.l_mark[-2].node);
                    AddChild(funDeclNode, yystack.l_mark[0].node);
                    yyval.node = funDeclNode; 
                 }
break;
case 15:
#line 181 "src/parser.y"
	{ NewScope(); scope++; }
break;
case 16:
#line 182 "src/parser.y"
	{
                    UpScope();
                    scope--; 

                    tree* funDeclNode = MakeTree(FUNDECL);
                    AddChild(funDeclNode, yystack.l_mark[-4].node);
                    AddChild(funDeclNode, yystack.l_mark[0].node);
                    yyval.node = funDeclNode; 
                 }
break;
case 17:
#line 193 "src/parser.y"
	{
                    tree* funcTypeNameNode = MakeTree(FUNCTYPENAME);
                    AddChild(funcTypeNameNode, yystack.l_mark[-1].node);
                    int exists = -1; 
                    int key = STInsert(yystack.l_mark[0].strval, funcTypeNameNode->children[0]->val, FUNCTION, &scope, &exists); /*global*/
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = MakeTreeWithVal(IDENTIFIER, key);
                    childNode->data_type   = currentScope->strTable[key]->data_type;
                    childNode->sym_type    = currentScope->strTable[key]->symbol_type;
                    childNode->scope       = currentScope->strTable[key]->scope;
                    childNode->childNumber = currentScope->strTable[key]->childNumber;
                    AddChild(funcTypeNameNode, childNode);
                    yyval.node = funcTypeNameNode;
                 }
break;
case 18:
#line 211 "src/parser.y"
	{
                    tree* formalDeclListNode = MakeTree(FORMALDECLLIST);
                    AddChild(formalDeclListNode, yystack.l_mark[-1].node);
                    AddChild(formalDeclListNode, yystack.l_mark[0].node);
                    yyval.node = formalDeclListNode;
                 }
break;
case 19:
#line 219 "src/parser.y"
	{
                    yyval.node = NULL;
                 }
break;
case 20:
#line 223 "src/parser.y"
	{
                    yyval.node = yystack.l_mark[0].node;
                 }
break;
case 21:
#line 228 "src/parser.y"
	{
                    tree* formalDeclNode = MakeTree(FORMALDECL);
                    AddChild(formalDeclNode, yystack.l_mark[-2].node);
                    int exists = -1; 
                    int key;
                    if(yystack.l_mark[0].node == NULL){
                        key = STInsert(yystack.l_mark[-1].strval, formalDeclNode->children[0]->val, SCALAR, &scope, &exists);
                    }
                    else{
                        key = STInsert(yystack.l_mark[-1].strval, formalDeclNode->children[0]->val, ARRAY, &scope, &exists);
                    }
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = MakeTreeWithVal(IDENTIFIER, key);
                    childNode->data_type   = currentScope->strTable[key]->data_type;
                    childNode->sym_type    = currentScope->strTable[key]->symbol_type;
                    childNode->scope       = currentScope->strTable[key]->scope;
                    childNode->childNumber = currentScope->strTable[key]->childNumber;
                    AddParam(currentScope->strTable[key]->data_type, currentScope->strTable[key]->symbol_type);
                    AddChild(formalDeclNode, childNode);
                    AddChild(formalDeclNode, yystack.l_mark[0].node); 
                    yyval.node = formalDeclNode;
                 }
break;
case 22:
#line 254 "src/parser.y"
	{
                    yyval.node = NULL;
                 }
break;
case 23:
#line 258 "src/parser.y"
	{
                    yyval.node = MakeTree(ARRAYDECL);
                 }
break;
case 24:
#line 263 "src/parser.y"
	{
                    tree* funBodyNode = MakeTree(FUNBODY);
                    AddChild(funBodyNode, yystack.l_mark[-2].node);
                    AddChild(funBodyNode, yystack.l_mark[-1].node);
                    yyval.node = funBodyNode;
                 }
break;
case 25:
#line 271 "src/parser.y"
	{
                    yyval.node = NULL;
                 }
break;
case 26:
#line 275 "src/parser.y"
	{
                    tree* localDeclListNode = MakeTree(LOCALDECLLIST);
                    AddChild(localDeclListNode, yystack.l_mark[-1].node);
                    AddChild(localDeclListNode, yystack.l_mark[0].node);
                    yyval.node = localDeclListNode;
                 }
break;
case 27:
#line 283 "src/parser.y"
	{
                    yyval.node = NULL;
                 }
break;
case 28:
#line 287 "src/parser.y"
	{   
                    tree* statementListNode = MakeTree(STATEMENTLIST);
                    AddChild(statementListNode, yystack.l_mark[-1].node);
                    AddChild(statementListNode, yystack.l_mark[0].node);
                    yyval.node = statementListNode;
                 }
break;
case 29:
#line 295 "src/parser.y"
	{
                    tree* statementNode = MakeTree(STATEMENT);
                    AddChild(statementNode, yystack.l_mark[0].node);
                    yyval.node = statementNode;
                 }
break;
case 30:
#line 301 "src/parser.y"
	{   
                    tree* statementNode = MakeTree(STATEMENT);
                    AddChild(statementNode, yystack.l_mark[0].node);
                    yyval.node = statementNode;    
                 }
break;
case 31:
#line 307 "src/parser.y"
	{
                    tree* statementNode = MakeTree(STATEMENT);
                    AddChild(statementNode, yystack.l_mark[0].node);
                    yyval.node = statementNode;
                 }
break;
case 32:
#line 313 "src/parser.y"
	{
                    tree* statementNode = MakeTree(STATEMENT);
                    AddChild(statementNode, yystack.l_mark[0].node);
                    yyval.node = statementNode;
                 }
break;
case 33:
#line 319 "src/parser.y"
	{
                    tree* statementNode = MakeTree(STATEMENT);
                    AddChild(statementNode, yystack.l_mark[0].node);
                    yyval.node = statementNode;
                 }
break;
case 34:
#line 326 "src/parser.y"
	{
                    tree* compoundStmtNode = MakeTree(COMPOUNDSTMT);
                    AddChild(compoundStmtNode, yystack.l_mark[-1].node);
                    yyval.node = compoundStmtNode;
                 }
break;
case 35:
#line 333 "src/parser.y"
	{
 /*                   printf("top of assignStmt\n");*/
                    tree* assignStmtNode = MakeTree(ASSIGNSTMT);
                    AddChild(assignStmtNode, yystack.l_mark[-3].node);
                    AddChild(assignStmtNode, yystack.l_mark[-1].node); 
                    yyval.node = assignStmtNode;

                    tree* current = yystack.l_mark[-3].node; 
                    while(current->numChildren > 0){
                        current = current->children[0];
                    }
                    int varDataType = current->data_type;
                    if(CheckTypeMismatch(yystack.l_mark[-1].node, varDataType) == 1){
                        yyerror("Type mismatch in assignment.");
                    }
                 }
break;
case 36:
#line 350 "src/parser.y"
	{
                    tree* assignStmtNode = MakeTree(ASSIGNSTMT);
                    AddChild(assignStmtNode, yystack.l_mark[-1].node);
                    yyval.node = assignStmtNode;
                 }
break;
case 37:
#line 357 "src/parser.y"
	{
                    tree* condStmtNode = MakeTree(CONDSTMT);
                    AddChild(condStmtNode, yystack.l_mark[-3].node);
                    AddChild(condStmtNode, yystack.l_mark[-1].node);
                    AddChild(condStmtNode, yystack.l_mark[0].node);
                    yyval.node = condStmtNode;
                 }
break;
case 38:
#line 366 "src/parser.y"
	{
                    yyval.node = NULL; 
                 }
break;
case 39:
#line 370 "src/parser.y"
	{
                    yyval.node = yystack.l_mark[0].node;
                 }
break;
case 40:
#line 375 "src/parser.y"
	{
                    tree* loopStmtNode = MakeTree(LOOPSTMT);
                    AddChild(loopStmtNode, yystack.l_mark[-2].node);
                    AddChild(loopStmtNode, yystack.l_mark[0].node);
                    yyval.node = loopStmtNode;
                 }
break;
case 41:
#line 383 "src/parser.y"
	{
                    tree* returnStmtNode = MakeTree(RETURNSTMT);
                    AddChild(returnStmtNode, yystack.l_mark[0].node); 
                    yyval.node = returnStmtNode;
                 }
break;
case 42:
#line 390 "src/parser.y"
	{
                    yyval.node = NULL; 
                 }
break;
case 43:
#line 394 "src/parser.y"
	{
                    yyval.node = yystack.l_mark[-1].node;
                 }
break;
case 44:
#line 399 "src/parser.y"
	{
                    tree* varNode = MakeTree(VAR);
                    int key = -1; 
                    symEntry* entry = STLookup(yystack.l_mark[-1].strval, &key); 
                    tree* childNode = MakeTreeWithVal(IDENTIFIER, key);
                    if(yystack.l_mark[0].node != NULL){
                        if(entry->symbol_type != ARRAY){
                            yyerror("Non-array identifier used as an array.");
                        }
                        else{
                            tree* placeholderTreeNode = yystack.l_mark[0].node; 
                            while(placeholderTreeNode->numChildren > 0){
                                placeholderTreeNode = placeholderTreeNode->children[0];
                            }
                            if(placeholderTreeNode->nodeKind == IDENTIFIER){
                                if(placeholderTreeNode->data_type != INT_TYPE){
                                    yyerror("Array indexed using non-integer expression.");
                                }
                            }
                            else if(placeholderTreeNode->nodeKind == INTEGER){
                                if(yylval.value >= entry->arrayBound){
                                    yyerror("Statically sized array indexed with constant, out-of-bounds expression.");
                                }
                            }
                            else{
                                yyerror("Array indexed using non-integer expression.");
                            }
                        }
                    }
                    if(key == -1){
                        yyerror("Undeclared variable");
                    }
                    else{
                        childNode->data_type   = entry->data_type;
                        childNode->sym_type    = entry->symbol_type;
                        childNode->scope       = entry->scope;
                        childNode->childNumber = entry->childNumber;
                    }
                    AddChild(varNode, childNode);
                    AddChild(varNode, yystack.l_mark[0].node);
                    yyval.node = varNode;
                 }
break;
case 45:
#line 443 "src/parser.y"
	{
                    yyval.node = NULL;
                 }
break;
case 46:
#line 447 "src/parser.y"
	{
                    yyval.node = yystack.l_mark[-1].node;
                 }
break;
case 47:
#line 452 "src/parser.y"
	{
                    tree* expressionNode = MakeTree(EXPRESSION);
                    AddChild(expressionNode, yystack.l_mark[0].node);
                    yyval.node = expressionNode;

                 }
break;
case 48:
#line 459 "src/parser.y"
	{
                    /*
                    tree* expressionNode = MakeTree(EXPRESSION);
                    AddChild(expressionNode, $1);
                    AddChild(expressionNode, $2);
                    AddChild(expressionNode, $3);
                    $$ = expressionNode;
                    */
                    tree* expressionNode = yystack.l_mark[-1].node;
                    AddChild(expressionNode, yystack.l_mark[-2].node);
                    AddChild(expressionNode, yystack.l_mark[0].node);
                    yyval.node = expressionNode;

                 }
break;
case 49:
#line 475 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(RELOP, LTE);
                 }
break;
case 50:
#line 479 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(RELOP, LT);
                 }
break;
case 51:
#line 483 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(RELOP, GT);
                 }
break;
case 52:
#line 487 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(RELOP, GTE);
                 }
break;
case 53:
#line 491 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(RELOP, EQ);
                 }
break;
case 54:
#line 495 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(RELOP, NEQ);
                 }
break;
case 55:
#line 500 "src/parser.y"
	{
                    /*
                    tree* addExprNode = MakeTree(ADDEXPR);
                    AddChild(addExprNode, $1);
                    $$ = addExprNode;
                    */
                    yyval.node = yystack.l_mark[0].node;
                 }
break;
case 56:
#line 509 "src/parser.y"
	{
                    /*
                    tree* addExprNode = MakeTree(ADDEXPR);
                    AddChild(addExprNode, $1);
                    AddChild(addExprNode, $2);
                    AddChild(addExprNode, $3);
                    $$ = addExprNode;
                    */
                    tree* addOpNode = yystack.l_mark[-1].node;
                    AddChild(addOpNode, yystack.l_mark[-2].node);
                    AddChild(addOpNode, yystack.l_mark[0].node);
                    yyval.node = addOpNode;
                 }
break;
case 57:
#line 524 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(ADDOP, ADD);
                 }
break;
case 58:
#line 528 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(ADDOP, SUB);
                 }
break;
case 59:
#line 533 "src/parser.y"
	{
                    /*
                    tree* termNode = MakeTree(TERM);
                    AddChild(termNode, $1);
                    $$ = termNode;
                    */
                    yyval.node = yystack.l_mark[0].node;
                 }
break;
case 60:
#line 542 "src/parser.y"
	{
                    /*
                    tree* termNode = MakeTree(TERM);
                    AddChild(termNode, $1);
                    AddChild(termNode, $2);
                    AddChild(termNode, $3);
                    $$ = termNode;
                    */
                    tree* mulOpNode = yystack.l_mark[-1].node;
                    AddChild(mulOpNode, yystack.l_mark[-2].node);
                    AddChild(mulOpNode, yystack.l_mark[0].node);
                    yyval.node = mulOpNode;
                 }
break;
case 61:
#line 557 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(MULOP, MUL);
                 }
break;
case 62:
#line 561 "src/parser.y"
	{
                    yyval.node = MakeTreeWithVal(MULOP, DIV); 
                 }
break;
case 63:
#line 566 "src/parser.y"
	{
                    tree* factorNode = MakeTree(FACTOR);
                    AddChild(factorNode, yystack.l_mark[-1].node);
                    yyval.node = factorNode;
                 }
break;
case 64:
#line 572 "src/parser.y"
	{
                    tree* factorNode = MakeTree(FACTOR);
                    AddChild(factorNode, yystack.l_mark[0].node);
                    yyval.node = factorNode;
                 }
break;
case 65:
#line 578 "src/parser.y"
	{
                    tree* factorNode = MakeTree(FACTOR);
                    AddChild(factorNode, yystack.l_mark[0].node);
                    yyval.node = factorNode;
                 }
break;
case 66:
#line 584 "src/parser.y"
	{
                    /*
                    tree* factorNode = MakeTree(FACTOR);
                    AddChild(factorNode, MakeTreeWithVal(INTEGER, $1));
                    $$ = factorNode;
                    */
                    tree* intConstNode = MakeTreeWithVal(INTEGER, yystack.l_mark[0].value);
                    yyval.node = intConstNode;
                 }
break;
case 67:
#line 594 "src/parser.y"
	{
                    tree* factorNode = MakeTree(FACTOR);
                    AddChild(factorNode, MakeTreeWithVal(CHAR, yystack.l_mark[0].value));
                    yyval.node = factorNode;
                 }
break;
case 68:
#line 600 "src/parser.y"
	{
                    tree* factorNode = MakeTree(FACTOR);
                    int exists = -1; 
                    int key = STInsert(yystack.l_mark[0].strval, CHAR_TYPE, SCALAR, &scope, &exists);
                    if (exists != -1){
                        yyerror("Symbol declared multiple times."); 
                    }
                    tree* childNode = MakeTreeWithVal(CHAR, key);
                    childNode->data_type   = currentScope->strTable[key]->data_type;
                    childNode->sym_type    = currentScope->strTable[key]->symbol_type;
                    childNode->scope       = currentScope->strTable[key]->scope;
                    childNode->childNumber = currentScope->strTable[key]->childNumber;
                    AddChild(factorNode, childNode);
                    yyval.node = factorNode;
                 }
break;
case 69:
#line 617 "src/parser.y"
	{
                    tree* funcCallExprNode = MakeTree(FUNCCALLEXPR);
                    int key = -1; 
                    symEntry* entry = STLookup(yystack.l_mark[-2].strval, &key); 

                     /*Make sure that the output() function does not throw any errors*/
                     if(!strcmp(yystack.l_mark[-2].strval, "output") && key == -1){
                        table_node* temp = currentScope;
                        int tempScope = 1;
                        int exists = -1;
                        /*set scope to be global*/
                        currentScope = symbolTableHead;
                        key = STInsert("output", INT_TYPE, FUNCTION, &tempScope, &exists);
                        NewScope();
                        tempScope++;
                        /*insert arg (a single integer)*/
                        STInsert("arg1", INT_TYPE, SCALAR, &tempScope, &exists);
                        AddParam(INT_TYPE, SCALAR);
                        UpScope();
                        ConnectParams(key);
                        /*return to original scope*/
                        currentScope = temp;
                        entry = STLookup(yystack.l_mark[-2].strval, &key);
                     }

                    tree* childNode = MakeTreeWithVal(IDENTIFIER, key);
                    if(key == -1){
                        yyerror("Undefined function");
                    }
                    else{
                        childNode->data_type   = entry->data_type;
                        childNode->sym_type    = entry->symbol_type;
                        childNode->scope       = entry->scope;
                        childNode->childNumber = entry->childNumber;

                        if(CountArguments() < symbolTableHead->strTable[key]->size){
                            yyerror("Too few arguments provided in function call.");
                        }
                        else if(CountArguments() > symbolTableHead->strTable[key]->size){
                            yyerror("Too many arguments provided in function call.");
                        }
                        else{
                            param* currentParam = symbolTableHead->strTable[key]->params; 
                            arg* currentArg = argumentListHead; 
                            while(currentParam != NULL && currentArg != NULL){
                                if(currentParam->data_type != currentArg->data_type){
                                    yyerror("Argument type mismatch in function call.");
                                    break;
                                }
                                if(currentParam->symbol_type != currentArg->symbol_type){
                                    yyerror("Argument type mismatch in function call.");
                                    break;
                                }
                                currentParam = currentParam->next; 
                                currentArg = currentArg->next; 
                            }
                        }
                        ResetArguments();
                    }
                    AddChild(funcCallExprNode, childNode);
                    AddChild(funcCallExprNode, yystack.l_mark[0].node);
                    yyval.node = funcCallExprNode;
                 }
break;
case 70:
#line 682 "src/parser.y"
	{
                    yyval.node = yystack.l_mark[-1].node;
                 }
break;
case 71:
#line 686 "src/parser.y"
	{
                    yyval.node = NULL; 
                 }
break;
case 72:
#line 691 "src/parser.y"
	{
                    tree* argListNode = MakeTree(ARGLIST);
                    AddChild(argListNode, yystack.l_mark[0].node);
                        int desiredArgumentDataType = -1;
                        int desiredArgumentSymbolType = -1;
                        tree *placeholderTreeNode = argListNode;
						while (placeholderTreeNode->numChildren > 0) {
							placeholderTreeNode = placeholderTreeNode->children[0];
						}
                        if (placeholderTreeNode->nodeKind == IDENTIFIER) { /*"var" non-terminal*/
						  if (currentScope->strTable[placeholderTreeNode->val]->symbol_type == FUNCTION) {
						    desiredArgumentDataType = INT_TYPE; 
						  } 
                          else { /*scalar or array*/
                            desiredArgumentDataType = currentScope->strTable[placeholderTreeNode->val]->data_type;
						    desiredArgumentSymbolType = currentScope->strTable[placeholderTreeNode->val]->symbol_type;
						  }
						} 
                        else { /*constant */
                            if(placeholderTreeNode->nodeKind == INTEGER){
                                desiredArgumentDataType = INT_TYPE; 
                                desiredArgumentSymbolType = SCALAR;
                            }
                            else if(placeholderTreeNode->nodeKind == CHAR){
                                desiredArgumentDataType = CHAR_TYPE; 
                                desiredArgumentSymbolType = SCALAR;
                            }
						}
                        AddArgument(desiredArgumentDataType, desiredArgumentSymbolType);
                    yyval.node = argListNode;
                 }
break;
case 73:
#line 723 "src/parser.y"
	{
                    tree* argListNode = MakeTree(ARGLIST);
                    AddChild(argListNode, yystack.l_mark[-2].node);
                    AddChild(argListNode, yystack.l_mark[0].node);
                    int desiredArgumentDataType = -1;
                    int desiredArgumentSymbolType = -1;
                    tree *placeholderTreeNode = argListNode->children[1];
					while (placeholderTreeNode->numChildren > 0) {
						placeholderTreeNode = placeholderTreeNode->children[0];
					}
                    if (placeholderTreeNode->nodeKind == IDENTIFIER) { /*"var" non-terminal*/
					  if (currentScope->strTable[placeholderTreeNode->val]->symbol_type == FUNCTION) {
					    desiredArgumentDataType = INT_TYPE;
					  } 
                      else { /*scalar or array*/
                        desiredArgumentDataType = currentScope->strTable[placeholderTreeNode->val]->data_type;
					    desiredArgumentSymbolType = currentScope->strTable[placeholderTreeNode->val]->symbol_type;
					  }
					} 
                    else { /*constant*/
                        if(placeholderTreeNode->nodeKind == INTEGER){
                            desiredArgumentDataType = INT_TYPE; 
                            desiredArgumentSymbolType = SCALAR;
                        }
                        else if(placeholderTreeNode->nodeKind == CHAR){
                            desiredArgumentDataType = CHAR_TYPE; 
                            desiredArgumentSymbolType = SCALAR;
                        }
					}
                    AddArgument(desiredArgumentDataType, desiredArgumentSymbolType);
                    yyval.node = argListNode;
                 }
break;
#line 1406 "obj/y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
