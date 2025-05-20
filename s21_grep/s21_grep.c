#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define buff_size 5000

struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int error_flag;
  int count_files;
  int count_match;
  int number_line;
};

void s21_grep(const char *path, struct flags *config, const char *pattern);
void printfAuxData(struct flags *config, const char *path);
void printMainData(const char *line, struct flags *config, const regex_t *regex,
                   const char *path);
void create_pattern2(struct flags *config, char **pattern, const char *optarg);
int flags_reader(int argc, char *argv[], struct flags *config, char **pattern);

int main(int argc, char *argv[]) {
  struct flags config = {0};
  char *pattern = NULL;
  int current_file_index = flags_reader(argc, argv, &config, &pattern);
  if (config.error_flag == 1) {
    fprintf(stderr, "error flag\n");
    return 1;
  }

  for (int i = current_file_index; i < argc; i++) {
    s21_grep(argv[i], &config, pattern);
  }
  free(pattern);
  return 0;
}

int flags_reader(int argc, char *argv[], struct flags *config, char **pattern) {
  int current_flag = 0;
  const char *s_options = "e:ivclnhsf:o";
  while ((current_flag = getopt(argc, argv, s_options)) != -1) {
    switch (current_flag) {
      case 'e':
        config->e = 1;
        create_pattern2(config, pattern, optarg);
        break;
      case 'i':
        config->i = 1;
        break;
      case 'v':
        config->v = 1;
        break;
      case 'c':
        config->c = 1;
        break;
      case 'l':
        config->l = 1;
        break;
      case 'n':
        config->n = 1;
        break;
      case 'h':
        config->h = 1;
        break;
      case 's':
        config->s = 1;
        break;
      case 'f':
        config->f = 1;
        create_pattern2(config, pattern, optarg);
        break;
      case 'o':
        config->o = 1;
        break;
      case '?':
        config->error_flag = 1;
        break;
    }
  }
  if (!config->e && !config->f) {
    create_pattern2(config, pattern, argv[optind++]);
  }
  config->count_files = argc - optind;
  return optind;
}

void create_pattern2(struct flags *config, char **pattern, const char *optarg) {
  if (!(*pattern)) {
    *pattern = strdup(optarg);
    if (*pattern) config->e = 1;
  } else {
    size_t new_size = strlen(*pattern) + strlen(optarg) + 2;
    *pattern = realloc(*pattern, new_size);
    if (*pattern) strcat(strcat(*pattern, "|"), optarg);
  }
}

void s21_grep(const char *path, struct flags *config, const char *pattern) {
  FILE *file = fopen(path, "r");
  if (!file) {
    if (!config->s) {
      fprintf(stderr, "grep: %s: No such file or directory\n", path);
    }
  }
  regex_t regex;
  if (regcomp(&regex, pattern, config->i ? REG_ICASE : REG_EXTENDED) != 0) {
    fclose(file);
    fprintf(stderr, "invalid regex pattern");
  }
  // char line[1024];
  char *line = NULL;
  size_t len = 0;
  struct flags mutableConfig = *config;
  while (getline(&line, &len, file) != -1) {
    mutableConfig.number_line++;
    printMainData(line, &mutableConfig, &regex, path);
  }
  printfAuxData(&mutableConfig, path);
  regfree(&regex);
  free(line);
  fclose(file);
}

void printMainData(const char *line, struct flags *config, const regex_t *regex,
                   const char *path) {
  if (regexec(regex, line, 0, NULL, 0) == config->v) {
    config->count_match++;
    if (!config->c && !config->l) {
      if (config->count_files > 1 && !config->h) printf("%s:", path);
      if (config->n) printf("%d:", config->number_line);
      printf("%s", line);
    }
  }
}
void printfAuxData(struct flags *config, const char *path) {
  if (config->c) {
    if (config->l) {
      printf("%s:1\n", path);
    } else {
      printf("%d\n", config->count_match);
    }
  }
  if (config->l && config->count_match) {
    printf("%s\n", path);
  }
}