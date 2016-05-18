#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXPATH 200

void* tf(void*);
int* read_file(char*, int*);
void sort (int*, int);
void write_file(int*, int, char*);

struct threadargs {
 char filein[MAXPATH];
 char fileout[MAXPATH];
};

typedef struct threadargs threadArgs;

int main(int argc, char* argv[]) {
	int i, n;
	void* exitStatus;
	pthread_t tid;
	threadArgs *ta;
	
	if (argc != 4) {
		fprintf(stderr, "Errore numero argomenti.\n");
		return -1;
	}
	
	/* Numero file */
	n = atoi(argv[1]);
	
	for (i=0; i<n; i++) {
		/* Alloco struct */
		ta = (threadArgs*) malloc(sizeof(threadArgs));
		
		/* Genero nomi file */
		sprintf(ta->filein, "%s", (char*) argv[2]);
		sprintf(ta->fileout, "%s", (char*) argv[3]);
		
		sprintf(ta->filein, "%s%d%s", ta->filein, i+1, ".txt");
		sprintf(ta->fileout, "%s%d%s", ta->fileout, i+1, ".txt");
		
		/* Creo thread */
		pthread_create(&tid, NULL, tf, (void*) ta);
		pthread_join(tid, &exitStatus);
	}
	
	return 0;
}

void* tf(void* arg) {
	threadArgs *ta;
	char* filename;
	int size, *vett=NULL;
	int i;
	ta = (threadArgs*) arg;
	
	filename = ta->filein;
	
	vett = read_file(filename, &size);
	
	sort(vett, size);
	
	write_file(vett, size, ta->fileout);
	
	free(vett);
	free(ta);
	
	return (void*)0;
}

int* read_file(char* filename, int* size) {
	
	FILE* fp;
	int* vett;
	int i=0, v, N;
	
	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Errore apertura file in lettura.\n");
		exit(-1);
	}
	
	fscanf(fp, "%d", &N);
	
	vett = (int*) malloc(N*sizeof(int));
	
	while (fscanf(fp, "%d", &v) > 0) {
		vett[i] = v;
		i++;
	}
	
	*size = N;
	
	fclose(fp);
	
	return vett;
}

void sort (int* vett, int size) {

	int i,j, tmp;
	
	for(i=0; i<size-1; i++) {
		for(j=0; j<size-1-i; j++) {
			if (vett[j]>vett[j+1]) {
				tmp = vett[j];
				vett[j] = vett[j+1];
				vett[j+1] = tmp;
			}
		}
	}
	
	return;
}

void write_file(int* vett, int size, char* filename) {
	FILE* fp;
	int i;
	
	if ((fp = fopen(filename, "w+")) == NULL) {
		fprintf(stderr, "Errore apertura file in scrittura.\n");
		exit(-2);
	}
	
	for(i=0; i<size; i++) {
		fprintf(fp, "%d\n", vett[i]);
	}
	
	fclose(fp);
}
