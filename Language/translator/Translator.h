#ifndef TRANSLATOR_H_DEFINED
#define TRANSLATOR_H_DEFINED

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


class Translator_c
{
#define OUT_LOCATION_CHANGED 0
#define  IN_LOCATION_CHANGED 0
	const int default_labels_number = 2;
	void load_default_labels ();

	static const int max_label_length = 20;
	struct Label_t
	{
		char* name;
		int adress;
	};
	int find_n_of_label (char* name);
	int add_label       (char* name, int adress);

	int	n_labels;
	Label_t	labels [100];
	void	load_labels ();


private:

	int	input_length;
	int	cur_word_number;
	int	n_words;
	char*	input_buffer;
	char**	words;

	int	cur_char_number;
	int	output_length;
	char*	where_nops_start;
	char*	cur_char;
	char*	output_buffer;
	
	void	find_nops ();

public:
	 Translator_c () 
	{
		n_labels = 0;
//		labels = {};
		cur_word_number = 0;
	}
	~Translator_c () 
	{
		free (input_buffer);
		free (words);
		free (output_buffer);
		for (int i = 0; i < n_labels; i++)
			free (labels[i].name);
	}

	void	Load ();
	void	Translate ();
	void	Output ();
	
};

#endif
