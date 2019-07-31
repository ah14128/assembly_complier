#include "zx256.h"

int main(int argc, char* argv[])
{
	if(argc >= 3)
	{
		fprintf(stderr, "Usage: ./zxpre.exe FILENAME\n");
		return 1;
	}
	char mnemonic[6];
	char label[8];
	char instruction[256];
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
	int success = preprocess_line(label, mnemonic, instruction, inputfile);
	if(success != 1)
	{
		if(success == 0)
		{
			fprintf(stderr, "Unclosed string.\n");
			return 1;
		}
		else if(success == 2)
		{
			fprintf(stderr, "Double label.\n");
			return 1;
		}

	}
	fclose(inputfile);
	return 0;
}

