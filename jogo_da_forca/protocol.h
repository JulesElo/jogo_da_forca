#ifndef PROTOCOL_H
#define PROTOCOL_H

// Tamanho máximo para a palavra a ser adivinhada
#define MAX_WORD_LEN 32 
// Tamanho máximo para a dica da palavra
#define MAX_HINT_LEN 64 
// Número máximo de tentativas permitidas ao jogador
#define MAX_ATTEMPTS 6

// Define os possíveis status do jogo, enviados do servidor para o cliente
typedef enum {
    JOGO_CONTINUA = 0,  // O jogo ainda está em andamento
    JOGO_VENCEU   = 1,  // O jogador adivinhou a palavra e venceu
    JOGO_PERDEU   = -1  // O jogador esgotou as tentativas e perdeu
} StatusJogo;

// Estrutura de dados para a resposta enviada do servidor para o cliente
typedef struct {
    char dica[MAX_HINT_LEN];              // A dica para a palavra oculta
    char palavra_oculta[MAX_WORD_LEN];    // A palavra com as letras adivinhadas reveladas (ex: "_ _ A _")
    int tentativas_restantes;             // O número de tentativas restantes para o jogador
    StatusJogo status;                    // O status atual do jogo (continua, venceu, perdeu)
    char letras_tentadas[26];             // Armazena as letras que o jogador já tentou
} Resposta;

#endif