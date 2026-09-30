#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "overflow.h"

#define BUFF_SIZE 1024

#define FLAG_IND 1
#define NUMS_START_IND 2

#define EPS_IND 0
#define NUM1_IND 1
#define NUM2_IND 2
#define NUM3_IND 3

#define SUCCESS 10
#define INCORRECT_NUM 11
#define NOT_ENOUGH_ARGS 12
#define INCORRECT_FLAG 13
#define OVERFLOW 14
#define NOT_A_NUM -2

void solution(double eps, double a, double b, double c)
{
	if (fabs(a) < eps) {
		if (fabs(b) < eps) {
			if (fabs(c) < eps) {
				puts("уравнение содержит бесконечное количество корней");
			} else {
				puts("уравнение не содержит корней");
			}
		} else {
			printf("уравнение содержит единственный корень: %.5lf\n", -c / b);
		}
	} else {
		double D = b * b - 4.0 * a * c;
		
		if(isinf(D)) {
			puts("Ошибка переполнения");
			return;
		}

		if (fabs(D) < eps) {
			printf("уравнение содержит один корень: %.5lf\n", -b / (2.0 * a));
		} else if (D > eps) {
			double x1 = (-b - sqrt(D)) / (2.0 * a);
			double x2 = (-b + sqrt(D)) / (2.0 * a);

			if(isinf(x1) || isinf(x2)) {
				puts("Ошибка переполнения");
				return;
			}
			
			printf("уравнение содержит два корня: %.5lf, %.5lf\n", x1, x2);
		} else {
			puts("уравнение не содержит действительных корней");
		}
	}
}

int q_flag(int count_nums, char *argv[])
{
	if(count_nums < 4)
		return NOT_ENOUGH_ARGS;

	double nums[4];

	for(int i = 0; i < 4; i++) {
		char *end_ptr;
		double num = strtod(argv[NUMS_START_IND + i], &end_ptr);

		if(*end_ptr != '\0')
			return INCORRECT_NUM;
		else if(i == 0 && num < 0)
			return INCORRECT_NUM;
		
		if(isinf(num))
			return OVERFLOW;

		nums[i] = num;
	}
	

	double num_1 = nums[NUM1_IND];
	double num_2 = nums[NUM2_IND];
	double num_3 = nums[NUM3_IND];
	double eps = nums[EPS_IND];

	int eq12 = fabs(num_1 - num_2) < eps;
	int eq13 = fabs(num_1 - num_3) < eps;
	int eq23 = fabs(num_2 - num_3) < eps;

	if (eq12 && eq13) {
		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_1, num_1, num_1);
		solution(eps, num_1, num_1, num_1);
	} else if (eq12) {
		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_1, num_1, num_3);
		solution(eps, num_1, num_1, num_3);
		
		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_1, num_3, num_1);
		solution(eps, num_1, num_3, num_1);

		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_3, num_1, num_1);
		solution(eps, num_3, num_1, num_1);
	} else if (eq13) {
		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_1, num_2, num_1);
		solution(eps, num_1, num_2, num_1);

		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_1, num_1, num_2);
		solution(eps, num_1, num_1, num_2);

		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_2, num_1, num_1);
		solution(eps, num_2, num_1, num_1);
	} else if (eq23) {
		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_1, num_2, num_2);
		solution(eps, num_1, num_2, num_2);
		
		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_2, num_1, num_2);
		solution(eps, num_2, num_1, num_2);

		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_2, num_2, num_1);
		solution(eps, num_2, num_2, num_1);
	} else {
		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_1, num_2, num_3);
		solution(eps, num_1, num_2, num_3);

		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_1, num_3, num_2);
		solution(eps, num_1, num_3, num_2);

		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_2, num_1, num_3);
		solution(eps, num_2, num_1, num_3);

		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_2, num_3, num_1);
		solution(eps, num_2, num_3, num_1);

		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_3, num_2, num_1);
		solution(eps, num_3, num_2, num_1);

		printf("При a = %.5lf, b = %.5lf, c = %.5lf: ", num_3, num_1, num_2);
		solution(eps, num_3, num_1, num_2);
	}

	return SUCCESS;	
}

int m_flag(int count_nums, char *argv[])
{
	if(count_nums < 2)
		return NOT_ENOUGH_ARGS;

	char max_num[BUFF_SIZE];
	char min_num[BUFF_SIZE];

	snprintf(max_num, sizeof(max_num), "%lld", LLONG_MAX);
	snprintf(min_num, sizeof(min_num), "%lld", LLONG_MIN);

	int flag = int_overflow(argv[NUMS_START_IND], max_num, min_num);

	if(flag == NOT_A_NUM)
		return NOT_A_NUM;
	
	if(flag == -1 || flag == 1)
		return OVERFLOW;

	long long first = atoll(argv[NUMS_START_IND]);
	long long second = atoll(argv[NUMS_START_IND + 1]);
	
	if(second == 0)
		return INCORRECT_NUM;

	if(first == LLONG_MIN && second == -1)
		return OVERFLOW;

	if(first % second == 0)
		puts("Первое число кратно второму");
	else
		puts("Первое число не кратно второму");

	return SUCCESS;
}

int t_flag(int count_nums, char *argv[])
{
	if(count_nums < 4)
		return NOT_ENOUGH_ARGS;

	double nums[4];

	for(int i = 0; i < 4; i++) {
		char *end_ptr;
		double num = strtod(argv[NUMS_START_IND + i], &end_ptr);

		if(*end_ptr != '\0' || num < 0)
			return INCORRECT_NUM;

		if(isinf(num))
			return OVERFLOW;

		nums[i] = num;
	}

	bool flag = false;

	if(fabs(pow(nums[NUM1_IND], 2) + pow(nums[NUM2_IND], 2) - pow(nums[NUM3_IND], 2)) < nums[EPS_IND])
		flag = true;
	else if (fabs(pow(nums[NUM1_IND], 2) + pow(nums[NUM3_IND], 2) - pow(nums[NUM2_IND], 2)) < nums[EPS_IND])
		flag = true;
	else if (fabs(pow(nums[NUM2_IND], 2) + pow(nums[NUM2_IND], 2) - pow(nums[NUM1_IND], 2)) < nums[EPS_IND])
		flag = true;

	if(flag)
		printf("Стороны %.5lf, %.5lf, %.5lf могут быть длинами сторон прямоугольного треугольника\n", nums[NUM1_IND], nums[NUM2_IND], nums[NUM3_IND]);
	else
		printf("Стороны %.5lf, %.5lf, %.5lf не могут быть длинами сторон прямоугольного треугольника\n", nums[NUM1_IND], nums[NUM2_IND], nums[NUM3_IND]);

	return SUCCESS;
}

void err_switch(char flag, int error_num)
{
	switch(error_num) {
		case INCORRECT_NUM:
			puts("Некорректно введенное число");
			break;
		case NOT_ENOUGH_ARGS:
			puts("Введено недостаточно аргументов");
			break;
		case INCORRECT_FLAG:
			printf("Введен некорректный флаг: %c\n", flag);
			break;
		case OVERFLOW:
			puts("Введенное число выходит за границы типа данных");
			break;
		case SUCCESS:
			break;
		default:
			puts("Неизвестная ошибка");
	}
}

void flag_switch(char *flag, int count_nums, char *argv[])
{
	int err;

	switch(*flag) {
		case 'q':
			err = q_flag(count_nums, argv);
			break;
		case 'm':
			err = m_flag(count_nums, argv);
			break;
		case 't':
			err = t_flag(count_nums, argv);
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

	if(*flag != '/' && *flag != '-') {
		puts("Некорректно введен флаг");
		return 1;
	}
	flag++;

	flag_switch(argv[FLAG_IND], argc - NUMS_START_IND, argv);

	return 0;
}

