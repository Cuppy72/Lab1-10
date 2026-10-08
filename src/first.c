#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

#include "overflow.h"

#define ASCII_PAD 55

#define BUFF_SIZE 30

#define NUM_POS 1
#define FLAGS_START_POS 2

#define MAX_BUFF_SIZE 15

#define MAX_BASE 10

#define ABS(X) ((X) > 0) ? (X) : (-(X))

#define NOT_A_NUM -2
#define SUCCESS 10
#define OVERFLOW 11
#define EMPTY 12
#define NON_POSITIVE 13
#define NON_CONDITION 14
#define UNKNOWN_FLAG 15
#define ZERO_DIVISION 16

#define MAX_ITERATION 1e5

int h_flag(long long num)
{
	if(num == 0)
		return ZERO_DIVISION;

	int count_valid = 0;

	for(long long i = 1; i <= 100; i++)
		if(i % num == 0) {
			count_valid++;
			printf("%lld\n", i);
		}
	
	if(count_valid == 0) return EMPTY;
	return SUCCESS;
}

int p_flag(long long num)
{
	num = ABS(num);

	int count_iters = 0;

	if(num != 2)
		for(long long i = 2; i < (num / 2) + 1 && count_iters < MAX_ITERATION; i++, count_iters++)
			if(num % i == 0) {
				puts("Число составное");
				return SUCCESS;
			}

	if(count_iters < MAX_ITERATION)
		puts("Число простое");
	else
		puts("Превышен лимит операций");

	return SUCCESS;
}

int s_flag(long long num)
{
	if(num == 0) {
		puts("0");
		return SUCCESS;
	}

	num = ABS(num);

	char buffer[MAX_BUFF_SIZE];
	int count_nums = 0;

	char *ptr_buffer = buffer;

	while(num > 0) {
		*ptr_buffer++ = (char)(num % 16);
		count_nums++;

		num /= 16;
	}

	for(int i = count_nums - 1; i >= 0; i--){
		if(buffer[i] < 10)
			printf("%d ", buffer[i]);
		else 
			printf("%c ", buffer[i] + ASCII_PAD);
	}

	puts("");
	return SUCCESS;
}

static long long int_pow(long long num, long long base)
{
	long long total = 1;
	for(int i = 0; i < num; i++)
		total *= base;

	return total;
}

static int length_number(long long num)
{
	int len = 0;
	
	while(num > 0) {
		len++;
		num /= 10;
	}

	return len;
}

static void paddings(int count_spaces)
{
	for(int i = 0; i < count_spaces; i++)
		putchar(' ');
}

int e_flag(long long num)
{
	if(num > 10) return NON_CONDITION;
	if(num < 1) return NON_POSITIVE;

	int count_spaces = length_number(int_pow(num, MAX_BASE)) + 1;

	for(int i = 1; i <= num; i++) {
		for(int j = 1; j <= 10; j++) {
			long long result = int_pow(i, j);

			int len = length_number(result);
			
			printf("%lld", int_pow(i, j));
			paddings(count_spaces - len);
		}

		putchar('\n');
	}

	return SUCCESS;
}

int a_flag(long long num)
{
	if(num < 0) return NON_POSITIVE;
	if(num == LONG_MAX) return OVERFLOW;

	long long mul1, mul2;
	if(num % 2 == 0) {
		mul1 = num / 2;
		mul2 = num + 1;
	} else {
		mul1 = num;
		mul2 = (num + 1) / 2;
	}

	if(mul1 > LONG_MAX / mul2) return OVERFLOW;

	long long total = ((1 + num) * num) / 2;
	printf("%lld\n", total);

	return SUCCESS;
}

int f_flag(long long num)
{
	if(num < 0) return NON_POSITIVE;

	long long result = 1;
	while(num > 1) {
		if(result > LONG_MAX / num) return OVERFLOW;

		result *= num;
		num--;
	}

	printf("%lld\n", result);

	return SUCCESS;
}

void error_message(int error_num, char flag)
{
	switch(error_num) {
		case OVERFLOW:
			puts("Ошибка переполнения");
			break;
		case EMPTY:
			puts("Подходящие числа отсутсвуют");
			break;
		case NON_POSITIVE:
			puts("Число должно быть больше нуля");
			break;
		case NON_CONDITION:
			puts("Число не подходит под условие функции");
			break;
		case UNKNOWN_FLAG:
			printf("Введен неизвестный флаг %c\n", flag);
			break;
		case ZERO_DIVISION:
			printf("Ошибка деления на ноль");
			break;
		case SUCCESS:
		default:
			return;
	}
}

void switch_func(long long num, char symb)
{
	int error;

	switch(symb) {
		case 'h':
			error = h_flag(num);
			break;
		case 'p':
			error = p_flag(num);
			break;
		case 's':
			error = s_flag(num);
			break;
		case 'e':
			error = e_flag(num);
			break;
		case 'a':
			error = a_flag(num);
			break;
		case 'f':
			error = f_flag(num);
			break;
		default:
			error = UNKNOWN_FLAG;
	}

	error_message(error, symb);
}

void flag_scan(long long num, const char *flag)
{
	if(*flag != '/' && *flag != '-')
		return;

	flag++;
	while(*flag != '\0') {
		switch_func(num, *flag);
		flag++;
	}
}

int main(int argc, char *argv[])
{
	if(argc <= 2) {
		puts("Введено слишком мало аргументов");
		return 1;
	}

	char max_num[BUFF_SIZE];
	char min_num[BUFF_SIZE];

	snprintf(max_num, sizeof(max_num), "%lld", LLONG_MAX);
	snprintf(min_num, sizeof(min_num), "%lld", LLONG_MIN);

	int flag = int_overflow(argv[NUM_POS], max_num, min_num);
	if(flag == NOT_A_NUM) {
		puts("Введено некорректное число");
		return 1;
	}

	if(flag == -1 || flag == 1) {
		puts("Ошибка переполнения");
		return 1;
	}

	long long num = atoll(argv[NUM_POS]);

	for(int i = FLAGS_START_POS; i < argc; i++)
		flag_scan(num, argv[i]);
	
	return 0;
}
