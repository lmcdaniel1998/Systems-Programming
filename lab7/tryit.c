#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
  pid_t child;
  int err, status;

  if(argc != 2) {
    printf("usage: tryit command\n");
    exit(EXIT_FAILURE);
  }

  /* create child */
  if(child = fork()) {
    /* parent */
    if(child == -1) {
      perror("child fork failure");
      exit(EXIT_FAILURE);
    }

    if(wait(&status) == -1) {
      perror("wait failure");
      exit(EXIT_FAILURE);
    }
    
    if(WIFEXITED(status)) {
      if(WEXITSTATUS(status) == 0) {
        printf("Process %d succeeded.\n", child);
        return 0;
      }
      else {
        printf("Process %d exited with an error value.\n", child);
        return -1;
      }
    }
    else {
     perror("child did not exit");
     exit(EXIT_FAILURE);
    }
  }
  /* child */
  else {
    execve(argv[1], &argv[1], NULL);
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }
  return 0;
}
