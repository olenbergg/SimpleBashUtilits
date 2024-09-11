#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int flag;
  int flags;
  char last;
  char prelast;
  int strcnt;
} opt;

int parser(int argc, char *argv[], opt *options);
int reader(char *argv[], opt *options);
void flagVT(char k, opt *options);
void flagE(char k, opt *options);
void flagBN(char k, opt *options);
void flagBE(char k, opt *options);

int main(int argc, char *argv[]) {
  int check = 1;
  opt options = {
      .b = 0,
      .e = 0,
      .n = 0,
      .s = 0,
      .t = 0,
      .v = 0,
      .flag = 0,
      .flags = 0,
      .last = '\n',
      .prelast = ' ',
      .strcnt = 1,
  };
  check = parser(argc, argv, &options);
  while (optind != argc) {
    check = reader(argv, &options);
    optind = optind + 1;
  }
  return check;
}
int parser(int argc, char *argv[], opt *options) {
  int flag;
  int longindex = 0;
  static struct option long_options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"sqeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };

  while ((flag = getopt_long(argc, argv, "+bEnsTvet", long_options,
                             &longindex)) != -1) {
    switch (flag) {
      case 'b':
        options->b = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case '?':
        fprintf(stderr, "usage: cat [-belnstuv] [file ...]");
        exit(1);
    }
  }
  return 0;
}
int reader(char *argv[], opt *options) {
  FILE *file;
  char c;
  int counterfiles = 1;
  file = fopen(argv[optind], "r");
  if (file != NULL) {
    while (1) {
      c = fgetc(file);
      counterfiles = counterfiles + 1;
      if (c == EOF) {
        options->flag = 1;
        break;
      }
      options->flag = 1;
      // flagS;
      if (((options->last == '\n') && (c == '\n')) &&
          (options->prelast == '\n') && (options->s)) {
        continue;
      }
      flagBN(c, options);
      flagE(c, options);
      flagBE(c, options);
      flagVT(c, options);
      options->prelast = options->last;
      options->last = c;
    }
    fclose(file);
    options->strcnt = 1;
  } else {
    for (int i = 0; i < counterfiles; i++) {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[optind]);
    }
  }
  return 0;
}
void flagVT(char k, opt *options) {
  if (k == '\t' && options->t) {
    options->flag = 0;
    printf("^I");
  }
  if ((k >= 0 && k < 32) && (options->v) && (k != '\n') && (k != '\t')) {
    options->flag = 0;
    printf("^");
    printf("%c", k + 64);
  } else if ((k == 127) && (options->v) && (k != '\n') && (k != '\t')) {
    options->flag = 0;
    printf("^");
    printf("%c", k - 64);
  }
  if (options->flag == 1) {
    printf("%c", k);
  }
}

void flagE(char k, opt *options) {
  if ((k == '\n') && (options->e && !options->b)) {
    printf("$");
  }
}
void flagBN(char k, opt *options) {
  if ((options->last == '\n' && ((options->b && k != '\n' && !options->e)))) {
    options->n = 0;
    printf("%6d\t", options->strcnt);
    ++options->strcnt;
  } else if (options->b && options->n) {
    options->n = 0;
  }
  if (options->last == '\n' && options->n) {
    printf("%6d\t", options->strcnt);
    ++options->strcnt;
  }
}

void flagBE(char k, opt *options) {
  if ((options->last == '\n' && ((options->b && k != '\n' && options->e))) ||
      ((k == '\n') && (options->e && options->b))) {
    if (k != '\n') {
      options->n = 0;
      printf("%6d\t", options->strcnt);
      ++options->strcnt;
      options->flags = 1;
    } else if ((k == '\n' && options->flags == 1)) {
      if ((k == '\n')) {
        printf("$");
      }
      options->flags = 0;
      options->n = 0;
    } else {
      options->n = 0;
      printf("      \t");
      if ((k == '\n')) {
        printf("$");
      }
    }
  }
}