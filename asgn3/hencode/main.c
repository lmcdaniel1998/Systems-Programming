#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include "node.h"

#define CHARS 256
#define BUFF 8192

void tree_in_order(HuffmanNode *head);
char *readfile(int fd_i, int fd_o, int reset);
int enocode_data(int fd_i, int fd_o, char *code_array[]);
int write_header(int fd, unsigned int char_array[]);
void create_code(HuffmanNode *root, char *code_array[]);
HuffmanNode *create_huff_tree(unsigned int *char_freq);
unsigned int *cnt_freq(char *line, unsigned int char_array[]);
void secondary(int input_file, int output_fd);

int main(int argc, char* argv[]) { 
  int input_file, output_file, reset;
  char *in_file, *file_contents, *out_file;
  /* locates input parameters */
  if(argc > 1) {
    in_file = argv[1];
    input_file = open(in_file, O_RDONLY);
    if(input_file == -1) {
      perror("input file not found");
      exit(EXIT_FAILURE);
    }
    /* reads input file contents into char array */
    //reset = 1;
    //file_contents = readfile(input_file, reset);
    /* check if output file was specified */
    if(argc > 2) {
      out_file = argv[2];
      output_file = open(out_file, O_WRONLY);
      if(!output_file) {
        perror("output file not found");
        exit(EXIT_FAILURE);
      }
    }
    /* if no output file is specified write to stdout */
    else {
      output_file = STDOUT_FILENO;
    }
  }
  secondary(input_file, output_file);
  return 0;
}
  
void secondary(int input_fd, int output_fd) {
  HuffmanNode *head;
  unsigned int *char_pointer;
  unsigned int char_array[CHARS];
  char *code_array[CHARS];
  char *line;
  ssize_t read_check;
  off_t fp_reset;
  int c, z, i;
  /* allocate space for reading file by parts */
  line = malloc(sizeof(char) * BUFF);
  if(!line) {
    perror("space for line not malloced");
    exit(EXIT_FAILURE);
  }
  /* read entire file and count char freqs */
  read_check = read(input_fd, line, BUFF);
  while(read_check > 0) {
    char_pointer = cnt_freq(line, char_array);
    read_check = read(input_fd, line, BUFF);
  }

  /*int p;
  for(p = 0; p < CHARS; p++) {
    if(char_pointer[p] != 0) {
      printf("char %c has freq: %d\n", p, char_pointer[p]); 
    }
  }*/

  /* reset file pointer to beginning of file */
  fp_reset = lseek(input_fd, 0, SEEK_SET);
  if(fp_reset == -1) {
    perror("file pointer not reset");
    exit(EXIT_FAILURE);
  }
  /* create frequency list for file */
  //char_pointer = cnt_freq(file_contents, char_array);
  /* create a huffman tree with list and return head */
  head = create_huff_tree(char_pointer);
  //tree_in_order(head);
  for(i = 0; i < CHARS; i++) {
    code_array[i] = NULL;
  }
  /* creates codes for all chars in file and stores in code array */
  create_code(head, code_array);
  /*int k;
  for(k = 0; k < CHARS; k++) {
    if(code_array[k] != NULL) {
      printf("char: %c has code: %s\n", k, code_array[k]);
    }
  }*/
  i = write_header(output_fd, char_array);
  i = encode_data(input_fd, output_fd, code_array);
}
