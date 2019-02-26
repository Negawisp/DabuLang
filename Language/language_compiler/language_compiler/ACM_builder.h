#ifndef ACM_BUILDER_H_INCLUDED
#define ACM_BUILDER_H_INCLUDED


int Build_ACM_Math     (struct tree_node* root, FILE* out);

int Build_ACM_Complex_Oper (struct tree_node* cur_node, FILE* out)
{
    int func_data = 0;
    while (cur_node)
    {
        func_data = cur_node -> data;
        if (cur_node->node_type == TYPE__KEY_WORD)
        switch (func_data)
        {
    #define DEF_KEY_WORD( num, name, meaning, code ) \
        case num:                                    \
            {                                        \
                code;                                \
            } break;
        #include "key_words_list.h"
    #undef DEF_KEY_WORD
        }
        if (cur_node->node_type == TYPE__VARIABLE)
        {
            Build_ACM_Math(cur_node->left_branch, out);
            if (ARF) return 0;
                int    RAM   = cur_node->data;
                fprintf (out, "PUSH x1\n");
            fprintf (out, "PUSH %d\n", RAM);
            fprintf (out, "PUSH 8\n");
            fprintf (out, "MUL\n");
                fprintf (out, "ADD\n");
                fprintf (out, "POP x3\n");
                fprintf (out, "POP [x3]\n");

                fprintf (out, "PUSH [x1]\n");
                fprintf (out, "PUSH 1\n");
                fprintf (out, "ADD\n");
                fprintf (out, "POP [x1]\n");
        }
        cur_node = cur_node -> right_branch;
    }

    return 0;
}

int Build_ACM_Math     (struct tree_node* root, FILE* out)
{

    if (root->node_type == TYPE__OPERATOR)
    {
        if (EQUAL_DOUBLE(root->data, Op_Not_number))
        {
            NULL_CHECK (root, CASE_OF_LEFT);
            Build_ACM_Math (root->left_branch,  out);
            if (ARF) return 0;
            int postfix = (long)root % 10000;
            fprintf (out, "PUSH 0\n");
            fprintf (out, "JE NULL%d\n", postfix);
            fprintf (out, "PUSH 0\n");
            fprintf (out, "JMP NNUL%d\n", postfix);
            fprintf (out, "NULL%d:\n", postfix);
            fprintf (out, "PUSH 1\n");
            fprintf (out, "NNUL%d:\n", postfix);
            return 0;
        }
        NULL_CHECK (root, CASE_OF_LEFT);
        NULL_CHECK (root, CASE_OF_RIGHT);

        Build_ACM_Math (root->left_branch,  out);
        if (ARF) return 0;
        Build_ACM_Math (root->right_branch, out);
        if (ARF) return 0;
        int data = root->data;
        switch (data)
        {
        #define DEF_OPERATOR( num, name, meaning, code )\
        case num:            \
            {                \
                code;        \
            } break;
            #include "operators_list.h"
        #undef DEF_OPERATOR
        }
        return 0;
    }
    if (root->node_type == TYPE__NUMERIC)
    {
        fprintf (out, "PUSH %lg\n", root->data);
        return 0;
    }
    if (root->node_type == TYPE__VARIABLE)
    {
        fprintf (out, "PUSH x1\n");
    fprintf (out, "PUSH %lg\n", root->data);
    fprintf (out, "PUSH 8\n");
    fprintf (out, "MUL\n");
        fprintf (out, "ADD\n");
        fprintf (out, "POP x3\n");
        fprintf (out, "PUSH [x3]\n");
        return 0;
    }

    if (root->node_type == TYPE__FUNCTION)
    {
        struct tree_node* cur_node = root;

        fprintf (out, "PUSH x1\n");
        fprintf (out, "PUSH x1\n");
    fprintf (out, "PUSH [x1]\n");
    fprintf (out, "PUSH 8\n");
    fprintf (out, "MUL\n");
        fprintf (out, "ADD\n");
        fprintf (out, "PUSH 8\n");
        fprintf (out, "ADD\n");
        fprintf (out, "POP x2\n");
        fprintf (out, "PUSH 0\n");
        fprintf (out, "POP [x2]\n");

        int i = 1;
        while (cur_node->right_branch != NULL)
        {
            cur_node = cur_node->right_branch;
            Build_ACM_Math(cur_node->left_branch, out);
            if (ARF) return 0;
            fprintf (out, "PUSH x2\n");
        fprintf (out, "PUSH %d\n", i);
        fprintf (out, "PUSH 8\n");
        fprintf (out, "MUL\n");
            fprintf (out, "ADD\n");
            fprintf (out, "POP x3\n");
            fprintf (out, "POP [x3]\n");
            i++;
        }

        fprintf (out, "PUSH %d\n", i-1);
        fprintf (out, "POP [x2]\n");
        fprintf (out, "PUSH x2\n");
        fprintf (out, "POP x1\n");
        int func_data = root->data;
        char* func_name = Names_Array[func_data]._Name;
        fprintf (out, "CALL %s\n", func_name);
    }
    return 0;
}

int Build_ACM_function (struct tree_node* root, FILE* out)
{
    //TO DO CHECKS!!!
    struct tree_node* cur_node = root->left_branch;
    int func_data = cur_node->data;
    char* func_name = Names_Array[func_data]._Name;
    fprintf (out, "%s:\n", func_name);


    while (cur_node)
    {
        func_data = cur_node -> data;
        if (cur_node->node_type == TYPE__KEY_WORD)
        switch (func_data)
        {
    #define DEF_KEY_WORD( num, name, meaning, code ) \
        case num:                                    \
            {                                        \
                code;                                \
            } break;
        #include "key_words_list.h"
    #undef DEF_KEY_WORD
        }
        if (cur_node->node_type == TYPE__VARIABLE)
        {
            Build_ACM_Math(cur_node->left_branch, out);
            if (ARF) return 0;
                int    RAM   = cur_node->data;
                fprintf (out, "PUSH x1\n");
            fprintf (out, "PUSH %d\n", RAM);
            fprintf (out, "PUSH 8\n");
            fprintf (out, "MUL\n");
                fprintf (out, "ADD\n");
                fprintf (out, "POP x3\n");
                fprintf (out, "POP [x3]\n");

                fprintf (out, "PUSH [x1]\n");
                fprintf (out, "PUSH 1\n");
                fprintf (out, "ADD\n");
                fprintf (out, "POP [x1]\n");
        }
        cur_node = cur_node -> right_branch;
    }

    fprintf (out, "PUSH 911.402\n");
    fprintf (out, "OUT\n");
    fprintf (out, "PUSH 402.911\n");
    fprintf (out, "OUT\n");
    fprintf (out, "PUSH 911.402\n");
    fprintf (out, "OUT\n");
    fprintf (out, "PUSH 402.911\n");
    fprintf (out, "OUT\n");
    fprintf (out, "PUSH 911.402\n");
    fprintf (out, "OUT\n");
    fprintf (out, "PUSH 402.911\n");
    fprintf (out, "OUT\n");
    fprintf (out, "JMP ENDdd\n");
    return 0;
}

int Build_ACM_code (struct tree_node* root, FILE* out)
{
    fprintf (out, "PUSH x7\n");
    fprintf (out, "POP x1\n");
    fprintf (out, "PUSH 0\n");
    fprintf (out, "POP [0]\n");
    fprintf (out, "PUSH 0\n");

    fprintf (out, "CALL Lok'Tar!\n");
    fprintf (out, "JMP ENDdd\n");
    struct tree_node* cur_node = root;

    while (cur_node)
    {
        Build_ACM_function(cur_node, out);
        if (ARF) return 0;
        cur_node = cur_node->right_branch;
    }

    fprintf (out, "\nENDdd:\n"
                    "POP\n");
    return 0;
}


#endif // ACM_BUILDER_H_INCLUDED
