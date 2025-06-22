#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "protocol.h"

// Define o comando para limpar a tela do terminal, adaptando para Windows ou Linux
#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

static time_t inicio_jogo;             // Armazena o timestamp do início do jogo
static bool jogo_terminou = false;     // Flag que sinaliza se o jogo terminou (para a thread de tempo)
static pthread_mutex_t mutex_jogo_terminou = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger 'jogo_terminou'

void* thread_tempo_total(void* arg) {
    // Registra o tempo de início do jogo
    inicio_jogo = time(NULL);
    printf("Jogo iniciado! Cronometrando o tempo total...\n");

    pthread_mutex_lock(&mutex_jogo_terminou);
    while (!jogo_terminou) {
        pthread_mutex_unlock(&mutex_jogo_terminou);
        sleep(1);
        pthread_mutex_lock(&mutex_jogo_terminou);
    }
    pthread_mutex_unlock(&mutex_jogo_terminou);

    // Calcula o tempo total de jogo quando a flag 'jogo_terminou' for verdadeira
    time_t fim_jogo = time(NULL);
    double tempo_total_segundos = difftime(fim_jogo, inicio_jogo);
    printf("\n--- Jogo finalizado! Tempo total de jogo: %.0f segundos ---\n", tempo_total_segundos);

    return NULL;
}

void client(int readfd, int writefd) {
    char letra_para_enviar; // A letra que será enviada para o servidor
    Resposta resp;          // Estrutura para armazenar a resposta recebida do servidor
    pthread_t thread_tempo; // Variável para controlar a thread de tempo total
    char entrada[MAX_WORD_LEN]; // Buffer para a entrada do usuário

    // Inicia a thread que vai cronometrar o tempo total do jogo
    if (pthread_create(&thread_tempo, NULL, thread_tempo_total, NULL) != 0) {
        printf("Erro ao criar a thread de tempo total.\n");
        exit(0);
    }

    // Pequena pausa para permitir que a thread de tempo imprima sua mensagem inicial
    // antes que a tela seja limpa pela primeira vez
    sleep(1); 

    while (true) {
        if (read(readfd, &resp, sizeof(Resposta)) <= 0) {
            printf("Erro ao ler dados do servidor ou o servidor encerrou inesperadamente.\n");
            break;
        }

        // Limpa o terminal antes de imprimir o novo estado do jogo
        system(CLEAR_SCREEN);

        // Exibe o estado atual do jogo para o usuário
        printf("--- Jogo da Forca ---\n");
        printf("Dica: %s\n", resp.dica);
        printf("Palavra: %s\n", resp.palavra_oculta);
        printf("Tentativas restantes: %d\n", resp.tentativas_restantes);
        printf("Letras já tentadas: %s\n", resp.letras_tentadas);

        // Verifica o status do jogo recebido do servidor
        if (resp.status == JOGO_VENCEU) {
            printf("\nParabéns! Você venceu!\n");
            printf("A palavra era: %s\n", resp.palavra_oculta);
            // Sinaliza para a thread de tempo que o jogo terminou
            pthread_mutex_lock(&mutex_jogo_terminou);
            jogo_terminou = true;
            pthread_mutex_unlock(&mutex_jogo_terminou);
            break;
        } else if (resp.status == JOGO_PERDEU) {
            printf("\nVocê perdeu. Fim de jogo!\n");
            printf("A palavra correta era: %s\n", resp.palavra_oculta);
            // Sinaliza para a thread de tempo que o jogo terminou
            pthread_mutex_lock(&mutex_jogo_terminou);
            jogo_terminou = true;
            pthread_mutex_unlock(&mutex_jogo_terminou);
            break;
        }

        printf("Digite uma letra: ");

        fgets(entrada, sizeof(entrada), stdin);

        // Prepara a letra a ser enviada ao servidor.
        // Se o usuário digitou algo, pega o primeiro caractere; caso contrário, envia '\0'.
        if (strlen(entrada) > 0) {
            letra_para_enviar = entrada[0];
        } else {
            letra_para_enviar = '\0';
        }

        // Envia a letra para o servidor via pipe
        if (write(writefd, &letra_para_enviar, sizeof(char)) < 0) {
            printf("Erro ao enviar a letra para o servidor.\n");
            break;
        }
    }

    if (pthread_join(thread_tempo, NULL) != 0) {
        printf("Erro ao aguardar a thread de tempo total.\n");
    }
    pthread_mutex_destroy(&mutex_jogo_terminou);
}