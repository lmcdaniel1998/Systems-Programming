#include <stdio.h>
#include <stdlib.h>


char *read_long_line(FILE *file) { 
  
  char *line = NULL;
  size_t len = 0;
  size_t size = 1;
  int c;
  
  /* initially allocate two spaces in memory */
  line = malloc(size);
  /* check if memory was allocated */
  if(line) {
    c = fgetc(file);
    if(c == EOF) {
      return NULL;
    }
    /* go through file until EOF or end of line is reached */
    while(1) {
      if(c == EOF) {
        if(feof(file)) {
          line[len] = EOF;
          break;
        }
        break;
      }
      if(c == '\n') {
        break;
      }
      /* if line length is larger than memory reallocate space for line */
      if(len > size) {
        size = size * 2;
        line = realloc(line, size);
        /* if there is no more memory for the program exit */
        if(!line) {
          perror("realloc");
          exit(EXIT_FAILURE);
        }
      }
      line[len] = c;
      len++;
      c = fgetc(file);
    }
  }
  else {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  /* add null terminator to end of array */
  if(len > size) {
    size = len + 1;
    line = realloc(line, size);
    if(!line) {
      perror("realloc");
      exit(EXIT_FAILURE);
    }
  }
  line[len + 1] = '\0';

  /* return char pointer that points to line location in memory */
  return line;
}
