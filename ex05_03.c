#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void sigUsr (int signo) {
  return;
}

int main() {

  pid_t pid;

  if (signal(SIGUSR1, sigUsr) == SIG_ERR) {
    fprintf (stderr, "Signal Handler Error.\n");
    return (-1);
  }
  if (signal(SIGUSR2, sigUsr) == SIG_ERR) {
    fprintf (stderr, "Signal Handler Error.\n");
    return (-2);
  }
  
  pid = fork();

  if(pid > (pid_t)0) {
    while(1) {
      sleep(1);
      kill(pid, SIGUSR2);
      pause();
      printf("Father woke\n");
    }
  } else {
    while(1) {
      pause();
      printf("Child woke\n");
      sleep(1);
      kill(getppid(), SIGUSR1);
    }
  }
  return 0;
}
