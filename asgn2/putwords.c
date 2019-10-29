#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

unsigned hash(char *s, long hashsize);

void putwords(node *hashtab[], int numWords) {
  int i;
  printf("The top %d (out of words) are: \n", numWords);
  for(i = 0; i < numWords; i++) {
    printf("%9d %-s\n", hashtab[i]->count, hashtab[i]->key);
  }
}
