#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

char *msg = "";

void handler(int num) {
  printf("%s", msg);
  fflush(stdout);
}

void timeit(int sec) {
  
  struct sigaction sa;
  struct itimerval val;
  
  int half_sec = sec * 2;

  sa.sa_handler = handler;
  sa.sa_flags = 0;

  /* initialize signal */
  sigemptyset(&sa.sa_mask);

  /* give action */
  sigaction(SIGALRM, &sa, NULL);

  /* set tick */
  val.it_value.tv_sec = 0;
  val.it_value.tv_usec = 500000;
  val.it_interval.tv_sec = 0;
  val.it_interval.tv_usec = 500000;
  msg = "Tick...";
  
  setitimer(ITIMER_REAL, &val, NULL);

  /* pause every num */
  while(half_sec--) {
    if((half_sec % 2) == 1) {
      msg = "Tick...";
      pause();
    }
    else {
      msg = "Tock\n";
      pause();
    }
  }
}

int main(int argc, char *argv[]) {

  int seconds;

  if(argc != 2) {
    perror("invalid number of arguments");
    exit(EXIT_FAILURE);
  }

  else {
    seconds = atoi(argv[1]);
    if(seconds == 0) {
      perror("please enter a number to count to");
      exit(EXIT_FAILURE);
    }
    if(seconds < 0) {
      perror("please enter a positive time to count to");
      exit(EXIT_FAILURE);
    }
    timeit(seconds);
  }
  return 0;
}
