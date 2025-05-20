#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int error_flag;
} flags;

void print_file_content(FILE *file);
void cat_no_arg();
flags flags_reader(int argc, char *argv[]);
void file_reader(int argc, char *argv[], flags config);
int file_check(const char *file_name);
void file_print(const char *file_name, const flags *config);
void flags_test(int argc, char *argv[], flags config);

#endif