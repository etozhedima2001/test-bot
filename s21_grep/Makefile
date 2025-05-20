SHELL := /bin/bash
FILE=test.txt
PATTERNS=тревога
CHECK=2>&1 valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all
ERROR=ERROR 
CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

all: s21_grep

s21_grep: s21_grep.o
	$(CC) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -c

tests:
	grep вечная test.txt > result_diff.log
	grep_sys = result_diff.log
	echo $(grep_sys)
clean:
	rm -f *.o s21_grep

rebuild: clean all
