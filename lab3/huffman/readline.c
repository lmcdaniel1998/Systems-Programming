#include <stdio.h>
#include <stdlib.h>


/* this function returns a point to line data from a file */
/* if EOF is encountered at the end of a line return NULL on next function call */
char *readline(FILE *file) { 
  
  char *line = NULL;
  size_t len = 0;
  size_t size = 1;
  int c;
  
  /* initially allocate two spaces in memory */
  line = malloc(size);
  /* check if memory was allocated */
  if(line) {
    /* if next character in file is EOF return NULL */
    c = fgetc(file);
    if(c == EOF) {
      return NULL;
    }
    /* go through file until EOF or end of line is reached */
    while(1) {
      if(c == EOF) {
        /* if EOF is encountered at the end of the line add it to the end of the string then break*/
        if(feof(file)) {
          if(len > size) {
            size = len + 1;
            line = realloc(line, size);
            if(line) {
              line[len] = EOF;
              break;
            }
            else {
              perror("realloc");
              exit(EXIT_FAILURE);
            }
          }
        }
      }
      /* break out of infinite while loop when end of line */
      if(c == '\n') {
        if(len > size) {
          size = len + 1;
          line = realloc(line, size);
          if(!line) {
            perror("realloc");
            exit(EXIT_FAILURE);
          }
        }
        line[len] = c;
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
      /* add character to line */
      line[len] = c;
      /* increment line length */
      len++;
      /* get next char */
      c = fgetc(file);
    }
  }
  else {
    perror("malloc");
    exit(EXIT_FAILURE);
    return line;
  }
  return line;
}
