#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "settings.h"


#define CASE_OF_LEFT       'LEFT'
#define CASE_OF_RIGHT      'RGHT'
#define CASE_OF_PARENT     'prnt'

#define TYPE__FUNCTION      'cnuF'
#define TYPE__OPERATOR      'repO'
#define TYPE__KEY_WORD      'WyeK'
#define TYPE__VARIABLE      'lbrV'
#define TYPE__CONSTANT      'tsnC'
#define TYPE__NUMERIC       'emuN'
#define TYPE__LINKER        'kniL'
#define TYPE__UNKNOWN       'onuD'

#define EQUAL_DOUBLE( d1, d2 )  ( -0.000001 <= d1-d2 && d1-d2 <= 0.000001 )

#define DOT_DUMP_TREE_BY_ELEM( base_elem )                             \
{                                                                      \
    FILE* dmp = fopen("dmp.gv", "wb");                                 \
    fprintf (dmp, "digraph G {\n");                                    \
    dot_dump_tree (base_elem, dmp,  __LINE__);                         \
    fprintf (dmp, "\n}\n");                                            \
    fclose (dmp);                                                      \
    /*system ("xdot dmp.gv >> xdot.log 2>> xdot.log");   */                \
}

#define DUMP_NODE( node )                                                                      \
{                                                                                              \
    fprintf (stderr, " ~~~ Dump of node on address 0x%p ~~ \n", node);                         \
    fprintf (stderr, "  Data:             %lf (%s)\n", node -> data, get_uncifered_value(node));  \
    fprintf (stderr, "  Good canary:      %.4s\n", (char*)&node->good_canary);                   \
    fprintf (stderr, "  Canary1:          %.4s\n", (char*)&node->canary1);                       \
    fprintf (stderr, "  Canary2:          %.4s\n", (char*)&node->canary2);                       \
    fprintf (stderr, "  Node type:        %.4s\n", (char*)&node->node_type);                     \
    fprintf (stderr, "  Parent pointer:       0x%p\n", node->parent);                           \
    fprintf (stderr, "  Left  branch pointer: 0x%p\n", node->left_branch);                      \
    fprintf (stderr, "  Right branch pointer: 0x%p\n", node->right_branch);                     \
}


#define PAUSE( assert_text )                                        \
{   char t[2] = {}; while (!(t[0] == 'y' || t[0] =='n'))            \
                { printf("Continue?\n[y/n] "); scanf("%s", t); }    \
    if ( t[0] == 'n') assert (!assert_text);                        \
}

#define CHECK_CANARY( node )                                                                         \
{                                                                                                    \
    if ( node -> canary1 != node -> good_canary )                                                    \
    {                                                                                                \
        fprintf (stderr, "ERROR: Detected dead canary1 at function\n"                                \
                         "%s, line %d.\n",  __PRETTY_FUNCTION__, __LINE__);                          \
    }                                                                                                \
    if ( node -> canary2 != node -> good_canary )                                                    \
    {                                                                                                \
        fprintf (stderr, "ERROR: Detected dead canary2 at function\n"                                \
                         "%s, line %d.\n",  __PRETTY_FUNCTION__, __LINE__);                          \
    }                                                                                                \
    if ( ( node -> canary1 != node -> good_canary ) || ( node -> canary2 != node -> good_canary ) )  \
        {DUMP_NODE (node); DOT_DUMP_TREE_BY_ELEM (node);   }                                         \
}

#define CHECK_CREATED( node )                                                        \
if (node == NULL)                                                                    \
{                                                                                    \
    fprintf (stderr, "ERROR: Function %s received NULL\n"                            \
                     "as a node argument on line %d.",  __PRETTY_FUNCTION__, line);  \
    PAUSE ("Look carefully what you return.\n");                                     \
}

#define CHECK_LEFT( node )                                                                        \
{                                                                                                 \
    if ( node -> left_branch )                                                                    \
    {                                                                                             \
        if ( node -> left_branch -> parent != node )                                              \
        {                                                                                         \
            fprintf (stderr, "ERROR: Detected wrong connection with left-branch\n"                \
                             "node at function %s on line %d.\n", __PRETTY_FUNCTION__, __LINE__); \
            DOT_DUMP_TREE_BY_ELEM(node);                                                          \
            DOT_DUMP_TREE_BY_ELEM(node -> left_branch->parent);                                   \
        }                                                                                         \
    }                                                                                             \
}

#define CHECK_RIGHT( node )                                                                        \
{                                                                                                  \
    if ( node -> right_branch )                                                                    \
    {                                                                                              \
        if ( node -> right_branch -> parent != node )                                              \
        {                                                                                          \
            fprintf (stderr, "ERROR: Detected wrong connection with right-branch\n"                \
                             "node at function %s on line %d.\n", __PRETTY_FUNCTION__, __LINE__);  \
            DOT_DUMP_TREE_BY_ELEM(node);                                                           \
            DOT_DUMP_TREE_BY_ELEM(node -> right_branch->parent);                                   \
        }                                                                                          \
    }                                                                                              \
}


#define CHECK_NODE( node )   \
CHECK_CANARY   (node);       \
CHECK_LEFT     (node);       \
CHECK_RIGHT    (node);


#define NULL_CHECK( node, case_ )                                                               \
switch (case_)                                                                                  \
{                                                                                               \
case CASE_OF_LEFT:                                                                              \
    {                                                                                           \
        if (node -> left_branch == NULL)                                                        \
        {                                                                                       \
            fprintf (stderr, "ERROR: Wrong interaction with LEFT-branch pointer\n"              \
                             "of node at function %s on line %d.", __PRETTY_FUNCTION__, __LINE__);  \
            DUMP_NODE (node);                                                                   \
            ARF = 1;                                                                            \
            return 0;                                                                           \
        }                                                                                       \
    } break;                                                                                    \
                                                                                                \
case CASE_OF_RIGHT:                                                                             \
    {                                                                                           \
        if (node -> right_branch == NULL)                                                       \
        {                                                                                       \
            fprintf (stderr, "ERROR: Wrong interaction with RIGHT-branch pointer\n"             \
                             "of node at function %s on line %d.", __PRETTY_FUNCTION__, __LINE__);  \
            DUMP_NODE (node);                                                                   \
            ARF = 1;                                                                            \
            return 0;                                                                           \
        }                                                                                       \
    } break;                                                                                    \
                                                                                                \
case CASE_OF_PARENT:                                                                             \
    {                                                                                             \
        if (node -> parent != NULL)                                                                \
        {                                                                                           \
            fprintf (stderr, "ERROR: Wrong interaction with a parent pointer\n"                      \
                             "of node at function %s on line %d.", __PRETTY_FUNCTION__, __LINE__);    \
            DUMP_NODE (node);                                                                          \
        }                                                                                               \
    } break;                                                                                             \
                                                                                                          \
default:                                                                                                   \
    {                                                                                                       \
        fprintf (stderr, "ERROR: Macro \"NULL_CHECK\" received\n"                                            \
                         "wrong \"case_\" argument in function %s on line %d.", __PRETTY_FUNCTION__, __LINE__);  \
        PAUSE ("Look carefully after your syntax.");                                                         \
    }                                                                                                        \
}


/*Almighty Return Flag*/
int ARF = 0;


struct tree_node
{
    int canary1;

    int                good_canary;
    double             data;
    int                node_type;

    struct tree_node*  parent;
    struct tree_node*  left_branch;
    struct tree_node*  right_branch;

    int canary2;
};
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   NAMES   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~ ARE WORKED OUT HERE! ~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


struct Name
{
    int    _type;
    char   _Name  [MAX_NAME_LENGTH];
};

struct Name     Names_Array [MAX_VAR_COUNT] = {};
int             Names_Count = 0;

int add_name (char* new_name)
{
    int i = 0;

    for (i = 0; i<MAX_VAR_COUNT; i++)
    {
        if (Names_Array[i]._Name[0] == '\0')
        {
            Names_Count++;
            strcat (Names_Array[i]._Name, new_name);
            Names_Array[i]._type = TYPE__UNKNOWN;
            return i;
        }
        if (strcmp (Names_Array[i]._Name, new_name) == 0 )
            return i;
    }

    fprintf (stderr, "SYNTAX ERROR: overflow in number of names:\n"
                     "Tried to make \"%s\" the No%d name.\n", new_name, i);
    ARF = 1;
    return -1;
}


char* get_uncifered_value (const struct tree_node* node);
int dot_dump_tree (struct tree_node* node, FILE* outF, int line);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


struct tree_node* node_construct (struct tree_node* node, double data, int node_type, int canary, int line)
{
    CHECK_CREATED (node);

    node -> data = data;
    node -> node_type = node_type;
    node -> canary1     = canary;
    node -> canary2     = canary;
    node -> good_canary = canary;

    CHECK_CREATED (node);
    CHECK_NODE    (node);
    return node;
}

struct tree_node* node_add (struct tree_node* base, const int connection_case,
                            struct tree_node* new_, int line)
{
    if (ARF) return NULL;
    CHECK_CREATED (base);
    CHECK_CREATED (new_);

    //CHECK_NODE    (base);
    //CHECK_NODE    (new_);


    switch (connection_case)
    {
    case CASE_OF_LEFT:
        {
            base   -> left_branch = new_;
            new_   -> parent      = base;
        }break;

    case CASE_OF_RIGHT:
        {
            base   -> right_branch = new_;
            new_   -> parent       = base;
        }break;

    case CASE_OF_PARENT:
        {
            fprintf(stderr, "ERROR: No, you cannot add a parent. Kids don't make their\n"
                            "parents, live with it. (line %d)", line);
            PAUSE ("lul\n");
        }break;
    }

    CHECK_NODE (base);
    CHECK_NODE (new_);

    return new_;
}


struct tree_node* node_remove (struct tree_node* node, int line)
{
    CHECK_CREATED (node);
    CHECK_NODE    (node);
    NULL_CHECK    (node, CASE_OF_LEFT);
    NULL_CHECK    (node, CASE_OF_RIGHT);

    struct tree_node* parent = node -> parent;
    free (node);
    return parent;
}


int branch_check (struct tree_node* base, int line)
{
    CHECK_NODE (base);

    if (base -> left_branch )
        branch_check (base -> left_branch,  line);
    if (base -> right_branch )
        branch_check (base -> right_branch, line);

    return 0;
}


int branch_remove (struct tree_node* base, int line)
{
    CHECK_NODE(base);
    CHECK_CREATED(base);
    if (base -> left_branch )
        branch_remove (base -> left_branch,  line);
    if (base -> right_branch )
        branch_remove (base -> right_branch, line);

    free (base);
    return 0;
}


char* get_uncifered_value (const struct tree_node* node)
{
    int data = node -> data;
    switch (node -> node_type)
    {
    case TYPE__KEY_WORD:
        {
            #define DEF_KEY_WORD( num, name, meaning, code ) \
            if (data == num)                                 \
                return name;                                 \
            else
            #include "key_words_list.h"
            #undef DEF_KEY_WORD
            { fprintf (stderr, "Ara!\nNo such key word number %d!", data); ARF = 1; return "lul"; }
        } break;

    case TYPE__OPERATOR:
        {
            #define DEF_OPERATOR( num, name, meaning, code ) \
            if (data == num)                                 \
                return name;                                 \
            else
            #include "operators_list.h"
            #undef DEF_OPERATOR
            { fprintf (stderr, "Ara!\nNo such operator number %d!", data); ARF = 1; return "aaa"; }
        } break;

    case TYPE__UNKNOWN:
        {
            if (data < Names_Count)
                return Names_Array[data]._Name;
            else
            {
                fprintf (stderr, "Tried to get name of variable No%d, while\nthere are only %d vars.", data, Names_Count);
                ARF = 1;
                return "aaa";
            }
        } break;
    default:
        return "yep.";
    }
}


    /*~~*//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
    /*~*//**//**//**//**//**//*      X-DOT DUMPING         *//**//**//**//**//**//**//*~*/
    /**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//*~~*/

int dot_tree (struct tree_node* node, FILE* outF, int line)
{
    CHECK_CREATED (node);

    fprintf (outF, "Node%p\n[shape = record, ", node);
    if (node->left_branch == NULL)
        fprintf (outF, "color = green, ");
    else
        fprintf (outF, "color = black, ");

    fprintf     (outF, " label = \"{");
    fprintf     (outF, " <adr> ");

    switch (node->node_type)
    {
    case TYPE__FUNCTION:
        {

        }break;

    case TYPE__UNKNOWN:
        {
            int name_num = node->data;
            fprintf (outF, " %s", Names_Array[name_num]._Name);
        }break;

    case TYPE__CONSTANT:
        {
            fprintf (outF, " %lf", node->data);
        }break;
    }


    fprintf     (outF, "| <left> LEFT | <rght> RIGHT");
    fprintf     (outF, "}\"];\n");


    if (node->left_branch)
    {
        fprintf (outF, "Node%p : <left> -> Node%p : <adr> ", node, node->left_branch);
        fprintf (outF, "[color = blue];\n");
    }

    if (node->right_branch)
    {
        fprintf (outF, "Node%p : <rght> -> Node%p : <adr> ", node, node->right_branch);
        fprintf (outF, "[color = green];\n");
    }



    if (node->left_branch)
        dot_tree (node->left_branch,  outF, line);
    if (node->right_branch)
        dot_tree (node->right_branch, outF, line);

    return 0;
}
#define DOT_TREE_BY_ELEM( base_elem )                                  \
{                                                                      \
    FILE* dmp = fopen("dmp.gv", "wb");                                 \
    fprintf (dmp, "digraph G {\n");                                    \
    dot_tree (base_elem, dmp,  __LINE__);                              \
    fprintf (dmp, "\n}\n");                                            \
    fclose (dmp);                                                      \
    system ("xdot dmp.gv");                                            \
}

int dot_dump_tree (struct tree_node* node, FILE* outF, int line)
{
    CHECK_CREATED (node);
    CHECK_NODE    (node);

    int canary_error = 0;
    if (node->canary1 != node->good_canary || node->canary2 != node->good_canary)
        canary_error = 1;

    int right_error    = 0;
    if (node->right_branch)
        if (node->right_branch->parent != node)
            right_error = 1;

    int left_error     = 0;
    if (node-> left_branch)
        if (node-> left_branch->parent != node)
            left_error = 1;


    fprintf (outF, "Node%p\n[shape = record, ", node);
    if (canary_error)
        fprintf (outF, "color = red, ");
    else
    {
        if (node->left_branch == NULL)
            fprintf (outF, "color = green, ");
        else
            fprintf (outF, "color = black, ");
    }

    fprintf     (outF, " label = \"{");
    fprintf     (outF, " <adr> %p | DATA: %lf ",node, node->data);

    switch (node->node_type)
    {
    case TYPE__KEY_WORD:
        {
            int name_num = node->data;
            if (name_num == KW_Opn_brct_number)
                fprintf (outF, "\\l(Condition)");
            else
            if (name_num == KW_Cls_brct_number)
                fprintf (outF, "\\l(Then / Else)");
            else
    #define DEF_KEY_WORD( num, name, meaning, code )  \
    if (EQUAL_DOUBLE (node->data, num))               \
        fprintf (outF, "\\l(%s)", name);             \
    else
            #include "key_words_list.h"
            { }
    #undef DEF_KEY_WORD                                   //TO DO : CHECK FOR BROKEN KEY_WORDS
        }break;

    case TYPE__OPERATOR:
        {
    #define DEF_OPERATOR( num, name, meaning, code )  \
    if (EQUAL_DOUBLE (node->data, num))               \
        fprintf (outF, "\\l(%s)", name);             \
    else
            #include "operators_list.h"
            { }
    #undef DEF_OPERATOR                                   //TO DO : CHECK FOR BROKEN KEY_WORDS
        }break;

    case TYPE__FUNCTION:
        {
            int name_num = node->data;
            fprintf (outF, "\\l(%s)", Names_Array[name_num]._Name);
        }break;

    case TYPE__VARIABLE:
        {
            fprintf (outF, "\\l(VARIABLE)");
        }break;

    case TYPE__UNKNOWN:
        {
            int name_num = node->data;
            fprintf (outF, "\\l(%s)\\l(This node is wrong)", Names_Array[name_num]._Name);
        }break;
    }

    fprintf     (outF, " | Node_type: %.4s | ",     (char*)&node->node_type);
    fprintf     (outF, " Canary1:   %.4s \\l",      (char*)&node->canary1);
    fprintf     (outF, " Canary2:   %.4s \\l",      (char*)&node->canary2);
    fprintf     (outF, " Should be: %.4s |",        (char*)&node->good_canary);
    fprintf     (outF, " <left> LEFT:  %p | <rght> RIGHT:  %p", node->left_branch, node->right_branch);
    fprintf     (outF, " | <prnt> Parent:  %p",            node->parent);
    fprintf     (outF, "}\"];\n");

    if (node->left_branch)
    {
        fprintf (outF, "Node%p : <left> -> Node%p : <adr> ", node, node->left_branch);
    if ( left_error)
        fprintf (outF, "[color = red];\n");
    else
        fprintf (outF, "[color = blue];\n");
    }

    if (node->right_branch)
    {
        fprintf (outF, "Node%p : <rght> -> Node%p : <adr> ", node, node->right_branch);
    if (right_error)
        fprintf (outF, "[color = red];\n");
    else
        fprintf (outF, "[color = green];\n");
    }


    if (node->parent)
    {
        fprintf (outF, "Node%p : <prnt> -> Node%p : <adr> ", node, node->parent);
        fprintf (outF, "[color = gray];\n");
    }

    if (node->right_branch)
        dot_dump_tree (node->right_branch, outF, line);
    if (node->left_branch)
        dot_dump_tree (node->left_branch,  outF, line);

    return 0;
}



#endif //TREE_H_INCLUDED
