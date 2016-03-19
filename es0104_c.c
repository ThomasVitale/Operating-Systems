#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFDIM 1024

int main(int argc, char* argv[]) {
    char buffer[BUFDIM];
    int srcfd, dstfd;
    int count;
    
    if (argc != 3) {
      fprintf(stderr, "Errore \n");
      return -1;
    }
    
    if ((srcfd = open(argv[1], O_RDONLY)) == -1) {
      fprintf(stderr, "Errore sorgente \n");
      return -2;
    }
    
    if ((dstfd = creat(argv[2], O_RDWR)) == -1) {
      fprintf(stderr, "Errore destinazione \n");
      return -3;
    }

    while((count = read(srcfd, buffer, BUFDIM)) > 0) {
      write(dstfd, buffer, count);
    }

    fprintf(stdout, "%s", "File scritto con successo \n");

    close(srcfd);
    close(dstfd);

    return 0;
 };
