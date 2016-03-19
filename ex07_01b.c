#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char* argv[]) {

	int i, j, n;
	int* vet;
	int retValue;
	pid_t pid;

	if (argc != 2) {
		fprintf(stderr, "Errore numero argomenti.\n");
		return -1;
	}
	
	n = atoi(argv[1]);
	
	vet = (int*) malloc(n*sizeof(int));
	
	if (vet == NULL) {
		fprintf(stderr, "Errore allocazione vet.\n");
		return -2;
	}
	
	fprintf(stdout, "Numeri binari: \n");
	
	for(i=0; i<=n; i++) {
		if (i==n) {
			for(j=0; j<n; j++) {
				fprintf(stdout, "%d ", vet[j]);
			}
			fprintf(stdout, "\n");
			exit(0);
		} else {
			
			vet[i] = 0;
			
			if(fork()) {
				pid = wait(&retValue);
				vet[i] = 1;
				
				if (fork()) {
					pid = wait(&retValue);
					exit(0);
				}
			}
		}
	}
	
	free(vet);
	
	return 0;
}
