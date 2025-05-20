#!/bin/bash
PROGRAM="./s21_grep"
PROGRAM_SYS="grep"
PATTERN="сон"
TEST_FILE="test.txt"
DEBUG_FILE="debug.log"
GREENS='\e[32m'
GREENF='\e[0m'
REDS='\e[31m'
REDF='\e[0m'
FLAGS=(
    "-e"
    "-i"
    "-v"
    "-c"
    "-l"
    "-n"
    "-h"
    "-s"
    "-f"
)

> "$DEBUG_FILE"


# Проверка наличия файла для тестов
if [ ! -f "$TEST_FILE" ]; then
    echo -e "${REDS}Ошибка: файл $TEST_FILE не найден${REDF}" | tee -a "$DEBUG_FILE"
    exit 1
fi

grep_tests() {
    flag="$1"
    test_case="$2"
    if [ "$flag" = "-f" ]; then
        case "test_case" in
            "one_pattern") echo "сон" > patterns.txt ;;
            "multiple_patterns") echo -e "сон\nдень\nутро" > patterns.txt ;;
            "empty_file") > patterns.txt ;; # Пустой файл
            "special_characters") echo -e "\\(сон\n\\^день\n\\.утро" > patterns.txt ;;
            #"long_pattern") yes "сон" | head -c 1000000 > patterns.txt ;; # Очень длинный шаблон
            "nonexistent_file") rm -f patterns.txt ;; # Несуществующий файл
        esac

        # Указание файла шаблонов
        pattern_file="patterns.txt"
        if [ "$2" = "nonexistent_file" ]; then
            pattern_file="nonexistent_patterns.txt"
        fi

        # Запуск тестов
        grep_example=$("$PROGRAM_SYS" "$flag" "$pattern_file" "$TEST_FILE" 2>/dev/null)
        s21_grep_example=$("$PROGRAM" "$flag" "$pattern_file" "$TEST_FILE" 2>/dev/null)
        [ -f patterns.txt ] && rm -f patterns.txt
    else
        grep_example=$("$PROGRAM_SYS" "$flag" "$PATTERN" "$TEST_FILE" 2>/dev/null)
        s21_grep_example=$("$PROGRAM" "$flag" "$PATTERN" "$TEST_FILE" 2>/dev/null)
    fi

    echo "====== Тест $flag $test_case ======" >> "$DEBUG_FILE"
    echo "System grep output:" >> "$DEBUG_FILE"
    echo "$grep_example" >> "$DEBUG_FILE"
    echo "s21_grep output:" >> "$DEBUG_FILE"
    echo "$s21_grep_example" >> "$DEBUG_FILE"
    echo "==================================" >> "$DEBUG_FILE"

    # Сравнение результатов
    if diff <(echo "$grep_example") <(echo "$s21_grep_example") > /dev/null; then
        echo -e "${GREENS}тест $flag $2 пройден${GREENF}"
    else
        echo -e "${REDS}тест $flag $2 не пройден${REDF}";
        exit 1;
    fi
}

# Запуск тестов для флагов
for flag in "${FLAGS[@]}"; do
    if [ "$flag" = "-f" ]; then
        grep_tests "$flag" "one_pattern"
        grep_tests "$flag" "multiple_patterns"
        grep_tests "$flag" "empty_file"
        grep_tests "$flag" "special_characters"
        #grep_tests "$flag" "long_pattern"
        grep_tests "$flag" "nonexistent_file"
    else
        grep_tests "$flag"
    fi
done


