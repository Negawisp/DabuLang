#ifndef TREE_BUILDER_H_INCLUDED
#define TREE_BUILDER_H_INCLUDED




int               Node_Current_Number = 0;
#define NEXT      Node_Current_Number++
#define CUR_NODE  Nodes_Array[Node_Current_Number]

#define CHECK_NODE_NUMBER( what_to_return )                                               \
{                                                                                         \
    if (Node_Current_Number >= Nodes_Count)                                               \
    {                                                                                     \
        fprintf (stderr, "Program or syntax ERROR: Function %s expected to\n"             \
                         "get a new node, but there are no left! (Line %d)", __PRETTY_FUNCTION__, __LINE__);  \
        ARF = 1;                                                                          \
        return what_to_return;                                                            \
    }                                                                                     \
}

#define CHECK_TYPE( type )                                                                    \
{                                                                                             \
    if (CUR_NODE.node_type != type)                                                           \
    {                                                                                         \
        fprintf (stderr, "Build or syntax ERROR: Function %s expected %s node type,\n"        \
                         "where she met %.4s node type. (Node No%d)", __PRETTY_FUNCTION__,    \
                         #type, (char*)&CUR_NODE.node_type, Node_Current_Number);             \
        ARF = 1;                                                                              \
        return NULL;                                                                          \
    }                                                                                         \
}

#define CHECK_DATA( data_required )                                                                      \
    if (!EQUAL_DOUBLE(CUR_NODE.data, data_required))                                                     \
    {                                                                                                    \
        ARF = 1;                                                                                         \
        fprintf (stderr, "Syntax ERROR: A \"%d\" node expected instead of this node:\n", data_required); \
        DUMP_NODE((&CUR_NODE));                                                                          \
        DUMP_NODE((&CUR_NODE-1));                                                                          \
        DUMP_NODE((&CUR_NODE-2));                                                                          \
        return NULL;                                                                                     \
    }

#define CHECK_UNKNOWN                                                                                    \
{                                                                                                        \
    if (Names_Array [(int)CUR_NODE.data]._type != TYPE__UNKNOWN)                                         \
    {                                                                                                    \
        fprintf (stderr, "The name %s was already used.\n", Names_Array [(int)CUR_NODE.data]._Name);     \
        ARF = 1;                                                                                         \
        return 0;                                                                                        \
    }                                                                                                    \
}

int Local_Variables_Decrement = 0;

struct tree_node* Get_Or ();

struct tree_node* Get_Atom ()
{
    CHECK_NODE_NUMBER(NULL);
    struct tree_node* ret_node = NULL;
    if (CUR_NODE.node_type == TYPE__KEY_WORD && EQUAL_DOUBLE (CUR_NODE.data, KW_Opn_brct_number))
    {
        NEXT;
        ret_node = Get_Or();
        if (ARF) return NULL;
        CHECK_NODE_NUMBER(NULL);
        CHECK_TYPE (TYPE__KEY_WORD);
        CHECK_DATA (KW_Cls_brct_number);
        NEXT;
        return ret_node;
    }

    if (CUR_NODE.node_type == TYPE__OPERATOR && EQUAL_DOUBLE (CUR_NODE.data, Op_Not_number))
    {
        ret_node = &CUR_NODE;
        NEXT;
        CHECK_NODE_NUMBER(NULL);
        node_add (ret_node, CASE_OF_LEFT, Get_Atom(), __LINE__);
        if (ARF) return NULL;
        return ret_node;
    }

    if (CUR_NODE.node_type == TYPE__UNKNOWN)
    {
        int name_number = CUR_NODE.data;
        if (Names_Array[name_number]._type == TYPE__UNKNOWN)
        {
            ARF = 1;
            fprintf (stderr, "Syntax ERROR: Usage of undeclared name \"%s\"\n", Names_Array[name_number]._Name);
            return NULL;
        }
        if (Names_Array[name_number]._type == TYPE__VARIABLE)
        {
            CUR_NODE.node_type = TYPE__VARIABLE;
            ret_node = &CUR_NODE;
            ret_node->data = ret_node->data - Local_Variables_Decrement;
            NEXT;
            return ret_node;
        }
        if (Names_Array[name_number]._type == TYPE__FUNCTION)
        {
            CUR_NODE.node_type = TYPE__FUNCTION;
            ret_node = &CUR_NODE;
            struct tree_node* cur_node = ret_node;
            NEXT;
            CHECK_NODE_NUMBER(NULL);
            CHECK_TYPE (TYPE__KEY_WORD);
            CHECK_DATA (KW_Opn_brct_number);
            NEXT;
            CHECK_NODE_NUMBER(NULL);

            while (CUR_NODE.node_type != TYPE__KEY_WORD || !EQUAL_DOUBLE (CUR_NODE.data, KW_Cls_brct_number))
            {
                CHECK_TYPE (TYPE__KEY_WORD);
                CHECK_DATA (KW_Comma_number);
                node_add(cur_node, CASE_OF_RIGHT, &CUR_NODE, __LINE__);
                cur_node = cur_node->right_branch;
                NEXT;
                CHECK_NODE_NUMBER(NULL);

                node_add (cur_node, CASE_OF_LEFT, Get_Or(), __LINE__);
                if (ARF) return NULL;
                CHECK_NODE_NUMBER (NULL);
            }

            NEXT;
            CHECK_NODE_NUMBER(NULL);
            return ret_node;
        }
        if (Names_Array[name_number]._type == TYPE__VARIABLE)
        {
            ret_node = &CUR_NODE;
            NEXT;
            return ret_node;
        }

        ARF = 1;
        fprintf (stderr, "PROGRAMM ERROR!!: Variable \"%s\" had strange unsuspected type %.4s.\n", Names_Array[name_number]._Name, (char*)&Names_Array[name_number]._type);
        return NULL;
    }

    if (CUR_NODE.node_type == TYPE__NUMERIC)
    {
        ret_node = &CUR_NODE;
        NEXT;
        return ret_node;
    }
    ARF = 1;
    fprintf (stderr, "Syntax ERROR: Function %s noticed that user tried to\n"
                     "MULTIPLY or DIVIDE this node:\n", __PRETTY_FUNCTION__);
    DUMP_NODE ((&CUR_NODE));
    return NULL;
}

struct tree_node* Get_Monomial ()
{
    CHECK_NODE_NUMBER(NULL);
    struct tree_node* ret_node = Get_Atom();
    if (ARF) return NULL;
    CHECK_NODE_NUMBER(NULL);
    if (CUR_NODE.node_type == TYPE__OPERATOR && (EQUAL_DOUBLE (CUR_NODE.data, Op_Mult_number) || EQUAL_DOUBLE (CUR_NODE.data, Op_Div_number)))
    {
        node_add (&CUR_NODE, CASE_OF_LEFT, ret_node, __LINE__);
        ret_node = &CUR_NODE;
        NEXT;
        CHECK_NODE_NUMBER(NULL);
        node_add (ret_node, CASE_OF_RIGHT, Get_Monomial(), __LINE__);
        if (ARF) return NULL;
        CHECK_NODE_NUMBER(NULL);
    }
    return ret_node;
}

struct tree_node* Get_Polynomial ()
{
    CHECK_NODE_NUMBER(NULL);
    struct tree_node* ret_node  = Get_Monomial();
    if (ARF) return NULL;
    CHECK_NODE_NUMBER(NULL);
    if (CUR_NODE.node_type == TYPE__OPERATOR && (EQUAL_DOUBLE (CUR_NODE.data, Op_Add_number) || EQUAL_DOUBLE (CUR_NODE.data, Op_Sub_number)))
    {
        node_add (&CUR_NODE, CASE_OF_LEFT, ret_node, __LINE__);
        ret_node = &CUR_NODE;
        NEXT;
        CHECK_NODE_NUMBER(NULL);
        node_add (ret_node, CASE_OF_RIGHT, Get_Polynomial(), __LINE__);
        if (ARF) return NULL;
        CHECK_NODE_NUMBER(NULL);
    }
    return ret_node;
}


struct tree_node* Get_Comparison ()
{
    CHECK_NODE_NUMBER(NULL);
    struct tree_node* ret_node = Get_Polynomial();
    if (ARF) return NULL;
    CHECK_NODE_NUMBER(NULL);

    if ( CUR_NODE.node_type == TYPE__OPERATOR            &&
        (EQUAL_DOUBLE (CUR_NODE.data, Op_Less_eq_number) ||
         EQUAL_DOUBLE (CUR_NODE.data, Op_Less_number)    ||
         EQUAL_DOUBLE (CUR_NODE.data, Op_Equal_number)))
    {
        ARF = 1;
        fprintf (stderr, "\nWEST is always MORE than EAST!!\n"
                         "SYNTAX ERROR on word number %d", Node_Current_Number+1);
        return NULL;
    }

    if ( CUR_NODE.node_type == TYPE__OPERATOR            &&
        (EQUAL_DOUBLE (CUR_NODE.data, Op_More_eq_number) ||
         EQUAL_DOUBLE (CUR_NODE.data, Op_More_number)    ||
         EQUAL_DOUBLE (CUR_NODE.data, Op_Unequal_number)))
    {
        node_add (&CUR_NODE, CASE_OF_LEFT, ret_node, __LINE__);
        ret_node = &CUR_NODE;
        NEXT;
        CHECK_NODE_NUMBER (NULL);
        node_add (ret_node, CASE_OF_RIGHT, Get_Polynomial(), __LINE__);
        if (ARF) return NULL;
        CHECK_NODE_NUMBER(NULL);
    }
    return ret_node;
}

struct tree_node* Get_And ()
{
    CHECK_NODE_NUMBER(NULL);
    struct tree_node* ret_node = Get_Comparison();
    if (ARF) return NULL;
    CHECK_NODE_NUMBER(NULL);
    if (CUR_NODE.node_type == TYPE__OPERATOR && EQUAL_DOUBLE (CUR_NODE.data, Op_And_number))
    {
        node_add (&CUR_NODE, CASE_OF_LEFT, ret_node, __LINE__);
        ret_node = &CUR_NODE;
        NEXT;
        CHECK_NODE_NUMBER(NULL);
        node_add (ret_node, CASE_OF_RIGHT, Get_And(), __LINE__);
        if (ARF) return NULL;
        CHECK_NODE_NUMBER(NULL);
    }
    return ret_node;
}

struct tree_node* Get_Or ()
{
    CHECK_NODE_NUMBER(NULL);
    struct tree_node* ret_node = Get_And();
    if (ARF) return NULL;
    CHECK_NODE_NUMBER(NULL);

    if (CUR_NODE.node_type == TYPE__OPERATOR && EQUAL_DOUBLE (CUR_NODE.data, Op_Or_number))
    {
        node_add (&CUR_NODE, CASE_OF_LEFT, ret_node, __LINE__);
        ret_node = &CUR_NODE;
        NEXT;
        CHECK_NODE_NUMBER(NULL);
        node_add (ret_node, CASE_OF_RIGHT, Get_Or(), __LINE__);
        if (ARF) return NULL;
        CHECK_NODE_NUMBER(NULL);
    }
    return ret_node;
}

struct tree_node* Get_Operator ()
{
    CHECK_NODE_NUMBER (NULL);

    int node_data = CUR_NODE.data;
    switch (CUR_NODE.node_type)
    {
    case TYPE__UNKNOWN:
        {
            struct tree_node* ret_node = &CUR_NODE;
            CUR_NODE.node_type = TYPE__VARIABLE;
            CUR_NODE.data = CUR_NODE.data - Local_Variables_Decrement;
            NEXT;

            CHECK_TYPE(TYPE__OPERATOR);
            CHECK_DATA(Op_Assign_number);
            NEXT;

            node_add (ret_node, CASE_OF_LEFT, Get_Or(), __LINE__);
            CHECK_TYPE(TYPE__KEY_WORD);
            CHECK_DATA(KW_Pt_n_comma_number);
            NEXT;
            return ret_node;
        } break;
    case TYPE__KEY_WORD:
        {
            switch (node_data)
            {
            case KW_In_number:
                {
                    struct tree_node* ret_node = &CUR_NODE;
                    NEXT;
                    CHECK_NODE_NUMBER(NULL);
                    CHECK_TYPE (TYPE__UNKNOWN);
                    if (Names_Array [(int)CUR_NODE.data]._type != TYPE__VARIABLE)
                    {
                        fprintf (stderr, "SYNTAX ERROR: Usage of undeclared name \"%s\"!\n", Names_Array [(int)CUR_NODE.data]._Name);
                        DUMP_NODE((&CUR_NODE));
                        ARF = 1;
                        return NULL;
                    }
                    CUR_NODE.data = CUR_NODE.data - Local_Variables_Decrement;
                    CUR_NODE.node_type = TYPE__VARIABLE;
                    node_add (ret_node, CASE_OF_LEFT, &CUR_NODE, __LINE__);
                    NEXT;
                    CHECK_TYPE (TYPE__KEY_WORD);
                    CHECK_DATA (KW_Pt_n_comma_number);
                    NEXT;
                    return ret_node;
                } break;
            case KW_If_number:
                {
                    struct tree_node* ret_node = &CUR_NODE;
                    NEXT;
                    CHECK_NODE_NUMBER (NULL);
                    CHECK_TYPE (TYPE__KEY_WORD);
                    CHECK_DATA (KW_Opn_brct_number);
                    node_add(ret_node, CASE_OF_LEFT, &CUR_NODE, __LINE__);
                    NEXT;
                    CHECK_NODE_NUMBER (NULL);

                    node_add (ret_node->left_branch, CASE_OF_LEFT, Get_Or(), __LINE__);
                    CHECK_TYPE(TYPE__KEY_WORD);
                    CHECK_DATA (KW_Cls_brct_number);
                    node_add (ret_node->left_branch, CASE_OF_RIGHT, &CUR_NODE, __LINE__);
                    NEXT;
                    CHECK_NODE_NUMBER (NULL);
                    node_add (ret_node->left_branch->right_branch, CASE_OF_LEFT, Get_Operator(), __LINE__);

                    if (CUR_NODE.node_type == TYPE__KEY_WORD && EQUAL_DOUBLE(CUR_NODE.data, KW_Else_number))
                    {
                        NEXT;
                        node_add (ret_node->left_branch->right_branch, CASE_OF_RIGHT, Get_Operator(), __LINE__);
                    }
                    return ret_node;
                } break;

            case KW_While_number:
                {
                    struct tree_node* ret_node = &CUR_NODE;
                    NEXT;
                    CHECK_NODE_NUMBER(NULL);
                    CHECK_TYPE (TYPE__KEY_WORD);
                    CHECK_DATA (KW_Opn_brct_number);
                    node_add(ret_node, CASE_OF_LEFT, &CUR_NODE, __LINE__);
                    NEXT;
                    CHECK_NODE_NUMBER (NULL);

                    node_add (ret_node->left_branch, CASE_OF_LEFT, Get_Or(), __LINE__);
                    CHECK_TYPE (TYPE__KEY_WORD);
                    CHECK_DATA (KW_Cls_brct_number);
                    NEXT;
                    CHECK_NODE_NUMBER (NULL);

                    node_add (ret_node->left_branch, CASE_OF_RIGHT, Get_Operator(), __LINE__);
                    return ret_node;
                } break;

            case KW_Out_number:
                {
                    struct tree_node* ret_node = &CUR_NODE;
                    NEXT;
                    CHECK_NODE_NUMBER(NULL);
                    node_add (ret_node, CASE_OF_LEFT, Get_Or(), __LINE__);
                    if (ARF == 1) return NULL;
                    CHECK_TYPE (TYPE__KEY_WORD);
                    CHECK_DATA (KW_Pt_n_comma_number);
                    NEXT;
                    return ret_node;
                } break;

            case KW_Var_number:
                {
                    struct tree_node* ret_node = &CUR_NODE;
                    NEXT;
                    CHECK_NODE_NUMBER(NULL);

                    CHECK_TYPE(TYPE__UNKNOWN);
                    CHECK_UNKNOWN;
                    CUR_NODE.node_type = TYPE__VARIABLE;
                    Names_Array [(int)CUR_NODE.data]._type = TYPE__VARIABLE;
                    node_add (ret_node, CASE_OF_LEFT, &CUR_NODE, __LINE__);
                    ret_node->left_branch->data = ret_node->left_branch->data - Local_Variables_Decrement;
                    NEXT;
                    CHECK_NODE_NUMBER(NULL);

                    CHECK_TYPE (TYPE__OPERATOR);
                    CHECK_DATA (Op_Assign_number);
                    NEXT;
                    CHECK_NODE_NUMBER(NULL);

                    node_add(ret_node->left_branch, CASE_OF_LEFT, Get_Or(), __LINE__);

                    CHECK_TYPE(TYPE__KEY_WORD);
                    CHECK_DATA(KW_Pt_n_comma_number);
                    NEXT;
                    return ret_node;
                } break;

            case KW_Begin_number:
                {
                    NEXT;
                    CHECK_NODE_NUMBER(NULL);
                    struct tree_node* ret_node = Get_Operator();
                    struct tree_node* cur_oper = ret_node;
                    if (ARF == 1) return NULL;
                    CHECK_NODE_NUMBER(NULL);
                    while ( CUR_NODE.node_type != TYPE__KEY_WORD || CUR_NODE.data != KW_End_number)
                    {
                        node_add(cur_oper, CASE_OF_RIGHT, Get_Operator(), __LINE__);
                        if (ARF) return NULL;
                        CHECK_NODE_NUMBER(NULL);
                        cur_oper = cur_oper->right_branch;
                    }

                    NEXT;
                    return ret_node;
                } break;

            case KW_Return_number:
                {
                    struct tree_node* ret_node = &CUR_NODE;
                    NEXT;
                    CHECK_NODE_NUMBER(NULL);

                    node_add (ret_node, CASE_OF_LEFT, Get_Or(), __LINE__);
                    CHECK_TYPE(TYPE__KEY_WORD);
                    CHECK_DATA(KW_Pt_n_comma_number);
                    NEXT;
                    return ret_node;
                } break;

            default:
                {
                    fprintf (stderr, "PROGRAMM ERROR!!!: Can't work with KeyWord number %d!\n"
                                     "(File %s, line %d)", node_data, __FILE__, __LINE__);
                    ARF = 1;
                    return NULL;
                }
            }
        } break;

    default:
        {
            fprintf (stderr, "PROGRAMM ERROR!!!: Function %s can't work\n"
                             "with types %.4s as with operators!!\n", __PRETTY_FUNCTION__, (char*)&CUR_NODE.node_type);
            ARF = 1;
            return NULL;
        }
    }
}

struct tree_node* Get_Function ()
{
    CHECK_NODE_NUMBER (NULL);

    CHECK_TYPE (TYPE__UNKNOWN);
    CHECK_UNKNOWN;
    CUR_NODE.node_type = TYPE__FUNCTION;
    Names_Array [(int)CUR_NODE.data]._type = TYPE__FUNCTION;
    struct tree_node* function_name_node = &CUR_NODE;
    Local_Variables_Decrement = function_name_node->data;
    struct tree_node* cur_param = function_name_node;
    NEXT;

    CHECK_NODE_NUMBER (NULL);
    CHECK_TYPE (TYPE__KEY_WORD);  //TO DO: CHECK FOR "{"
    NEXT;

    CHECK_NODE_NUMBER (NULL);

    while (CUR_NODE.node_type == TYPE__KEY_WORD && EQUAL_DOUBLE (CUR_NODE.data, KW_Var_number))
    {
        node_add (cur_param, CASE_OF_LEFT, &CUR_NODE, __LINE__);
        cur_param = &CUR_NODE;
        NEXT;

        CHECK_NODE_NUMBER (NULL);
        CHECK_TYPE (TYPE__UNKNOWN);
        CHECK_UNKNOWN;
        CUR_NODE.node_type = TYPE__VARIABLE;
        Names_Array [(int)CUR_NODE.data]._type = TYPE__VARIABLE;
        node_add (cur_param, CASE_OF_RIGHT, &CUR_NODE, __LINE__);
        cur_param->right_branch->data = cur_param->right_branch->data - Local_Variables_Decrement;

        NEXT;
        CHECK_NODE_NUMBER (NULL);
    }

    CHECK_TYPE (TYPE__KEY_WORD); //TO DO: CHECK FOR "}"
    NEXT;

    CHECK_NODE_NUMBER(NULL);
    node_add (function_name_node, CASE_OF_RIGHT, Get_Operator(), __LINE__);


    return function_name_node;
}

struct tree_node* Get_Code ()
{
    CHECK_NODE_NUMBER (NULL);
    CHECK_TYPE (TYPE__KEY_WORD);
    CHECK_DATA (KW_Function_number);

    struct tree_node* ret_node = &CUR_NODE;
    NEXT;
    node_add (ret_node, CASE_OF_LEFT, Get_Function(), __LINE__);
    struct tree_node* current_node = ret_node;
    if (ARF) return NULL;

    while (CUR_NODE.node_type == TYPE__KEY_WORD && Node_Current_Number <= Nodes_Count)
    {
        CHECK_NODE_NUMBER (NULL);
        node_add (current_node, CASE_OF_RIGHT, &CUR_NODE, __LINE__);
        current_node = &CUR_NODE;
        NEXT;
        node_add (current_node, CASE_OF_LEFT, Get_Function(), __LINE__);
        if (ARF) return NULL;
    }

    if (Node_Current_Number != Nodes_Count)
    {
        fprintf (stderr, "WHAT a FIASCO! (in Get_Code)\n");
        PAUSE ("??????\n");
    }
    return ret_node;
}

#endif // TREE_BUILDER_H_INCLUDED
