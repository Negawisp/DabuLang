#include <math.h>
#ifndef LEXIC_BUILDER_H_INCLUDED
#define LEXIC_BUILDER_H_INCLUDED

char Main_String_With_Program_Code[MAIN_STRING_MAX_LENGTH] = {};
int  Main_String_Read_Position     = 0;
#define CH                Main_String_With_Program_Code[Main_String_Read_Position]
#define POS               Main_String_Read_Position
#define NOW( char )      (Main_String_With_Program_Code[Main_String_Read_Position] == char)

struct tree_node* Nodes_Array = NULL;
int               Nodes_Count = 0;
#define NEW_NODE         Nodes_Array [Nodes_Count]

int skip_phrase_spaces ()
{
    int line = __LINE__;
    CHECK_CREATED (Main_String_With_Program_Code);

    while (isspace(CH)) { POS++; if (NOW('\0')) return 1; }
    return 0;
}

int skip (char* what_to_skip, int start_pos)
{
    int line = __LINE__;
    POS = start_pos;
    CHECK_CREATED (what_to_skip);
    CHECK_CREATED (Main_String_With_Program_Code);

    int i = 0;
    while (what_to_skip[i] == CH && what_to_skip[i] != '\0')
    {
        if (NOW ('\0')) return -1;
        POS++;
        i++;
    }

    if (what_to_skip[i] != '\0')
    {
        POS = start_pos;
        return 1;
    }
    else
        return 0;
}

double get_numeric ()
{
    skip_phrase_spaces();
    double numeric = 0;
    do
    {
        numeric = numeric * 10 + CH - '0';
        POS++;
    }
    while ('0' <= CH && CH <= '9');

    if (!NOW('.'))
        return numeric;

    POS++;
    int signs_aft_pnt = 0;

    if (CH < '0' || '9' < CH)
    {
        fprintf (stderr, "LEXIC ERROR: Numbers after point required.\n"
                         "(char No%d in phrase: %c, Function %s.)", POS, CH, __PRETTY_FUNCTION__);
        ARF = 1;
        return -1;
    }

    do
    {
        numeric = numeric*10 + CH - '0';
        signs_aft_pnt++;
        POS++;
    }
    while ('0' <= CH && CH <= '9');
    numeric = numeric / (10*signs_aft_pnt);
    return numeric;
}


int Get_Name ()
{
    char new_name [MAX_NAME_LENGTH] = "";

    skip_phrase_spaces();

    int i = 0;
    while (('a' <= CH && CH <= 'z') || NOW ('_')  ||
           ('A' <= CH && CH <= 'Z') || NOW ('\'') ||
           ('0' <= CH && CH <= '9') || NOW ('!') )
    {
        new_name [i] = CH;
        POS++;
        i++;
    }

    int name_number = add_name (new_name);
    if (ARF)
        return -1;

    return name_number;
}


int build_lexic_array ()
{
    int line = __LINE__;
    CHECK_CREATED (Nodes_Array);
    CHECK_CREATED (Main_String_With_Program_Code);

    skip_phrase_spaces();
    while (!NOW('\0') && Nodes_Count < MAX_NODES_COUNT-1)
    {
        skip_phrase_spaces();
        int start_pos = POS;

        if ('0' <= CH && CH <= '9')
        {
            double numeric = get_numeric();
            if (ARF)
                return 1;
            else
                node_construct (&NEW_NODE, numeric, TYPE__NUMERIC, TYPE__NUMERIC, __LINE__);
        }
    else
#define DEF_OPERATOR( num, name, meaning, code )                                        \
        if (skip (name, start_pos) == 0)                                                \
            node_construct (&NEW_NODE, num, TYPE__OPERATOR, TYPE__OPERATOR, __LINE__);  \
    else
        #include "operators_list.h"
#undef DEF_OPERATOR

#define DEF_KEY_WORD( num, name, meaning, code )                                        \
        if (skip (name, start_pos) == 0)                                               \
            node_construct (&NEW_NODE, num, TYPE__KEY_WORD, TYPE__KEY_WORD, __LINE__);  \
    else
        #include "key_words_list.h"
#undef DEF_KEY_WORD
    {
        POS = start_pos;
        if (('a' <= CH && CH <= 'z') ||
            ('A' <= CH && CH <= 'Z') || NOW('_'))
        {
            int name_number = Get_Name();
            if (ARF)
                return 1;
            node_construct (&NEW_NODE, name_number, TYPE__UNKNOWN, TYPE__UNKNOWN, __LINE__);
        }
        else
        {
            POS = start_pos;
            fprintf (stderr, "LEXIC ERROR: Unexpected lexema on symbol No%d.\n"
                             "Continues as \"%s\".", POS, &CH);
            ARF = 1;
            return 1;
        }
    }
    Nodes_Count++;
    }
    return 0;
}


#endif // LEXIC_BUILDER_H_INCLUDED
