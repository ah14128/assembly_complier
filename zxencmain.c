#include "zx256.h"

int main(int argc, char* argv[])
{
	if(argc > 3)
	{
		fprintf(stderr,"Usage: ./zxenc.exe FILENAME");
		return 1;
	}
	if(argc == 3)
	{
		int ibytes[256];
		char* instruction = argv[2];
		if(encode(instruction, ibytes) == 0)
		{
			fprintf(stderr, "Could not find instructions\n");
			return 1;
		}	
	}
	FILE* inputfile = stdin;
	if(argc == 2)
	{
		char* inputfilename = argv[1];
		inputfile = fopen(inputfilename, "r");
		if(inputfile == NULL)
		{
			fprintf(stderr, "Unable to open \"%s\"\n", inputfilename);
			return 1;
		}
	}
	int ibytes[256];
	char line[256];
	while(fgets(line, 256, inputfile) != NULL)
	{
		if(encode(line, ibytes)== 0)
		{
			fprintf(stderr, "Could not find instructions\n");
			return 1;
		}
	}
	return 0;
}