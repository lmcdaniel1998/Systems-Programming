#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "node.h"

#define CHARS 256

int readheader(int fd_in, unsigned int char_array[]) {
  ssize_t char_count_buff = 4;
  uint32_t *char_count;
  ssize_t char_buff = 1;
  uint8_t *character;
  ssize_t char_freq_buff = 4;
  uint32_t *char_freq;
  int read_check, x;
  
  /* allocate space for char count */
  char_count = malloc(sizeof(uint32_t));
  if(!char_count) {
    perror("could not allocate space for char count");
    exit(EXIT_FAILURE);
  }

  /* read char count from file */
  read_check = read(fd_in, char_count, char_count_buff);
  if(read_check < 0) {
    perror("Number of chars not read");
    exit(EXIT_FAILURE);
  }
  /* allocate space for character */
  character = malloc(sizeof(uint8_t));
  if(!character) {
    perror("could not allocate space for char");
    exit(EXIT_FAILURE);
  }
  /* allocate space for char freq */
  char_freq = malloc(sizeof(uint32_t));
  if(!char_freq) {
    perror("could not allocate space for char freq");
    exit(EXIT_FAILURE);
  }

  /* cycle through remainder of header to get char array */
  for(x = 0; x < ((*char_count) * 2); x = x + 2) {
    /* read in the next char in header */
    read_check = read(fd_in, character, char_buff);
    if(read_check < 0) {
      perror("could not read in character");
      exit(EXIT_FAILURE);
    }
    /* read in next char freq in header */
    read_check = read(fd_in, char_freq, char_freq_buff);
    if(read_check < 0) {
      perror("could not read in char freq");
      exit(EXIT_FAILURE);
    }
    /* put char freq into char array */
    char_array[*character] = *char_freq;
  }
  return 0;
}
