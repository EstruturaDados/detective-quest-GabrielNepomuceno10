#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------ ESTRUTURAS ------------------

// Estrutura da sala (árvore binária fixa)
struct Sala {
    char nome[50];
    char pista[50];
    struct Sala *esquerda;
    struct Sala *direita;
};

// Estrutura da árvore de busca (para armazenar as pistas)
struct NoPista {
    char pista[50];
    struct NoPista *esquerda;
    struct NoPista *direita;
};

// ------------------ FUNÇÕES DE SALAS ------------------

// Cria uma nova sala
struct Sala* criarSala(const char *nome, const char *pista) {
    struct Sala *nova = (struct Sala*)malloc(sizeof(struct Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ------------------ FUNÇÕES DA ÁRVORE DE PISTAS ------------------

// Insere uma nova pista na árvore de busca
struct NoPista* inserirPista(struct NoPista *raiz, const char *pista) {
    if (raiz == NULL) {
        struct NoPista *novo = (struct NoPista*)malloc(sizeof(struct NoPista));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    
    return raiz;
}

// Busca uma pista específica
int buscarPista(struct NoPista *raiz, const char *pista) {
    if (raiz == NULL)
        return 0;

    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0)
        return 1;
    else if (cmp < 0)
        return buscarPista(raiz->esquerda, pista);
    else
        return buscarPista(raiz->direita, pista);
}

// Exibe todas as pistas em ordem alfabética
void exibirPistasEmOrdem(struct NoPista *raiz) {
    if (raiz != NULL) {
        exibirPistasEmOrdem(raiz->esquerda);
        printf(" - %s\n", raiz->pista);
        exibirPistasEmOrdem(raiz->direita);
    }
}

// Libera memória da árvore de pistas
void liberarArvorePistas(struct NoPista *raiz) {
    if (raiz == NULL) return;
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

// ------------------ EXPLORAÇÃO DA MANSÃO ------------------

void explorarSalas(struct Sala *atual, struct NoPista **raizPistas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Se a sala tem pista, coleta automaticamente
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            if (!buscarPista(*raizPistas, atual->pista)) {
                *raizPistas = inserirPista(*raizPistas, atual->pista);
                printf("Pista adicionada à coleção!\n");
            } else {
                printf("Você já possui essa pista.\n");
            }
        }

        // Se não há saídas (nó-folha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Você chegou ao final da mansão.\n");
            return;
        }

        // Escolhas do jogador
        printf("\nEscolha seu caminho:\n");
        if (atual->esquerda != NULL)
            printf(" (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf(" (d) Ir para %s\n", atual->direita->nome);
        printf(" (p) Ver pistas coletadas\n");
        printf(" (s) Sair da exploração\n");
        printf("Digite sua opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL)
            atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita != NULL)
            atual = atual->direita;
        else if (escolha == 'p') {
            printf("\n--- Pistas Coletadas ---\n");
            if (*raizPistas == NULL)
                printf("Nenhuma pista encontrada ainda.\n");
            else
                exibirPistasEmOrdem(*raizPistas);
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

// ------------------ FUNÇÃO PRINCIPAL ------------------

int main() {
    // Cria as salas (estrutura fixa)
    struct Sala *hall = criarSala("Hall de Entrada", "Chave Dourada");
    struct Sala *biblioteca = criarSala("Biblioteca", "Página Rasgada");
    struct Sala *cozinha = criarSala("Cozinha", "");
    struct Sala *adega = criarSala("Adega", "Cálice Antigo");
    struct Sala *jardim = criarSala("Jardim", "Rosa Azul");
    struct Sala *salaJantar = criarSala("Sala de Jantar", "");

    // Conexões (mapa fixo)
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = salaJantar;
    biblioteca->direita = jardim;
    cozinha->esquerda = adega;
    cozinha->direita = NULL;

    // Árvore de pistas começa vazia
    struct NoPista *raizPistas = NULL;

    printf("Bem-vindo à Mansão das Pistas!\n");
    explorarSalas(hall, &raizPistas);

    printf("\nExploração encerrada.\nPistas encontradas:\n");
    if (raizPistas == NULL)
        printf("Nenhuma pista foi descoberta.\n");
    else
        exibirPistasEmOrdem(raizPistas);

    liberarArvorePistas(raizPistas);
    free(hall);
    printf("\nFim do jogo.\n");

    return 0;
}
