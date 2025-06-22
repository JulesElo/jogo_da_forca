# Jogo da Forca

Este projeto implementa uma versão do clássico jogo da forca, utilizando conceitos fundamentais de **Sistemas Operacionais**: **Comunicação Interprocessos (IPC) via Pipes** e **Multithreading**. A aplicação é dividida em um modelo cliente-servidor para demonstrar a interação entre processos separados.

## Funcionalidades

* **Arquitetura Cliente-Servidor:** Separação da lógica do jogo (Servidor) da interface do usuário (Cliente).
* **Comunicação via Pipes:** Utilização de dois pipes para a troca de informações entre o processo cliente e o processo servidor.
* **Dicionário de Palavras Dinâmico:** O servidor escolhe palavras e dicas aleatoriamente de uma lista pré-definida.
* **Contagem de Tentativas:** O jogador possui um número limitado de tentativas para adivinhar a palavra.
* **Feedback Visual:** Exibição da palavra oculta, dicas, tentativas restantes e letras já tentadas.
* **Cronômetro de Jogo:** Uma thread separada no cliente mede e exibe o tempo total de jogo.
* **Mensagem Final:** Exibe a palavra correta ao final do jogo, seja em caso de vitória ou derrota.

## Tecnologias Utilizadas

* **Linguagem de Programação:** C
* **Conceitos de SO:**
    * `fork()`: Criação de processos.
    * `pipe()`: Comunicação Interprocessos (IPC).
    * `pthread` (POSIX Threads): Multithreading.
    * `pthread_mutex_t`: Sincronização de threads.

## 📦 Como Compilar e Rodar

Para compilar e executar o projeto, você precisará de um compilador C (como GCC) e um sistema operacional Unix-like (Linux, macOS) ou um ambiente de desenvolvimento que simule um (como WSL no Windows).

1.  **Clone o Repositório:**
    ```bash
    git clone [https://github.com/JulesElo/jogo_da_forca.git](https://github.com/JulesElo/jogo_da_forca.git)
    cd jogo_da_forca
    ```

2.  **Compile os Arquivos:**
    Utilize o `gcc` para compilar os arquivos-fonte. O flag `-pthread` é essencial para vincular a biblioteca de threads.
    ```bash
    gcc main.c client.c server.c -o forca_game -pthread
    ```

3.  **Execute a Aplicação:**
    ```bash
    ./forca_game
    ```

## 📂 Estrutura do Projeto

* `main.c`: Ponto de entrada do programa. Responsável por criar os pipes e "bifurcar" o processo em cliente e servidor usando `fork()`.
* `protocol.h`: Define as estruturas de dados e constantes compartilhadas entre cliente e servidor, garantindo a interoperabilidade da comunicação. Inclui a `struct Resposta` e o `enum StatusJogo`.
* `client.c`: Implementa a lógica do lado do cliente (interface com o usuário). Lê a entrada, exibe o estado do jogo e gerencia a thread de cronometragem.
* `server.c`: Implementa a lógica do lado do servidor (regras do jogo). Gerencia o dicionário, a palavra secreta, as tentativas e o status do jogo.

## 📄 Licença

Este projeto está licenciado sob a Licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

---

**Autores:** 
* Jules Eloísio Moraes Lima
* Vitor Hugo Granato Moreira do Prado