#include <stdio.h>


int main ()
{
	char a [10] = {};

	a[0] = 'H';
	a[1] = 'm';


	printf ("%x%x", (int)a[0], (int)a[1]);

	return 0;
}
