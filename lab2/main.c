#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_long_line(FILE *file);

int main() {
  
  char *curr;
  char *prev;
  int c, v;
  
  if(stdin == NULL) {
    perror("stdin");
    exit(EXIT_FAILURE);
  }
  /* put first line in file and set to prev */
  curr = (char*) read_long_line(stdin);
  prev = curr;
  for(c = 0; curr[c] != '\0'; c++) {
    if(curr[c] != EOF) {
      putchar(curr[c]);
    }
    else {
      break;
    }
  }
  putchar('\n');
  /* set second line in file to curr */
  curr = (char*) read_long_line(stdin);

  /* loop through file until end */
  while(curr) {
    v = strcmp(curr, prev);
    /* if curr is not equal to prev add to file */
    if(v != 0) {
      for(c = 0; curr[c] != '\0'; c++) {
        if(curr[c] != EOF) {
          putchar(curr[c]);
        }
        else {
          break;
        }
      }
      putchar('\n');
      prev = curr;
      curr = (char*) read_long_line(stdin);
    }
    /* if curr is equal to prev go to the next line */
    else {
      curr = (char*) read_long_line(stdin);
    }
  }
  free(curr);
  free(prev);
  return 0;
}
