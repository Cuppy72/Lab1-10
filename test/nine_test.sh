#!/bin/bash

BINARY="../bin/nine"
PASS=0
FAIL=0

run_message_test() {
    local description="$1"
    shift
    local expected="$1"
    shift

    actual=$("$BINARY" "$@" 2>&1)

    if [ "$actual" = "$expected" ]; then
        echo "✓ PASS: $description"
        ((PASS++))
    else
        echo "✗ FAIL: $description"
        echo "  Ожидалось: $expected"
        echo "  Получено:  $actual"
        ((FAIL++))
    fi
}

run_output_test() {
    local description="$1"
    local pattern="$2"
    shift 2

    actual=$("$BINARY" "$@" 2>&1 | head -1)

    if [[ $actual =~ $pattern ]]; then
        echo "✓ PASS: $description"
        ((PASS++))
    else
        echo "✗ FAIL: $description"
        echo "  Ожидался шаблон: $pattern"
        echo "  Получено:  $actual"
        ((FAIL++))
    fi
}

check_correctness() {
    local description="$1"
    shift

    output=$("$BINARY" "$@" 2>&1)

    lo="$1"; hi="$2"; shift 2

    result=$(echo "$output" | awk -v lo="$lo" -v hi="$hi" '
    /^Массив до замены:$/ { stage = 1; next }
    /^Массив после замены минимума и максимума:$/ { stage = 2; next }
    /^Размер массива A:/ { stage = 0; next }
    /^Массив A:$/ { stage = 3; next }
    /^Массив B:$/ { stage = 4; next }
    /^Массив C:$/ { stage = 5; next }
    stage == 1 { for (i = 1; i <= NF; i++) before[++nb] = $i }
    stage == 2 { for (i = 1; i <= NF; i++) after[++na] = $i }
    stage == 3 { for (i = 1; i <= NF; i++) a[na_a++] = $i }
    stage == 4 { for (i = 1; i <= NF; i++) b[na_b++] = $i }
    stage == 5 { for (i = 1; i <= NF; i++) c[na_c++] = $i }
    END {
        if (nb != 20 || na != 20) { print "bad size"; exit }

        for (i = 1; i <= nb; i++) {
            if (before[i] < lo || before[i] > hi) { print "out of range"; exit }
        }

        minv = before[1]; maxv = before[1]; maxi = 1; mini = 1
        for (i = 1; i <= nb; i++) {
            if (before[i] < minv) { minv = before[i]; mini = i }
            if (before[i] > maxv) { maxv = before[i]; maxi = i }
        }

        for (i = 1; i <= nb; i++) expected[i] = before[i]
        expected[mini] = before[maxi]
        expected[maxi] = before[mini]

        for (i = 1; i <= nb; i++)
            if (after[i] != expected[i]) { print "bad swap"; exit }

        for (i = 1; i <= nb; i++) {
            ok = 0
            for (j = 1; j <= na; j++)
                if (before[i] == after[j]) { ok = 1; break }
            if (!ok) { print "lost element"; exit }
        }

        if (na_a < 10 || na_a > 10000 || na_b < 10 || na_b > 10000) { print "bad dyn size"; exit }
        if (na_c != na_a) { print "bad C size"; exit }

        for (i = 0; i < na_a; i++) {
            best = -1
            for (j = 0; j < na_b; j++) {
                d = a[i] - b[j]; if (d < 0) d = -d
                if (best < 0 || d < best) best = d
            }
            found = 0
            for (j = 0; j < na_b; j++) {
                d2 = a[i] - b[j]; if (d2 < 0) d2 = -d2
                if (d2 == best && c[i] - a[i] == b[j]) { found = 1; break }
            }
            if (!found) { print "bad nearest"; exit }
        }

        print "OK"
    }')

    if [ "$result" = "OK" ]; then
        echo "✓ PASS: $description"
        ((PASS++))
    else
        echo "✗ FAIL: $description ($result)"
        ((FAIL++))
    fi
}

echo "=== Тестирование задания nine ==="
echo

echo "--- Основные тесты ---"

run_output_test "корректный запуск с положительным диапазоном" "^Массив до замены:$" 1 100

run_output_test "корректный запуск с отрицательным диапазоном" "^Массив до замены:$" -50 50

run_output_test "вывод первой строки фиксированного массива" "^Массив до замены:$" 0 9

check_correctness "один прогон: swapping и ближайший элемент (диапазон [-100..100])" -100 100

check_correctness "второй прогон: swapping и ближайший элемент (диапазон [0..1000])" 0 1000

check_correctness "третий прогон: swapping и ближайший элемент (диапазон [-1000..1000])" -1000 1000

echo
echo "--- Тесты ошибок ---"

run_message_test "мало аргументов" "Введено недостаточно аргументов"

run_message_test "один аргумент" "Введено недостаточно аргументов" 5

run_message_test "некорректное первое число" "Введено некорректное число" abc 10

run_message_test "некорректное второе число" "Введено некорректное число" 1 1x

run_message_test "число со знаком в середине" "Введено некорректное число" 1- 5

run_message_test "два знака подряд" "Введено некорректное число" +-5 9

run_message_test "пустая строка вместо числа" "Введено некорректное число" "" 5

run_message_test "обратный диапазон" "Левая граница диапазона должна быть меньше правой" 10 5

run_message_test "равные границы диапазона" "Левая граница диапазона должна быть меньше правой" 7 7

run_message_test "выход за диапазон int" "Введено некорректное число" 2147483648 3000000000

# Проверка отсечения широкого диапазона: границы лежат в int, но разность больше
# INT_MAX - INT_MIN (например, ровно на 1). Для такого входа программа должна
# напечатать сообщение об ошибке и завершиться с кодом 1.
wide_min=$(( -2147483647 - 1 ))
wide_max=2147483647
wide_output=$("$BINARY" "$wide_min" "$wide_max" 2>&1)
if [ "$wide_output" = "Диапазон [a..b] не должен превышать диапазон типа int" ]; then
    echo "✓ PASS: слишком широкий диапазон (ровно на 1 больше ёмкости int)"
    ((PASS++))
else
    echo "✗ FAIL: слишком широкий диапазон (ровно на 1 больше ёмкости int)"
    echo "  Ожидалось: Диапазон [a..b] не должен превышать диапазон типа int"
    echo "  Получено:  $wide_output"
    ((FAIL++))
fi

echo
echo "Пройдено: $PASS | Провалено: $FAIL | Всего: $((PASS + FAIL))"

[ $FAIL -eq 0 ] && exit 0 || exit 1
