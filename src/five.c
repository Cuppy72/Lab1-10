#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define ASCII_TAB 97

#define EPS_IND 1
#define X_IND 2

#define MAX_ITERATION 1e5

double a_sum(double eps, double x)
{
	double numer = 1.0;
	double denominatior = 1.0;

	double total = 1.0;

	int n = 1;

	int count_iterations = 0;
	while(count_iterations++ < MAX_ITERATION) {
		numer *= x;
		denominatior *= (double)n;

		double cur_num = numer / denominatior;

		total += cur_num;
		if(fabs(cur_num) < eps)
			return total;

		n++;
	}

	return total;
}

static double b_sum(double x, double n)
{
	double numer = -pow(x, 2.0);
	double denominatior = ((2.0 * (double)n - 1.0) * (2.0 * (double)n));

	return numer / denominatior;
}

static double c_sum(double x, double n)
{
	double numer = 9.0 * pow(x, 2.0) * pow((double)n, 2);
	double denominatior = (3.0 * (double)n - 1.0) * (3.0 * (double)n - 2.0);

	return numer / denominatior;
}

static double d_sum(double x, double n)
{
	double numer = -(pow(x, 2.0) * (2.0 * (double)n - 1.0));
	double denominatior = (2.0 * (double)n);

	return numer / denominatior;
}

double adaptive_sum(double eps, double x, double total, double (*func)(double, double))
{
	double prev_num = 1.0;

	int n = 1;
	int count_iterations = 0;
	while(count_iterations++ < MAX_ITERATION) {
		double cur_num = prev_num * func(x, (double)n);

		total += cur_num;
		if(fabs(cur_num) < eps)
			return total;

		prev_num = cur_num;
		n++;
	}

	return total;
}

static double a_integral(double x, double eps)
{
	if(fabs(x) > eps)
		return log(1.0 + x) / x;

	return NAN;
}

static double b_integral(double x, double eps)
{
	const double e = 2.718281828459045;

	return pow(e, -pow(x, 2.0) / 2.0);
}

static double c_integral(double x, double eps)
{
	if(fabs(x - 1) > eps)
		return log(1.0 / (1.0 - x));

	return NAN;
}

static double d_integral(double x, double eps)
{
	return pow(x, x);
}

static double left_integral(int n, double eps, double (*func)(double, double))
{
	double total = 0.0;
	double step = 1.0 / (double)n;

	for(int i = 0; i < n; i++) {
		double x = i * step;

		double result = func(x, eps);
		if(isnan(result))
			continue;

		total += result;
	}

	return step * total;
}

double adaptive_integral(double eps, double (*func)(double, double))
{
	double prev_result;

	int n = 10;
	int count_iterations = 0;

	while(count_iterations++ < MAX_ITERATION) {
		double cur_result = left_integral(n, eps, func);

		if(count_iterations > 1 && fabs(cur_result - prev_result) < eps)
			return cur_result;

		n *= 2;
		prev_result = cur_result;
	}

	return prev_result;
}

	
	
void print_result(double eps, double x)
{
	double (*sum_funcs[3])(double, double) = {b_sum, c_sum, d_sum};
	double (*integral_funcs[4])(double, double) = {a_integral, b_integral, c_integral, d_integral};

	puts("Значения сумм:");
	printf("a). %lf\n", a_sum(eps, x));

	for(int i = 0; i < 3; i++)
		printf("%c). %lf\n", ASCII_TAB + i + 1, adaptive_sum(eps, x, (i == 2) ? 0.0 : 1.0, sum_funcs[i]));

	puts("\nЗначения интегралов:");
	for(int i = 0; i < 4; i++)
		printf("%c). %lf\n", ASCII_TAB + i, adaptive_integral(eps, integral_funcs[i]));
}

int main(int argc, char *argv[])
{
	if(argc < 3) {
		puts("Введено недостаточно аргументов");
		return 1;
	}

	char *eps_ptr = argv[EPS_IND];
	char *x_ptr = argv[X_IND];

	char *end_ptr;

	if(*eps_ptr == '-') {
		puts("Введенное эпсилон должно быть положительным числом");
		return 1;
	}

	double eps = strtod(eps_ptr, &end_ptr);
	if(*end_ptr != '\0') {
		puts("Неверно введенное значение эпсилон");
		return 1;
	}

	double x = strtod(x_ptr, &end_ptr);
	if(*end_ptr != '\0') {
		puts("Неверно введенное знаение x");
		return 1;
	}

	if(isinf(eps) || isinf(x)) {
		puts("Ошибка переполнения");
		return 1;
	}

	print_result(eps, x);
	return 0;
}

