#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "node.h"

#define CHARS 256
#define BUFF 8192

int add_byte(int fd, unsigned char *byte);
unsigned char encode_char(int fd_i, int fd_o, char character, char *code_array[], unsigned char new_byte, int *code_count, int *bit_count); 
int encode_chunk(int fd_i, int fd_o, char *chunk, char *code_array[]); 

int encode_data(int fd_i, int fd_o, char *code_array[]) {
  ssize_t read_check;
  char *buff_chunk;
  int encode_success;
  
  /* allocate space for buff chunk */
  buff_chunk = malloc(sizeof(char) * BUFF);
  if(!buff_chunk) {
    perror("malloc failed for buff chunck");
    exit(EXIT_FAILURE);
  }
  /* loop through file until end is reached */
  while((read_check = read(fd_i, buff_chunk, BUFF - 1)) > 0) {
    encode_success = encode_chunk(fd_i, fd_o, buff_chunk, code_array);
    if(encode_success != 0) {
      perror("encode line failed");
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}

int encode_chunk(int fd_i, int fd_o, char *chunk, char *code_array[]) {
  int z;
  int len = strlen(chunk);
  int zero = 0;
  int zero2 = 0;
  int *code_count_ptr;
  int *bit_count_ptr;
  /* allocate space for code count ptr */
  code_count_ptr = malloc(sizeof(int));
  if(!code_count_ptr) {
    perror("could not allocate space for code count");
    exit(EXIT_FAILURE);
  }
  /* allocate space for bit count ptr */
  bit_count_ptr = malloc(sizeof(int));
  if(!bit_count_ptr) {
    perror("could not allocate space for bit count");
    exit(EXIT_FAILURE);
  }
  code_count_ptr = &zero;
  bit_count_ptr = &zero2;
  unsigned char new_byte = '\0';
  for(z = 0; z < len; z++) {
    new_byte = encode_char(fd_i, fd_o, chunk[z], code_array, new_byte, code_count_ptr, bit_count_ptr);
    if(new_byte != 0) {
      perror("encode char failed");
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}

/* returns pointer to last byte used but not put in file */
unsigned char encode_char(int fd_i, int fd_o, char character, char *code_array[], unsigned char new_byte, int *code_count, int *bit_count) {
  int boolean, added_byte;
  char *code_pointer;
  unsigned char *new_byte_ptr;
  /* assign pointer to current byte */
  new_byte_ptr = &new_byte;
  /* set code pointer to characters place in code array */
  code_pointer = code_array[character];
  if(code_pointer == NULL) {
    perror("code does not exist for char");
    exit(EXIT_FAILURE);
  }
  int code_len = strlen(code_pointer);
  /* loop through code and set bits of new byte then place into file when full */
  while((code_pointer[*code_count]) != '\0') {
    /* check if next bit in code is 0 or 1 */
    if((code_pointer[*code_count]) == '1') {
      boolean = 1;
    }
    else if((code_pointer[*code_count]) == '0') {
      boolean = 0;
    }
    else {
      /* something is wrong with code so exit program with failure */
      return -1;
    }
    /* swithc is based off of what position of the bit we are on and the next value in the code */
    switch(*bit_count) {
      case 0:
        if(boolean == 1) {
          /* reverse order of 0 through 7 for little endian */
          new_byte = new_byte | (1 << 7);
        }
        (*bit_count)++;
      case 1:
        if(boolean == 1) {
          new_byte = new_byte | (1 << 6);
        }
        (*bit_count)++;
      case 2:
        if(boolean == 1) {
          new_byte = new_byte | (1 << 5);
        }
        (*bit_count)++;
      case 3:
        if(boolean == 1) {
          new_byte = new_byte | (1 << 4);
        }
        (*bit_count)++;
      case 4:
        if(boolean == 1) {
          new_byte = new_byte | (1 << 3);
        }
        (*bit_count)++;
      case 5:
        if(boolean == 1) {
          new_byte = new_byte | (1 << 2);
        }
        (*bit_count)++;
      case 6:
        if(boolean == 1) {
          new_byte = new_byte | (1 << 1);
        }
        (*bit_count)++;
      case 7:
        if(boolean == 1) {
          new_byte = new_byte | (1 << 0);
        }
        /* if part of the code still hasn't been put in a byte */
        /* add new_byte to file and create a new new_byte */
        if((code_len - (*code_count)) > 0) {
          added_byte = add_byte(fd_o, new_byte_ptr);
          if(added_byte != 0) {
            perror("byte not added to file");
            exit(EXIT_FAILURE);
          }
          new_byte = '\0';
          (*bit_count) = 0;
        }
    }
    (*code_count)++;
  }
  return *new_byte_ptr;
}

int add_byte(int fd, unsigned char *byte) {
  ssize_t written;
  written = write(fd, byte, 1);
  if(written != 1) {
    return -1;
  }
  else {
    return 0;
  }
}
