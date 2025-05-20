.PHONY: s21_cat clean rebuild

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11


s21_cat:
	@$(CC) $(CFLAGS) *.c -o s21_cat
	@echo "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"
	@echo "┃          cat is ready to work!          ┃"
	@echo "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"


clean:
	@rm -rf s21_cat
	@echo "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"
	@echo "┃             ДАННЫЕ УДАЛЕНЫ!             ┃"
	@echo "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"

rebuild: clean s21_cat