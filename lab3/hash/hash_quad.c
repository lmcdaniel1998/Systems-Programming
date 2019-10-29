#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash.h"

/* define node structure here */
/*struct Node {
  char *key;
  int value;
}*/

int quadratic_probe(char *key, size_t hash_size, Node *hash_table[]);
unsigned hash(char *key, size_t hash_size);

/* init function creates and initializes hashtable */
Node **init(size_t hash_size) {
  int i;
  char *null_pointer;
  /* hash table is a double pointer */
  Node *hash_table[hash_size];
  /* initialize all nodes to have 0 key and value */
  for(i = 0; i < hash_size; i++) {
    /*hash_table[i] = malloc(sizeof(Node));
    if(!hash_table[i]) {
      perror("malloc");
      exit(1);
    }*/
    hash_table[i] = NULL;
  }
  return hash_table;
}

Node *insert(char *key, int value, size_t hash_size, Node *hash_table[]) {
  /* get hash value for node */
  unsigned hash_value, new_hash;
  float load_factor;
  Node *location = NULL;
  /*location = malloc(sizeof(Node));
  if(!location) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }*/
    
  hash_value = hash(key, hash_size);

  /* inserts node into table if node is NOT already in table */
  if(hash_table[hash_value] != NULL) {
    //printf("%d \n", hash_table[hash_value]);
    //hash_table[hash_value]->key = key;
    //hash_table[hash_value]->value = 1;
    //location = hash_table[hash_value];
    //num_nodes++;
  }
  else {
    //printf("null \n");
    /* check if both nodes have same key */
    //if(strcmp(hash_table[hash_value]->key, key)) {
      //hash_table[hash_value]->value = ((hash_table[hash_value]->value) + 1);
      //location = hash_table[hash_value];
    //}
    /* quadratic probe for new location for node in hash table */
    //else {
      //new_hash = quadratic_probe(key, hash_size, hash_table);
      /* return NULL if key cannot be hashed */
      //if(new_hash < 0) {
        //return NULL;
      //}
      /* if new hash has has same key as new location increment value */
      //if(strcmp(hash_table[new_hash]->key, key)) {
        //hash_table[new_hash]->value = ((hash_table[new_hash]->value) + 1);
        //location = hash_table[new_hash];
      //}
      /* add node to new hash location in table */
      //else {
        //hash_table[new_hash]->key = key;
        //hash_table[new_hash]->value = value;
        //location = hash_table[new_hash];
        //location = hash_table[new_hash];
        //num_nodes++;
      //}
    //}
  }
  /*load_factor = get_load_factor(num_nodes, hash_size);
  if(load_factor > 0.5) {
    rehash_table();
  }*/
  /* return node pointer to where value was hashed */
  return location;
}

/* -- start rehash_table here -- */

unsigned hash(char *key, size_t hash_size) {
  unsigned hash_value;
  int i, len, horner_hash;

  len = length(key);

  /* only use first eight letters of key */
  for(i = 0; i < (len || 8); i++) {
    /* horner hash formula */
    horner_hash = key[i] * (pow(31, (len - 1 - i)));
    hash_value = hash_value + horner_hash;
  }
  hash_value = hash_value % hash_size;
  return hash_value;
}

/* used for finding new location in hashtable for node */
int quadratic_probe(char *key, size_t hash_size, Node *hash_table[]) {
  unsigned current_hash, new_hash;
  int probe, probe_limit;

  current_hash = hash(key, hash_size);
  probe_limit = hash_size;
  probe = 1;

  while(probe <= probe_limit) {
    new_hash = current_hash + pow(probe, 2);
    new_hash = new_hash % hash_size;
    /* check if new hash location is empty */
    if(hash_table[new_hash] == NULL) {
      return new_hash;
    }
    /* check if new hash location is key */
    else if(strcmp(hash_table[new_hash]->key, key)) {
      return new_hash;
    }
    else {
      probe++;
    }
  }
  return -1;
}

/* gets the size of a word in a char * */
int length(char *word) {
  int len;
  char c;
  /* count until null terminator */
  for(len = 0; (c = word[len]) != '\0'; len++) {
    ;
  }
  return len;
}
