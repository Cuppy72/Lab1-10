#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

#define FIX_SIZE 20

#define A_IND 1
#define B_IND 2

#define MIN_DYN_SIZE 10
#define MAX_DYN_SIZE 10000

#define MIN_VALUE -1000
#define MAX_VALUE 1000

#define INT_SPAN (((unsigned long long)INT_MAX - (unsigned long long)INT_MIN) + 1ULL)

#define INCORRECT_NUM 25
#define INCORRECT_RANGE 26
#define WIDE_RANGE 27
#define ALLOC_ERROR 28

#define SUCCESS 10

static int read_int(char *str, long long *num)
{
	char *ptr = str;

	if(*ptr == '-' || *ptr == '+')
		ptr++;

	if(*ptr == '\0')
		return false;

	for(; *ptr != '\0'; ptr++) {
		if(*ptr < '0' || *ptr > '9')
			return false;
	}

	char *end_ptr = NULL;

	errno = 0;
	*num = strtoll(str, &end_ptr, 10);

	if(errno != 0 || end_ptr == str || *end_ptr != '\0')
		return false;

	if(*num < INT_MIN || *num > INT_MAX)
		return false;

	return true;
}

static unsigned long long rand_bits(unsigned bits)
{
	unsigned long long value = 0;

	for(unsigned i = 0; i < bits; i++)
		value = (value << 1) | ((unsigned long long)rand() & 1u);

	return value;
}

static int rand_in_range(int left, int right)
{
	unsigned long long span = (unsigned long long)right - (unsigned long long)left + 1ULL;

	unsigned bits = 0;

	while((1ULL << bits) < span)
		bits++;

	unsigned long long limit = (UINT64_MAX / span) * span;

	unsigned long long value;

	do {
		value = rand_bits(bits);
	} while(value >= limit);

	return (int)((unsigned long long)left + value % span);
}

static void print_array(int *arr, int size)
{
	for(int i = 0; i < size; i++)
		printf("%d ", arr[i]);

	puts("");
}

static void swap_min_max(int *arr, int size)
{
	int min_ind = 0;
	int max_ind = 0;

	for(int i = 1; i < size; i++) {
		if(arr[i] < arr[min_ind])
			min_ind = i;

		if(arr[i] > arr[max_ind])
			max_ind = i;
	}

	int tmp = arr[min_ind];

	arr[min_ind] = arr[max_ind];
	arr[max_ind] = tmp;
}

static int find_nearest(int value, int *arr, int size)
{
	int nearest = arr[0];
	long long best_diff = (long long)value - nearest;

	if(best_diff < 0)
		best_diff = -best_diff;

	for(int i = 1; i < size; i++) {
		long long diff = (long long)value - arr[i];

		if(diff < 0)
			diff = -diff;

		if(diff < best_diff) {
			best_diff = diff;
			nearest = arr[i];
		}
	}

	return nearest;
}

static int err_switch(int err)
{
	switch(err) {
		case INCORRECT_NUM:
			puts("Введено некорректное число");
			break;
		case INCORRECT_RANGE:
			puts("Левая граница диапазона должна быть меньше правой");
			break;
		case WIDE_RANGE:
			puts("Диапазон [a..b] не должен превышать диапазон типа int");
			break;
		case ALLOC_ERROR:
			puts("Ошибка во время выделения памяти");
			break;
		case SUCCESS:
		default:
			return 0;
	}

	return 1;
}

static int part_one(int left, int right)
{
	int arr[FIX_SIZE];

	srand(time(NULL));

	for(int i = 0; i < FIX_SIZE; i++)
		arr[i] = rand_in_range(left, right);

	puts("Массив до замены:");
	print_array(arr, FIX_SIZE);

	swap_min_max(arr, FIX_SIZE);

	puts("Массив после замены минимума и максимума:");
	print_array(arr, FIX_SIZE);

	return SUCCESS;
}

static int part_two(void)
{
	int size_a = rand_in_range(MIN_DYN_SIZE, MAX_DYN_SIZE);
	int size_b = rand_in_range(MIN_DYN_SIZE, MAX_DYN_SIZE);

	int *arr_a = malloc(sizeof(int) * size_a);
	int *arr_b = malloc(sizeof(int) * size_b);
	int *arr_c = malloc(sizeof(int) * size_a);

	if(arr_a == NULL || arr_b == NULL || arr_c == NULL) {
		free(arr_a);
		free(arr_b);
		free(arr_c);

		return ALLOC_ERROR;
	}

	for(int i = 0; i < size_a; i++)
		arr_a[i] = rand_in_range(MIN_VALUE, MAX_VALUE);

	for(int i = 0; i < size_b; i++)
		arr_b[i] = rand_in_range(MIN_VALUE, MAX_VALUE);

	for(int i = 0; i < size_a; i++)
		arr_c[i] = arr_a[i] + find_nearest(arr_a[i], arr_b, size_b);

	printf("Размер массива A: %d, размер массива B: %d\n", size_a, size_b);

	puts("Массив A:");
	print_array(arr_a, size_a);

	puts("Массив B:");
	print_array(arr_b, size_b);

	puts("Массив C:");
	print_array(arr_c, size_a);

	free(arr_a);
	free(arr_b);
	free(arr_c);

	return SUCCESS;
}

int main(int argc, char *argv[])
{
	if(argc < 3) {
		puts("Введено недостаточно аргументов");
		return 1;
	}

	long long left;
	long long right;

	if(!read_int(argv[A_IND], &left) || !read_int(argv[B_IND], &right))
		return err_switch(INCORRECT_NUM);

	if(left >= right)
		return err_switch(INCORRECT_RANGE);

	if(right - left > INT_DIFF)
		return err_switch(WIDE_RANGE);

	int err = part_one((int)left, (int)right);
	if(err != SUCCESS)
		return err_switch(err);

	err = part_two();
	if(err != SUCCESS)
		return err_switch(err);

	return 0;
}
