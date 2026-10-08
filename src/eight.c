#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

#include "files.h"

#define INPUT_IND 1
#define OUTPUT_IND 2

#define MIN_BASE 2
#define MAX_BASE 36

#define INCORRECT_FORMAT 21
#define BUFF_OVERFLOW 22
#define NUM_OVERFLOW 23
#define EMPTY_FILE 24

static int correct_file(char *path)
{
	char *symb = strrchr(path, '.');
	if(symb == NULL)
		return false;

	symb++;
	if(strcmp(symb, "txt") == 0)
		return true;

	return false;
}

static int digit_value(char symb)
{
	if('0' <= symb && symb <= '9')
		return symb - '0';

	if('A' <= symb && symb <= 'Z')
		return symb - 'A' + 10;

	if('a' <= symb && symb <= 'z')
		return symb - 'a' + 10;

	return -1;
}

static int read_word(FILE *input_file, char *buff, char *symb)
{
	if(input_file == NULL)
		return FILE_OPEN_ERROR;

	char *ptr_buff = buff;

	while((*symb = fgetc(input_file)) != EOF) {
		if(!isspace(*symb))
			break;
	}

	if(*symb == EOF) {
		*ptr_buff = '\0';
		return SUCCESS;
	}

	do {
		if(digit_value(*symb) == -1)
			return INCORRECT_FORMAT;

		if(ptr_buff - buff > BUFF_SIZE - 1)
			return BUFF_OVERFLOW;

		*ptr_buff++ = *symb;

	} while((*symb = fgetc(input_file)) != EOF && !isspace(*symb));

	*ptr_buff = '\0';

	return SUCCESS;
}

static void strip_leading_zeroes(char *word)
{
	char *ptr_digit = word;

	while(*ptr_digit == '0' && *(ptr_digit + 1) != '\0')
		ptr_digit++;

	if(ptr_digit != word)
		memmove(word, ptr_digit, strlen(ptr_digit) + 1);
}

static int pick_min_base(char *word)
{
	int min_base = digit_value(*word) + 1;

	if(min_base < MIN_BASE)
		min_base = MIN_BASE;

	for(word++; *word != '\0'; word++) {
		if(digit_value(*word) + 1 > min_base)
			min_base = digit_value(*word) + 1;
	}

	return min_base;
}

static int to_decimal(char *word, int base, char *result)
{
	unsigned long long total = 0;
	unsigned long long limit = ULLONG_MAX / (unsigned long long)base;

	for(; *word != '\0'; word++) {
		unsigned long long digit = (unsigned long long)digit_value(*word);

		if(total > limit)
			return NUM_OVERFLOW;

		total *= (unsigned long long)base;

		if(ULLONG_MAX - total < digit)
			return NUM_OVERFLOW;

		total += digit;
	}

	if(total == 0) {
		*result = '0';
		*(result + 1) = '\0';

		return SUCCESS;
	}

	char reverse_num[BUFF_SIZE];
	char *ptr_reverse = reverse_num;

	while(total > 0) {
		*ptr_reverse++ = (total % 10) + '0';
		total /= 10;
	}

	char *ptr_result = result;
	while(ptr_reverse != reverse_num)
		*ptr_result++ = *--ptr_reverse;

	*ptr_result = '\0';

	return SUCCESS;
}

static int err_switch(int err)
{
	switch(err) {
		case INCORRECT_FORMAT:
			puts("Во входном файле встречается некорректное число");
			break;
		case BUFF_OVERFLOW:
			puts("Переполнение буффера");
			break;
		case NUM_OVERFLOW:
			puts("Число не помещается в диапазон типа long long");
			break;
		case EMPTY_FILE:
			puts("Во входном файле отсутствуют числа");
			break;
		case FILE_OPEN_ERROR:
			puts("Ошибка во время открытия файла");
			break;
		case FILE_CLOSE_ERROR:
			puts("Ошибка во время закрытия файла");
			break;
		case SUCCESS:
		default:
			return 0;
	}

	return 1;
}

static int process_files(FILE *input, FILE *output)
{
	int err;

	char word_buff[BUFF_SIZE];
	char dec_buff[BUFF_SIZE];

	char symb;
	bool empty_file = true;

	do {
		err = read_word(input, word_buff, &symb);
		if(err != SUCCESS)
			return err;

		if(*word_buff == '\0')
			break;

		empty_file = false;

		strip_leading_zeroes(word_buff);

		int min_base = pick_min_base(word_buff);

		err = to_decimal(word_buff, min_base, dec_buff);
		if(err != SUCCESS)
			return err;

		fprintf(output, "%s %d %s\n", word_buff, min_base, dec_buff);

	} while(symb != EOF);

	if(empty_file)
		return EMPTY_FILE;

	return SUCCESS;
}

int main(int argc, char *argv[])
{
	if(argc < 3) {
		puts("Введено недостаточно аргументов");
		return 1;
	}

	int err;

	char *input_path = argv[INPUT_IND];
	char *output_path = argv[OUTPUT_IND];

	if(equal_filenames(input_path, output_path)) {
		puts("Имена входного и выходного файлов совпадают");
		return 1;
	}

	if(!correct_file(input_path) || !correct_file(output_path)) {
		puts("Некорректный формат файла");
		return 1;
	}

	FILE *input = fopen(input_path, "r");
	FILE *output = fopen(output_path, "w");

	err = validate_open_files(input, output);
	if(err != SUCCESS)
		return err_switch(err);

	err = process_files(input, output);

	int close_err = safety_close_files(input, output);
	if(err == SUCCESS)
		err = close_err;

	return err_switch(err);
}
