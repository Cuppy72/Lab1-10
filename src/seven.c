#include <stdio.h>

#define NOT_ENOUGH_ARGS 10
#define INCORRECT_FLAG 11

#define FLAG_IND 1

#define INPUT_FILE_1 2
#define INPUT_FILE_2 3
#define OUTPUT_FILE 4 

int not_equal_names(char *file_name_1, char *file_name_2)
{

}

int r_flag(int argc, char *argv[])
{
	if(argc < 5)
		return NOT_ENOUGH_ARGS;
	
	char *input_1 = argv[INPUT_FILE_1];
	char *input_2 = argv[INPUT_FILE_2];

	char output = argv[INPUT_FILE_3];
		
}

void flag_switch(char *flag, int argc, char *argv[])
{
	int err;

	switch(flag) {
		case 'r':
			err = r_flag(argc, argv);
			break;
		case 'a':
			err = a_flag(argc, argv);
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
	flag_switch(flag, argc, argc);

	return 0;
}
