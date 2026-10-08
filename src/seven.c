#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "files.h"

#define NOT_ENOUGH_ARGS 16
#define INCORRECT_FLAG 17
#define EQUAL_FILENAMES 18
#define BUFF_OVERFLOW 19
#define FILE_REMOVE_ERROR 20

#define FLAG_IND 1

#define FILE_1_IND 2
#define FILE_2_IND 3
#define FILE_3_IND 4 

#define ASCII_TAB 32

static int create_tmp_cpy(char *input_filename, char *tmp_filename)
{
	int err;

	if(equal_filenames(input_filename, "tmp1.txt") || equal_filenames(input_filename, "tmp2.txt"))
		return EQUAL_FILENAMES;

	FILE *file_1 = fopen(input_filename, "r");
	FILE *tmp_file_1 = fopen(tmp_filename, "w");

	err = validate_open_files(file_1, tmp_file_1);
	if(err != SUCCESS)
		return err;

	err = file_cpy(file_1, tmp_file_1);
	if(err != SUCCESS)
		return err;

	err = safety_close_files(file_1, tmp_file_1);
	if(err != SUCCESS)
		return err;

	return SUCCESS;
}

static int read_word(FILE *input_file, char *buff, char *symb)
{
	if(input_file == NULL)
		return FILE_OPEN_ERROR;

	char *ptr_buff = buff;

	while((*symb = fgetc(input_file)) != EOF) {
		if(isspace(*symb))
			break;

		if(ptr_buff - buff > BUFF_SIZE - 1)
			return BUFF_OVERFLOW;

		*ptr_buff++ = *symb;
	}

	while((*symb = fgetc(input_file)) != EOF) {
		if(!isspace(*symb)) {
			ungetc(*symb, input_file);
			break;
		}
	}

	*ptr_buff = '\0';

	return SUCCESS;
}

int r_flag(int argc, char *argv[])
{
	int status_code, err;

	if(argc < 5)
		return NOT_ENOUGH_ARGS;
	
	char *input_1 = argv[FILE_1_IND];
	char *input_2 = argv[FILE_2_IND];

	char *output = argv[FILE_3_IND];

	err = create_tmp_cpy(input_1, "tmp1.txt");
	if(err != SUCCESS)
		return err;

	err = create_tmp_cpy(input_2, "tmp2.txt");
	if(err != SUCCESS)
		return err;

	FILE *tmp_file_1 = fopen("tmp1.txt", "r");
	FILE *tmp_file_2 = fopen("tmp2.txt", "r");

	FILE *tmp_out = fopen("tmp_out.txt", "w");

	err = validate_open_files(tmp_file_1, tmp_file_2);
	if(err != SUCCESS)
		return err;

	if(tmp_out == NULL) {
		err = safety_close_files(tmp_file_1, tmp_file_2);
		if(err != SUCCESS)
			return FILE_CLOSE_ERROR;

		return FILE_OPEN_ERROR;
	}

	char buff[BUFF_SIZE];
	char symb;

	while(true) {
		err = read_word(tmp_file_1, buff, &symb);
		if(err != SUCCESS)
			return err;

		fprintf(tmp_out, "%s ", buff);
		if(symb == EOF) {
			 do {
				err = read_word(tmp_file_2, buff, &symb);
				if(err != SUCCESS)
					return err;

				fprintf(tmp_out, "%s ", buff);
			} while(symb != EOF);

			break;
		}

		err = read_word(tmp_file_2, buff, &symb);
		if(err != SUCCESS)
			return err;

		fprintf(tmp_out, "%s ", buff);
		if(symb == EOF) {
			do {
				err = read_word(tmp_file_1, buff, &symb);
				if(err != SUCCESS)
					return err;

				fprintf(tmp_out, "%s ", buff);
			} while(symb != EOF);

			break;
		}
	}

	err = safety_close_files(tmp_file_1, tmp_file_2);
	if(err != SUCCESS)
		return err;

	if(fclose(tmp_out))
		return FILE_CLOSE_ERROR;

	FILE *tmp_file = fopen("tmp_out.txt", "r");
	FILE *output_file = fopen(output, "w");

	err = validate_open_files(tmp_file, output_file);
	if(err != SUCCESS)
		return err;

	err = file_cpy(tmp_file, output_file);
	if(err != SUCCESS)
		return err;

	err = safety_close_files(tmp_file, output_file);
	if(err != SUCCESS)
		return err;

	return SUCCESS;
}

static void to_lowercase(char *word)
{
	while(*word != '\0') {
		if('A' <= *word && *word <= 'Z')
			*word += ASCII_TAB;

		word++;
	}
}

static void to_fourth(char *result, char num, int *count_nums)
{
	while(num > 0) {
		*result++ = (num % 4) + '0';
		num /= 4;

		(*count_nums)++;
	}
}

static void to_eight(char *result, char num, int *count_nums)
{
	while(num > 0) {
		*result++ = (num % 8) + '0';
		num /= 8;

		(*count_nums)++;
	}
}

int a_flag(int argc, char *argv[])
{
	int err;

	if(argc < 4)
		return NOT_ENOUGH_ARGS;
	
	char *input = argv[FILE_1_IND];
	char *output = argv[FILE_2_IND];

	err = create_tmp_cpy(input, "tmp1.txt");
	if(err != SUCCESS)
		return err;

	FILE *input_tmp = fopen("tmp1.txt", "r");
	FILE *output_tmp = fopen("tmp_out.txt", "w");

	err = validate_open_files(input_tmp, output_tmp);
	if(err != SUCCESS)
		return err;

	char word_buff[BUFF_SIZE];

	char num_buff[64];
	int count_nums = 0;

	int i = 1;
	char symb;

	do {
		err = read_word(input_tmp, word_buff, &symb);
		if(err != SUCCESS)
			return err;

		if(i % 10 == 0) {
			to_lowercase(word_buff);

			for(int j = 0; word_buff[j] != '\0'; j++) {
				count_nums = 0;

				to_fourth(num_buff, word_buff[j], &count_nums);

				for(int k = count_nums - 1; k >= 0; k--) 
					fputc(num_buff[k], output_tmp);
			}

			fputc(' ', output_tmp);

		} else if(i % 2 == 0) {
			to_lowercase(word_buff);

			fprintf(output_tmp, "%s ", word_buff);

		} else if(i % 5 == 0) {
			for(int j = 0; word_buff[j] != '\0'; j++) {
				count_nums = 0;

				to_eight(num_buff, word_buff[j], &count_nums);

				for(int k = count_nums - 1; k >= 0; k--)
					fputc(num_buff[k], output_tmp);
			}

			fputc(' ', output_tmp);
		} else
			fprintf(output_tmp, "%s ", word_buff);

		i++;

	} while(symb != EOF);

	err = safety_close_files(input_tmp, output_tmp);
	if(err != SUCCESS)
		return err;

	FILE *tmp_file = fopen("tmp_out.txt", "r");
	FILE *output_file = fopen(output, "w");

	err = validate_open_files(tmp_file, output_file);
	if(err != SUCCESS)
		return err;

	err = file_cpy(tmp_file, output_file);
	if(err != SUCCESS)
		return err;

	err = safety_close_files(tmp_file, output_file);
	if(err != SUCCESS)
		return err;

	return SUCCESS;
}

void err_switch(char flag, int err)
{
	switch(flag) {
		case NOT_ENOUGH_ARGS:
			puts("Введено недостаточно аргументов");
			break;
		case INCORRECT_FLAG:
			printf("Введен некорректный флаг - %c\n", flag);
			break;
		case EQUAL_FILENAMES:
			puts("Введенное имя файла является недопустимым");
			break;
		case BUFF_OVERFLOW:
			puts("Переполнение буффера");
			break;
		case FILE_OPEN_ERROR:
			puts("Ошибка во время открытия файла");
			break;
		case FILE_CLOSE_ERROR:
			puts("Ошибка во время закрытия файла");
			break;
		case WRITE_FILE_ERROR:
			puts("Ошибка записи данных в файл");
			break;
		case FILE_REMOVE_ERROR:
			puts("Ошибка удаления временного файла");
			break;
		case SUCCESS:
		default:
	}
}

void flag_switch(char *flag, int argc, char *argv[])
{
	int err;

	switch(*flag) {
		case 'r':
			err = r_flag(argc, argv);
			
			if(remove("tmp1.txt") || remove("tmp2.txt") || remove("tmp_out.txt"))
				err = FILE_REMOVE_ERROR;
			
			break;
		case 'a':
			err = a_flag(argc, argv);

			if(remove("tmp1.txt") || remove("tmp_out.txt"))
				err = FILE_REMOVE_ERROR;

			break;
		default:
			err = INCORRECT_FLAG;
	}

	err_switch(*flag, err);

	flag++;
	if(*flag != '\0')
		puts("Может быть исполнен только один флаг");
}

int main(int argc, char *argv[])
{
	if(argc < 4) {
		puts("Введено недостаточно аргументов");
		return 1;
	}

	char *flag = argv[FLAG_IND];

	if(*flag != '-' && *flag != '/') {
		puts("Некорректно введен флаг");
		return 1;
	}

	flag++;
	flag_switch(flag, argc, argv);

	return 0;
}
