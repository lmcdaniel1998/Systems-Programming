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
int decode(HuffmanNode *head, int fp_in, int fp_out);
int readheader(int fd_in, unsigned int char_array[]);
int enocode_data(int fd_i, int fd_o, char *code_array[]);
int write_header(int fd, char *code_array[]);
void create_code(HuffmanNode *root, char *code_array[]);
HuffmanNode *create_huff_tree(unsigned int *char_freq);
unsigned int *cnt_freq(char *line, unsigned int char_array[]);
void secondary(int input_file, int output_fd);

int main(int argc, char* argv[]) { 
  int input_file, output_file, reset;
  char *in_file, *file_contents, *out_file;
  /* locates input parameters */
  if(argc > 1) {
    if(strcmp(argv[1], "-") == 0) {
      input_file = STDIN_FILENO;
    }
    else { 
      in_file = argv[1];
      input_file = open(in_file, O_RDONLY);
      if(input_file == -1) {
        perror("input file not found");
        exit(EXIT_FAILURE);
      }
    }
    /* check if output file was specified */
    if(argc > 2) {
      out_file = argv[2];
      output_file = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
      if(output_file == -1) {
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
  unsigned int char_array[CHARS] = {0};
  char *code_array[CHARS];
  char *line;
  ssize_t read_check;
  off_t fp_reset;
  int c, z, i;
  /* read header from input file */
  i = readheader(input_fd, char_array);
  /* create a huffman tree with list and return head */
  char_pointer = &(char_array[0]);

  /*int g;
  for(g = 0; g < CHARS; g++) {
    if(char_pointer[g] != 0) {
      printf("char: %c has freq: %d\n", g, char_pointer[g]);
    }
  }*/
  
  head = create_huff_tree(char_pointer);
  //tree_in_order(head);

  for(i = 0; i < CHARS; i++) {
    code_array[i] = NULL;
  }
  /* creates codes for all chars in file and stores in code array */
  create_code(head, code_array);
  /*int y;
  for(y = 0; y < CHARS; y++) {
    if(code_array[y] != NULL) {
      printf("char: %c has code: %s\n", y, code_array[y]);
    }
  }*/
  /* decode header and write into outfile */
  c = decode(head, input_fd, output_fd);
}
