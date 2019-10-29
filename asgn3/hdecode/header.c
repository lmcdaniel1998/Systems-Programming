#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "node.h"

#define CHARS 256

int write_header(int fd, char *code_array[]) {
  int i, x;
  ssize_t written;
  uint32_t char_count = 0;
  uint32_t *count_pointer;
  uint8_t character;
  uint8_t *char_pointer;
  uint32_t char_freq;
  uint32_t *char_freq_pointer;
  /* count characters in array */
  for(i = 0; i < 256; i++) {
    if(code_array[i] != NULL) {
      char_count++;
    }
  }
  /* write count in header */
  count_pointer = &char_count;
  written = write(fd, count_pointer, 4);
  if(written != 4) {
    perror("char count not written");
    exit(EXIT_FAILURE);
  }
  /* write chars and associated freqs into file */
  for(x = 0; x < 256; x++) {
    if(code_array[x] != NULL) {
      /* write char */
      character = x;
      char_pointer = &character;
      written = write(fd, char_pointer, 1);
      if(written != 1) {
        perror("char not written");
        exit(EXIT_FAILURE);
      }
      /* write char freq */
      char_freq = *(code_array[x]);
      char_freq_pointer = &char_freq;
      written = write(fd, char_freq_pointer, 4);
      if(written != 4) {
        perror("char freq not written");
        exit(EXIT_FAILURE);
      }
    }
  }
  return 0;
}
