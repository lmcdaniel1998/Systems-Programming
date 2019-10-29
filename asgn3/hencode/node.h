#ifndef NODE_H
#define NODE_H

struct huffman_node {
  unsigned char key;
  unsigned char code;
  int freq;
  int has_data;
  struct huffman_node *next;
  struct huffman_node *left;
  struct huffman_node *right;
};

typedef struct huffman_node HuffmanNode;

#endif
