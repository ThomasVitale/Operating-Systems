#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char* argv[]) {
  FILE* sorgente;
  char string[128];
  char** vett;
  int i,j;

  if (argc != 2) {
    fprintf(stderr, "Errore numero argomenti.\n");
    return (-1);
  }

  if ((sorgente = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Errore apertura file.\n");
    return (-2);
  }

  vett = (char**)malloc(10*sizeof(char*));

  i = 0;
  while (fscanf(sorgente, "%s", string) != EOF) {
		vett[i] = (char*)malloc(10*sizeof(char));
		if (strcmp(string,"end") != 0) {    
    	strcpy(vett[i++],string);
		} else {
      	vett[i] = NULL;
        if (fork() > 0) {          
	  			sleep(3);
        } else {
          execvp(vett[0], vett);
        }
				
				for(j=i; j>=0; j--)
					free(vett[j]);	

				i = 0;
    }
  }

  return 0;

}
