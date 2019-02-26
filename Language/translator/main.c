#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>


struct StructString
{
	char *String;
	int length;
};


char* loadTextFromFile (FILE *file)
{
	assert (file);

	if (fseek (file, 0, SEEK_END) == -1) {
		perror ("Could not fseek in file.");
		return NULL;
	}

	long bufferLength = ftell (file);
	if (bufferLength == -1) {
		perror ("Could not ftell file.");
		return NULL;
	}

	if (fseek (file, 0, SEEK_SET) == -1) {
		perror ("Could not fseek in file.");
		return NULL;
	}

	char *buffer = (char *) calloc (bufferLength + 1, sizeof (char));
	if (buffer == NULL) {
		perror ("Can't allocate buffer");
		goto error;
	}

	fread (buffer, sizeof (char), bufferLength, file);
	if (strlen (buffer) != bufferLength) {
		perror ("Unequal sizes of file and initialString");
		goto error;
	}

	return buffer;

	error:
	free (buffer);
	return NULL;
}


long countStrings (const char *txt)
{
	assert (txt);

	int i = 0;
	long nStrings = 1;

	while (txt[i])
	{
		if (txt[i] == '\n')
			nStrings++;
		i++;
	}

	return nStrings;
}


int makePoem (char *buffer, char **Poem)
{
	assert (buffer);
	assert (Poem);

	int curCharNumber = 0;
	int curStrNumber  = 0;
	Poem[0] = &buffer[0];


	while (buffer[curCharNumber])
	{
		if (buffer[curCharNumber] == '\n')
		{
			buffer[curCharNumber] = '\0';

			if (buffer[curCharNumber-1] == '\r')
			{
				buffer[curCharNumber-1] = '\0';
			}

			Poem[++curStrNumber] = &buffer[++curCharNumber];
		} else curCharNumber++;
	}

	return 0;
}


int makeStructPoem (char *buffer, struct StructString *Poem)
{
	assert (buffer);
	assert (Poem);

	int curCharNumber = 0;
	int curStrNumber  = 0;

	Poem[0].String = &buffer[0];

	while (buffer[curCharNumber])
	{
		if (buffer[curCharNumber] == '\n')
		{
			buffer[curCharNumber] = '\0';

			Poem[curStrNumber].length = &buffer[curCharNumber] - Poem[curStrNumber].String;
			if (buffer[curCharNumber-1] == '\r')
			{
				buffer[curCharNumber-1] = '\0';
				Poem[curStrNumber].length --;
			}

			Poem[++curStrNumber].String = &buffer[++curCharNumber];
		} else curCharNumber++;
	}

	return 0;
}


int isLetter (const char ch)
{
	return ('a' <= ch && ch <= 'z') ||
	       ('A' <= ch && ch <= 'Z') ||
	       ('à' <= ch && ch <= 'ÿ') ||
	       ('À' <= ch && ch <= 'ß');
}

int compareAlphabet (const void* str1, const void* str2)
{
	assert (str1);
	assert (str2);

	int str1CharNumber = 0;
	int str2CharNumber = 0;

#define str1Char (*(char**)str1)[str1CharNumber]
#define str2Char (*(char**)str2)[str2CharNumber]
	while (!isLetter (str1Char) && str1Char != '\0')
		str1CharNumber++;

	while (!isLetter (str2Char) && str2Char != '\0')
		str2CharNumber++;

	return strcasecmp(&str1Char, &str2Char);

#undef str1Char
#undef str2Char
}


int compareRevAlphabet (const void* str1, const void* str2)
{
	assert (str1);    //  ((const struct StructString*)str1)->length
	assert (str2);

	const struct StructString *Str1 = str1;
	const struct StructString *Str2 = str2;

	int str1CharNumber = Str1->length - 1;
	int str2CharNumber = Str2->length - 1;
#define str1Char (Str1 -> String[str1CharNumber])
#define str2Char (Str2 -> String[str2CharNumber])

	for (str1CharNumber; (str1CharNumber >= 0) && !isLetter (str1Char); str1CharNumber--);

	for (str1CharNumber; (str2CharNumber >= 0) && !isLetter (str2Char); str2CharNumber--);

	if (str1CharNumber == 0)
		return 1;

	if (str2CharNumber == 0)
		return -1;


	while  (str1Char == str2Char  &&
	        str1CharNumber-- != 0 &&
	        str2CharNumber-- != 0);

	if (str2CharNumber == 0)
	{
		if (str1CharNumber == 0)
			return 0;
		else return -1;
	}

	if (str1CharNumber == 0)
		return 1;

	return (str1Char < str2Char)? -1 : +1;

#undef str1Char
#undef str2Char
}


int printStrArray (char **str, int nStrings, FILE *file, int os)
{
	assert (str);
	assert (file);

	int i = 0;

	for (i = 0; i < nStrings; i++)
	{
		fputs (str[i], file);

		if (os)
			 fputs ("\r\n", file);
		else fputs (  "\n", file);
	}
	return 0;
}

int printStructArray (struct StructString *str, int nStrings, FILE *file, int os)
{
	assert (str);
	assert (file);

	int i = 0;
	for (i = 0; i < nStrings; i++)
	{
		fputs (str[i].String,  file);

		if (os)
			 fputs ("\r\n", file);
		else fputs (  "\n", file);
	}
	return 0;
}


int getUserChoise (char *questionToUser, char *ret1Answer, char *ret0Answer)
{
	assert (questionToUser);
	assert (ret1Answer);
	assert (ret0Answer);

	int choiseStrLength = 0;
	if (strlen (ret0Answer) > strlen (ret1Answer))
		 choiseStrLength = strlen(ret0Answer);
	else choiseStrLength = strlen(ret1Answer);

	char *choise = calloc (choiseStrLength + 2 , sizeof (char));
	if (choise == NULL)
	{
		perror ("Could not allocate user's choise string.");
	}

	printf ("%s", questionToUser);

	scanf  ("%s", choise);

	while (strcmp (choise, ret1Answer) && strcmp (choise, ret0Answer))
	{
		printf ("Follow the rules! Enter \"%s\" or \"%s\":  ", ret1Answer, ret0Answer);
		scanf  ("%s", choise);
	}

	int returnChoise = (!strcmp (choise, ret1Answer));
	free (choise);

	return returnChoise;
}


int main ()
{
	//Initializing block
	FILE* input  = fopen ("input.txt",  "rb");
	FILE* output = fopen ("output.txt", "wb");
	char* buffer = NULL;
	char** poem  = NULL;
	struct StructString* structPoem = NULL;

	if (input  == NULL)
	{
		perror ( "Could not allocate pointer for input file.");
		goto closingBlock;
	}

	if (output == NULL)
	{
		perror ("Could not allocate pointer for output file.");
		goto closingBlock;
	}

	//Read text from file into buffer
	buffer = loadTextFromFile (input);
	long nStrings = countStrings (buffer);


	char* announseString = "Alphabetical sort of strings in input.txt\n\n"
			"Enter \"alph\" to sort by beginnings of strings\n"
			"Or enter \"end\" to sort by endings of strings.\n\n"
			"Make your choise:  ";

	//Work with user
	int sortChoise = getUserChoise (announseString, "alph", "end");

	if (sortChoise)
	{
		//Alpabetic sort
		poem = calloc (nStrings, sizeof (char*));
		if (poem == NULL)
		{
			perror ("Could not allocate array of pointers.");
			goto closingBlock;
		}
		makePoem (buffer, poem);
		qsort (poem, nStrings-1, sizeof(char*), compareAlphabet);
	}
	else
	{
		//Reverse alphabetic sort
		structPoem = calloc (nStrings, sizeof (struct StructString));
		if (structPoem == NULL)
		{
			perror ("Could not allocate array of structures.");
			goto closingBlock;
		}
		makeStructPoem(buffer, structPoem);
		qsort (structPoem, nStrings-1, sizeof(struct StructString), compareRevAlphabet);
	}

	//Output
	char* winOrLin = "Sort finished. Do you have Unix-type OS or Windows? [win/unix]\n\n";
	int os = getUserChoise(winOrLin, "win", "unix");

	if (sortChoise) printStrArray (poem, nStrings-1, output, os);
	else printStructArray (structPoem, nStrings-1, output, os);

	closingBlock:
	fclose (input);

	fclose (output);

	if (poem)
	{
		free (poem);
	}

	if (structPoem)
	{
		free (structPoem);
	}

	if (buffer)
		free (buffer);

	printf("\n\nThanks for using our soft soft :3\n\n"
			       "Peace!");

	return 0;
}