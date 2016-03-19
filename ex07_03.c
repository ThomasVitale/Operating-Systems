#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXPATH 200
#define MAXSIZE 1000

void* tf(void*);
int* read_file(char*, int*);
void sort (int*, int);
int merge(int*, int*, int*, int, int);
void print(int*, int);
void write_file(int*, int, char*);

int **matrix;
int *thFlags;

struct threadargs {
	int id;
	char filein[MAXPATH];
};

typedef struct threadargs threadArgs;

int main(int argc, char* argv[]) {

	int i, n, m, indexr, indexc, index;
	pthread_t *tid;
	threadArgs *ta;
	
	int ndone=0;
	int t=0;
	int *vettMerge;
	int sizeMerge=0;
	
	int *vt, vtSize, *vtmp, vtmpSize;
	
	if (argc != 3) {
		fprintf(stderr, "Errore numero argomenti.\n");
		return -1;
	}
	
	/* Numero file */
	n = atoi(argv[1]);
	
	/* Alloco tid */
	tid = (pthread_t*) malloc(n*sizeof(pthread_t));
	
	/* Alloco e inizializzo flags */
	thFlags = (int*) malloc(n*sizeof(int));
	
	for(m=0; m<n; m++) {
		thFlags[m] = 0;
	}
	
	/* Alloco matrix */
	fprintf(stdout, "Alloco matrix.\n");
	matrix = malloc(n*sizeof(int*));
	for(index=0; index<n; index++) {
		matrix[index] = malloc(MAXSIZE*sizeof(int));
	}
	
	/* Inizializzo matrix */
	fprintf(stdout, "Inizializzo matrix.\n");
	
	for (indexr=0; indexr<n; indexr++) {
		for(indexc=0; indexc<MAXSIZE; indexc++) {
			matrix[indexr][indexc] = 0;
		}
	}
	
	/* Ciclo thread */
	for (i=0; i<n; i++) {
	
		fprintf(stdout, "Creo thread.\n");
		
		/* Alloco struct */
		ta = (threadArgs*) malloc(sizeof(threadArgs));
		
		ta->id = i;
		
		/* Genero nomi file */
		sprintf(ta->filein, "%s", (char*) argv[2]);
		
		sprintf(ta->filein, "%s%d%s", ta->filein, i+1, ".txt");
		
		/* Creo thread */
		pthread_create(&tid[i], NULL, tf, (void*) ta);
	}
	
	/* Alloco vettMerge */
	vettMerge = (int*) malloc((MAXSIZE*n)*sizeof(int));
	
	/* Inizializzo vettMerge */
	for (m=0; m<(MAXSIZE*n); m++) {
		vettMerge[m] = 0;
	}
	
	fprintf(stdout, "Inizio operazioni di merging: n=%d\n", n);
	
	/* Operazione merge */
	while(ndone < n) {
		if (thFlags[t] == 1) {
			thFlags[t] = 0;
			vt = (int*) (matrix[t]+1);
			vtSize = matrix[t][0];
			vtmpSize = sizeMerge;
			
			/* Alloco vtmp */
			vtmp = (int*) malloc((MAXSIZE*n)*sizeof(int));
			
			/* Inizializzo vtmp tramite vettMerge */
			for (m=0; m<vtmpSize; m++) {
				vtmp[m] = vettMerge[m];
			}
			
			sizeMerge = merge(vt, vtmp, vettMerge, vtSize, vtmpSize);
			
			free(vtmp);
			
			fprintf(stdout, "Step %d: vett merge= ", ndone);
			print(vettMerge, sizeMerge);
			ndone++;
		}
		
		t++;
		t = t%n;
		sleep(1);
	}
	
	write_file(vettMerge, sizeMerge, "finaloutput.txt");
	
	/* Libera memoria */
	free(vettMerge);
	
	for(index=0; index<n; index++) {
		free(matrix[index]);
	}
	
	free(matrix);
	free(tid);
	
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
	
	/* Copia vettore in matrix */
	matrix[ta->id][0] = size;
	
	for (i=1; i<=size; i++) {
		matrix[ta->id][i] = vett[i-1];
	}
	thFlags[ta->id] = 1;
	
	free(vett);
	free(ta);
	
	pthread_exit((int*)NULL);
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

int merge(int* v1, int* v2, int* vettMerge, int size_v1, int size_v2) {
	int size_vettMerge;
  int i,j,k;
  
  i=j=k=0; 

  while (i<size_v1 && j<size_v2) { 
    if (v1[i]<v2[j])
			vettMerge[k++] = v1[i++];
    else
			vettMerge[k++] = v2[j++];
	} 
	
  while (i<size_v1) {   
		vettMerge[k++] = v1[i++];
  }
  
  while (j<size_v2) {
		vettMerge[k++] = v2[j++];
  }
  
  size_vettMerge = size_v1+size_v2; 
  
  return size_vettMerge;  
}

void print(int* vett, int size) {
	int i;
	
	for(i=0; i<size; i++) {
		fprintf(stdout, "%d ", vett[i]);
	}
	fprintf(stdout, "\n");
	
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
