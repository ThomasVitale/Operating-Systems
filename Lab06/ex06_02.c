#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define N 132

void child(int*, int*);
void father(int*, int*);

int main() {

	int fd1[2], fd2[2];
	pid_t pid;

	pipe(fd1);
	pipe(fd2);
	
	pid = fork();
	
	if (pid == 0) {
		child(fd1, fd2);
	} else {
		father(fd1, fd2);
	}
	
	wait((void *)0); 
	
	return 0;
}

void father(int fd1[2], int fd2[2]) {
	char line[N];
	int i=0;
	
	close(fd1[0]);
	close(fd2[1]);
	
	while(i<10) {
		fprintf(stdout, "I'm the father (PID=%d)\n", getpid());
		
		sprintf(line, "X");
		write(fd1[1], line, strlen(line)+1);
		i++;
		
		read(fd2[0], line, N);
	}
	
	sprintf(line, "end");
	write(fd1[1], line, strlen(line)+1);
	close(fd1[1]);
	close(fd2[0]);
	
	return;
	
}

void child(int fd1[2], int fd2[2]) {

	char line[N];
	
	close(fd1[1]);
	close(fd2[0]);
	
	while(1) {
		read(fd1[0], line, N);
		
		if (strcmp(line, "end") == 0) {	
			close(fd1[0]);
			close(fd2[1]);
			exit(0);
		} else {
			fprintf(stdout, "I'm the child (PID=%d)\n", getpid());
			sprintf(line, "X");
			write(fd2[1], line, strlen(line)+1);
		}
		
		
	}
	
}
