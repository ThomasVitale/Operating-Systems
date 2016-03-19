#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void child(int,int);

int main(int argc, char* argv[]) {

	FILE *myFile;
	int n, i, j;
	int fd;

	if (argc != 2) {
		fprintf(stderr, "Errore numero argomenti.\n");
		return -1;
	}
	
	/* Leggo il numero n di elementi presenti nel file */
	if ((myFile = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Errore apertura file fopen.\n");
		return -2;
	}
	
	fscanf(myFile, "%d", &n);
	fclose(myFile);
	
	/* Ordinamento */
	if ((fd = open(argv[1], O_RDWR)) == -1) {
		fprintf(stderr, "Errore apertura file open.\n");
		return -3;
	} 
	
	for(i=1; i<n; i++) {
		for(j=1; j<n-i; j++) {
			if (fork() > 0) {
				wait((int*)0);
			} else {
				child(fd, j-1);
				exit(0);
			}
		}
	}
	
	close(fd);
	
	return 0;
}

void child(int fd, int j) {
	
	int v1, v2;
	
	/* Lettura */
	lseek(fd, j*sizeof(int), SEEK_SET);
	
	if (read(fd, &v1, sizeof(int)) < 0) {
		fprintf(stderr, "Errore read v1.\n");
		exit(-1);
	}
	
	if(read(fd, &v2, sizeof(int)) < 0) {
		fprintf(stderr, "Errore read v2.\n");
		exit(-2);
	}
	
	/* Scambio */
	
	if (v1 > v2) {
	
		lseek(fd, j*sizeof(int), SEEK_SET);
		
		if (write(fd, &v2, sizeof(int)) < 0) {
		fprintf(stderr, "Errore write v2.\n");
		exit(-3);
		}
	
		if (write(fd, &v1, sizeof(int)) < 0) {
			fprintf(stderr, "Errore write v1.\n");
			exit(-4);
		}
		
	}
	
	return;
	
}
