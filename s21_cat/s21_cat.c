#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cat_no_arg();
  } else if (argc > 1) {
    flags config = flags_reader(argc, argv);
    if (config.error_flag == 1) {
      fprintf(stderr, "Try 'cat --help' for more information.\n");
      return 1;
    }
    file_reader(argc, argv, config);
  }
  return 0;
}

void cat_no_arg() {
  char buffer[4096];
  int fd = STDIN_FILENO;
  int bytes_read = read(fd, buffer, 4096);
  while (bytes_read > 0) {
    printf("%.*s", bytes_read, buffer);
    bytes_read = read(fd, buffer, 4096);
  }
}

flags flags_reader(int argc, char *argv[]) {
  flags config = {0, 0, 0, 0, 0, 0, 0};
  const struct option long_options[] = {{"number-nonblank", 0, &config.b, 1},
                                        {"number", 0, &config.n, 1},
                                        {"squeeze-blank", 0, &config.s, 1},
                                        {NULL, 0, NULL, 0}};

  int current_flag = getopt_long(argc, argv, "+bevEnstT", long_options, NULL);
  while (current_flag != -1) {
    switch (current_flag) {
      case 'b':
        config.b = 1;
        break;
      case 'e':
        config.e = 1;
        config.v = 1;
        break;
      case 'v':
        config.v = 1;
        break;
      case 'E':
        config.e = 1;
        break;
      case 'n':
        config.n = 1;
        break;
      case 's':
        config.s = 1;
        break;
      case 't':
        config.t = 1;
        config.v = 1;
        break;
      case 'T':
        config.t = 1;
        break;
      case '?':
        config.error_flag = 1;
        break;
    }
    current_flag = getopt_long(argc, argv, "+bevEnstT", long_options, NULL);
  }
  if ((config.n == 1) && (config.b == 1)) {
    config.n = 0;
  }
  return config;
}

void file_reader(int argc, char *argv[], flags config) {
  for (int i = optind; i < argc; i++) {
    if (file_check(argv[i])) {
      file_print(argv[i], &config);
    } else {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
    }
  }
}

int file_check(const char *file_name) {
  int result = 0;
  FILE *fp = NULL;
  fp = fopen(file_name, "r");
  if (fp != NULL) {
    result = 1;
    fclose(fp);
  }
  return result;
}

void file_print(const char *file_name, const flags *config) {
  FILE *fp = NULL;
  fp = fopen(file_name, "r");
  if (fp != NULL) {
    int counter = 1;
    int empty_line = 0;
    int ch_prev = '\n';
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
      if (config->s && ch == '\n' && ch_prev == '\n') {  //флаг s
        empty_line++;
        if (empty_line > 1) {
          continue;
        }
      } else {
        empty_line = 0;
      }
      if (ch_prev == '\n' &&
          ((config->b && ch != '\n') || config->n))  //флаг b и n
        printf("%6d\t", counter++);
      if (config->t && ch == '\t') {  //флаг t
        printf("^");
        ch = 'I';
      }
      if (config->e && ch == '\n') {  //флаг e
        printf("$");
      }
      if (config->v) {  //флаг v
        if (ch == '\r') {
          printf("^M");
        } else if ((ch >= 0 && ch < 9) || (ch > 10 && ch < 32) ||
                   (ch > 126 && ch <= 160)) {
          printf("^%c", (ch < 32) ? (ch + 64) : (ch - 64));
        } else {
          printf("%c", ch);
        }

      } else {
        printf("%c", ch);
      }
      ch_prev = ch;
    }
    fclose(fp);
  }
}