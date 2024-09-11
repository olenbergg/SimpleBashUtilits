#ifndef S21_GREP
#define S21_GREP

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
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
  int wrong;
  char *pattern;
} options;

void parser(int argc, char *argv[], options *opt);
void reader(char *filename, options *opt, int f_count);
void print_line(int f_count, options *opt, char *filename, int num, char *line);
void add_pattern(options *opt, char *new_pattern);
void f_flag(options *opt, char *new_file);
void print_sum(char *filename, int f_count, int count, options *opt);
void print_matches_in_line(int f_count, char *line, regex_t *reg,
                           char *filename, int num, options *opt);
#endif