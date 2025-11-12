#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa cada sala (nó da árvore)
struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
};

// Função para criar dinamicamente uma nova sala
struct Sala* criarSala(const char *nome) {
    struct Sala *nova = (struct Sala*)malloc(sizeof(struct Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função que permite explorar a mansão interativamente
void explorarSalas(struct Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Caso a sala não tenha saídas (nó-folha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Você encontrou o final da mansão.\n");
            return;
        }

        printf("Escolha seu caminho:\n");
        if (atual->esquerda != NULL)
            printf(" (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf(" (d) Ir para %s\n", atual->direita->nome);
        printf(" (s) Sair da exploração\n");
        printf("Digite sua opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } 
        else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } 
        else if (escolha == 's') {
            printf("Você decidiu sair da mansão.\n");
            return;
        } 
        else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

// Função para liberar memória da árvore
void liberarMansao(struct Sala *raiz) {
    if (raiz == NULL) return;
    liberarMansao(raiz->esquerda);
    liberarMansao(raiz->direita);
    free(raiz);
}

int main() {
    // Criação estática da árvore (mapa fixo da mansão)
    struct Sala *hallEntrada = criarSala("Hall de Entrada");
    struct Sala *biblioteca = criarSala("Biblioteca");
    struct Sala *cozinha = criarSala("Cozinha");
    struct Sala *salaJantar = criarSala("Sala de Jantar");
    struct Sala *jardim = criarSala("Jardim");
    struct Sala *adega = criarSala("Adega Secreta");

    // Conectando os nós (estrutura fixa)
    hallEntrada->esquerda = biblioteca;
    hallEntrada->direita = cozinha;

    biblioteca->esquerda = salaJantar;
    biblioteca->direita = jardim;

    cozinha->esquerda = adega;
    cozinha->direita = NULL;

    // Início da exploração
    printf("Bem-vindo à Mansão Misteriosa!\n");
    explorarSalas(hallEntrada);

    // Libera memória antes de encerrar
    liberarMansao(hallEntrada);

    printf("\nExploração encerrada. Fim do jogo.\n");
    return 0;
}
