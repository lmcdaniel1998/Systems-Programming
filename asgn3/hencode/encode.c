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

struct ByteAndPos {
  int bit_count;
  int code_count;
  uint8_t byte;
  char *leftover_code;
  int leftover_code_len;
};

int add_byte(int fd, uint8_t byte);
struct ByteAndPos *encode_char(int fd_i, int fd_o, uint8_t character, char *code_array[], struct ByteAndPos *bandp); 
int encode_chunk(int fd_i, int fd_o, uint8_t *chunk, char *code_array[]); 

int encode_data(int fd_i, int fd_o, char *code_array[]) {
  ssize_t read_check;
  uint8_t *buff_chunk;
  int encode_success;
  
  /* allocate space for buff chunk */
  buff_chunk = malloc(sizeof(uint8_t) * BUFF);
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

int encode_chunk(int fd_i, int fd_o, uint8_t *chunk, char *code_array[]) {
  int z;
  struct ByteAndPos *bandp;
  bandp = (struct ByteAndPos*) malloc(sizeof(struct ByteAndPos));
  if(!bandp) {
    perror("couldn't malloc for byte and position");
    exit(EXIT_FAILURE);
  }
  bandp->bit_count = 0;
  bandp->code_count = 0;
  bandp->byte = '\0';
  bandp->leftover_code = NULL;
  bandp->leftover_code_len = 0;
 
  z = 0; 
  while(chunk[z] != '\0') {
    bandp = encode_char(fd_i, fd_o, chunk[z], code_array, bandp);
    z++;
  }
  return 0;
}

/* returns pointer to last byte used but not put in file */
struct ByteAndPos *encode_char(int fd_i, int fd_o, uint8_t character, char *code_array[], struct ByteAndPos *bandp) {
  int boolean, v;
  int added_byte;
  char *code_pointer;

  //printf("new_byte: %x new_byte pos: %d\n", bandp->byte, bandp->bit_count);
  /* assign pointer to char code */
  code_pointer = (char*) code_array[(int)character];
  /* assign pointer to code length */
  ssize_t code_len;
  code_len = strlen(code_pointer);

  if(bandp->code_count < bandp->leftover_code_len && bandp->bit_count <= 8) {
    for(v = bandp->code_count; v < bandp->leftover_code_len; v++) {
      if(bandp->bit_count <= 8) {
        if((bandp->leftover_code)[v] == '1') {
          boolean = 1;
        }
        else if((bandp->leftover_code)[v] == '0') {
          boolean = 0;
        }
        else {
          return NULL;
        }
        if(boolean == 1) {
          bandp->byte = bandp->byte | (1 << (7 - (bandp->bit_count)));
          //printf("%d new byte from leftover: %x\n", (7 - (bandp->bit_count)), bandp->byte);
        }
        (bandp->bit_count)++;
        (bandp->code_count)++; 
      }
      else {
        break;
      }
    }
  }
  if((bandp->leftover_code_len - v) == 0) {
    bandp->code_count = 0;
  }
  


  if(bandp->code_count < code_len && bandp->bit_count <= 8) {
    int z;
    /* loop through code and set bits of new byte then place into file when full */
    //printf("char %c has code_pointer: %s and length of: %d\n", character, code_array[(int)character], (int)code_len);
    for(z = bandp->code_count; z < code_len; z++) {
      //printf("code_count: %d and bit_count: %d\n", bandp->code_count, bandp->bit_count);
      /* check if next bit in code is 0 or 1 */
      if(bandp->bit_count < 8) {
        if((code_pointer[bandp->code_count]) == '1') {
          boolean = 1;
        }
        else if((code_pointer[bandp->code_count]) == '0') {
          boolean = 0;
        }
        else {
          /* something is wrong with code so exit program with failure */
          return NULL;
        }
        if(boolean == 1) {
          bandp->byte = bandp->byte | (1 << (7 - (bandp->bit_count)));
          //printf("%d new byte: %x\n", (7 - (bandp->bit_count)), bandp->byte);
        }
        (bandp->bit_count)++;
        (bandp->code_count)++;
      }
      else {
        break;
      }
    }
  }
  //printf("bit_count: %d\n", bandp->bit_count);

  /* if part of the code still hasn't been put in a byte */
  if((code_len - bandp->code_count) != 0) {
    bandp->leftover_code = code_pointer;
    bandp->leftover_code_len = code_len;
  }
  else {
    bandp->code_count = 0;
    bandp->leftover_code = NULL;
    bandp->leftover_code_len = 0;
  }
  /* add new byte to file and create a new byte */
  if(bandp->bit_count >= 8) {
    added_byte = add_byte(fd_o, bandp->byte);
    if(added_byte != 0) {
      perror("byte not added to file");
      exit(EXIT_FAILURE);
    }
    bandp->byte = '\0';
    bandp->bit_count = 0;
  }
  //printf("bandp byte: %x has bit count: %d\n", bandp->byte, bandp->bit_count);
  return bandp;
}

int add_byte(int fd, uint8_t byte) {
  ssize_t written;
  uint8_t *byte_ptr;
  //printf("byte to be written: %x\n", byte); 
  byte_ptr = &byte;
  written = write(fd, byte_ptr, 1);
  if(written != 1) {
    return -1;
  }
  else {
    return 0;
  }
}
