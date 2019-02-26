
#define PRINT(wtp) fprintf (out, "%s\n", wtp);
DEF_KEY_WORD (1,  "Dabu",       KW_Begin,       {} )   // (I obey)
DEF_KEY_WORD (2,  "Swobu",      KW_End,         {} )   // (As you say)

DEF_KEY_WORD (3,  "Trom-Ka",    KW_Var,    /* (Well met) */              \
            {                                                            \
                Build_ACM_Math(cur_node->left_branch->left_branch, out); \
                if (ARF) return 0;                                       \
                int    RAM   = cur_node->left_branch->data;              \
                fprintf (out, "PUSH x1\n");                      \
            fprintf (out, "PUSH %d\n", RAM);                     \
            fprintf (out, "PUSH 8\n");                           \
            fprintf (out, "MUL\n");                              \
                fprintf (out, "ADD\n");                          \
                fprintf (out, "POP x3\n");                       \
                fprintf (out, "POP [x3]\n");                     \
                                                                 \
                fprintf (out, "PUSH [x1]\n");                    \
                fprintf (out, "PUSH 1\n");                       \
                fprintf (out, "ADD\n");                          \
                fprintf (out, "POP [x1]\n");                     \
            } )

DEF_KEY_WORD (4,  "Lo'Gosh",    KW_Function,    {} )   // (Spirit of wolf)

DEF_KEY_WORD (5,  "Mag'Har",  KW_While,  /* Uncorrupted */                           \
            {                                                                        \
                int postfix = (long)cur_node % 10000;                                \
                fprintf (out, "Whle%d:\n", postfix);                                 \
                Build_ACM_Math (cur_node->left_branch->left_branch, out);            \
                if (ARF) return 0;                                                   \
                fprintf (out, "PUSH 0\n");                                           \
                fprintf (out, "JNE Then%d\n", postfix);                              \
                fprintf (out, "JMP AftW%d\n", postfix);                              \
                                                                                     \
                fprintf (out, "Then%d:\n", postfix);                                 \
                Build_ACM_Complex_Oper (cur_node->left_branch->right_branch, out);   \
                if (ARF) return 0;                                                   \
                fprintf (out, "JMP Whle%d\n", postfix);                              \
                fprintf (out, "AftW%d:\n", postfix);                                 \
            } )

DEF_KEY_WORD (6,  "Kagh",       KW_Goto,        {} )   // (run!)
DEF_KEY_WORD (7,  "Cha",        KW_Label,       {} )   // (name)

DEF_KEY_WORD (8,  "Gol'Kosh",   KW_If,              /* (By my axe) */                              \
        {                                                                                          \
            int postfix = (long)cur_node % 10000;                                                  \
            Build_ACM_Math (cur_node->left_branch->left_branch, out);                              \
            fprintf (out, "PUSH 0\n");                                                             \
            fprintf (out, "JNE Then%d\n", postfix);                                                \
            if (cur_node->left_branch->right_branch->right_branch != NULL)                         \
                fprintf (out, "JMP Else%d\n", postfix);                                            \
            else                                                                                   \
                fprintf (out, "JMP AThn%d\n", postfix);                                            \
                                                                                                   \
            fprintf (out, "Then%d:\n", postfix);                                                   \
            Build_ACM_Complex_Oper (cur_node->left_branch->right_branch->left_branch, out);        \
            if (ARF) return 0;                                                                     \
            fprintf (out, "JMP AThn%d\n", postfix);                                                \
                                                                                                   \
            if (cur_node->left_branch->right_branch->right_branch != NULL)                         \
            {                                                                                      \
                fprintf (out, "Else%d:\n", postfix);                                               \
                Build_ACM_Complex_Oper (cur_node->left_branch->right_branch->right_branch, out);   \
                if (ARF) return 0;                                                                 \
                fprintf (out, "JMP AThn%d\n", postfix);                                            \
            }                                                                                      \
            fprintf (out, "AThn%d:\n", postfix);                                                   \
        } )
DEF_KEY_WORD (9,  "Dae'Mon",    KW_Else,        {} )   // (Twisted)

DEF_KEY_WORD (10, "Zug-Zug",    KW_Pt_n_comma,  {} )   // (ok)

DEF_KEY_WORD (11, "{",          KW_Opn_brct,    {} )   // (
DEF_KEY_WORD (12, "}",          KW_Cls_brct,    {} )   // )

DEF_KEY_WORD (13, "Lok'Tra",    KW_Out,                        \
            {       /* (A warsong)*/                           \
                Build_ACM_Math (cur_node->left_branch, out);   \
                if (ARF) return 0;                             \
                fprintf (out, "OUT\n");                        \
            } )

DEF_KEY_WORD (16, "Lok-Regar", KW_In,    /*Ready for orders*/    \
            {                                                    \
                int RAM = cur_node->left_branch->data;           \
                fprintf (out, "PUSH x1\n");                      \
            fprintf (out, "PUSH %d\n", RAM);                     \
            fprintf (out, "PUSH 8\n");                           \
            fprintf (out, "MUL\n");                              \
                fprintf (out, "ADD\n");                          \
                fprintf (out, "POP x3\n");                       \
                fprintf (out, "IN\n");                           \
                fprintf (out, "POP [x3]\n");                     \
            })

DEF_KEY_WORD (14, "Lok-Narash", KW_Return,                    \
            {       /* (arm yourselves) */                    \
                Build_ACM_Math (cur_node->left_branch, out);  \
                if (ARF) return 0;                            \
                fprintf (out, "POP x2\n");                    \
                fprintf (out, "POP x1\n");                    \
                fprintf (out, "PUSH x2\n");                   \
                fprintf (out, "RET\n");                       \
            } )

DEF_KEY_WORD (15, "^", KW_Comma,       {} )   // (arm yourselves)


                                                        //MAIN = Lok'Tar! (Victory!)
