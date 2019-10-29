#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#define CHARS 256
/* returns an array of Huffman codes. For each character, the integer ascii representation
as the index into the array, with the resulting Huffman code for that character stord at the location */
void create_code_helper(HuffmanNode *root, char *code_array[], char *code);
char *append(char dir, char *code);

char *create_code(HuffmanNode *root, char *code_array[]) {
  int q, v;
  char *original_code;
  original_code = malloc(sizeof(char) * (CHARS + 1));
  if(!original_code) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  create_code_helper(root, code_array, original_code);
}

void create_code_helper(HuffmanNode *root, char *code_array[], char *code) {
  if(root != NULL) {
    /* check if node is a leaf that contains a char and its freq */
    if((root->right == NULL) && (root->left == NULL)) {
      code_array[root->key] = code;
    }
    else {
      create_code_helper(root->left, code_array, append('0', code));
      create_code_helper(root->right, code_array, append('1', code));
    }
  }
}

char *append(char dir, char *code) {
  size_t len = strlen(code);
  char *new_code = malloc(len + 1 + 1);
  if(!new_code) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  strcpy(new_code, code);
  new_code[len] = dir;
  new_code[len + 1] = '\0';
  return new_code;
}
