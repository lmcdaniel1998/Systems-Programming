#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
  pid_t parent, child;
  int status;

  printf("Hello, world!\n");

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
    /* print parent id */
    parent = getpid();
    printf("This is the parent, pid %d.\n", parent);

    if(WIFEXITED(status)) {
      /* print out parent process id */
      parent = getpid();
      printf("This is the parent, pid %d, signing off.\n", parent);
      return WEXITSTATUS(status);
    }
    else {
     perror("child did not exit");
     exit(EXIT_FAILURE);
    }
  }
  /* child */
  else {
    /* print out child process id */
    child = getpid();
    printf("This is the child, pid %d.\n", child);
    exit(EXIT_SUCCESS);
  }
  return 0;
}
