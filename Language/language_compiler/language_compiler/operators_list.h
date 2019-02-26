


DEF_OPERATOR (1,  "+", Op_Add,   { fprintf(out, "ADD\n"); })
DEF_OPERATOR (2,  "-", Op_Sub,   { fprintf(out, "SUB\n"); })
DEF_OPERATOR (3,  "*", Op_Mult,  { fprintf(out, "MUL\n"); })
DEF_OPERATOR (4,  "/", Op_Div,   { fprintf(out, "DIV\n"); })

DEF_OPERATOR (5,  ">=", Op_More_eq,                  \
            {                                        \
                int adr = (long)root % 10000;         \
                fprintf (out, "JBE BE%d\n", adr);    \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JMP BACK%d\n", adr);  \
                fprintf (out, "BE%d:\n", adr);       \
                fprintf (out, "PUSH 1\n");           \
                fprintf (out, "BACK%d:\n", adr);     \
            })
DEF_OPERATOR (6,  "<=", Op_Less_eq,                  \
            {                                        \
                int adr = (long)root % 10000;         \
                fprintf (out, "JAE AE%d\n", adr);    \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JMP BACK%d\n", adr);  \
                fprintf (out, "AE%d:\n", adr);       \
                fprintf (out, "PUSH 1\n");           \
                fprintf (out, "BACK%d:\n", adr);     \
            })
DEF_OPERATOR (7,  ">",  Op_More,                     \
            {                                        \
                int adr = (long)root % 10000;         \
                fprintf (out, "JB B%d\n", adr);      \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JMP BACK%d\n", adr);  \
                fprintf (out, "B%d:\n", adr);        \
                fprintf (out, "PUSH 1\n");           \
                fprintf (out, "BACK%d:\n", adr);     \
            })
DEF_OPERATOR (8,  "<",  Op_Less,                     \
            {                                        \
                int adr = (long)root % 10000;        \
                fprintf (out, "JA A%d\n", adr);      \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JMP BACK%d\n", adr);  \
                fprintf (out, "A%d:\n", adr);        \
                fprintf (out, "PUSH 1\n");           \
                fprintf (out, "BACK%d:\n", adr);     \
            })
DEF_OPERATOR (9,  "==", Op_Equal,                    \
            {                                        \
                int adr = (long)root % 10000;        \
                fprintf (out, "JE E%d\n", adr);      \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JMP BACK%d\n", adr);  \
                fprintf (out, "E%d:\n", adr);        \
                fprintf (out, "PUSH 1\n");           \
                fprintf (out, "BACK%d:\n", adr);     \
            })

DEF_OPERATOR (14,  "!=", Op_Unequal,                  \
            {                                        \
                int adr = (long)root % 10000;        \
                fprintf (out, "JNE E%d\n", adr);     \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JMP BACK%d\n", adr);  \
                fprintf (out, "E%d:\n", adr);        \
                fprintf (out, "PUSH 1\n");           \
                fprintf (out, "BACK%d:\n", adr);     \
            })

DEF_OPERATOR (10, "&&",  Op_And,                     \
            {                                        \
                int adr = (long)root % 10000;         \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JE BACK1%d\n", adr);  \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JE BACK2%d\n", adr);  \
                fprintf (out, "PUSH 1\n");           \
                fprintf (out, "JMP BACK3%d\n", adr); \
                                                     \
                fprintf (out, "BACK1%d:\n", adr);    \
                fprintf (out, "POP\n");              \
                fprintf (out, "BACK2%d:\n", adr);    \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "BACK3%d:\n", adr);    \
            })
DEF_OPERATOR (11, "OR",  Op_Or,                      \
            {                                        \
                int adr = (long)root % 10000;        \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JNE BACK1%d\n", adr); \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JNE BACK2%d\n", adr); \
                fprintf (out, "PUSH 0\n");           \
                fprintf (out, "JMP BACK3%d\n", adr); \
                                                     \
                fprintf (out, "BACK1%d:\n", adr);    \
                fprintf (out, "POP\n");              \
                fprintf (out, "BACK2%d:\n", adr);    \
                fprintf (out, "PUSH 1\n");           \
                fprintf (out, "BACK3%d:\n", adr);    \
            })
DEF_OPERATOR (12, "!",   Op_Not,    {  })


DEF_OPERATOR (13, "=",  Op_Assign,  { fprintf (stderr, "SYNTAX ERROR: misspelled math encountered: assignment\n"
                                               "is not a comparison."); ARF = 1; return 1; })

//DEF_OPERATOR (11, "^", {})
