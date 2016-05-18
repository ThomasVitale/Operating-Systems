#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE *sorgente;
    FILE *destinazione;
    char buffer[1024];
    int num_elem;
    
    if (argc != 3) {
      fprintf(stderr, "Errore \n");
      return -1;
    }
    
    if ((sorgente = fopen(argv[1], "rb")) == NULL) {
      fprintf(stderr, "Errore sorgente \n");
      return -2;
    }
    
    if ((destinazione = fopen(argv[2],"wb")) == NULL) {
      fprintf(stderr, "Errore destinazione \n");
      return -3;
    }

    while((num_elem = fread(buffer, 1, sizeof(buffer), sorgente)) != 0) {
      fwrite(buffer, 1, num_elem, destinazione);
    }

    fprintf(stdout, "File scritto con successo \n");

    fclose(sorgente);
    fclose(destinazione);

    return 0;
 };
