#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BUFF_SIZE 1024

#define FLAG_IND 1
#define ARGS_START_IND 2

#define ACSII_PAD 55

#define SUCCESS 10
#define NOT_ENOUGH_ARGS 11
#define BUFF_OVERFLOW 12
#define FILE_OPEN_ERROR 13
#define FILE_CLOSE_ERROR 14
#define WRITE_FILE_ERROR 15
#define REMOVE_FILE_ERROR 16
#define FLAG_ERROR 17

static int add_prefix(char *old_name, char *new_name)
{
	if(strlen(old_name) + 4 > BUFF_SIZE)
		return BUFF_OVERFLOW;

	char *last_slash = strrchr(old_name, '/');

	if(last_slash == NULL) {
		strcpy(new_name, "out_");
		strcat(new_name, old_name);
	} else {
		strncpy(new_name, old_name, last_slash - old_name + 1);
		strcat(new_name, "out_");
		strcat(new_name, last_slash + 1);
	}

	return SUCCESS;
}

static int result_name(bool name_flag, int argc, char *argv[], char *new_name)
{
	int err = SUCCESS;

	if((name_flag && argc < 4) || (!name_flag && argc < 3))
		return NOT_ENOUGH_ARGS;
	
	if(name_flag)
		strcpy(new_name, argv[ARGS_START_IND + 1]);
	else
		err = add_prefix(argv[ARGS_START_IND], new_name);

	return err;
}

static int write_all(FILE *destination, FILE *src)
{
	int status_code;

	if(destination == NULL) {
		status_code = fclose(src);

		if(status_code)
			return FILE_CLOSE_ERROR;
	} else if(src == NULL) {
		status_code = fclose(destination);

		if(status_code)
			return FILE_CLOSE_ERROR;
	}

	if(destination == NULL || src == NULL)
		return FILE_OPEN_ERROR;

	char buff[BUFF_SIZE];
	int bytes_read, bytes_write;

	while((bytes_read = fread(buff, sizeof(char), BUFF_SIZE, src)) > 0) {
		bytes_write = fwrite(buff, sizeof(char), bytes_read, destination);

		if(bytes_read != bytes_write)
			return WRITE_FILE_ERROR;
	}

	return SUCCESS;
}

static int to_hex(char num, unsigned char *hex_num)
{
	int count_nums = 0;

	char reverse_hex_num[10];

	while(num > 0) {
		reverse_hex_num[count_nums++] = num % 16;
		num /= 16;
	}

	for(int i = count_nums - 1; i >= 0; i--)
		*hex_num++ = reverse_hex_num[i];

	return count_nums;
}

static int d_func(FILE *source, FILE *tmp_file)
{
	char symb;

	while((symb = fgetc(source)) != EOF) {
		if(isdigit(symb))
			continue;

		fputc(symb, tmp_file);
	}
	
	return SUCCESS;
}

static int i_func(FILE *source, FILE *tmp_file)
{
	int counter = 0;
	char symb;

	while((symb = fgetc(source)) != EOF) {
		if(isalpha(symb))
			counter++;

		if(symb == '\n') {
			fprintf(tmp_file, "%d\n", counter);
			counter = 0;
		}
	}

	return SUCCESS;
}

static int s_func(FILE *source, FILE *tmp_file)
{
	int counter = 0;
	char symb;

	while((symb = fgetc(source)) != EOF) {
		if(!isalpha(symb) && !isdigit(symb) && !isspace(symb))
			counter++;

		if(symb == '\n') {
			fprintf(tmp_file, "%d\n", counter);
			counter = 0;
		}
	}

	return SUCCESS;
}

static int a_func(FILE *source, FILE *tmp_file)
{
	int status_code_1, status_code_2;

	char symb;

	while((symb = fgetc(source)) != EOF) {
		if(isdigit(symb)) {
			fputc(symb, tmp_file);
			continue;
		}

		char hex_num[10];
		
		int nums_write = 0;
		int nums_read = to_hex(symb, hex_num); 

		for(int i = 0; i < nums_read; i++) {
			int n;

			if(hex_num[i] < 9)
				n = fprintf(tmp_file, "%d", hex_num[i]);
			else
				n = fprintf(tmp_file, "%c", hex_num[i] + ACSII_PAD);

			if(n != 1) {
				status_code_1 = fclose(tmp_file);
				status_code_2 = fclose(source);
	
				if(status_code_1 || status_code_2)
					return FILE_CLOSE_ERROR;

				return WRITE_FILE_ERROR;
			}

			nums_write += n;
		}

		if(nums_write != nums_read) {
			status_code_1 = fclose(tmp_file);
			status_code_2 = fclose(source);
	
			if(status_code_1 || status_code_2)
				return FILE_CLOSE_ERROR;

			return WRITE_FILE_ERROR;
		}
	}

	return SUCCESS;
}

int adaptive_func(bool name_flag, int argc, char *argv[], int (*func)(FILE*, FILE*))
{
	int status_code, status_code_1, status_code_2;

	char new_name[BUFF_SIZE];

	int err = result_name(name_flag, argc, argv, new_name);
	if(err != SUCCESS)
		return err;

	FILE *source = fopen(argv[ARGS_START_IND], "r");
	FILE *tmp_file = fopen("tmp.txt", "w");
		
	if(source == NULL) {
		status_code = fclose(tmp_file);

		if(status_code)
			return FILE_CLOSE_ERROR;
	} else if(tmp_file == NULL) {
		status_code = fclose(source);

		if(status_code)
			return FILE_CLOSE_ERROR;
	}

	if(source == NULL || tmp_file == NULL)
		return FILE_OPEN_ERROR;

	
	status_code = func(source, tmp_file);
	if(status_code != SUCCESS)
		return status_code;

	status_code_1 = fclose(source);
	status_code_2 = fclose(tmp_file);

	if(status_code_1 || status_code_2)
		return FILE_CLOSE_ERROR;

	FILE *destination = fopen(new_name, "w");
	tmp_file = fopen("tmp.txt", "r");

	err = write_all(destination, tmp_file);	
	if(err != SUCCESS)
		return err;

	status_code_1 = fclose(destination);
	status_code_2 = fclose(tmp_file);

	if(status_code_1 || status_code_2)
		return FILE_CLOSE_ERROR;

	if(remove("tmp.txt"))
		return REMOVE_FILE_ERROR;

	return SUCCESS;
}

void error_switch(char flag, int error)
{
	switch(error) {
		case NOT_ENOUGH_ARGS:
			puts("Введено недостаточно аргументов");
			break;
		case BUFF_OVERFLOW:
			puts("Введен слишком большой путь к файлу");
			break;
		case FILE_OPEN_ERROR:
			puts("Произошла ошибка при открытии файла");
			break;
		case FILE_CLOSE_ERROR:
			puts("Произошла ошибка при закрытии файла");
			break;
		case WRITE_FILE_ERROR:
			puts("Произошла ошибка записи в файл");
			break;
		case REMOVE_FILE_ERROR:
			puts("Произошла ошибка удаления временного файла");
			break;
		case SUCCESS:
		default:
	}
}

void flag_switch(char *flag, int argc, char *argv[])
{
	int err;
	bool name_flag = false;

	if(*flag == 'n') {
		flag++;
		name_flag = true;
	}
	
	switch(*flag) {
		case 'd':
			err = adaptive_func(name_flag, argc, argv, d_func);
			break;
		case 'i':
			err = adaptive_func(name_flag, argc, argv, i_func);
			break;
		case 's':
			err = adaptive_func(name_flag, argc, argv, s_func);
			break;
		case 'a':
			err = adaptive_func(name_flag, argc, argv, a_func);
			break;
		default:
			err = FLAG_ERROR;
	}

	error_switch(*flag, err);

	flag++;
	if(*flag != '\0')
		puts("Может быть исполнен только один флаг");
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
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
