#ifndef  SETTINGS_H_INCLUDED
#define  SETTINGS_H_INCLUDED


#define SINGLE_TABULATION_LENGTH   4
#define SINGLE_TABULATION          "    "
#define MAX_BRANCH_LENGTH          100

#define MAX_NAME_LENGTH            50
#define MAX_VAR_COUNT              50

#define MAX_NODES_COUNT            200
#define MAIN_STRING_MAX_LENGTH     4096

#endif // SETTINGS_H_INCLUDED

enum Key_Words
{
    #define DEF_KEY_WORD( num, name, meaning, code ) \
    meaning##_number = num,
    #include "key_words_list.h"
    #undef DEF_KEY_WORD
    key_words_count
};

enum Key_Operators
{
    #define DEF_OPERATOR( num, name, meaning, code ) \
    meaning##_number = num,
    #include "operators_list.h"
    #undef DEF_OPERATOR
    operators_count
};
