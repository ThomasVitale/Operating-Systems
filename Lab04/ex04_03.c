#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void pLeft(void);
void pRight(void);

int main() {
	pid_t pid;

	fprintf(stdout, "%s%d%s", "P1 (pid=", getpid(), ")\n");
	
	pid = fork();

	if (pid==0) {
		pRight();
		exit(0);
	} else {
		pLeft();
		wait((int*) 0);
		fprintf(stdout, "%s%d%s", "P9 (pid=", getpid(), ")\n");
		return 0;
	}
}

void pRight() {
	pid_t pid;
	
	fprintf(stdout, "%s%d%s", "P3 (pid=", getpid(), ")\n");
	
	pid = fork();

	if (pid == 0) {
		sleep(1);
		fprintf(stdout, "%s%d%s", "P6 (pid=", getpid(), ")\n");
		exit(0);
	} else {
		wait((int*) 0);
		sleep(1);
		fprintf(stdout, "%s%d%s", "P8 (pid=", getpid(), ")\n");
		exit(0);
	}
}

void pLeft() {
	pid_t pid;

	fprintf(stdout, "%s%d%s", "P2 (pid=", getpid(), ")\n");

	pid = fork();

	if (pid == 0) {
		sleep(1);
		fprintf(stdout, "%s%d%s", "P5 (pid=", getpid(), ")\n");
		exit(0);
	} else {	
		sleep(1);
		fprintf(stdout, "%s%d%s", "P4 (pid=", getpid(), ")\n");
		while(wait((int*)0) != pid);
		sleep(1);
		fprintf(stdout, "%s%d%s", "P7 (pid=", getpid(), ")\n");
		return;
	}
}


