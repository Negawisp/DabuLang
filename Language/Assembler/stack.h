//
// Created by Blind on 10.10.2017.
//


#include <cstring>
#include <stdlib.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* MACRO-CONSTANTS */

#define _DATA_ALLOC_ERROR        /*~~~~~~*/    'alloc'
#define _STACK_OVERFLOW_ERROR    /*~~~~~~*/    'ovFlw'
#define _STACK_OVERDOWN_ERROR    /*~~~~~~*/    'tooLw'
#define _CANARY_ERROR            /*~~~~~~*/    'cnry'
#define _HASH_ERROR              /*~~~~~~*/    'hash'
#define _PRE_PUSH                /*~~~~~~*/    1
#define _PRE_POP                 /*~~~~~~*/    1

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*WORKS FOR ERROR LOGS*/
char* ErrorInfo = NULL;
int   ErrorInfoUsed = 0;

void ExitFree()
{
	if (!ErrorInfo) free(ErrorInfo);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* MACROSES FOR CHECKING PROPER WORK OF STACK FUNCTIONS */

#define _GENERATE_ERROR_REPORT( reportStr )                                                \
{                                                                                          \
	if (!ErrorInfoUsed)                                                                    \
	{                                                                                      \
		std::atexit (ExitFree);                                                            \
		ErrorInfoUsed = 1;                                                                 \
	}                                                                                      \
    { if (ErrorInfo) free(ErrorInfo); ErrorInfo = (char*)calloc (200, sizeof(char)); }     \
	sprintf (ErrorInfo, "| in \"%s.%s()\":\n"                                              \
	                    "| %s"                                                             \
	                    "\n-----------------------\n\n", name, funcName, reportStr);       \
}

#define _DATA_ALLOCATED_OK()                                             \
{ if (data == NULL)                                                      \
	{                                                                    \
		_GENERATE_ERROR_REPORT("Could not allocate data array.")         \
		return _DATA_ALLOC_ERROR;                                        \
	}                                                                    \
}

#define _PUSH_OK(prePush)                                                 \
{                                                                         \
	if (count + prePush > maxCount)                                       \
	{                                                                     \
		{ _GENERATE_ERROR_REPORT("Try to overflow stack has happened.") } \
		return _STACK_OVERFLOW_ERROR;                                     \
	}                                                                     \
}

#define _COUNT_OK(prePop)                                                                      \
{                                                                                              \
	if (count < prePop)                                                                        \
	{                                                                                          \
		if (prePop) { _GENERATE_ERROR_REPORT("Detected try to pop from empty stack."   ); }    \
		else        { _GENERATE_ERROR_REPORT("Detected decreased count of empty stack."); }    \
                                                                                               \
		return _STACK_OVERDOWN_ERROR;                                                          \
	}                                                                                          \
}

#define _CANARIES_OK()                                                                                   \
{                                                                                                        \
	int flag = 0;                                                                                        \
	if (canary1 != canaryValue)                                                                          \
	{                                                                                                    \
		_GENERATE_ERROR_REPORT("Detected unexpected change of initial bytes of data of the stack.");     \
		flag = 1;                                                                                        \
	}                                                                                                    \
	if (canary2 != canaryValue)                                                                          \
	{                                                                                                    \
		_GENERATE_ERROR_REPORT("Detected unexpected change of closing bytes of data of the stack.");     \
		flag = 1;                                                                                        \
	}                                                                                                    \
	if (flag) return _CANARY_ERROR;                                                                      \
}

#define _HASH_OK()                                                          \
{                                                                           \
	if (hash != countHash())                                                \
	{                                                                       \
		_GENERATE_ERROR_REPORT("Spoiled data detected by checking hash.")   \
		                                                                    \
		return _HASH_ERROR;                                                 \
	}                                                                       \
}

/*----------------- ~END~ MACROSES FOR CHECKING PROPER WORK OF STACK FUNCTIONS ~END~ ----------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

template <typename elem_t>
struct stack_t
{
#define private private

private:
	int canary1;
	int     hash;
	size_t  count;
	size_t  maxCount;
	char*   name;
	char*   type;
	elem_t* data;
	int     canaryValue;
	int     sizeOfStack;
	int canary2;


public:

	int Count()
	{
		return count;
	}

	int MaxCount()
	{
		return maxCount;
	}

	int countHash()
	{
		int currentHash = 0;
		for (int i = 0; i < sizeOfStack; i++)
		{
			currentHash += (i%10) * (int)(((char*)(&count))[i]);
		}
		return currentHash;
	}

#define _PRINT_DATA_UNIT()                                   \
{                                                            \
	if (!strcmp (type, "int"))                               \
    {                                                        \
        fprintf (stderr,                                     \
                 "      data[%d]:  %d\n", i, data[i]);       \
    }                                                        \
                                                             \
	if (!strcmp (type, "long"))                              \
    {                                                        \
        fprintf (stderr,                                     \
                 "      data[%d]:  %ld\n", i, data[i]);      \
    }                                                        \
	                                                         \
	if (!strcmp (type, "long long"))                         \
    {                                                        \
        fprintf (stderr,                                     \
                 "      data[%d]:  %lld\n", i, data[i]);     \
    }                                                        \
                                                             \
	if (!strcmp (type, "double"))                            \
    {                                                        \
        fprintf (stderr,                                     \
                 "      data[%d]:  %lg\n", i, data[i]);      \
    }                                                        \
                                                             \
	if (!strcmp (type, "float"))                             \
    {                                                        \
        fprintf (stderr,                                     \
                 "      data[%d]:  %f\n", i, data[i]);       \
    }                                                        \
                                                             \
	if (!strcmp (type, "char*"))                             \
    {                                                        \
        fprintf (stderr,                                     \
                 "      data[%d]:  %s\n", i, data[i]);       \
    }                                                        \
                                                             \
	if (!strcmp (type, "char"))                              \
    {                                                        \
        fprintf (stderr,                                     \
                 "      data[%d]:  %c\n", i, data[i]);       \
    }                                                        \
}
	int Dump()
	{
		char* funcName = "Dump";

		fprintf (stderr, "  ~~~~ DUMP BEGIN ~~~~\n");

		fprintf (stderr, "   Stack %s:\n", name);

		if (data == NULL)
		{
			fprintf (stderr, "   Not constructed yet.");
			return 0;
		}

		_DATA_ALLOCATED_OK ();
		fprintf (stderr, "   Element type:  ~>  %s\n\n", type);

		fprintf (stderr, "   Canary1:       ~>  %d\n", canary1);
		fprintf (stderr, "   Canary2:       ~>  %d\n", canary2);
		fprintf (stderr, "   Required:      ~>  %d\n\n", canaryValue);

		fprintf (stderr, "   Saved hash:    ~>  %d\n", hash);
		fprintf (stderr, "   Current hash:  ~>  %d\n\n", countHash ());

		fprintf (stderr,
		         "   Max count:     ~>  %d\n"
				 "   Count:         ~>  %d\n\n",
		         maxCount, count);

		fprintf (stderr,
		         "   %s.data:\n", name);
		for (int i = 0; i < count; i++)
		{
			_PRINT_DATA_UNIT();
		}

		fprintf (stderr, "  ~~~~~ DUMP END ~~~~~\n\n");
		return 0;
	}
#undef _PRINT_DATA_UNIT

	int Construct (size_t bufferLength, char* Name, char* Type, int CanaryValue)
	{
		char* funcName = "Construct";
		canaryValue = CanaryValue;

		count = 0;
		maxCount = bufferLength;

		data = (elem_t*) calloc (maxCount+1, sizeof (elem_t));
		_DATA_ALLOCATED_OK ();

		canary1 = canaryValue;
		canary2 = canaryValue;

		name = Name;
		type = Type;

		sizeOfStack = 3*sizeof (int) + 2*sizeof (size_t) + 2*sizeof(char*) + sizeof(elem_t);
		hash = countHash ();

		_HASH_OK ();
		_CANARIES_OK ();

		return 0;
	}

	int Destruct ()
	{
		char* funcName = "Destruct";

		_DATA_ALLOCATED_OK ();
		free(data);
		count = 0;
		maxCount = 0;
		name = {};
		type = {};
		canary1 = 0;
		canary2 = 0;
		canaryValue = 0;
		hash = 0;
		sizeOfStack = 0;

		return 0;
	}

	int Push (elem_t value)
	{
		char* funcName = "Push";

		_CANARIES_OK ();
		_HASH_OK ();
		_DATA_ALLOCATED_OK ();
		_PUSH_OK  ( _PRE_PUSH);
		_COUNT_OK (!_PRE_POP );

		data[count++] = value;
		hash = countHash ();

		_CANARIES_OK ();
		_HASH_OK ();
		_DATA_ALLOCATED_OK ();
		_PUSH_OK  (!_PRE_PUSH);
		_COUNT_OK (!_PRE_POP );

		return 0;
	}

	int Pop (elem_t* popTarget)
	{
		char* funcName = "Pop";

		_CANARIES_OK ();
		_HASH_OK ();
		_DATA_ALLOCATED_OK ();
		_PUSH_OK  (!_PRE_PUSH);
		_COUNT_OK ( _PRE_POP );

		*popTarget = data[--count];
		hash = countHash ();

		_CANARIES_OK ();
		_HASH_OK ();
		_DATA_ALLOCATED_OK ();
		_PUSH_OK  (!_PRE_PUSH);
		_COUNT_OK (!_PRE_POP );

		return 0;
	}
};

#undef _DATA_ALLOC_ERROR
#undef _STACK_OVERFLOW_ERROR
#undef _STACK_OVERDOWN_ERROR
#undef _CANARY_ERROR
#undef _HASH_ERROR

#undef _PRE_PUSH
#undef _PRE_POP

#undef _DATA_ALLOCATED_OK
#undef _PUSH_OK
#undef _COUNT_OK
#undef _CANARIES_OK
#undef _HASH_OK

#undef _GENERATE_ERROR_REPORT

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------- MACRO-FUNCTIONS -------------------------------------*/

#define _PRINT_ERROR_INFO                                                        \
{                                                                                \
	fprintf(stderr, "------------------------\nIn file \"%s\", line #%d):\n",    \
	        __FILE__, __LINE__);                                                 \
	fprintf(stderr, "%s", ErrorInfo);                                            \
}


/*              ~~~~~~~~~~~~~~ USER'S FUNCTIONS ~~~~~~~~~~~~                        */

#define MAKE_STRUCT( structName, bufferLength, elementType, cnryVal )            \
	stack_t<elementType> structName = {};                                        \
	structName.Construct (bufferLength, #structName, #elementType, cnryVal);     \


#define PUSH(stack, value)                                                       \
{                                                                                \
	if (stack.Push(value)) { _PRINT_ERROR_INFO; stack.Dump (); }                 \
}


#define POP(stack, void)                                                         \
{                                                                                \
	if (stack.Pop(void))   { _PRINT_ERROR_INFO; stack.Dump (); }                 \
}

/*------------------------ MACRO-FUNCTIONS END ---------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
