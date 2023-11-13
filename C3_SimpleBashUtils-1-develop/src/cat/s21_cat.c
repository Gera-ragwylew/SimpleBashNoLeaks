#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME "s21_cat"

struct flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  char error;
};

int* parser_arg(int argc, char** argv, struct flags* flags,
                size_t* filesCount);
void options(char* arg, size_t length, struct flags* flags);
void rw_files(FILE* file, char** argv, int* argArray, size_t filesCount,
              struct flags flags);
void io(FILE* file, struct flags flags);
void flags_modify(int* c, struct flags flags, char last_symb, int* counter,
                  int* empty_flag, int* empty_line);

int main(int argc, char** argv) {
  struct flags flags = {0, 0, 0, 0, 0, 0, '0'};
  FILE* file = NULL;
  size_t filesCount = 0;
  int* argArray = NULL;
  
  argArray = parser_arg(argc, argv, &flags, &filesCount);
  if (flags.error == '0') {
    rw_files(file, argv, argArray, filesCount, flags);
  } else {
    printf("%s: unknown option -- %c\n", NAME, flags.error);
  }
  free(argArray);
  return 0;
}

int* parser_arg(int argc, char** argv, struct flags* flags, size_t* filesCount) {
  int* argArray = NULL;
  for (size_t i = 1; i < (size_t)argc; i++) {
    size_t length = strlen(argv[i]);
    if (argv[i][0] == '-') {
      options(argv[i], length, flags);
    } else {
      *filesCount += 1;
      argArray = (int*) realloc(argArray, *filesCount * sizeof(int));
      argArray[*filesCount - 1] = i;
    }
  }
  return argArray;
 }

void options(char* argv, size_t length, struct flags* flags) {
  int end = 0;
  for (size_t i = 1; (i < length) && (!end); i++) {
    if (argv[i] == 'b') {
      flags->b = 1;
    } else if (argv[i] == 'E') {
      flags->e = 1;
    } else if (argv[i] == 'e') {
      flags->e = 1;
      flags->v = 1;
    } else if (argv[i] == 'n') {
      flags->n = 1;
    } else if (argv[i] == 's') {
      flags->s = 1;
    } else if (argv[i] == 'T') {
      flags->t = 1;
    } else if (argv[i] == 't') {
      flags->t = 1;
      flags->v = 1;
    } else if ((argv[i] == '-') && (!strcmp("--number-nonblank", argv))) {
      flags->b = 1;
      end = 1;
    } else if ((argv[i] == '-') && (!strcmp("--number", argv))) {
      flags->n = 1;
      end = 1;
    } else if ((argv[i] == '-') && (!strcmp("--squeeze-blank", argv))) {
      flags->s = 1;
      end = 1;
    } else if (argv[i] == 'v') {
      flags->v = 1;
    } else {
      flags->error = argv[i];
    }
  }
  if (flags->b == 1) {
    flags->n = 0;
  }
}

void rw_files(FILE* file, char** argv, int* argArray, size_t filesCount,
              struct flags flags) {
  if (filesCount == 0) {
    io(stdin, flags);
  }
  for (size_t i = 0; i < filesCount; i++) {
    int number = argArray[i];
    if ((file = fopen(argv[number], "r")) == NULL) {
      printf("%s: %s: No such file or directory\n", NAME, argv[number]);
    } else {
      io(file, flags);
      fclose(file);
    } 
  }
}

void io(FILE* file, struct flags flags) {
  int c = fgetc(file);
  char last_symb = '\n';
  int counter = 1;
  int empty_flag = 0;
  int empty_line = 0;
  while (c != EOF) {
    flags_modify(&c, flags, last_symb, &counter, &empty_flag, &empty_line);
    if (!empty_flag && empty_line < 2) {
      fputc(c, stdout);
    }
    empty_flag = 0;
    last_symb = c;
    c = fgetc(file);
    if (c != '\n') {
      empty_line = 0;
    }
  }
}

void flags_modify(int* c, struct flags flags, char last_symb, int* counter,
                  int* empty_flag, int* empty_line) {
  if (flags.n && last_symb == '\n') {
    printf("%6d\t", *counter);
    *counter += 1;
  } else if (flags.b && last_symb == '\n' && *c != '\n') {
    printf("%6d\t", *counter);
    *counter += 1;
  } else if (flags.s && last_symb == '\n' && *c == '\n') {
    *empty_line += 1;
  } else if (flags.e && *c == '\n') {
    printf("$");
  } else if (flags.t && *c == '\t') {
    printf("^I");
    *empty_flag = 1;
  } else if (flags.v) {
    if (*c > 127 && *c < 160) printf("M-^");
    if ((*c < 32 && *c != '\n' && *c != '\t') || *c == 127) printf("^");
    if ((*c < 32 || (*c > 126 && *c < 160)) && *c != '\n' && *c != '\t')
      *c = *c > 126 ? *c - 128 + 64 : *c + 64;
  }
}