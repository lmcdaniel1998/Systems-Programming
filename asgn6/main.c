#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <signal.h>
#include "stage.h"
#include "makePipeline.h"
#include "parseMain.h"

void handler(int num) {
   return;
}

int main(int argc, char *argv[]) {
   
   Stage *stages;
   FILE *fd = stdin;
   
  sigset_t new_set, old_set;
  struct sigaction sigs;

  sigs.sa_handler = handler;

  sigemptyset(&sigs.sa_mask);
  sigs.sa_flags = 0;
  sigaction(SIGINT, &sigs, 0);
  sigemptyset(&new_set);
  sigemptyset(&old_set);
  sigaddset(&new_set, SIGINT);

  if (sigprocmask(SIG_BLOCK, &new_set, NULL) == -1) {
    perror("sigprocmask");
    exit(EXIT_FAILURE);
  }

   if (argc > 1) {
      if (argc > 2) {
         perror("Too many arguments");
         exit(EXIT_FAILURE);
      }
      if (fopen(argv[1], "r") == NULL) {
         perror(argv[1]);
         exit(EXIT_FAILURE);
      }
   }

   while (1) {
      
      if (argc == 1) { 
         printf("8-P ");
      }
      stages = parseMain(fd);
      if (stages) {
         pipeschild(stages);
      }
      if (sigprocmask(SIG_UNBLOCK, &new_set, NULL) == -1) {
        perror("sigprocmask2");
        exit(EXIT_FAILURE);
      }
      freeStages(stages);
      fflush(stdout);
   }
   return 0;
}
