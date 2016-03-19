#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

int figlioProduttore(pid_t, pid_t);
int figlioConsumatore(void);

static void sigUsr (int signo) {
  return;
}

int main() {

  pid_t pid1, pid2;
  int myStat;

   signal(SIGINT, SIG_DFL);

   if (signal(SIGUSR1, sigUsr) == SIG_ERR) {
    fprintf (stderr, "Signal Handler Error.\n");
    return (-1);
   }

   if (signal(SIGUSR2, sigUsr) == SIG_ERR) {
    fprintf (stderr, "Signal Handler Error.\n");
    return (-2);
   }

  if((pid1 = fork())) {
    if ((pid2 = fork())) {
			wait(&myStat);
		} else {
			figlioProduttore(getpid(), pid1);
    }
  } else {
		figlioConsumatore();
  }

  return 0;
}

int figlioProduttore(pid_t pidProduttore, pid_t pidConsumatore) {
	FILE *myfile;
	char stringa[1024];

	if ((myfile = fopen("myfile.txt", "w")) == NULL) {
		fprintf(stderr, "Error file open.\n");
		return (-3);
	}	
	
	sleep(1);
	
	fprintf(myfile, "%d\n", pidProduttore); 
	fclose(myfile);
	
	kill(pidConsumatore, SIGUSR2);
	
	pause();

	while(1) {
		sleep(1);

		if ((myfile = fopen("myfile.txt", "w")) == NULL) {
			fprintf(stderr, "Error file open.\n");
			return (-4);
		}
		
		fprintf(stdout, "%s\n", "Inserisci una stringa: ");
		fscanf(stdin, "%s", stringa);

		if (strcmp(stringa, "end") == 0) {
			fclose(myfile);
			kill(pidConsumatore, SIGINT);
			return 0;
		}

		fprintf(myfile, "%s", stringa);
		fclose(myfile);
		
		kill(pidConsumatore, SIGUSR2);
		
		pause();
	}

	return 0;
}

int figlioConsumatore(void) {
	FILE* myfile;
	pid_t pidProduttore;
	char stringa[1024];
	int i;

	pause();

	if ((myfile = fopen("myfile.txt", "r")) == NULL) {
		fprintf(stderr,"Error file open.\n");
		return (-5);
	}

	fscanf(myfile, "%d", &pidProduttore);
	fclose(myfile);
	
	kill(pidProduttore, SIGUSR1);
	
	pause();

	while(1) {
		sleep(1);

		if ((myfile = fopen("myfile.txt", "r")) == NULL) {
	 		fprintf(stderr,"Error file open.\n");
			return (-6);
		}

		fscanf(myfile, "%s", stringa);

		for(i=0; i<strlen(stringa); i++) {
			stringa[i] = toupper(stringa[i]);
		}

		fprintf(stdout, "%s\n", stringa);

		fclose(myfile);

		kill(pidProduttore, SIGUSR1);
		
		pause();
	}

	return 0;
}
