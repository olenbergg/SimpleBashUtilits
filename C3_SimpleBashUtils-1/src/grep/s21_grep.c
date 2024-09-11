#include "s21_grep.h"

int main(int argc, char *argv[]) {
  options opt = {};
  parser(argc, argv, &opt);

  if (!opt.f && !opt.e) {
    add_pattern(&opt, argv[optind]);
    optind++;
  }
  if (opt.pattern[strlen(opt.pattern) - 1] == '|') {
    opt.pattern[strlen(opt.pattern) - 1] = '\0';
  }
  int f_count = argc - optind;

  while (optind < argc && opt.pattern != NULL) {
    reader(argv[optind], &opt, f_count);
    optind++;
  }

  if (opt.pattern != NULL) {
    free(opt.pattern);
  }
  return 0;
}

void parser(int argc, char **argv, options *opt) {
  int opts;
  while ((opts = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) != -1) {
    switch (opts) {
      case 'e':
        opt->e = 1;
        add_pattern(opt, optarg);
        break;
      case 'f':
        opt->f = 1;
        f_flag(opt, optarg);
        break;
      case 'i':
        opt->i = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 'c':
        opt->c = 1;
        break;
      case 'l':
        opt->l = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 'h':
        opt->h = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 'o':
        opt->o = 1;
        break;
      case '?':
        printf(
            "usage: grep (-abcdDEFGHhIiJLlMmnOopqRSsUVvwXxZz) (-A num) (-B "
            "num] [-C[num]]\n");
        printf(
            "        [-e pattern] [-f file] [--binary-files=value] "
            "[--color=when]\n");
        printf(
            "        [--context[=num]] [--directories=action] [--label] "
            "[--line-buffered]\n");
        printf("        [--null] [pattern] [file ...]\n");
        exit(1);
    }
  }
}

void add_pattern(options *opt, char *new_pattern) {
  if (opt->pattern == NULL) {
    opt->pattern = (char *)malloc(strlen(new_pattern) + 2);
    if (opt->pattern == NULL) {
      printf("Unable to allocate memory");
    }
    strcpy(opt->pattern, new_pattern);
    strcat(opt->pattern, "|");
  } else {
    opt->pattern = (char *)realloc(
        opt->pattern, strlen(opt->pattern) + strlen(new_pattern) + 2);
    if (opt->pattern == NULL) {
      printf("Unable to allocate memory");
    }
    strcat(opt->pattern, new_pattern);
    strcat(opt->pattern, "|");
  }
}

void reader(char *filename, options *opt, int f_count) {
  FILE *file = fopen(filename, "r");
  if (file != NULL) {
    if (opt->v && opt->o) {
      opt->o = 0;
    }
    regex_t reg;
    regmatch_t begin;
    char *line = NULL;
    size_t len = 0;
    int match, cflag = 0;
    if (opt->i) {
      cflag = (REG_EXTENDED | REG_ICASE);
    } else {
      cflag = REG_EXTENDED;
    }
    int count = 0, num = 0;
    regcomp(&reg, opt->pattern, cflag);
    int check;
    while ((check = getline(&line, &len, file)) != -1) {
      num++;
      if (((strcspn(line, "\n") == 0) && (opt->c) && (opt->e) && !(opt->f) &&
           !(opt->i) && !(opt->v) && !(opt->l) && !(opt->n) &&
           !(opt->h) & !(opt->s) & !(opt->o)) ||
          ((strcspn(line, "\n") == 0) && (opt->c) && !(opt->e) && !(opt->f) &&
           !(opt->i) && !(opt->v) && !(opt->l) && !(opt->n) &&
           !(opt->h) & !(opt->s) & !(opt->o)))
        continue;
      match = regexec(&reg, line, 1, &begin, 0);
      if (opt->v) match = !match;
      if (match == 0) {
        count++;
        check = 0;
        if (!(opt->c || opt->l || opt->o)) {
          print_line(f_count, opt, filename, num, line);
        } else if (!(opt->c || opt->l) && (opt->o)) {
          print_matches_in_line(f_count, line, &reg, filename, num, opt);
        }
      }
    }
    print_sum(filename, f_count, count, opt);
    fclose(file);
    free(line);
    regfree(&reg);
  } else {
    printf("grep: %s: No such file or directory\n", filename);
  }
}

void print_line(int f_count, options *opt, char *filename, int num,
                char *line) {
  if (f_count > 1 && !opt->h) printf("%s:", filename);
  if (opt->n) {
    printf("%d:%s", num, line);
  } else {
    printf("%s", line);
  }
  if (line[strlen(line) - 1] != '\n') {
    printf("\n");
  }
}

void print_sum(char *filename, int f_count, int count, options *opt) {
  if (opt->c) {
    if (opt->l && count > 1) {
      count = 1;
    }
    if (f_count > 1 && !opt->h) {
      printf("%s:", filename);
    }
    printf("%d\n", count);
  }
  if (opt->l && count > 0) {
    printf("%s\n", filename);
  }
}

void f_flag(options *opt, char *new_file) {
  FILE *file = fopen(new_file, "r");
  char *line = NULL;
  size_t len = 0;
  if (file == NULL) {
    printf("grep: %s: No such file or directory\n", new_file);
  }
  int get = getline(&line, &len, file);
  while (get != -1) {
    add_pattern(opt, line);
    get = getline(&line, &len, file);
  }
  fclose(file);
  if (line) {
    free(line);
  }
}

void print_matches_in_line(int f_count, char *line, regex_t *reg,
                           char *filename, int num, options *opt) {
  if (f_count > 1 && !opt->h) printf("%s:", filename);
  regmatch_t matches[1000];
  char *ptr = line;
  if (opt->n) {
    while (regexec(reg, ptr, 1000, matches, 0) == 0) {
      for (int i = 0; i < 1000 && matches[i].rm_so != -1; i++) {
        printf("%d:%.*s\n", num, (int)(matches[i].rm_eo - matches[i].rm_so),
               ptr + matches[i].rm_so);
      }
      ptr += matches[0].rm_eo;
    }
  } else {
    while (regexec(reg, ptr, 1000, matches, 0) == 0) {
      for (int i = 0; i < 1000 && matches[i].rm_so != -1; i++) {
        printf("%.*s\n", (int)(matches[i].rm_eo - matches[i].rm_so),
               ptr + matches[i].rm_so);
      }
      ptr += matches[0].rm_eo;
    }
  }
}