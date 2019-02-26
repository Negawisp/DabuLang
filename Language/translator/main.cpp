#include <iostream>
#include "Translator.h"


int main ()
{
	Translator_c translator;
	translator.Load ();
	translator.Translate ();
	translator.Output ();

	return 0;
}
