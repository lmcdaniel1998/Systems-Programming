#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

//char *readline(FILE *file);
//char **getwords(char *line);

/*struct Node {
  char *key;
  int value;
};*/

Node **init(size_t hash_size);
Node *insert(char *key, int value, size_t hash_size, Node *hash_table[]);

int main() {
 
  /*int a;
  char *name[] = { "illegal", "ho", "mommy", "sadfasdfasdf", "x" };

  for(a = 0; a < 5; a++) {
    fputs(name[a], stdout);
  }*/
  /*
  int x;
  char *str;
  char **ptr;
  str = readline(stdin);
  ptr = getwords(str);
  x = 0;
  while(ptr[x] != NULL) {
    fputs(ptr[x], stdout);
    putchar('\n');
    x++;
  }*/

  char *line;
  char *new_word;
  char **words;
  size_t hash_size = 101;
  Node **hash_table;
  int c, z;

  char *arr[6] = { "hello", "my", "name", "is", "luke", "mcdaniel" };
  words = arr;
  //line = malloc(sizeof(char));
  //if(!line) {
    //perror("malloc");
    //exit(EXIT_FAILURE);
  //}
  /* set first line of file to curr */
  //line = (char*) readline(stdin);
  /* initialize hashtab */
  hash_table = init(hash_size);
  int h = 0;
  for(h; h < hash_size; h++) {
    hash_table[h] = NULL;
  }
  /* loop through file until end */
  //while(line) {
    //c = 0;
    //words = (char**) getwords(line);
    /* insert each word into hashtable */
  //}
  //printf("initialized table over \n");  
  putchar('\n');
  for(c = 0; c < 6; c ++) {
    printf("%s \n", words[c]);
    new_word = insert(words[c], 1, hash_size, hash_table);
  }
  
  for(z = 0; z < hash_size; z++) {
    if(hash_table[z] == NULL) {
      printf("%d. null \n", z);
    }
    else {
      printf("%d. %d \n", z, hash_table[z]);
    }
  }
     //printf("%s \n", line);
     /*x = 0;
     while(words[x] != NULL) {
       fputs(words[x], stdout);
       putchar('\n');
       x++;
     }*/
     //printf("there are %d words \n", x);
     //fputs(*(words), stdout);
     //putchar('\n'); 
     //line = readline(stdin);
  //}
  return 0;
}
