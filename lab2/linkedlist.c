#include <stdio.h>
#include <stdlib.h>

struct Node {
  char *data;
  struct Node *next;
};

void push(struct Node** head_ref, void* new_data, size_t data_size) {
  struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

  new_node->data = malloc(data_size);
  new_node->next = (*head_ref);
  
  int i;
  for(i = 0; i < data_size; i++) {
    *(char *) (new_node->data + i) = *(char *) (new_data + i);
  }
  (*head_ref) = new_node;
}

void printList(struct Node *node, void (*fptr)(void *)) {
  while(node != NULL) {
    (*fptr)(node->data);
    node = node->next;
  }
}
