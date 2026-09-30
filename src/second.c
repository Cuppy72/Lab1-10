#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define EPS_IND 1
#define MAX_ITERATION 1e5

#define NOT_A_NUM -2

#define BUFF_SIZE 1024

double gamma_equation(double eps);

double e_lim(double eps)
{
	double prev_result = 0.0;

	int n = 1.0;
	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		double cur_result = pow(1.0 + (1.0 / (double)n), (double)n);

		if(count_iters > 1 && fabs(cur_result - prev_result) < eps)
			return cur_result;

		n++;
		prev_result = cur_result;
	}

	return prev_result;
}

static double factorial(double n)
{
	double result = 1.0;

	while(n > 1.0) {
		result *= n;

		if(isinf(result))
			return result;

		n -= 1.0;
	}

	return result;
}

double e_row(double eps)
{
	double result = 0.0;

	int n = 0;
	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		double cur_num = 1.0 / factorial((double)n);
		
		result += cur_num;
		if(cur_num < eps)
			return result;

		n++;
	}

	return result;
}

double e_equation(double eps)
{
	double a = 2.0;
	double b = 3.0;
	double c;

	while(fabs(a - b) > eps) {
		c = (a + b) / 2.0;

		if(fabs(log(c) - 1.0) < eps)
			return c;
		
		if((log(a) - 1.0) * (log(c) - 1.0) < 0.0)
			b = c;
		else
			a = c;
	}

	return c;
}

double pi_lim(double eps)
{
	double prev_result = 0.0;
	double cur_result = 4.0;
	
	int n = 1;
	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		cur_result = (cur_result * 4.0 * (double)n * ((double)n + 1.0)) / pow(2.0 * (double)n + 1.0, 2.0);

		if(count_iters > 1 && fabs(cur_result - prev_result) < eps)
			return cur_result;
		
		n++;
		prev_result = cur_result;
	}

	return cur_result;
}

double pi_row(double eps)
{
	double result = 0.0;
	
	int n = 1;
	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		double cur_num;

		if(n % 2 == 1)
			cur_num = 4.0 / (2.0 * (double)n - 1.0);
		else
			cur_num = (-4.0) / (2.0 * (double)n - 1.0);
		
		result += cur_num;
		if(fabs(cur_num) < eps)
			return result;

		n++;
	}

	return result;
}

double pi_equation(double eps)
{
	double prev_result = 0.0;
	double x = 3.5;

	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		x += (cos(x) + 1.0) / sin(x);

		if(count_iters > 1 && fabs(x - prev_result) < eps)
			return x;

		prev_result = x;
	}

	return x;
}

double ln_lim(double eps)
{
	double prev_result = 0.0;

	int n = 1;
	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		double cur_result = (double)n * (pow(2.0, 1.0 / (double)n) - 1.0);

		if(count_iters > 1 && fabs(cur_result - prev_result) < eps)
			return cur_result;

		prev_result = cur_result;
		n++;
	}

	return prev_result;
}

double ln_row(double eps)
{
	double result = 0.0;

	int n = 1;
	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		double cur_num;

		if(n % 2 == 1)
			cur_num = 1.0 / (double)n;
		else
			cur_num = (-1.0) / (double)n;

		result += cur_num;
		if(fabs(cur_num) < eps)
			return result;

		n++;
	}

	return result;
}

double ln_equation(double eps)
{
	double prev_result = 0.0;
	double x = 1.0;

	int count_iters = 0;
	
	while(count_iters++ < MAX_ITERATION) {
		x -= (exp(x) - 2.0) / exp(x);

		if(count_iters > 1 && fabs(x - prev_result) < eps)
			return x;

		prev_result = x;
	}

	return x;
}

double sqrt_lim(double eps)
{
	double prev_result = -0.5;

	int count_iters = 0;
	
	while(count_iters++ < MAX_ITERATION) {
		double cur_result = prev_result - pow(prev_result, 2.0) / 2.0 + 1.0;

		if(fabs(cur_result - prev_result) < eps)
			return cur_result;

		prev_result = cur_result;
	}

	return prev_result;
}

double sqrt_row(double eps)
{
	double result = 1.0;

	int k = 2;
	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		double cur_num = pow(2.0, pow(2.0, -(double)k));

		result *= cur_num;
		if(cur_num < eps)
			return result;

		k++;
	}

	return result;
}

double sqrt_equation(double eps)
{
	double prev_result = 0.0;
	double x = 2.0;

	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		x -= ((x / 2.0) - (1.0 / x));

		if(count_iters > 1 && fabs(prev_result - x) < eps)
			return x;

		prev_result = x;
	}

	return x;
}

static double combinations(int m, int k) {
	if (k > m - k) {
		k = m - k;
	}
	
	double result = 1.0;
	
	for (int i = 1; i <= k; i++) {
		result *= ((double)m - (double)i + 1.0) / (long double)i;

		if(isinf(result))
			return result;
	}
	
	return result;
}

double gamma_lim(double eps)
{
	double prev_result;
	double total = 0.0;

	int n = 1;
	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		double cur_num = 1.0 / (double)n;
		total += cur_num;

		double cur_result = total - log(n);
		if(count_iters > 1 && fabs(cur_result - prev_result) < eps)
			return cur_result;

		n++;
		prev_result = cur_result;
	}

	return prev_result;
}

static double machine_zero(void)
{
	double eps = 1.0;

	while(1.0 + eps / 2.0 > 1.0)
		eps /= 2.0;

	return eps;
}

double gamma_row(double eps)
{
	double total = 0.0;

	int k = 1;
	int count_iters = 0;

	while(count_iters++ < MAX_ITERATION) {
		double cur_num = (1.0 / (double)k - log(1.0 + 1.0 / (double)k));
		total += cur_num;

		if(fabs(cur_num) < eps)
			break;

		k++;
	}

	return total;
}

static bool is_prime(int num)
{
	for(int i = 2; i < (num / 2) + 1; i++) 
		if(num % i == 0)
			return false;

	return true;
}

double gamma_equation(double eps)
{
	double total;

	double prev_prod = 0;
	double cur_prod = 1.0;

	int p;
	int count_iters = 0;

	for(p = 2; count_iters++ < MAX_ITERATION; p++) {
		if(!is_prime(p))
			continue;

		cur_prod *= ((double)p - 1.0) / (double)p;
		total = -log(log((double)p) * cur_prod);

		if(count_iters > 1 && fabs(total - prev_prod) < eps)
			return total;

		prev_prod = total;
	}
	
	return total;
}	

void tab(int x)
{
	for(int i = 0; i < x; i++)
		putchar(' ');
}

void print_menu(double eps)
{
	const char *labels[] = {"", "lim", "row/prod", "equation"};
	const char *constants[] = {"e", "pi", "ln2", "sqrt(2)", "gamma"};

	const double (*funcs_for_e[5][3])(double) = {
		{e_lim, e_row, e_equation},
		{pi_lim, pi_row, pi_equation},
		{ln_lim, ln_row, ln_equation},
		{sqrt_lim, sqrt_row, sqrt_equation},
		{gamma_lim, gamma_row, gamma_equation}
	};

	
	for(int i = 0; i < 4; i++) {
		tab(6);

		printf("%s", labels[i]);

		tab(15 - strlen(labels[i]));
		putchar('|');
	}

	for(int i = 0; i < 5; i++) {
		putchar('\n');
		tab(6);

		printf("%s", constants[i]);

		tab(15 - strlen(constants[i]));
		putchar('|');

		for(int j = 0; j < 3; j++) {
			tab(6);

			printf("%lf", funcs_for_e[i][j](eps));
			tab(7);
			putchar('|');
		}
	}

	putchar('\n');
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		puts("Введено слишком мало аргументов");
		return 1;
	}

	char *eps_ptr = argv[EPS_IND];
	char *end_ptr;
	
	double eps = strtod(eps_ptr, &end_ptr);
	if(*end_ptr != '\0') {
		puts("Введено некорректное значение эпсилон");
		return 1;
	}

	if(eps < 0) {
		puts("Введенное эпсилон должно быть положительным числом");
		return 1;
	}

	if(isinf(eps)) {
		puts("Ошибка переполнения");
		return 1;
	}

	print_menu(eps);

	return 0;
}	

