#include "Translator.h"
#include "Text_works.cpp"



void Translator_c::find_nops ()
{
	int i = 0;
	cur_char = output_buffer;

	while (i != 7)
	{
		while (*(cur_char++) != 'H') {}
		for (i = 0; i < 7; i++)
			if (*(cur_char++) != 'm') break;
	}

	assert ((unsigned char)*cur_char == 0x90);
	cur_char_number  = cur_char - output_buffer;
	where_nops_start = cur_char;
	return;
}

int Translator_c::find_n_of_label (char* name)
{
	assert (name);

	printf ("\nLooking for %s...\n", name);

	int i = 0;
	for (i = 0; i < n_labels; i++)
	{
		printf ("There is %s.", labels[i].name);
		if (strcmp (name, labels[i].name) == 0)
		{
			printf ("Found'cha!\n");
			break;
		}
	}
	return i;
}

int Translator_c::add_label (char* name, int adress)
{
	assert (name);
	int i = find_n_of_label (name);  assert (i == n_labels);

	n_labels++;
	printf ("Adding label %s...\n", name);
	int name_length = strlen (name);  assert (name_length < max_label_length);
	labels[i].name = (char*)calloc (name_length + 1, sizeof(char));
	memcpy (labels[i].name, name, name_length);
	printf ("Label %s added.\n", labels[i].name);
	labels[i].adress = adress;
	return 0;
}


void Translator_c::load_default_labels ()
{
	static_assert (!OUT_LOCATION_CHANGED);
	add_label ("in",  0x00000AD2);
	
	static_assert ( !IN_LOCATION_CHANGED);
	add_label ("out", 0x00000AB0);
}

void Translator_c::load_labels ()
{
	load_default_labels ();
	
	cur_char_number = where_nops_start - output_buffer;

	for (cur_word_number = 0; cur_word_number < n_words; cur_word_number++)
	{
		printf ("%d\n", cur_word_number);
		if (ends_with (':', words[cur_word_number]))
		{
			add_label (words[cur_word_number], cur_char_number);
			printf ("Label No. %d (%s), adress %p", find_n_of_label (words[cur_word_number]), words[cur_word_number], cur_char_number);
		}
		else
		{
		#define DEF_CMD( name,  skip_instructions, code )            \
			if (strcasecmp (#name, words[cur_word_number]) == 0) \
			{	printf ("There was %s \n", #name);\
				skip_instructions;                           \
			} else
			#include "commands.h"
		#undef DEF_CMD
			{ assert (!"Wtf"); }
		}
		printf ("Cur_word_number in the end of loop:%d\n", cur_word_number);
	}

	return;
}


void Translator_c::Load ()
{
	input_buffer = load_file ("asm_text.tr", &input_length);
	output_buffer = load_file ("rustem.dummy", &output_length);
	
	n_words = count_words (input_buffer);

	words = (char**)calloc (n_words + 1, sizeof(char*));
	split_words (input_buffer, words);

	printf ("%d\n", n_words);
	for (int i = 0; i < n_words; i ++)
		printf ("%s\n", words [i]);

	find_nops ();
	load_labels ();
}


void Translator_c::Translate ()
{
	cur_char = where_nops_start;
	cur_char_number = cur_char - output_buffer;
	for (cur_word_number = 0; cur_word_number < n_words; cur_word_number++)
	{
#define DEF_CMD( name, skip_instructions, code )  \
		if (strcasecmp (#name, CUR_WORD) == 0)\
		{                                 \
			printf ("Assemblying %s\n", CUR_WORD);\
			code;\
		}\
		else
	#include "commands.h"
#undef  DEF_CMD
		{}
	}

	
	DB (0xC3);
}


void Translator_c::Output ()
{
	FILE* output = fopen ("gogo", "wb");
	fwrite (output_buffer, sizeof(char), output_length, output);
	fclose (output);
	system ("chmod a+x gogo");
}
