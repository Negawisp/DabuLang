#include <iostream>
#include <string.h>
#include "Enum.h"


int main ()
{
//#define BREAK_AND_FINISH goto ErrorExit;
#define BREAK_AND_FINISH scanf("%s", currentString);

	FILE* in  = fopen("ACM_code.txt", "r");
	FILE* out = fopen("a.txt",        "w");


	char* currentString = (char*)calloc (COMMAND_LENGTH*2, sizeof(char));
	if (currentString == NULL)
	{
		fprintf (stderr, "In file %s, line %d\n    Could not allocate \"char* currentString\".", __FILE__, __LINE__);
		BREAK_AND_FINISH;
	}
	char* currentCommand  = {};
	char* currentArgument = {};


	label_t* Label = (label_t*)calloc (LABELS_COUNT, (sizeof (label_t)));

	int lineNumber = 0;
	int commandCase  = CMD_NONE;


/*~~~    Finding, checking correctnes of, and remembering labels    ~~~*/
	int labelNumber = 0;
	int  byteNumber = 0;

	while (fscanf (in, "%s", currentString) == 1)
	{
		int curCommLen = strlen (currentString) - 1;
		if (currentString[curCommLen] == ':')
		{
			if (curCommLen == 0)
			{
				printf ("Word #%d \"%s\":\n"
			            "    Is invalid label: label cannot have empty name.)\n", lineNumber, currentString);
				BREAK_AND_FINISH;
			}

			strncpy (Label[labelNumber].Name, currentString, curCommLen);
			Label[labelNumber].Adress = byteNumber;
			labelNumber++;
		}
		else
		{
#define DEF_CMD( name, num, code )                                                   \
			if (strcasecmp(currentString, #name) == 0)                   \
			{                                                            \
				byteNumber++;                                        \
				MOVE_BYTES_##name (in, currentString, &byteNumber);  \
			} else

#include "commands.h"

#undef DEF_CMD
			{}
			lineNumber++;
		}
	}
/*~~~  !!!END!!!    Finding, checking correctnes of and remembering labels    !!!END!!!  ~~~*/

	lineNumber = 0;
	fseek(in, 0, SEEK_SET);


	while (fgets (currentString, COMMAND_LENGTH*2, in))
	{
		if (currentString[0] == '\n')
			continue;
		lineNumber++;


		currentCommand  = strtok (currentString, " \n\r");
		currentCommand  = removeInitialSpaces (currentCommand);
		currentArgument = strtok (NULL, " \n\r");

#define DEF_CMD(name, num, code)                                         \
		if (strcmp(currentCommand, #name) == 0)                          \
		{                                                                \
            if (name##_ASSEMBLE(currentArgument, Label,                  \
			                    lineNumber, labelNumber, out))           \
				BREAK_AND_FINISH;                                        \
		}                                                                \
		else                                                             \

#include "commands.h"

#undef DEF_CMD


		if (currentCommand[strlen (currentCommand)-1] == ':')
		{
			lineNumber;
		}
		else
		{
			printf ("On line #%d:\n"
					"    is invalid command:\n"
					"    \"%s\"", lineNumber, currentCommand);
			BREAK_AND_FINISH;
		}
	}


#undef BREAK_AND_FINISH
ErrorExit:
	if (in)  fclose (in);
	if (out) fclose (out);
	if (currentCommand) free(currentCommand);
	return 0;
}
