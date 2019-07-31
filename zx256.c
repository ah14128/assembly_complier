#include "zx256.h"

int check_printable(char c)
{
	return c >= 33 && c <= 127;
}

int preprocess_line(char label[], char mnemonic[], char instruction[], FILE* fp)
{
	char line[256];
	
	while(fgets(line, 256, fp) != NULL)
	{	
		int flag = 0, flag2 = 0; //To check when we get to a certain stage
		int a = 0, b = 0, c = 0; //counters
		label[0] = '\0';
		for(int i = 0; i < strlen(line); i++)
		{
			
			if(line[i] == ';')
			{
				instruction[b] = '\0';
				break;
			}
			
			if(line[i] == ':')
			{
				if(flag2)
				{
					return 2; // If a second label is found in the line
				}
				c = 0;
				b = 0;
				for(int j = 0; j < i; j++)
				{
					if(check_printable(line[j]))
					{
						label[a] = line[j];
						a++;
					}
				}
				label[a] = ':';
				label[a+1] = '\0';
				i++;
				flag2 = 1; // 1 label found
			}
			
			if(check_printable(line[i]))
			{
				if(flag)
				{
					instruction[b] = line[i];
					b++;
				}
				else
				{
					mnemonic[c] = line[i];
					c++;
					if(line[i+1] == ' ')
					{
						flag = 1; // mnemonic read
						continue;
					}
				}
			}
			if(line[i] == '"')
			{
				for(int j = i+1; j < strlen(line); j++)
				{
					instruction[b] = line[j];
					b++;
					if(line[j] == '"')
					{
						i = j;
						break;
					}
					if(j == strlen(line)-1)
					{
						return 0; // If the string is not closed
					}
				}
			}
		}
		instruction[b] = '\0'; // Set the end of the strings to a null byte
		mnemonic[c] = '\0';
		if(instruction[0] == '\0' && mnemonic[0] == '\0')
		{
			continue;
		}
		else
		{
			print_line(label, mnemonic, instruction);
		}
	}
	return 1;
}

void print_line(char label[], char mnemonic[], char instruction[])
{
	printf("%s",label);
	for(int n = 0; n <9 - strlen(label); n++)
	{
		printf(" ");
	}
	if(instruction[0] == NULL)
	{
		printf("%s\n", mnemonic);
	}
	else
	{
		printf("%s %s\n", mnemonic, instruction);
	}
	
}
	
int encode(char instruction[], int ibytes[])
{
	int n = 0; // Length of individual words
	int a = 0; // number of bytes
	int b = 0;
	int c = 0; // length of any strings
	int flag = 0; // Check if there was a string
	unsigned int* number;
	char word[256];
	char string[8];
	char label[8];
	ibytes[a] ='\0';
	for(int i = 1; i <= strlen(instruction)-1; i++)
	{
		if(i == strlen(instruction)-1) // Last block of instruction
		{
			for(int j = b; j <= i; j++)
			{
				if(instruction[j] != ' ' || instruction[j] != ',')
				{	
					string[n] = instruction[j];
					n++;
				}
			}
			string[n] = '\0';
			n = 0;
			if(define(string) == 0)
			{
				sscanf(string, "0x%x", &number);
				ibytes[a] = number;
			}
			else if(check_register(string) != 0)
			{
				ibytes[a] = check_register(string);
			}
			else
			{
				ibytes[a] = define(string);
			}
			a++;
		}
		if(instruction[i] == ' ' || instruction[i] == ',')
		{
			for(int j = b; j < i; j++)
			{
				if(instruction[j] != ' ' || instruction[j] != ',')
				{	
					string[n] = instruction[j];
					n++;
				}
			}
			string[n] = '\0';
			n = 0;
			b = i+1;
			if(define(string) == 0)
			{
				if(!strcmp(string, "db")) // Check if there is a string
				{
					for(int k = b+1; k <strlen(instruction)-1; k++)
					{
						word[c] = instruction[k];
						c++;
					}
					word[c] = '\0';
					for(a = 0; a <= c; a++)
					{
						
						if(word[a] == 92)
						{
							ibytes[a] = 0x0a;
							break;
						}
						ibytes[a] = word[a] - 0;
					
					}
					ibytes[a+1] = '\0';
					i = a;
					flag = 1;
				}
				else if(check_register(string) != 0)
				{
					ibytes[a] = check_register(string);
				}
				else
				{
					sscanf(string, "0x%x", &number);
					ibytes[a] = number;
				}
			}
			else
			{
				ibytes[a] = define(string);
			}
			a++;
		}
		if(flag)
		{
			break;
		}
	}
	ibytes[a] ='\0';
	print_ibytes(ibytes, a);
	return a;
}

void print_ibytes(int ibytes[], int ibytesLength)
{
	for(int i = 1; i <= ibytesLength; i++)
	{
		printf("0x%02x ", ibytes[i-1]);
		if((i % 4) == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
}

int check_register(char* instruction)
{
	if(!(strcmp(instruction,"%a")))
	{
		return 0xc0;
	}
	if(!(strcmp(instruction,"%b")))
	{
		return 0xc1;
	}
	if(!(strcmp(instruction,"%c")))
	{
		return 0xc2;
	}
	if(!(strcmp(instruction,"%d")))
	{
		return 0xc3;
	}
	if(!(strcmp(instruction,"%ip")))
	{
		return 0xc4;
	}
	if(!(strcmp(instruction,"%sp")))
	{
		return 0xc5;
	}
	if(!(strcmp(instruction,"%bp")))
	{
		return 0xc6;
	}
	if(!(strcmp(instruction,"%flags")))
	{
		return 0xc7;
	}
	return 0;
}

int define(char instruction[])
{
	if(!(strcmp(instruction, "mov")))
	{
		return 0x01;
	}
	if(!(strcmp(instruction, "add")))
	{
		return 0x04;
	}
	if(!(strcmp(instruction, "sub")))
	{
		return 0x05;
	}
	if(!(strcmp(instruction, "mul")))
	{
		return 0x06;
	}
	if(!(strcmp(instruction, "div")))
	{
		return 0x07;
	}
	if(!(strcmp(instruction, "and")))
	{
		return 0x08;
	}
	if(!(strcmp(instruction, "or")))
	{
		return 0x09;
	}
	if(!(strcmp(instruction, "xor")))
	{
		return 0x0a;
	}
	if(!(strcmp(instruction, "cmp")))
	{
		return 0x0b;
	}
	if(!(strcmp(instruction, "not")))
	{
		return 0x14;
	}
	if(!(strcmp(instruction, "jmp")))
	{
		return 0x20;
	}
	if(!(strcmp(instruction, "jle")))
	{
		return 0x21;
	}
	if(!(strcmp(instruction, "jl")))
	{
		return 0x22;
	}
	if(!(strcmp(instruction, "je")))
	{
		return 0x23;
	}
	if(!(strcmp(instruction, "jne")))
	{
		return 0x24;
	}
	if(!(strcmp(instruction, "jge")))
	{
		return 0x25;
	}
	if(!(strcmp(instruction, "jg")))
	{
		return 0x26;
	}
	if(!(strcmp(instruction, "syscall")))
	{
		return 0x30;
	}
	else
	{
		return 0;
	}
}


	