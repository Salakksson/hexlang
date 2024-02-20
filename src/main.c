#include "interpret.h"
void print_help()
{
	printf(
		"Hexlang is a turing complete language which operates on a single array\n"
		"It contains only 6 tokens: array indices, +, {, }, i, o\n"
		"Array indices start from 0\n"
		"x+y sets x to x+y, x and y being array indices\n"
		"Anything inside x{} will be repeated untill x is 0\n"
		"ix takes one byte of user input and stores it at x\n"
		"ox output the value at x\n"
		"Index 0 is initialised to 1 while the rest of the array is 0s\n"
	);
}

void print_syntax(char* name)
{
	printf(
			"Syntax: %s -f <filename> <flags>\n"
			"-help for information on the language\n"
			"-debug for debugging info\n"
			"-f <filename> to specify filename\n"
			"-i <input> to specify program input\n"
			"-if <filename> to specify input filename\n"
			"-of <filename> to specify output filename\n"
			"-16a for 16 bit array (long array)\n"
			"-16i for 16 bit values (long ints)\n"
			"-od to output spaced integers\n"
			"-ox to output spaced hexadecimals\n"
			"-id to read input as spaced integers\n"
			"-ix to read input as spaced hexadecimals\n"
			,name
		);
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		print_syntax(argv[0]);
		return 0;
	}
	char* filename;
	char* input;
	bool isInput = false;
	char* ifilename;
	flag flags = 0;
	bool error = false;
	for (int i = 1; i < argc; i++)
	{
		if(!strcmp(argv[i], FLAG_HELP))
		{
			print_help();
			return 0;
			continue;
		}
		if(!strcmp(argv[i], FLAG_FILENAME))
		{
			if ((i+1) < argc)
			{
				filename = (char*)malloc(strlen(argv[i+1]) + 1);
				strcpy(filename, argv[i+1]);
				continue;
			}
			printf("Arg %d: ERROR: Failed to find filename\n", i+1);
			continue;
		}
		if(!strcmp(argv[i], FLAG_INPUT))
		{
			if(isInput)
			{
				printf("Arg %d: ERROR: Contraditing arguments", i+1);
				continue;
			}
			if ((i+1) < argc)
			{
				filename = argv[i+1];
				continue;
			}
			printf("Arg %d: ERROR: Failed to find input values\n", i+1);
			error = true;
			continue;
		}
		if(!strcmp(argv[i], FLAG_INFFILE))
		{
			if ((i+1) < argc)
			{
				ifilename = (char*)malloc(strlen(argv[i+1]) + 1);
				strcpy(ifilename, argv[i+1]);
			}
			else
			{
				printf("Arg %d: ERROR: Failed to find input file\n", i+1);
				error = true;
				continue;
			}
		}
		if(!strcmp(argv[i], "-debug"))
		{
			flags = flags | FLAGB_DEBUG;
			continue;
		}
		if(!strcmp(argv[i], "-16a"))
		{
			flags = flags | FLAGB_ARRAY16;
			continue;
		}
		if(!strcmp(argv[i], "-16i"))
		{
			flags = flags | FLAGB_INT16;
			continue;
		}
		if(!strcmp(argv[i], "-od"))
		{
			if (flags & FLAGB_OUTX)
			{
				printf("Arg %d: ERROR: Contraditing arguments", i+1);
				error = true;
				continue;
			}
			flags = flags | FLAGB_OUTD;
			continue;
		}
		if(!strcmp(argv[i], "-ox"))
		{
			if (flags & FLAGB_OUTD)
			{
				printf("Arg %d: ERROR: Contraditing arguments", i+1);
				error = true;
				continue;
			}
			flags = flags | FLAGB_OUTX;
			continue;
		}
		if(!strcmp(argv[i], "-id"))
		{
			if (flags & FLAGB_INX)
			{
				printf("Arg %d: ERROR: Contraditing arguments", i+1);
				error = true;
				continue;
			}
			flags = flags | FLAGB_IND;
			continue;
		}
		if(!strcmp(argv[i], "-ix"))
		{
			if (flags & FLAGB_IND)
			{
				printf("Arg %d: ERROR: Contraditing arguments", i+1);
				error = true;
				continue;
			}
			flags = flags | FLAGB_INX;
			continue;
		}
		printf("ERROR: Unknown flag: %s", argv[i]);
	}
	if (error)
	{
		printf("Failed to interpret arguments\n");
		return -3;
	}
	FILE* fp = fopen(filename, "r");
	if (!fp)
	{
		printf("ERROR: Couldnt find file '%s'\n", filename);
		return -1;
	}
	fseek(fp, 0L, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char* code = (char*)malloc(len);
	fread(code, 1, len, fp);
	fclose(fp);

	printf("test");
	interpret(code, flags);

}
