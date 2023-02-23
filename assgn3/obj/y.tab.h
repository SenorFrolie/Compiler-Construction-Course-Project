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
extern YYSTYPE yylval;
