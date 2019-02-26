#pragma GCC diagnostic ignored "-Wmultichar"
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "lexic_builder.h"
#include "tree_builder.h"
#include "ACM_builder.h"

int main()
{
    Nodes_Array = calloc (MAX_NODES_COUNT + 1, sizeof (struct tree_node));
    FILE* code_file = fopen("Warchief.orc", "rb");
    fread (Main_String_With_Program_Code, sizeof(char), MAIN_STRING_MAX_LENGTH-1, code_file);

    build_lexic_array();
    if (ARF) return -1;

    struct tree_node* root = Get_Code();
    if (ARF) return -1;

//  DOT_DUMP_TREE_BY_ELEM(root);

    FILE* acm = fopen ("ACM_code.txt", "wb");
    Build_ACM_code (root, acm);
    fclose (acm);

    system ("./Assembler");
    system ("./Processor");

    free (Nodes_Array);
    return 0;
}
