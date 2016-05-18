#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/wait.h>
#include <unistd.h>

void figlioProduttore(pid_t, int*);
void figlioConsumatore(int*);

int main() {
	pid_t pid;
	int fd[2];
	
	pipe(fd);
	
	pid = fork();
	
	if (pid == 0) {
		/* Figlio Consumatore */
		figlioConsumatore(fd);
	} else {
		if (fork()) {
			/* Padre */
			wait((int*)0);
		} else {
			/* Figlio Produttore */
			figlioProduttore(pid, fd);
		}
	}
	
	return 0;
	
}

void figlioProduttore(pid_t pidConsumatore, int fd[2]) {

	char line[128], tmp[128];
	pid_t pidProduttore;
	int msgProd=0;
	
	close(fd[0]);
	pidProduttore = getpid();
	
	while(1) {
		fprintf(stdout, "Produttore (pidConsumatore=%d): ", pidConsumatore);
		fflush(stdout);
		
		fscanf(stdin, "%s", line);
		sprintf(tmp, "%5d%s", pidProduttore, line);
		
		write(fd[1], tmp, (strlen(tmp)+1));
		
		msgProd++;
		
		if (strcmp(line, "end") == 0)
			break;
			
		sleep(1);
		
	}
	
	exit(msgProd);
}

void figlioConsumatore(int fd[2]) {
	
	char line[128], tmp[128];
	int msgCons=0;
	int i;
	pid_t pidProduttore;
	
	close(fd[1]);
	
	while(1) {
		read(fd[0], tmp, 128);
		sscanf(tmp, "%5d%s", &pidProduttore, line);
		
		if (strcmp(line, "end") == 0) 
			break;
		
		for(i=0; i<strlen(line); i++) {
			line[i] = toupper(line[i]);
		}
		
		fprintf(stdout, "Consumatore (pidProduttore=%d): %s\n", pidProduttore, line);
		fflush(stdout);
		
		msgCons++;
	}
	
	exit(msgCons);
}
