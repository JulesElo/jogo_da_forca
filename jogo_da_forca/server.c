#include <stdio.h>   
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <time.h>    
#include <ctype.h>   
#include <stdbool.h> 
#include "protocol.h" 

typedef struct {
    char palavra[MAX_WORD_LEN]; // A palavra real
    char dica[MAX_HINT_LEN];    // A dica para a palavra
} Dicionario;

// Dicionário de palavras
Dicionario dicionario[] = {
    {"COMPUTADOR", "Dispositivo eletrônico"},
    {"TECLADO", "Usado para digitar"},
    {"INTERNET", "Conecta o mundo"},
    {"PROCESSADOR", "Cérebro do PC"},
    {"MEMORIA", "Guarda dados temporários"},
    {"MONITOR", "Exibe informações visuais"},
    {"MOUSE", "Dispositivo de apontar"},
    {"IMPRESSORA", "Cria cópias em papel"},
    {"SOFTWARE", "Programas de computador"},
    {"HARDWARE", "Componentes físicos"},
    {"PROGRAMACAO", "Criação de algoritmos"},
    {"COMPILADOR", "Traduz código-fonte"},
    {"ALGORITMO", "Sequência de instruções"},
    {"VARIAVEL", "Armazena dados"},
    {"FUNCAO", "Bloco de código reutilizável"},
    {"LINGUAGEM", "Forma de comunicação"},
    {"DESENVOLVIMENTO", "Processo de criação"},
    {"SEGURANCA", "Proteção de dados"},
    {"REDE", "Conexão de dispositivos"},
    {"CRIPTOGRAFIA", "Proteção de informações"},
    {"SERVIDOR", "Fornece recursos"},
    {"CLIENTE", "Acessa recursos"},
    {"DADOS", "Informações brutas"},
    {"NUVEM", "Armazenamento remoto"},
    {"INTELIGENCIA", "Capacidade de aprender"},
    {"ROBOTICA", "Estudo de robôs"},
    {"AUTOMACAO", "Tarefas sem intervenção"},
    {"VIRTUAL", "Não físico, simulado"},
    {"REALIDADE", "Percepção do ambiente"},
    {"ALGORITMO", "Sequência de passos"},
    {"SISTEMA", "Conjunto de partes"},
    {"APLICATIVO", "Programa de celular"},
    {"INTERFACE", "Ponto de contato"},
    {"OTIMIZACAO", "Melhora de desempenho"},
    {"DIAGNOSTICO", "Identificação de problemas"},
    {"MANUTENCAO", "Conservação de sistemas"},
    {"MIGRACAO", "Transferência de dados"},
    {"INTEGRACAO", "União de componentes"},
    {"ANALISE", "Estudo detalhado"},
    {"DESIGN", "Criação de projetos"},
    {"TESTES", "Verificação de funcionalidade"},
    {"DEPLOY", "Implantação de software"},
    {"BACKUP", "Cópia de segurança"},
    {"RESTAURACAO", "Recuperação de dados"},
    {"ATUALIZACAO", "Melhoria de versão"},
    {"DEPURACAO", "Correção de erros"},
    {"ITERACAO", "Repetição de processo"},
    {"METODOLOGIA", "Abordagem sistemática"},
    {"PROTOTIPO", "Modelo inicial"},
    {"VALIDACAO", "Confirmação de requisitos"}
};

// Define o número total de palavras no dicionário
#define DICIONARIO_TAM (sizeof(dicionario) / sizeof(Dicionario))

void gerar_palavra(char* palavra, char* dica) {
    // Define uma palavra aleatória do dicionário
    srand(time(NULL));
    int i = rand() % DICIONARIO_TAM;
    strcpy(palavra, dicionario[i].palavra);
    strcpy(dica, dicionario[i].dica);
}

void ocultar_palavra(const char* palavra_real, char* palavra_oculta) {
    for (int i = 0; palavra_real[i] != '\0'; i++) {
        palavra_oculta[i] = '_';
    }
    palavra_oculta[strlen(palavra_real)] = '\0';
}

bool letra_na_palavra(char letra, const char* palavra_real) {
    for (int i = 0; palavra_real[i] != '\0'; i++) {
        if (palavra_real[i] == letra) return true;
    }
    return false;
}

void atualizar_palavra_oculta(const char* real, char* oculta, char letra) {
    for (int i = 0; real[i] != '\0'; i++) {
        if (real[i] == letra) {
            oculta[i] = letra;
        }
    }
}

bool palavra_completa(const char* oculta) {
    return strchr(oculta, '_') == NULL;
}

// Função para adicionar uma letra às letras tentadas, evitando duplicatas
void adicionar_letra_tentada(char* letras_tentadas, char letra) {
    // Converte a letra para maiúscula antes de adicionar
    char letra_lower = toupper(letra);
    // Verifica se a letra já foi tentada
    if (strchr(letras_tentadas, letra_lower) == NULL) {
        // Se não foi, adiciona ao final da string e garante que seja terminada em null
        int len = strlen(letras_tentadas);
        if (len < 26 - 1) { // Garante espaço para a letra e o null terminator
            letras_tentadas[len] = letra_lower;
            letras_tentadas[len + 1] = '\0';
        }
    }
}

void server(int readfd, int writefd) {
    char palavra_real[MAX_WORD_LEN];  // Armazena a palavra completa que deve ser adivinhada
    char dica[MAX_HINT_LEN];          // A dica para a palavra
    char palavra_oculta[MAX_WORD_LEN]; // A palavra com letras adivinhadas e '_' para as ocultas
    int tentativas_restantes = MAX_ATTEMPTS; // Número de tentativas que o jogador ainda possui
    char letra_recebida;              // A letra recebida do cliente
    Resposta resp;                    // Estrutura para a resposta a ser enviada ao cliente
    char letras_tentadas_servidor[26] = ""; // Inicializa a string de letras tentadas

    // Inicializa o jogo: gera uma palavra e sua dica, e oculta a palavra
    gerar_palavra(palavra_real, dica);
    ocultar_palavra(palavra_real, palavra_oculta);

    while (true) {

        // Copia a dica, palavra oculta atual e tentativas restantes para a resposta
        strcpy(resp.dica, dica);
        strcpy(resp.palavra_oculta, palavra_oculta); 
        resp.tentativas_restantes = tentativas_restantes;
        strcpy(resp.letras_tentadas, letras_tentadas_servidor);

        // Verifica se o jogo terminou por vitória (palavra completa)
        if (palavra_completa(palavra_oculta)) {
            resp.status = JOGO_VENCEU;
        } 
        // Se não venceu, verifica se o jogo terminou por perda (tentativas esgotadas)
        else if (tentativas_restantes <= 0) {
            // Revela a palavra completa na resposta final quando o jogador perde
            resp.status = JOGO_PERDEU;
            strcpy(resp.palavra_oculta, palavra_real); 
        } 
        // Se nenhuma das condições acima for verdadeira, o jogo continua
        else {
            resp.status = JOGO_CONTINUA;
        }

        // Envia a resposta atualizada para o processo cliente.
        if (write(writefd, &resp, sizeof(Resposta)) < 0) {
            printf("Erro ao escrever a resposta para o cliente.\n");
            break;
        }

        if (resp.status == JOGO_VENCEU || resp.status == JOGO_PERDEU) {
            break; // Jogo acabou
        }

        // O servidor lê um 'char' diretamente do pipe
        if (read(readfd, &letra_recebida, sizeof(char)) <= 0) {
            printf("Erro ao ler a letra do cliente ou o cliente encerrou inesperadamente.\n");
            break; // Sai do loop em caso de erro de leitura ou se o cliente fechar o pipe
        }

        // Converte a letra recebida para maiúscula para facilitar a comparação (case-insensitive)
        char letra_processada = toupper(letra_recebida);

        // Se a letra for '\0' (recebida quando o usuário pressiona Enter sem digitar)
        // ou não for um caractere alfabético, penaliza o jogador
        if (letra_processada == '\0' || !isalpha(letra_processada)) {
            printf("Servidor: Entrada inválida ou vazia recebida. Penalidade de -1 tentativa.\n");
            if (tentativas_restantes > 0) { 
                tentativas_restantes--;
            }
        } 
        // Se a letra estiver na palavra, revela-a
        else if (letra_na_palavra(letra_processada, palavra_real)) {
            atualizar_palavra_oculta(palavra_real, palavra_oculta, letra_processada);
            adicionar_letra_tentada(letras_tentadas_servidor, letra_processada);
        } 
        // Se a letra não estiver na palavra, penaliza o jogador
        else {
            printf("Servidor: Letra '%c' não está na palavra. -1 tentativa.\n", letra_processada);
            if (tentativas_restantes > 0) { 
                tentativas_restantes--;
            }
            adicionar_letra_tentada(letras_tentadas_servidor, letra_processada);
        }
    }
}