#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE *sorgente;
    FILE *destinazione;
    char ch;
    
    if (argc != 3) {
      fprintf(stderr, "Errore \n");
      return -1;
    }
    
    if ((sorgente = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "Errore sorgente \n");
      return -2;
    }
    
    if ((destinazione = fopen(argv[2],"w+")) == NULL) {
      fprintf(stderr, "Errore destinazione \n");
      return -3;
    }

    while((ch = fgetc(sorgente)) != EOF) {
      fputc(ch, destinazione);
    }

    fprintf(stdout, "%s", "File scritto con successo \n");

    fclose(sorgente);
    fclose(destinazione);

    return 0;
 };
