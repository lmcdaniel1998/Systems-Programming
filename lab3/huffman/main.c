#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"

#define CHARS 256

void create_code(HuffmanNode *root, char *code_array[]);
HuffmanNode *create_huff_tree(unsigned int *char_freq);
unsigned int *cnt_freq(char *line, unsigned int char_array[]);
char *readline(FILE *file);
void secondary(FILE *file);

int main(int argc, char* argv[]) { 
  int i;
  /* locates input parameters */
  if(argc > 1) {
    for(i = 1; i < argc; i++) {
      FILE *file = fopen(argv[i], "r");
      if(!file) {
        perror("file io");
      }
      else {
        secondary(file);
        fclose(file);
      }
    }
  }
  else {
    secondary(stdin);
  }
  return 0;
}
  
void secondary(FILE *file) {
  HuffmanNode *head;
  unsigned int *char_pointer;
  unsigned int char_array[CHARS];
  char *code_array[CHARS];
  char *line;
  int c, z, i;
  
  /* set first line of file to curr */
  line = (char*) readline(file);
  /* loop through file until end */
  while(line) {
    char_pointer = cnt_freq(line, char_array);
    line = readline(file);
  }
  /* create a huffman tree with list and return head */
  head = create_huff_tree(char_pointer);
  for(i = 0; i < CHARS; i++) {
    code_array[i] = NULL;
  }
  create_code(head, code_array);
  
  int x;
  for(x = 0; x < CHARS; x++) {
    if(code_array[x] != NULL) {
      printf("0x%x: %s\n", x, code_array[x]);
    }
  } 
}
