#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "node.h"

HuffmanNode *decode_byte(HuffmanNode *true_head, uint8_t *new_byte, HuffmanNode *root, int fp_out);
HuffmanNode *left_right_tree(int boolean, HuffmanNode *head);
int leaf_check(HuffmanNode *current);
 
int decode(HuffmanNode *head, int fp_in, int fp_out) {
  ssize_t byte_buff = 1;
  uint8_t *new_byte;
  int *left_over_code_len;
  uint8_t *left_over_code;
  HuffmanNode *head_saver;
  int read_check, write_check, x;
  
  /* save true head of tree */
  head_saver = head;
  /* allocate space for a byte */
  new_byte = malloc(sizeof(uint8_t));
  if(!new_byte) {
    perror("could not allocate space for new reading byte");
    exit(EXIT_FAILURE);
  }
  /* read first byte of decoded file */
  read_check = read(fp_in, new_byte, byte_buff);
  /* check if file has any contents */
  if(read_check > 0) {
    /* go through file byte by byte until EOF */
    while(read_check > 0) {
      //printf("next byte is: %x\n", *new_byte);
      /* decode byte and set head to where program left off in tree */
      head = decode_byte(head_saver, new_byte, head, fp_out);
      //printf("head after byte is decoded: char %c and freq: %d\n", head->key, head->freq);
      free(new_byte);
      new_byte = malloc(sizeof(uint8_t));
      if(!new_byte) {
        perror("could not re malloc new byte");
        exit(EXIT_FAILURE);
      }
      /* read in next byte of file */
      read_check = read(fp_in, new_byte, byte_buff);
    }
  }
  return 0;
}

HuffmanNode *decode_byte(HuffmanNode *true_head, uint8_t *new_byte, HuffmanNode *root, int fp_out) {
  ssize_t write_check;
  unsigned char decoded_char;
  unsigned char *decoded_char_ptr;

  uint8_t *hold_byte;
  /* allocate space to copy bytes value to */
  hold_byte = malloc(sizeof(uint8_t));
  if(!hold_byte) {
    perror("could not allocate space for hold byte");
    exit(EXIT_FAILURE);
  }
  /* copy value of new_byte into hold_byte */
  memcpy(hold_byte, new_byte, 1);

  /* size of byte is 8 bits */
  decoded_char_ptr = malloc(sizeof(unsigned char));
  if(!decoded_char_ptr) {
    perror("could not allocate space for decoded byte");
    exit(EXIT_FAILURE);
  }
  /* position of first bit */
  int x = 7;
  while(x >= 0) {
    /* check if each bit in byte is 1 or 0 */
    if((*new_byte) & (1 << x)) {
      /* move root right in tree */
      //printf("next bit is: 1\n");
      root = left_right_tree(1, root);
      /* if a leaf write key to file and reset root */
      if(leaf_check(root)) {
        decoded_char = root->key;
        decoded_char_ptr = &decoded_char;
        //printf("decoded char: %c\n", *decoded_char_ptr);
        write_check = write(fp_out, decoded_char_ptr, 1);
        if(write_check <= 0) {
          perror("could not write char to file");
          exit(EXIT_FAILURE);
        }
        decoded_char_ptr = NULL;
        root = true_head;
      }
      /* copy original byte to new byte */
      memcpy(new_byte, hold_byte, 1);
    }
    else {
      //printf("next bit is: 0\n");
      /* moves root left in tree */
      root = left_right_tree(0, root);
      /* if a leaf write key to file and reset root */
      if(leaf_check(root)) {
        decoded_char = root->key;
        decoded_char_ptr = &decoded_char;
        write_check = write(fp_out, decoded_char_ptr, 1);
        if(write_check < 0) {
          perror("could not write char to file");
          exit(EXIT_FAILURE);
        }
        root = true_head;
      }
      /* copy original byte to new byte */
      memcpy(new_byte, hold_byte, 1);
    }
    x--;
  }
  return root;
}

HuffmanNode *left_right_tree(int boolean, HuffmanNode *head) {
  if(boolean == 1) {
    if(head->right != NULL) {
      head = head->right;
    }
  }
  if(boolean == 0) {
    if(head->left != NULL) {
      head = head->left;
    }
  }
  return head;
}

int leaf_check(HuffmanNode *current) {
  if((current->left == NULL) && (current->right == NULL)) {
    return 1;
  }
  else {
    return 0;
  }
}
