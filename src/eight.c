#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_IND 1
#define OUTPUT_IND 2

#include "files.h"

int correct_file(char *path)
{
	char *symb = strrchr(path, '.');
	if(symb == NULL)
		return false;

	symb++;
	if(strcmp(symb, "txt") == 0)
		return true;

	return false;
}

int read_files
int main(int argc, char *argv[])
{
	if(argc < 3) {
		puts("Введено недостаточно аргументов");
		return 1;
	}

	int err;

	char *input_path = argv[INPUT_IND];
	char *output_path = argv[OUTPUT_IND];

	if(!correct_file(input_path) || !correct_file(output_path)) {
		puts("Некорректный формат файла");
		return 1;
	}

	FILE *input = fopen(input_path, "r");
	FILE *output = fopen(output_path, "w");

	err = validate_open_files(input, output);
	if(err != SUCCESS)


	return 0;
}

