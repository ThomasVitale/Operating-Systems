#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char* argv[]) {
  FILE* sorgente;
  char string[1024], command[1024];

  if (argc != 2) {
    fprintf(stderr, "Errore numero argomenti.\n");
    return (-1);
  }

  if ((sorgente = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Errore apertura file.\n");
    return (-2);
  }

  command[0] = '\0';
  while (fscanf(sorgente, "%s", string) != EOF) {
		if (strcmp(string,"end") != 0) {
			strcat(command,string);
			strcat(command," ");
		} else {
			system(command);
			command[0] = '\0';
		}
  }

	fclose(sorgente);

  return 0;

}
