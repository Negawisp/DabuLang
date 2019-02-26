#ifndef TEXT_WORKS_CPP_DEFINED
#define TEXT_WORKS_CPP_DEFINED

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char* load_file (char* file_name, int* buffer_length)
{
	assert (file_name);	assert (buffer_length);

	FILE* input = fopen (file_name, "rb");	assert (input);

	if (fseek (input, 0, SEEK_END)) assert (0);
	int n_chars = ftell (input);	assert (n_chars != -1);
	if (fseek (input, 0, SEEK_SET)) assert (0);

	*buffer_length = n_chars;
	char*  buffer = (char*)calloc (n_chars + 1, sizeof(char)); assert (buffer);
	fread (buffer, sizeof(char), n_chars, input);

	fclose (input);
	return buffer;
}


int count_words (const char *buffer)
{
	assert (buffer);

	int i = 0;
	int n_words = -1;


	while (buffer[i])
		if (isspace (buffer[i++]))
		{
			while (isspace (buffer[i++]))  {}
			n_words ++;
		}

	return n_words;
}


int split_words (char *buffer, char **words)
{
	assert (buffer);
	assert (words);

	int cur_char_number = 0;
	int cur_word_number = 0;
	
	words[0] = &buffer[0];


	while (buffer[cur_char_number])
	{
		if (isspace (buffer[cur_char_number]))
		{
			buffer[cur_char_number] = '\0';
			while (isspace (buffer[++cur_char_number]))  {}

			words[cur_word_number++] = &buffer[cur_char_number];
		}
		cur_char_number++;
	}

	return 0;
}

bool ends_with (char e_end, char* word)
{
	while (*word != 0) word++;

	if (*(word - 1) == e_end)
	{	*(word - 1) = 0;
		return true;
	}
	else
		return false;
}

#endif
