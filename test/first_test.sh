#!/bin/bash

BINARY="../bin/first"
PASS=0
FAIL=0

run_test() {
    local description="$1"
    local args="$2"
    local expected="$3"
    
    actual=$($BINARY $args 2>&1)
    
    if [ "$actual" = "$expected" ]; then
        echo "✓ PASS: $description"
        ((PASS++))
    else
        echo " FAIL: $description"
        echo "  Ожидалось: $expected"
        echo "  Получено:  $actual"
        ((FAIL++))
    fi
}

echo "=== Тестирование задания first ==="

run_test "h_flag: числа кратные 5" "5 -h" "5
10
15
20
25
30
35
40
45
50
55
60
65
70
75
80
85
90
95
100"

run_test "h_flag: числа кратные 7" "7 -h" "7
14
21
28
35
42
49
56
63
70
77
84
91
98"

run_test "h_flag: нет кратных" "150 -h" "Подходящие числа отсутсвуют"

run_test "p_flag: простое число" "7 -p" "Число простое"

run_test "p_flag: простое число 2" "2 -p" "Число простое"

run_test "p_flag: составное число" "10 -p" "Число составное"

run_test "p_flag: составное число 15" "15 -p" "Число составное"

run_test "p_flag: отрицательное простое" "-7 -p" "Число простое"

run_test "s_flag: 255 в hex" "255 -s" "F F "

run_test "s_flag: 16 в hex" "16 -s" "1 0 "

run_test "s_flag: 256 в hex" "256 -s" "1 0 0 "

run_test "s_flag: 10 в hex" "10 -s" "A "

run_test "s_flag: 0 в hex" "0 -s" "0"

run_test "s_flag: 25 в hex" "25 -s" "1 9 "

run_test "e_flag: таблица степеней x=1" "1 -e" "1    1    1    1    1    1    1    1    1    1    "

run_test "e_flag: превышение ограничения" "11 -e" "Число не подходит под условие функции"

run_test "e_flag: отрицательное число" "-1 -e" "Число должно быть больше нуля"

run_test "a_flag: сумма до 5" "5 -a" "15"

run_test "a_flag: сумма до 10" "10 -a" "55"

run_test "a_flag: сумма до 100" "100 -a" "5050"

run_test "a_flag: сумма до 1" "1 -a" "1"

run_test "a_flag: отрицательное число" "-5 -a" "Число должно быть больше нуля"

run_test "f_flag: факториал 5" "5 -f" "120"

run_test "f_flag: факториал 0" "0 -f" "1"

run_test "f_flag: факториал 1" "1 -f" "1"

run_test "f_flag: факториал 10" "10 -f" "3628800"

run_test "f_flag: отрицательное число" "-5 -f" "Число должно быть больше нуля"

run_test "несколько флагов: -h -p" "6 -h -p" "6
12
18
24
30
36
42
48
54
60
66
72
78
84
90
96
Число составное"

run_test "флаг /h" "5 /h" "5
10
15
20
25
30
35
40
45
50
55
60
65
70
75
80
85
90
95
100"

run_test "флаг /p" "7 /p" "Число простое"

run_test "неизвестный флаг" "5 -x" "Введен неизвестный флаг x"

run_test "некорректное число" "abc -h" "Введено некорректное число"

run_test "мало аргументов" "5" "Введено слишком мало аргументов"

run_test "переполнение факториала" "21 -f" "Ошибка переполнения"

echo
echo "Пройдено: $PASS | Провалено: $FAIL | Всего: $((PASS + FAIL))"

[ $FAIL -eq 0 ] && exit 0 || exit 1
