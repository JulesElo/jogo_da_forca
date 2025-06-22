#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "protocol.h"

void client(int readfd, int writefd);
void server(int readfd, int writefd);

main() {
    int descritor,  // Usado para criar o processo filho pelo FORK
        pipe1[2],   // Comunicação pai → filho
        pipe2[2];   // Comunicação filho → pai

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        printf("Erro na chamada PIPE!\n");
        exit(0);
    }

    if ((descritor = fork()) < 0) {
        printf("Erro na chamada FORK!\n");
        exit(0);
    } else if (descritor > 0) { // Processo Pai
        close(pipe1[0]);    // Fecha leitura no pipe1
        close(pipe2[1]);    // Fecha escrita no pipe2

        client(pipe2[0], pipe1[1]); // Chama a função 'client'

        close(pipe1[1]);    // Fecha escrita no pipe1
        close(pipe2[0]);    // Fecha leitura no pipe2
        exit(0);
    } else {    // Processo filho
        close(pipe1[1]);    // Fecha escrita no pipe1
        close(pipe2[0]);    // Fecha leitura no pipe2

        server(pipe1[0], pipe2[1]); // Chama a função 'server'

        close(pipe1[0]);    // Fecha leitura no pipe1
        close(pipe2[1]);    // Fecha escrita no pipe2
        exit(0);
    }
}