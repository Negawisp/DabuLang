#define DB( value )                                   \
	{	size_t temp = value;                  \
		memcpy (this->cur_char, &temp, 1);    \
		cur_char += 1;	cur_char_number += 1; \
	}

#define DW( value )                                   \
	{	size_t temp = value;                  \
		memcpy (this->cur_char, &temp, 2);    \
		cur_char += 2;	cur_char_number += 2; \
	}

#define DD( value )                                   \
	{	size_t temp = value;                  \
		memcpy (this->cur_char, &temp, 4);    \
		cur_char += 4;	cur_char_number += 4; \
	}

#define DQ( value )                                   \
	{	size_t temp = value;                  \
		memcpy (this->cur_char, &temp, 8);    \
		cur_char += 8;	cur_char_number += 8; \
	}

#define NEXT cur_word_number++;
#define CUR_WORD words[cur_word_number]
#define GET_INT int _temp_int = 0; sscanf (words[cur_word_number], "%d", &_temp_int);

DEF_CMD( PUSH,	{ NEXT; if (CUR_WORD[0] == '[')
			{	if 	(CUR_WORD[1] == 'x' && CUR_WORD[3] == ']' && '1' <= CUR_WORD[2] && CUR_WORD[2] <= '8' )
				{ 	cur_char_number += 3; }
				
				else if (isdigit(CUR_WORD[1]))
				{	cur_char_number += 7; }
			} else
			{
				if	(isdigit (CUR_WORD[0]))
				{	cur_char_number += 5; }

				else if (CUR_WORD[0] == 'x')
				{	cur_char_number += 2; }
			}
		},

		{ NEXT;	if (CUR_WORD[0] == '[')
			{	if 	(CUR_WORD[1] == 'x' && CUR_WORD[3] == ']' && '1' <= CUR_WORD[2] && CUR_WORD[2] <= '8' )
				{ 	DW (0xFF41); DB (CUR_WORD[2] - '1' + 0x30); }
				
				else if (isdigit(CUR_WORD[1]))
				{	int adress = atof (CUR_WORD + 1);
					DW (0x34FF); DB (0x25); DD (adress);
				}
			} else
			{
				if	(isdigit (CUR_WORD[0]))
				{	int number = atof (CUR_WORD);
					DB (0x68); DD (number);
				}

				else if (CUR_WORD[0] == 'x')
				{	DB (0x41); DB (CUR_WORD[1] - '1' + 0x50); }
			}
		} )



DEF_CMD( POP,	{ NEXT; if (CUR_WORD[0] == '[')
			{	if 	(CUR_WORD[1] == 'x' && CUR_WORD[3] == ']' && '1' <= CUR_WORD[2] && CUR_WORD[2] <= '8' )
				{ 	cur_char_number += 3; }
				
				else if (isdigit(CUR_WORD[1]))
				{	cur_char_number += 7; }
			} else
			{
				if (CUR_WORD[0] == 'x')
				{	cur_char_number += 2; }
				else
				{	cur_char_number += 1; cur_word_number--; }
			}
		},
     
     		{ NEXT; if (CUR_WORD[0] == '[')
			{	if 	(CUR_WORD[1] == 'x' && CUR_WORD[3] == ']' && '1' <= CUR_WORD[2] && CUR_WORD[2] <= '8' )
				{ 	DW (0x8F41); DB (CUR_WORD[2] - '1' + 0x00); }
				
				else if (isdigit(CUR_WORD[1]))
				{	int adress = atof (CUR_WORD + 1);
					DW (0x048F); DB (0x25); DD (adress);
				}
			} else
			{
				if (CUR_WORD[0] == 'x')
				{	DB (0x41); DB (CUR_WORD[1] - '1' + 0x58); }
				else
				{	DB (0x58); cur_word_number--; }
			}
		}  )


DEF_CMD( JMP,	{ NEXT; cur_char_number += 2; },
      		{ NEXT; int label_number = find_n_of_label (CUR_WORD);  assert (label_number < n_labels);
			int label_adress = labels[label_number].adress; assert (label_adress);
			size_t delta = (label_adress - cur_char_number - 2);
			DB (0xEB); DB (delta);
		} )
/*
DEF_CMD( JE,   12,  DEF_JE   )
DEF_CMD( JNE,  13,  DEF_JNE  )
DEF_CMD( JA,   14,  DEF_JA   )
DEF_CMD( JAE,  15,  DEF_JAE  )
DEF_CMD( JB,   16,  DEF_JB   )
DEF_CMD( JBE,  17,  DEF_JBE  )
*/


DEF_CMD( ADD,	{ 	cur_char_number += 6; },
		{	DD (0x01485B58); DW (0x50D8); } )
DEF_CMD( SUB,	{	cur_char_number += 6; },
		{	DD (0x2948585B); DW (0x50D8); } )

DEF_CMD( MUL,	{	cur_char_number += 44; }, 
		{	DB (0x8F); DW (0x2504); DQ (0x007A6097);
			DB (0xDF); DW (0x252C); DQ (0x007A6097);
			DB (0x8F); DW (0x2504); DQ (0x007A6097);
			DB (0xDF); DW (0x252C); DQ (0x007A6097);
			DW (0xC9DE);
			DB (0xDF); DW (0x253C); DQ (0x007A6097);
			DB (0xFF); DW (0x2534); DQ (0x007A6097);
		} )
/*
DEF_CMD( DIV,  24,  DEF_DIV  )
DEF_CMD( SQRT, 25,  DEF_SQRT )
*/

DEF_CMD( CALL, 	{ NEXT;	cur_char_number += 5; },
      		{ NEXT; int label_number = find_n_of_label (CUR_WORD);  assert (label_number < n_labels);
			int label_adress = labels[label_number].adress; assert (label_adress);
			size_t delta = (label_adress - cur_char_number - 5);
			DB (0xE8); DD (delta);
		} )
DEF_CMD( RET,  	{ cur_char_number += 1; },
     		{ DB (0xC3); } )

DEF_CMD( IN,	{ cur_char_number += 12;},
		{	int  in_adress = labels[1].adress; assert (in_adress);
			size_t delta = ( in_adress - cur_char_number - 5);
			DB (0xE8); DD (delta); DW (0x34FF); DB (0x25); DD (0x007A6097);
		} )
DEF_CMD( OUT,  	{ cur_char_number += 6; }, 
		{	int out_adress = labels[0].adress; assert (out_adress);
			size_t delta = (out_adress - cur_char_number - 6);
			DW (0xE85E); DD (delta);
		} )
//DEF_CMD( CAT,  33,  DEF_CAT  )


#undef GET_INT
#undef CUR_WORD
#undef NEXT
#undef DQ
#undef DD
#undef DW
//#undef DB
