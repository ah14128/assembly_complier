#include <stdio.h>
#include <string.h>


int preprocess_line(char label[], char mnemonic[], char instruction[], FILE* fp);
void print_line(char label[], char mnemonic[], char instruction[]);
int check_printable(char c);
int define(char instruction[]);
int encode(char instruction[], int ibytes[]);
int check_register(char instruction[]);
void print_ibytes(int ibytes[], int ibytesLength);