#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"

#define CHARS 256
HuffmanNode *new_node(unsigned char key, int freq);
HuffmanNode *pop(HuffmanNode *head);
HuffmanNode *insert(HuffmanNode *head, HuffmanNode *newnode); 
int comes_before(HuffmanNode *a, HuffmanNode *b);
//HuffmanNode *combine(HuffmanNode *a, HuffmanNode *b);
void printlist(HuffmanNode *head);
/* returns true = 1 if tree rooted at node a comes before tree rooted at node b, false = 0 otherwise */
int comes_before(HuffmanNode *a, HuffmanNode *b) {
  if((a != NULL) && (b == NULL)) {
    return 1;
  }
  if((a == NULL) && (b != NULL)) {
    return 0;
  }
  if((a == NULL) && (b == NULL)) {
    return 0;
  }
  if((a != NULL) && (b != NULL)) {
    if(a->freq < b->freq) {
      return 1;
    }
    else if(a->freq == b->freq) {
      if(a->key < b->key) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else {
      return 0;
    }
  }
  return -1;
}

/* creates and returns a new huffman node with children a and b with the lesser node on the left
the nodes frequency value will be the sum of the a and be frequencies
the nodes char value will be the lesser of the a and b char ASCII values */
HuffmanNode *combine(HuffmanNode *head, HuffmanNode *second) {
  unsigned char new_key;
  int new_freq;
  HuffmanNode *new_node;
  /* if char a < char b then a is new key */
  if(head->freq < second->freq) {
    new_key = head->key;
    new_freq = head->freq + second->freq;
  }
  else {
    new_key = second->key;
    new_freq = head->freq + second->freq;
  }
  /* allocate space for new node if a comes before b */
  new_node = malloc(sizeof(HuffmanNode));
  if(!new_node) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  /* create new combo node */
  if(comes_before(head, second) == 1) {
    new_node->key = new_key;
    new_node->freq = new_freq;
    new_node->has_data = 1;
    new_node->next = NULL;
    new_node->right = second;
    new_node->left = head;
  }
  else {
    new_node->key = new_key;
    new_node->freq = new_freq;
    new_node->has_data = 1;
    new_node->next = NULL;
    new_node->right = second;
    new_node->left = head;
  }
  return new_node;
}

/* takes a line and counts the frequenies of all characters in that line */
unsigned int *cnt_freq(char *line, unsigned int char_array[]) {
  unsigned int *front = NULL;
  int i, c, n;
  for(n = 0; n < CHARS; n++) {
    char_array[n] = 0;
  }
  i = 0;
  while(line[i] != '\0') {
    /* c is index in char array */
    c = (unsigned char) line[i];
    /* increment counter at char's index */
    (char_array[c])++;
    i++;
  }
  front = &char_array[0];
  return front;
}

/* creates a huffman tree from characters with non-zero frequencies and returns root of tree */
HuffmanNode *create_huff_tree(unsigned int *char_freq) {
  
  int x; 
  int node_count = 0;
  HuffmanNode *a_node;
  HuffmanNode *b_node;
 
  /* create and initialize root node */
  HuffmanNode *root;
  root = NULL;

  /* create nodes and add them to node_list */
  for(x = 0; x < CHARS; x++) {
    if(char_freq[x] != 0) {
      HuffmanNode *newnode;
      newnode = new_node(x, char_freq[x]);
      root = insert(root, newnode);

      node_count++; 
    }
  }
  /* create a binary search tree out of node list */
  while(root->next != NULL) {
    /* get first two nodes in list and combine into tree */
    HuffmanNode *combo_node;
    a_node = root;
    root = pop(root);
    b_node = root;
    root = pop(root);
    combo_node = combine(a_node, b_node);
    root = insert(root, combo_node);
    node_count--;
  }
  return root;
}

HuffmanNode *pop(HuffmanNode *head) {
  HuffmanNode *new_head;
  new_head = head->next;
  head->next = NULL;
  return new_head;
}

HuffmanNode *insert(HuffmanNode *head, HuffmanNode *newnode) {
  HuffmanNode *front;
  /* if new node becomes new head */
  if(!head || head->freq >= newnode->freq) {
    /* if there is another node with the same frequency */
    if(head != NULL) {
      if(head->freq == newnode->freq) {
        if(head->key > newnode->key) {
          front = head;
          newnode->next = head->next;
          head->next = newnode;
          return front;
        }
      }
    }
    newnode->next = head;
    front = newnode;
  }
  else {
    /* search through list for nodes proper place */
    front = head;
    while(head->next && head->next->freq <= newnode->freq) {
      if(head->freq == newnode->freq) {
        if(head->key > newnode->key) {
          newnode->next = head->next;
          head->next = newnode;
          return front;
        }
      }
      head = head->next;
    }
    /* insert node into list */
    newnode->next = head->next;
    head->next = newnode;
  }
  return front;
}

HuffmanNode *new_node(unsigned char key, int freq) {
  HuffmanNode *new;
  new = malloc(sizeof(HuffmanNode));
  if(!new) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  new->key = key;
  new->freq = freq;
  new->has_data = 1;
  new->next = NULL;
  new->right = NULL;
  new->left = NULL;
  return new;
}

void printlist(HuffmanNode *head) {
  HuffmanNode *temp = head;
  while(temp != NULL) {
    printf("key %c char %d\n", temp->key, temp->freq);
    temp = temp->next;
  }
}
