#!/bin/bash

BINARY="../bin/eight"
PASS=0
FAIL=0

TEST_DIR=$(mktemp -d)
trap 'rm -rf "$TEST_DIR"' EXIT

run_test() {
    local description="$1"
    local input_content="$2"
    local expected="$3"

    printf "$input_content" > "$TEST_DIR/in.txt"
    actual=$($BINARY "$TEST_DIR/in.txt" "$TEST_DIR/out.txt" 2>&1)

    if [ "$actual" = "" ] && [ ! -f "$TEST_DIR/out.txt" ]; then
        echo "✗ FAIL: $description (программа не создала выходной файл)"
        ((FAIL++))
        rm -f "$TEST_DIR/out.txt"
        return
    fi

    result=$(cat "$TEST_DIR/out.txt" 2>/dev/null)

    if [ "$result" = "$expected" ]; then
        echo "✓ PASS: $description"
        ((PASS++))
    else
        echo "✗ FAIL: $description"
        echo "  Ожидалось: $expected"
        echo "  Получено:  $result"
        ((FAIL++))
    fi

    rm -f "$TEST_DIR/out.txt"
}

run_message_test() {
    local description="$1"
    local args="$2"
    local expected="$3"

    actual=$($BINARY $args 2>&1)

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

echo "=== Тестирование задания eight ==="
echo

echo "--- Основные тесты ---"

run_test "двоичное и восьмеричное число" "101 007\n" "101 2 5
7 8 7"

run_test "шестнадцатеричное в верхнем и нижнем регистре" "FF ff\n" "FF 16 255
ff 16 255"

run_test "пробелы табуляции и переносы строк" "  1\t2\t\t3 \n\n 4  \t\n" "1 2 1
2 3 2
3 4 3
4 5 4"

run_test "число 0" "0\n" "0 2 0"

run_test "ведущие нули отбрасываются" "0001010\n" "1010 2 10"

run_test "нулевое с ведущими нулями" "0000\n" "0 2 0"

run_test "буква z (max основание 36)" "z\n" "z 36 35"

run_test "смешанные основания" "2345 6Y G8\n" "2345 6 569
6Y 35 244
G8 17 280"

run_test "большое шестнадцатеричное число" "FFFFFFFFFFFFFFF\n" "FFFFFFFFFFFFFFF 16 1152921504606846975"

run_test "десятичное число без букв" "123456789\n" "123456789 10 123456789"

echo
echo "--- Тесты ошибок ---"

run_message_test "мало аргументов" "" "Введено недостаточно аргументов"

run_message_test "совпадают имена файлов" "$TEST_DIR/in.txt $TEST_DIR/in.txt" "Имена входного и выходного файлов совпадают"

run_message_test "некорректный формат входного файла" "$TEST_DIR/in.dat $TEST_DIR/out.txt" "Некорректный формат файла"

run_message_test "некорректный формат выходного файла" "$TEST_DIR/in.txt $TEST_DIR/out.dat" "Некорректный формат файла"

run_message_test "несуществующий входной файл" "$TEST_DIR/nofile.txt $TEST_DIR/out.txt" "Ошибка во время открытия файла"

printf "   \t\n" > "$TEST_DIR/empty.txt"
run_message_test "пустой файл (только пробелы)" "$TEST_DIR/empty.txt $TEST_DIR/out.txt" "Во входном файле отсутствуют числа"

printf "12@3\n" > "$TEST_DIR/bad.txt"
run_message_test "некорректный символ в числе" "$TEST_DIR/bad.txt $TEST_DIR/out.txt" "Во входном файле встречается некорректное число"

printf "ZZZZZZZZZZZZZZZZZZZZ\n" > "$TEST_DIR/big.txt"
run_message_test "переполнение long long" "$TEST_DIR/big.txt $TEST_DIR/out.txt" "Число не помещается в диапазон типа long long"

echo
echo "Пройдено: $PASS | Провалено: $FAIL | Всего: $((PASS + FAIL))"

[ $FAIL -eq 0 ] && exit 0 || exit 1
