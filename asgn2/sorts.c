#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/* sorts the array by count from greatest to least */
char** sort(node *hashtab[], long hash_size, int clip_size) {
  char **top_keys;
  node *current;
  int comparisons, hash_idx, hash_pos, y;
  comparisons = 0;

  /* allocate space for node */
  current = malloc(sizeof(node*));
  if(!current) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  /* loop throught hashtable */
  for(hash_idx = 1; hash_idx < hash_size; hash_idx++) {
    current = hashtab[hash_idx];
    hash_pos = hash_idx;
    while(hash_pos > 0) {
      if(hashtab[hash_pos - 1] == NULL) {
        comparisons++;
        hashtab[hash_pos] = NULL;
        hashtab[hash_pos - 1] = current;
        hash_pos = hash_pos - 1;
      }
      else {
        if((current->count) > (hashtab[hash_pos - 1]->count)) {
          comparisons++;
          hashtab[hash_pos] = hashtab[hash_pos - 1];
          hashtab[hash_pos - 1] = current;
          hash_pos = hash_pos - 1;
        }
        else {
          comparisons++;
          break;
        }
      }
    }
  }
  top_keys = malloc(sizeof(char*) * (clip_size + 1));
  if(!top_keys) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  for(y = 0; y < clip_size; y++) {
    if(hashtab[y] != NULL) {
      top_keys[y] = hashtab[y]->key;
    }
  }
  top_keys[clip_size + 1] = NULL;
  return top_keys;
}
