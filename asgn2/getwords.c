#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char **getwords(char *line) {
 
  char **words;
  char *word = NULL;
  int c, len;
  int word_switch;
  size_t size = 6;
  size_t word_size = 2;
  int word_count = 0;

  /* create space for words */
  words = malloc(sizeof(char*) * sizeof(line));
  if(!words) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  /* create space for average word */
  word = malloc(sizeof(char) * size);
  if(word) {
    len = 0;

    /* check if first character in line is alphabetical */
    if(isalpha(line[0])) {
      /* word_switch = 1 means we are in a word */
      word_switch = 1;
    }
    else {
      /* word_switch = 0 means we are NOT in a word */
      word_switch = 0;
    }

    /* c is place in line array */
    c = 0;
    /* while not at null terminator */
    while(line[c] != '\0') {
      /* in a word */
      if(word_switch) {
        /* is a letter 		middle of word */
        if(isalpha(line[c])) {
          /* not enough memory */
          if(len > size) {
            size = len * 2;
            word = realloc(word, (sizeof(char) * size));
            if(!word) {
              perror("realloc");
              exit(EXIT_FAILURE);
            }
          }
          /* add char to word an increase len */
          word[len] = tolower(line[c]);
          len++;
        }
        /* not a letter 	leaving a word */
        else {
          word_switch = 0;
          /* not enough memory */
          if(len + 1 > size) {
            size = len + 4;
            word = realloc(word, (sizeof(char) * size));
            if(!word) {
              perror("realloc");
              exit(EXIT_FAILURE);
            }
          }
          word[len + 1] = '\0';
          len = 0;

          /* check if there is room to store word */
          if(word_count > word_size) {
            int q = 0;
            size_t total_word_size = 0;
            word_size = word_size * 2;
            for(q = 0; q < word_count; q++) {
               total_word_size = sizeof(words[q]) + total_word_size;
            }
            words = realloc(words, ((sizeof(char*) *sizeof(words)) + total_word_size + sizeof(word)));
            if(!words) {
              perror("realloc");
              exit(EXIT_FAILURE);
            }
          }
          /* add pointer to word to words pointer */
          words[word_count] = word;
          word_count++;
          //free(word);
        }
      }
      /* not in a word */
      else {
        /* is a letter 		entering a word */
        if(isalpha(line[c])) {
          size = 6;
          word = malloc(sizeof(char) * size);
          word_switch = 1;
          word[0] = line[c];
          len = 1;
        }
      }
      c++;
    }
    /* if word ends with \0 still add to list */
    if(word_count > word_size) {
      int q = 0;
      size_t total_word_size = 0;
      word_size = word_size + 1;
      for(q = 0; q < word_count; q++) {
        total_word_size = sizeof(words[q]) + total_word_size;
      }
      words = realloc(words, ((sizeof(char*) *sizeof(words)) + total_word_size + sizeof(word)));
      if(!words) {
        perror("realloc");
        exit(EXIT_FAILURE);
      }
    }
    words[word_count] = word;
    word_count++;
  }
  if(word_count > word_size) {
    int q = 0;
    size_t total_word_size = 0;
    word_size = word_size + 1;
    for(q = 0; q < word_count; q++) {
      total_word_size = sizeof(words[q]) + total_word_size;
    }
    words = realloc(words, ((sizeof(char*) *sizeof(words)) + total_word_size + sizeof(NULL)));
    if(!words) {
      perror("realloc");
      exit(EXIT_FAILURE);
    }
  }
  words[word_count + 1] = NULL;
  return words;
}
