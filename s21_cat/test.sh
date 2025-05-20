#!/bin/bash
GREENS='\e[32m'
GREENF='\e[0m'
REDS='\e[31m'
REDF='\e[0m'
FILE_TEST='testing_file.txt'
PROGRAM="./s21_cat"
SOURCE_FILE="s21_cat.c"
FLAGS=(
	"-n"
	"-s"
	"-t"
	"-T"
	"-v"
	"-e"
	"--number-nonblank"
	"--number"
	"--squeeze-blank"
)


ALL_PASSED=true
generate_combinations() {
	local flags=("$@")
	local n=${#flags[@]}
	local combinations=()

	for ((i = 0; i < (1 << n); i++)); do
		local combinations=""
		for ((j = 0; j < n; j++)); do
			if ((i & (1 << j))); then
				combinations+="${flags[j]} "
			fi
		done
		combinations+=("$combination")
	done

	echo "${combinations[@]}"
}

ALL_COMBINATIONS=($(generate_combinations "${FLAGS[@]}"))

run_test() {
	local flag="$1"
	if diff <(cat $flag $FILE_TEST) <($PROGRAM $flag $FILE_TEST) > /dev/null; then
		echo -e "${GREENS}Тест на $flag пройден${GREENF}"
	else
		echo -e "${REDS}Тест на $flag не пройден${REDF}";
		exit 1;
	fi
}

#for flag in "${FLAGS[@]}"; do
	#run_test "$flag"
#done

run_test_combinations() {
	local combination="$1"
	if diff <(eval cat $combination $FILE_TEST) <(eval $PROGRAM $combination $FILE_TEST) > /dev/null; then
		echo -e "${GREENS}Тест на \"$combination\" пройден${GREENF}"
	else
		echo -e "${REDS}Тест на \"$combination\" не пройден${REDF}";
		exit 1;
	fi
}

for combination in "${ALL_COMBINATIONS[@]}"; do
	run_test_combinations "$combination"
done

if diff <(cat -etsnb $FILE_TEST) <($PROGRAM -etsnb $FILE_TEST) > /dev/null; then
		echo -e "${GREENS}Тест на все флаги пройден${GREENF}"
	else
		echo -e "${REDS}Тест на все флаги не пройден${REDF}";
		exit 1;
fi

clang_test() {
	cp ../../materials/linters/.clang-format .clang-format
	echo "clang-format скопирован в src"
	clang-format -n $SOURCE_FILE
	echo "Проверка кода по clang"
	#clang-format -i $SOURCE_FILE
	#echo "Форматирование кода по clang завершено"
	rm -f .clang-format
	echo "clang-format удален из src"
}
clang_test

run_memory_leak_test() {
	flag="$1"
	valgrind_output=$(valgrind --leak-check=full --show-leak-kinds=all --track-origin=yes $PROGRAM $flag $FILE_TEST 2>&1)
	if echo "$valgrind_output" | grep -q "definitely lost"; then
        echo -e "${REDS}Тест на утечку памяти с флагом $flag не пройден${REDF}"
        echo "$valgrind_output"
		exit 1
    else
        echo -e "${GREENS}Тест на утечку памяти с флагом $flag пройден${GREENF}"
    fi
}

for flag in "${FLAGS[@]}"; do
    run_memory_leak_test "$flag"
done

run_cppcheck() {
    echo -e "Запуск статического анализа с помощью cppcheck..."
    
    cppcheck --enable=all --inconclusive --language=c --std=c11 --force --verbose \
             --error-exitcode=1 --suppress=missingIncludeSystem --max-configs=100 "$SOURCE_FILE"
             
    # Проверяем код возврата
    if [[ $? -eq 0 ]]; then
        echo -e "${GREENS}cppcheck: Ошибки не обнаружены${GREENF}"
    else
        echo -e "${REDS}cppcheck: Найдены ошибки! Проверьте код.${REDF}"
        exit 1 # Завершаем скрипт с ошибкой, если cppcheck нашел проблемы
    fi
}
echo "Начало проверки проекта..."
run_cppcheck

if $ALL_PASSED; then
    echo -e "${GREENS}Все тесты пройдены успешно!${GREENF}"
else
    echo -e "${REDS}Некоторые тесты не пройдены.${REDF}"
fi