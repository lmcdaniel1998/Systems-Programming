#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF 8192

int readfile(int fd, int reset, unsigned char freq_array[]) {
  ssize_t buff = BUFF;
  ssize_t read_check;
  off_t fp_reset;
  char *file_content;
  /* allocate memory for original buff size */
  file_content = malloc(sizeof(char) * buff);
  if(!file_content) {
    perror("malloc memory not allocated");
    exit(EXIT_FAILURE);
  }
  /* read all of file contents and place into one char pointer */
  while((read_check = read(fd, file_content, buff)) > 0) {
      
  }
  /* reset file pointer to front of file */
  if(reset == 1) {
    fp_reset = lseek(fd, 0, SEEK_SET);
    if(fp_reset == -1) {
      perror("file pointer not reset");
      exit(EXIT_FAILURE);
    }
  }
  return file_content;
}
