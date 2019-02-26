
#include "Text_works.cpp"

int main ()
{
	int gg = 0;
	char* file = load_file ("hello_world", &gg);
	
	FILE* output = fopen ("gogo", "wb");
	fwrite (file, sizeof(char), gg, output);
	fclose (output);
	return 0;
}
