#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DIM 10

void child(int, int);

int main(int argc, char* argv[]) {

	int fd;
	int i, j, v;
	int vet = [7, 21, 19, -3, 56, 99, 21, 44, 31, 93];

	if (argc != 2) {
		fprintf(stderr, "Errore numero argomenti.\n");
		return -1;
	}
	
	if ((fd = creat(argv[1], O_RDWR)) < 0) {
		fprintf(stderr, "Errore apertura file.\n");
		return -2;
	}
	
	/* Scrivo file */
	fprintf(stdout, "%s", "Vettore inziale: ");
	for(i=0; i<DIM; i++) {
		write(fd, &vet[i], sizeof(int));
		fprintf(stdout, "%2d ", vet[i]);
	}
	fprintf(stdout, "%s", "\n");
	
	close(fd);
	
	/* Leggo file */
	
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "Errore apertura file in lettura.\n");
		return -3;
	}
	
	fprintf(stdout, "Vettore prima dell'ordinamento: ");
	for (i=0; i<DIM; i++) {
		lseek(fd, i*sizeof(int), SEEK_SET);
		if (read(fd, &v, sizeof(int)) < 0) {
			fprintf(stderr, "Errore lettura file.\n");
			return -4;
		}
		fprintf(stdout, "%2d ", v);
	}
	fprintf(stdout, "%s", "\n");
	
	close(fd);
	
	/* Ordino file */
	
	if ((fd = open(argv[1], O_RDWR)) < 0) {
		fprintf(stderr, "Errore apertura file in lettura e scrittura.\n");
		return -5;
	}
	
	for(i=0; i<DIM-1; i++) {
		for(j=0; i<DIM-1-i; j++) {
			if (fork() > 0) {
				wait((int*)0);
			} else {
				child(fd, j);
				exit(0);
			}	
		} 
	}
	
	close(fd)
	
	return 0;
}

void child(int fd, int j) {
	
	int v1, v2;
	
	lseek(fd, j*sizeof(int), SEEK_SET);
	if (read(fd, &v1, sizeof(int)) < 0 || read(fd, &v2, sizeof(int)) < 0) {
		fprintf(stderr, "Errore lettura file per ordinamento.\n");
		return;
	}
	
	if (v1 > v2) {
		lseek(fd, j*sizeof(int), SEEK_SET);
		if (write(fd, &v2, sizeof(int)) < 0 || write(fd, &v1, sizeof(int)) < 0) {
			fprintf(stderr, "Errore scrittura file per ordinamento.\n");
			return;
		}
	}

	return;
}
