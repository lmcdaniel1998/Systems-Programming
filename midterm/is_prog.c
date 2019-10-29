#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>

int is_program(char *fname);

int main(int argc, char *argv[]) {

  if(argc != 2) {
    perror("no file specified");
    exit(EXIT_FAILURE);
  }

  if(is_program(argv[1]) == 0) {
    printf("its a program\n");
  }
  else {
    printf("its not a program\n");
  }
  return 0;
}

int is_program(char *fname) {
  
  struct stat *buff;
  int success;

  buff = malloc(sizeof(struct stat*));
  if(!buff) {
    perror("buff not malloced");
    exit(EXIT_FAILURE);
  }

  success = lstat(fname, buff);
  if(success != 0) {
    perror("couldnt lstat");
    exit(EXIT_FAILURE);
  }

  if(!S_ISREG(buff->st_mode)) {
    return -1;
  }

  if(access(fname, X_OK) < 0) {
    return -1;
  }
  
  return 0;
}
