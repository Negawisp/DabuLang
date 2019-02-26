//
// Created by Blind on 22.10.2017.
//
#include <math.h>
#include "stack.h"


#define DEF_CMD( name, num, code ) CMD_##name = num,
enum Commands
{
#include "commands.h"

	CMD_NONE
};

#undef DEF_CMD

#define REGISTERS_COUNT    /*~~>*/         8
#define LABELS_COUNT       /*~~>*/       128
#define COMMAND_LENGTH     /*~~>*/        10
#define MAX_COMMANDS       /*~~>*/     10000
#define DOUBLE_FINENES     /*~~>*/   0.00001

#define STACK_SIZE         /*~~>*/       128
#define RAM_BYTES          /*~~>*/      2048


struct label_t
{
	char Name [COMMAND_LENGTH];
	int  Adress;
};


char* removeInitialSpaces(char* str)
{
	int i = 0;
	while (isspace (str[i]) || str[i] == '\t')
		i++;
	return &str[i];
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    The CPU structure:    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct CPU_t
{
	char             RAM [RAM_BYTES];
	int              command_pointer;
	int              command_count;

	stack_t <double> Stack;
	stack_t <int>    RetStack;
	double           Register [REGISTERS_COUNT+1];
};

void construct_CPU(CPU_t* CPU)
{
	CPU -> command_pointer = 0;
	CPU -> Stack.Construct (STACK_SIZE, "CPU_STACK", "double", 'M9ay');
	CPU -> RetStack.Construct (STACK_SIZE, "CPU_RETSTACK", "int", 'M9ay');

	CPU -> command_count = 0;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~      (END)    The CPU structure    (END)    ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//
~~~~~~~~~~~~~~~~~~~~~~~~~~   SET OF ---> "MAKE LABELS" <---- FUNCTIONS   ~~~~~~~~~~~~~~~~~~~~~~~*/
#define MOVE_BYTES_ARGS FILE* in, char* currentString, int* byteNumber

int MOVE_BYTES_PUSH (MOVE_BYTES_ARGS) {
	fscanf (in, "%s", currentString);
	if (currentString[0] == '[')
	{
		if (currentString[1] != 'x')
			*byteNumber += sizeof (int);
		else
			*byteNumber += sizeof (char);
	}
	else
		if (currentString[0] != 'x')
			*byteNumber += sizeof(double);
		else
			*byteNumber += sizeof(char);
}

int MOVE_BYTES_PUSL (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_PUSM (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_PURM (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_POP  (MOVE_BYTES_ARGS)
{
	int read_pointer = ftell (in);

	fscanf (in, "%s", currentString);
	if (currentString[0] == '[')
		if (currentString[1] != 'x')
			*byteNumber += sizeof(int);
		else
			*byteNumber += sizeof(char);
	else
	if (currentString[0] == 'x')
		*byteNumber += sizeof(char);
	else
		fseek(in, read_pointer, SEEK_SET);
}

int MOVE_BYTES_POPL (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_POPM (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_PORM (MOVE_BYTES_ARGS)
{

}


#define JMP_MOVE_BYTES_TMPLT          \
{                                     \
	fscanf (in, "%s", currentString); \
	*byteNumber += sizeof(int);       \
}

int MOVE_BYTES_JMP (MOVE_BYTES_ARGS)
{
	JMP_MOVE_BYTES_TMPLT;
}

int MOVE_BYTES_JE (MOVE_BYTES_ARGS)
{
	JMP_MOVE_BYTES_TMPLT;
}

int MOVE_BYTES_JNE (MOVE_BYTES_ARGS)
{
	JMP_MOVE_BYTES_TMPLT;
}

int MOVE_BYTES_JA (MOVE_BYTES_ARGS)
{
	JMP_MOVE_BYTES_TMPLT;
}

int MOVE_BYTES_JAE (MOVE_BYTES_ARGS)
{
	JMP_MOVE_BYTES_TMPLT;
}

int MOVE_BYTES_JB (MOVE_BYTES_ARGS)
{
	JMP_MOVE_BYTES_TMPLT;
}

int MOVE_BYTES_JBE (MOVE_BYTES_ARGS)
{
	JMP_MOVE_BYTES_TMPLT;
}

int MOVE_BYTES_CALL (MOVE_BYTES_ARGS)
{

	JMP_MOVE_BYTES_TMPLT;
}

int MOVE_BYTES_ADD (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_SUB (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_MUL (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_DIV (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_SQRT (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_RET (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_IN (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_OUT (MOVE_BYTES_ARGS)
{

}

int MOVE_BYTES_CAT (MOVE_BYTES_ARGS)
{

}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~   SET OF ---> ASSEMBLING <---- FUNCTIONS   ~~~~~~~~~~~~~~~~~~~~~~~~*/



/*    This macro is used to quick-edit set of arguments for each function.  */
#define ASSEMBLE_ARGUMENTS                             \
char* currentArgument, label_t* Label,                 \
int lineNumber, int labelNumber, FILE* out


#define PRINT_ERROR( command ) {                                       \
	printf ("ERROR: On line #%d: %s is an illegal argument for %s",    \
	        lineNumber, currentArgument, #command);                    \
	return 1;                                                          \
}



int PUSH_ASSEMBLE (ASSEMBLE_ARGUMENTS)
{

	long int reg_number = '!used';

	int RAM_case = 0;
	if (currentArgument[0] == '[')
	{
		RAM_case = 1;
	}

	if (currentArgument[RAM_case] == 'x')
	{
		fprintf (out, "%d ", CMD_PUSL + RAM_case*2);

		if (isdigit (currentArgument[1 + RAM_case]))
		{
			reg_number = strtol (currentArgument + 1 + RAM_case, NULL, 10);
			if (1 <= reg_number && reg_number <= REGISTERS_COUNT)
			{
				fprintf (out, "%ld ", reg_number);
				return 0;
			}
		}
	}


	if (RAM_case)
	{
		if (isdigit (currentArgument[1]))
		{
			int decimal_val_of_arg = atof (currentArgument + 1);
			fprintf (out, "%d ", CMD_PUSM);
			fprintf (out, "%d ", decimal_val_of_arg);
			return 0;
		}
	}
	else
		if (isdigit (currentArgument[0]) || currentArgument[0] == '-' || currentArgument[0] == '+')
		{
			double decimal_val_of_arg = atof (currentArgument);
			fprintf (out, "%d ", CMD_PUSH);
			fprintf (out, "%lf ", decimal_val_of_arg);
			return 0;
		}

	PRINT_ERROR (PUSH);
}


int PUSL_ASSEMBLE (ASSEMBLE_ARGUMENTS)
{
	printf("ERROR: Command on line #%d (PUSL) cannot be used by user.", lineNumber);
	return 1;
}


int PUSM_ASSEMBLE (ASSEMBLE_ARGUMENTS)
{
	printf("ERROR: Command on line #%d (PUSM) cannot be used by user.", lineNumber);
	return 1;
}


int PURM_ASSEMBLE (ASSEMBLE_ARGUMENTS)
{
	printf("ERROR: Command on line #%d (PURM) cannot be used by user.", lineNumber);
	return 1;
}


int POP_ASSEMBLE (ASSEMBLE_ARGUMENTS)
{
	long int decimal_val_of_arg = '!used';

	if (currentArgument == NULL)
	{
		fprintf (out, "%d ", CMD_POP);
		return 0;
	}

	int RAM_case = 0;
	if (currentArgument[0] == '[')
		RAM_case = 1;

	if (currentArgument[RAM_case] == 'x') {
		fprintf (out, "%d ", CMD_POPL + RAM_case*2);

		if (isdigit (currentArgument[1 + RAM_case]))
		{
			decimal_val_of_arg = strtol (currentArgument + 1 + RAM_case, NULL, COMMAND_LENGTH);
			if (1 <= decimal_val_of_arg && decimal_val_of_arg <= REGISTERS_COUNT)
			{
				fprintf (out, "%ld ", decimal_val_of_arg);
				return 0;
			}
		}
	}

	if (RAM_case)
	{
		if (isdigit (currentArgument[1]))
		{
			int decimal_val_of_arg = atof (currentArgument + 1);
			fprintf (out, "%d ", CMD_POPM);
			fprintf (out, "%d ", decimal_val_of_arg);
			return 0;
		}
	}

	else PRINT_ERROR (POP);
}


int POPL_ASSEMBLE (ASSEMBLE_ARGUMENTS)
{
	printf("ERROR: Command on line #%d (POPL) cannot be used by user.", lineNumber);
	return 1;
}


int POPM_ASSEMBLE (ASSEMBLE_ARGUMENTS)
{
	printf("ERROR: Command on line #%d (POPM) cannot be used by user.", lineNumber);
	return 1;
}


int PORM_ASSEMBLE (ASSEMBLE_ARGUMENTS)
{
	printf("ERROR: Command on line #%d (PORM) cannot be used by user.", lineNumber);
	return 1;
}



#define JMP_TMPLT(name)                                             \
{                                                                   \
	for (int i = 0; i < labelNumber; i ++)                          \
	{                                                               \
		if (strcmp(currentArgument, Label[i].Name) == 0)            \
		{                                                           \
			fprintf(out, "%d ", CMD_##name);                        \
			fprintf(out, "%d ", Label[i].Adress);                   \
			return 0;                                               \
		}                                                           \
	}                                                               \
                                                                    \
	PRINT_ERROR (name);                                             \
}

int JMP_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	JMP_TMPLT (JMP);
}

int JE_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	JMP_TMPLT (JE);
}

int JNE_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	JMP_TMPLT (JNE);
}

int JA_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	JMP_TMPLT (JA);
}

int JAE_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	JMP_TMPLT (JAE);
}

int JB_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	JMP_TMPLT (JB);
}

int JBE_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	JMP_TMPLT (JBE);
}

int CALL_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	JMP_TMPLT (CALL);
}




int ADD_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	fprintf(out, "%d ", CMD_ADD);
	return 0;
}


int SUB_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	fprintf(out, "%d ", CMD_SUB);
	return 0;
}


int MUL_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	fprintf(out, "%d ", CMD_MUL);
	return 0;
}


int DIV_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	fprintf(out, "%d ", CMD_DIV);
	return 0;
}

int SQRT_ASSEMBLE(ASSEMBLE_ARGUMENTS)
{
	fprintf (out, "%d ", CMD_SQRT);
	return 0;
}


int RET_ASSEMBLE (ASSEMBLE_ARGUMENTS)
{
	fprintf(out, "%d ", CMD_RET);
	return 0;
}

int IN_ASSEMBLE  (ASSEMBLE_ARGUMENTS)
{
	fprintf(out, "%d ", CMD_IN);
	return 0;
}

int OUT_ASSEMBLE  (ASSEMBLE_ARGUMENTS)
{

	fprintf(out, "%d ", CMD_OUT);
	return 0;
}

int CAT_ASSEMBLE  (ASSEMBLE_ARGUMENTS)
{

	fprintf(out, "%d ", CMD_CAT);
	return 0;
}

                              //\\                                        //\\
                             //  \\                                      //  \\
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~      (END) set of assembling functions (END)     ~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*                         -------------------------------------------------------
                          | Relax, dear reviewer, and behold this nice BEEBEEKA! |
                          -------------------------------------------------------

                                           _..-------++._                               БИЬИКА
	 БИБИКА                 	       _.-'/ |      _||  \"--._         БИБИКА
             БИБИКА              __.--'`._/_\j_____/_||___\    `----.
                           _.--'_____    |          \     _____    /           БИБИИИИКАААА
           БИБИКА        _j    /,---.\   |        =o |   /,---.\   |_
                        [__]==// .-. \\==`===========/==// .-. \\=[__]              БИБИКА
      БиБиКА              `-._|\ `-' /|___\_________/___|\ `-' /|_.'
                                `---'                     `---'
*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~        SET OF ---> LOADING <--- FUNCTIONS   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


#define LOAD_ARGUMENTS FILE* codeFile, CPU_t* CPU

#define STD_LOAD()                                               \
CPU -> command_pointer += 1;                                     \
fscanf (codeFile, "%p", (void*)&(CPU -> RAM[CPU -> command_pointer]));

void PUSH_LOAD (LOAD_ARGUMENTS)
{
	CPU -> command_pointer += 1;

	fscanf (codeFile, "%lf", (double*) &(CPU -> RAM[CPU -> command_pointer]));
	CPU -> command_pointer += sizeof(double) - 1;
}

void PUSL_LOAD (LOAD_ARGUMENTS)
{
	STD_LOAD ();
}

void PUSM_LOAD (LOAD_ARGUMENTS)
{
	CPU -> command_pointer += 1;

	fscanf (codeFile, "%d", (int*) &(CPU -> RAM[CPU -> command_pointer]));
	CPU -> command_pointer += sizeof(int) - 1;
}

void PURM_LOAD (LOAD_ARGUMENTS)
{
	STD_LOAD ();
}

void POP_LOAD (LOAD_ARGUMENTS)
{

}

void POPL_LOAD (LOAD_ARGUMENTS)
{
	STD_LOAD ();
}

void POPM_LOAD (LOAD_ARGUMENTS)
{
	CPU -> command_pointer += 1;

	fscanf (codeFile, "%d", (int*) &(CPU -> RAM[CPU -> command_pointer]));
	CPU -> command_pointer += sizeof(int) - 1;
}

void PORM_LOAD (LOAD_ARGUMENTS)
{
	STD_LOAD ();
}


#define LOAD_JMP_TMPLT()                                                \
CPU -> command_pointer += 1;                                            \
fscanf (codeFile, "%d", (int*) &(CPU -> RAM[CPU -> command_pointer]));  \
CPU -> command_pointer += sizeof(int) - 1;

void JMP_LOAD (LOAD_ARGUMENTS)
{
	LOAD_JMP_TMPLT();
}

void JE_LOAD (LOAD_ARGUMENTS)
{
	LOAD_JMP_TMPLT();
}

void JNE_LOAD (LOAD_ARGUMENTS)
{
	LOAD_JMP_TMPLT();
}

void JA_LOAD (LOAD_ARGUMENTS)
{
	LOAD_JMP_TMPLT();
}

void JAE_LOAD (LOAD_ARGUMENTS)
{
	LOAD_JMP_TMPLT();
}

void JB_LOAD (LOAD_ARGUMENTS)
{
	LOAD_JMP_TMPLT();
}

void JBE_LOAD (LOAD_ARGUMENTS)
{
	LOAD_JMP_TMPLT();
}

void CALL_LOAD (LOAD_ARGUMENTS)
{
	LOAD_JMP_TMPLT();
}

void ADD_LOAD (LOAD_ARGUMENTS)
{

}

void SUB_LOAD (LOAD_ARGUMENTS)
{

}

void MUL_LOAD (LOAD_ARGUMENTS)
{

}

void DIV_LOAD (LOAD_ARGUMENTS)
{

}

void SQRT_LOAD (LOAD_ARGUMENTS)
{

}

void RET_LOAD (LOAD_ARGUMENTS)
{

}

void IN_LOAD (LOAD_ARGUMENTS)
{

}

void OUT_LOAD (LOAD_ARGUMENTS)
{

}

void CAT_LOAD (LOAD_ARGUMENTS)
{

}

/*~~~~~~~~~~~~~~~~~~~~~~~~      (END)      Load functions     (END)    ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//
~~~~~~~~~~~~~~~~~~~~~~~~~~~~   SET OF --->   COMMAND  <---- FUNCTIONS   ~~~~~~~~~~~~~~~~~~~~~~~~*/


#define COMMAND__ARGUMENTS CPU_t* CPU

void DO_PUSH (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	PUSH (CPU->Stack, *(double*) &(CPU -> RAM[CPU -> command_pointer]));
	CPU -> command_pointer += sizeof(double) - 1;
}


void DO_PUSL (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	PUSH (CPU -> Stack, CPU -> Register [CPU -> RAM [CPU -> command_pointer]]);
}

void DO_PUSM (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	int argument_adress = *(int*) &( CPU -> RAM [CPU -> command_pointer] );
	PUSH (CPU -> Stack, *(double*) &(CPU -> RAM [argument_adress]));
	CPU -> command_pointer += sizeof(int) - 1;
}

void DO_PURM (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	int register_number = CPU -> RAM [CPU -> command_pointer];
	int argument_adress = (int) (CPU -> Register [register_number]);
	PUSH (CPU -> Stack, *(double*) &(CPU -> RAM [argument_adress]));
}

void DO_POP  (COMMAND__ARGUMENTS)
{
	double temp = 0;
	POP (CPU -> Stack, &temp);
}

void DO_POPL (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	POP (CPU -> Stack, &(CPU -> Register[CPU -> RAM[CPU -> command_pointer]]));
}

void DO_POPM (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	int argument_adress = *(int*) &(CPU -> RAM[CPU -> command_pointer]);
	POP (CPU -> Stack,  (double*) &(CPU -> RAM[argument_adress]));
	CPU -> command_pointer += sizeof(int) - 1;
}

void DO_PORM (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	int reg_number = CPU -> RAM[CPU -> command_pointer];
	int argument_adress = (int) (CPU -> Register[reg_number]);
	POP (CPU -> Stack,  (double*) &(CPU -> RAM[argument_adress]));
}

void DO_JMP  (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	CPU -> command_pointer = *(int*) &(CPU -> RAM[CPU -> command_pointer]);
	CPU -> command_pointer --;
}

void DO_JE  (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	int jump_pointer = *(int*) &(CPU -> RAM[CPU -> command_pointer]);
	double a = 0, b = 0;

	POP (CPU -> Stack, &b);
	POP (CPU -> Stack, &a);

	if (DOUBLE_FINENES >= a - b && a - b >= -DOUBLE_FINENES)
	{
		CPU -> command_pointer = jump_pointer;
		CPU -> command_pointer --;
	}
	else CPU -> command_pointer += sizeof(int) - 1;
}

void DO_JNE  (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	int jump_pointer = *(int*) &(CPU -> RAM[CPU -> command_pointer]);
	double a = 0, b = 0;

	POP (CPU -> Stack, &b);
	POP (CPU -> Stack, &a);

	if (DOUBLE_FINENES < a - b || a - b < -DOUBLE_FINENES)
	{
		CPU -> command_pointer = jump_pointer;
		CPU -> command_pointer --;
	}
	else CPU -> command_pointer += sizeof(int) - 1;
}

void DO_JA  (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	int jump_pointer = *(int*) &(CPU -> RAM[CPU -> command_pointer]);
	double a = 0, b = 0;

	POP (CPU -> Stack, &b);
	POP (CPU -> Stack, &a);

	if (a - b < -DOUBLE_FINENES)
	{
		CPU -> command_pointer = jump_pointer;
		CPU -> command_pointer --;
	}
	else CPU -> command_pointer += sizeof(int) - 1;
}

void DO_JAE  (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	int jump_pointer = *(int*) &(CPU -> RAM[CPU -> command_pointer]);
	double a = 0, b = 0;

	POP (CPU -> Stack, &b);
	POP (CPU -> Stack, &a);

	if (a - b <= DOUBLE_FINENES)
	{
		CPU -> command_pointer = jump_pointer;
		CPU -> command_pointer --;
	}
	else CPU -> command_pointer += sizeof(int) - 1;
}

void DO_JB  (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	int jump_pointer = *(int*) &(CPU -> RAM[CPU -> command_pointer]);
	double a = 0, b = 0;

	POP (CPU -> Stack, &b);
	POP (CPU -> Stack, &a);

	if (a - b > DOUBLE_FINENES)
	{
		CPU -> command_pointer = jump_pointer;
		CPU -> command_pointer --;
	}
	else CPU -> command_pointer += sizeof(int) - 1;
}

void DO_JBE  (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;
	int jump_pointer = *(int*) &(CPU -> RAM[CPU -> command_pointer]);
	double a = 0, b = 0;

	POP (CPU -> Stack, &b);
	POP (CPU -> Stack, &a);

	if (a - b >= -DOUBLE_FINENES)
	{
		CPU -> command_pointer = jump_pointer;
		CPU -> command_pointer --;
	}
	else CPU -> command_pointer += sizeof(int) - 1;
}

void DO_CALL (COMMAND__ARGUMENTS)
{
	CPU -> command_pointer ++;

	PUSH (CPU -> RetStack, CPU -> command_pointer + sizeof(int) - 1);

	CPU -> command_pointer = *(int*) &(CPU -> RAM[CPU -> command_pointer]);
	CPU -> command_pointer --;
}

void DO_RET  (COMMAND__ARGUMENTS)
{
	POP (CPU -> RetStack, &(CPU -> command_pointer));
}

void DO_ADD  (COMMAND__ARGUMENTS)
{
	double a = 0, b = 0;

	POP (CPU -> Stack, &b);
	POP (CPU -> Stack, &a);

	a = a + b;
	PUSH (CPU -> Stack, a);
}

void DO_SUB  (COMMAND__ARGUMENTS)
{
	double a = 0, b = 0;

	POP (CPU -> Stack, &b);
	POP (CPU -> Stack, &a);

	a = a - b;
	PUSH (CPU -> Stack, a);
}

void DO_MUL  (COMMAND__ARGUMENTS)
{
	double a = 0, b = 0;

	POP (CPU -> Stack, &b);
	POP (CPU -> Stack, &a);

	a = a * b;
	PUSH (CPU -> Stack, a);
}

void DO_DIV  (COMMAND__ARGUMENTS)
{
	double a = 0, b = 0;

	POP (CPU -> Stack, &b);
	POP (CPU -> Stack, &a);

	a = a / b;
	PUSH (CPU -> Stack, a);
}

void DO_SQRT (COMMAND__ARGUMENTS)
{
	double a = 0;

	POP (CPU -> Stack, &a);

	a = sqrt (a);
	PUSH (CPU -> Stack, a);
}

void DO_IN   (COMMAND__ARGUMENTS)
{
	double a = 0;

	scanf("%lf", &a);
	PUSH (CPU -> Stack, a);
}

void DO_OUT  (COMMAND__ARGUMENTS)
{
	double a = 0;
	POP (CPU -> Stack, &a)
	PUSH (CPU -> Stack, a);

	printf("%lf\n", a);
}

void DO_CAT  (COMMAND__ARGUMENTS)
{
	printf(""
	               "                                                            .-----.\n"
			       "                                                          ,' __/|_ `.\n"
			       "                                                         ,    {~)    :\n"
			       "                                        /:\\              :   //~_)  _:__\n"
			       "                            __,---.__ /::::\\             : ~~~/' \\~~====\n"
			       "                           `-.__     \\:::::/              :   ~;;~  .'\n"
			       "                             ;;:\\--.__`--.--._             `._____.'\n"
			       "                           ,;;'` `    `--.__  `-._\n"
			       "                           `,  ,\\       /,  `--.__;\n"
			       "                           <   (o) ___ (o)   >\n"
			       "                          <        \\:/        >\n"
			       "                           <     ._,\"._,     >\n"
			       "                   _.---._  `-.    ~~~    .-'\n"
			       "                 .'._.--. `.   `~:~~~~~~:'\n"
			       "                 `-'     `. `.  :        :\n"
			       "                          :__: :________  :___\n"
			       "                      ;'xx:XXxxxxxxxxxx:xxxXXX:xx:\n"
			       "                    :::xx:XXXX:xxxxxxxxx:XXXXXX:xxx:.\n"
			       "                   ::xxx:XXX/X;xxxxxxxxxx:XXXXXX:xxx:.\n"
			       "           |||    ::xxx:XXX// xxxxxxxxxx// XXXXXX:xxx:.         []\n"
			       "         ||||||  ||xxxx:XX//   xxxxxxxx//   XXXXXX:xx||     .:||:|:||.\n"
			       "    ___    |||   ||xxx:XX//  0  xxxxxx// 0   XXXXX:xx||    .:||^:|:^||.\n"
			       "  ,'   `.. |||   `::xx:XXXXX:xxxx/ \\xxxxx:XXXXXXXX:xx:'    ::|:::V:::|:\n"
			       "  |     || |||    `::xx:XXXXX:xxx___Xxxx:XXXXXXXX:xx:'     `::|UUUUU:|'\n"
			       "  |R.I.P|| |||     `::xx:XXXXXxxxxxxxxxxxXXXXXXX:x::'       `::|::::|'\n"
			       "  |     ||\"\"\"\"\"      `:xx:XX \\/  \\/  \\/ \\/XXXXX:xx:'       \"\"\"\"\"\"\"\"\"\"\"\"\n"
			       " \"\"\"\"\"\"\"\"\"             `.x:XXXXxxxxxxxxxxXXXXxx;''\n"
			       "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"   ~~~~~~~~~~~~~~~~~~~~~~   \"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n"
	);


}
