#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME "s21_grep"

struct flags {
  int e;
  int E;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char error;
};

void options(char* argv, size_t length, struct flags* flags, int* pattern_flag);
void rw_files(char** argv, int* argArray, size_t filesCount, struct flags flags,
              char* pattern);
void grep(FILE* file, struct flags flags, char* filename, size_t filesCount,
          char* pattern);
char* make_fpattern(char* pattern_filename);
void flags_modify(regmatch_t* matches, struct flags flags, char* string,
                  int status, char* filename, size_t filesCount,
                  int* string_count, int* match_string_count);

int main(int argc, char** argv) {
  struct flags flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '0'};
  int* argArray = NULL;
  size_t filesCount = 0;
  char pattern_for_e[10000] = {'\0'};
  char* pattern = NULL;
  int pattern_flag = 1;
  int flag_for_slash = 0;
  
  for (size_t i = 1; i < (size_t)argc; i++) {
    size_t length = strlen(argv[i]);
    if (argv[i][0] == '-') {
      options(argv[i], length, &flags, &pattern_flag);
    } else if (flags.e && pattern_flag) {
      if (flag_for_slash) {
        strcat(pattern_for_e, "|");
      }
      strcat(pattern_for_e, argv[i]);
      pattern_flag = 0;
      flag_for_slash = 1;
      flags.e = 0;
      pattern = pattern_for_e;
    } else if (pattern_flag && !flags.e) {
      pattern = argv[i];
      pattern_flag = 0;
    } else if (!pattern_flag) {
      filesCount += 1;
      argArray = realloc(argArray, filesCount * sizeof(int));
      argArray[filesCount - 1] = i;
    }
  }
  
  if (flags.error == '0') {
    if (flags.f) {
        pattern = make_fpattern(pattern);
      }
    if(pattern) {
      rw_files(argv, argArray, filesCount, flags, pattern);
    }
    if (flags.f) {
      free(pattern);
    }
  } else {
    printf("%s: unknown option --%c\n", NAME, flags.error);
  }
  free(argArray);
  return 0;
}

void rw_files(char** argv, int* argArray, size_t filesCount, struct flags flags,
              char* pattern) {
  FILE* file = NULL;
  for (size_t i = 0; i < filesCount; i++) {
    if ((file = fopen(argv[argArray[i]], "r")) == NULL) {
      if (!flags.s) {
        printf("%s: %s: No such file or directory\n", NAME, argv[argArray[i]]);
      }
    } else {
      grep(file, flags, argv[argArray[i]], filesCount, pattern);
      fclose(file);
    }
  }
}

void grep(FILE* file, struct flags flags, char* filename, size_t filesCount,
          char* pattern) {
  regex_t re;
  int match_string_count = 0;
  int string_count = 1;
  char c = 0;
  regmatch_t* matches = (regmatch_t*) malloc(2 * sizeof(regmatch_t));
  
  regcomp(&re, pattern, flags.E | flags.i);
  while (c != EOF) {
    int status = 1;
    char* string = (char*)malloc(sizeof(char));
    c = fgetc(file);
    string[0] = c;
    size_t i = 0;
    while (c != EOF && c != '\n') {
      i++;
      c = fgetc(file);
      string = (char*)realloc(string, (i + 1) * sizeof(char));
      string[i] = c;
    }

    if (c != EOF || string[0] != EOF) {
      string[i] = '\0';
      status = regexec(&re, string, 2, matches, 0);
      flags_modify(matches, flags, string, status, filename, filesCount,
                   &string_count, &match_string_count);
      
    }
    free(string);
  }
  if (flags.c && !flags.l) {
    printf("%d\n", match_string_count);
  } else if (flags.l && match_string_count){
    printf("%s\n", filename);
  }
  free(matches);
  regfree(&re);
}

void flags_modify(regmatch_t* matches, struct flags flags, char* string,
                  int status, char* filename, size_t filesCount,
                  int* string_count, int* match_string_count) {
  int match = status;
  if (flags.v) {
    match = !(status);
  }
  if (!match) {
    if (!flags.l) {
      if (filesCount > 1 && !flags.h) {
        printf("%s:", filename);
      }
      if (flags.n) {
        printf("%d:", *string_count);
      }
      if (!flags.c && !flags.o) {
        printf("%s\n", string);
      }
      if (flags.o) {
        printf("%.*s\n", (int)(matches[0].rm_eo - matches[0].rm_so),
               string + matches[0].rm_so);
      }
    }
    *match_string_count += 1;
  }
  *string_count += 1;
}

void options(char* argv, size_t length, struct flags* flags,
             int* pattern_flag) {
  for (size_t i = 1; i < length; i++) {
    if (argv[i] == 'e') {
      flags->e = 1;
      flags->E = 1;
      *pattern_flag = 1;
    } else if (argv[i] == 'E') {
      flags->E = 1;
    } else if (argv[i] == 'i') {
      flags->i = 2;
    } else if (argv[i] == 'v') {
      flags->v = 1;
    } else if (argv[i] == 'c') {
      flags->c = 1;
    } else if (argv[i] == 'l') {
      flags->l = 1;
    } else if (argv[i] == 'n') {
      flags->n = 1;
    } else if (argv[i] == 'h') {
      flags->h = 1;
    } else if (argv[i] == 's') {
      flags->s = 1;
    } else if (argv[i] == 'f') {
      flags->f = 1;
      flags->E = 1;
    } else if (argv[i] == 'o') {
      flags->o = 1;
    } else {
      flags->error = argv[i];
    }
  }
}

char* make_fpattern(char* pattern_filename) {
  char* fpattern = NULL;
  FILE* file = NULL;
  int i = 1;
  if ((file = fopen(pattern_filename, "r")) == NULL) {
    printf("%s: %s: No such file or directory\n", NAME, pattern_filename);
  } else {
    char c = fgetc(file);
    while (c != EOF) {
      fpattern = (char*) realloc(fpattern, i * sizeof(char));
      if (c != '\n') {
        fpattern[i - 1] = c;
      } else {
        fpattern[i - 1] = '|';
      }
      i++;
      c = fgetc(file);
    }
    if(fpattern[i - 2] == '|') {
      fpattern[i - 2] = '\0';
    } else {
      fpattern = (char*) realloc(fpattern, i * sizeof(char));
      fpattern[i - 1] = '\0';
    }
    fclose(file);
  }
  return fpattern;
}
