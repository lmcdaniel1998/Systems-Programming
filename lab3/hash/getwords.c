#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char **getwords(char *line) {
 
  char **words;
  char *word;
  int c, len, num;			// position in line, position in word, number of words
  size_t word_size = 0;			// actual size of word in bits
  int word_switch;			// determines if inside or outside of word
  size_t word_capacity = sizeof(char);	// size of memory for current word
  size_t words_capacity = sizeof(line);	// size of memory for all words
  size_t words_size = 0;		// actual size of words in bits

  /* create space for words */
  words = malloc(words_capacity);
  if(!words) {
    perror("malloc 1");
    exit(EXIT_FAILURE);
  }
  /* create space for average word */
  word = malloc(word_capacity);
  if(!word) {
    perror("malloc 2");
    exit(EXIT_FAILURE);
  }

  /* check if first character in line is alphabetical */
  if(isalpha(line[0])) {
    /* word_switch = 1 means we are in a word */
    word_switch = 1;
  }
  else {
    /* word_switch = 0 means we are NOT in a word */
    word_switch = 0;
  }
  /* initialize number of words */
  num - 0;
  /* initialize letter counter for word */
  len = 0;
  /* c is place in line array */
  c = 0;
  /* while not at null terminator */
  while(line[c] != '\0') {
    /* in a word */
    if(word_switch) {
      /* is a letter 		middle of word */
      if(isalpha(line[c])) {
        /* not enough memory */
        if(word_size > word_capacity) {
          word_capacity = word_size;
          word = realloc(word, word_capacity);
          if(!word) {
            perror("realloc 1");
            exit(EXIT_FAILURE);
          }
        }
        /* add char to word an increase len */
        word[len] = tolower(line[c]);
        len++;
        word_size = word_size + sizeof(line[c]);
      }
      /* not a letter 	leaving a word */
      else {
        word_switch = 0;
        /* not enough memory */
        if((word_size + sizeof('\0')) > word_capacity) {
          word_capacity = word_size + sizeof('\0');
          word = realloc(word, word_capacity);
          if(!word) {
            perror("realloc 2");
            exit(EXIT_FAILURE);
          }
        }
        word[len + 1] = '\0';
        len = 0;

        /* check if there is room to store word */
        if((words_size + sizeof(word)) > words_capacity) {
          words_capacity = words_size + sizeof(word);
          words = realloc(words, words_capacity);
          if(!words) {
            perror("realloc 3");
            exit(EXIT_FAILURE);
          }
        }
        /* add pointer to word to words pointer */
        words[num] = word;  //error is in this line has something to do with memory allocation!!!!!!!!!!!!!!!!!!!!!!
        num++;
        words_size = words_size + sizeof(word);
      }
    }
    /* not in a word */
    else {
      /* is a letter 		entering a word */
      if(isalpha(line[c])) {
        /* starting size of word */
        word_capacity = sizeof(char);
        /* allocate space for new word */
        word = malloc(word_capacity);
        if(!word) {
          perror("malloc 3");
          exit(EXIT_FAILURE);
        }
        word_switch = 1;
        word[0] = line[c];
        word_size = word_size + sizeof(line[c]);
        len = 1;
      }
    }
    c++;
  }

  /* add null terminator to end of words list */
  /*if((words_size + sizeof(NULL)) > sizeof(words_capacity)) {
    words_size = words_size + sizeof(NULL);
    words = realloc(words, words_size);
    if(!words) {
      perror("realloc 4");
      exit(EXIT_FAILURE);
    }
  }
  words[num + 1] = NULL;*/
  return words;
}
