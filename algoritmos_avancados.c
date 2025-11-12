#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// ------------------ ESTRUTURAS ------------------

// Estrutura da sala (mapa fixo)
struct Sala {
    char nome[50];
    char pista[50];
    struct Sala *esquerda;
    struct Sala *direita;
};

// Nó da árvore de pistas (BST)
struct NoPista {
    char pista[50];
    struct NoPista *esquerda;
    struct NoPista *direita;
};

// Nó da lista encadeada para tabela hash
struct NoHash {
    char pista[50];
    char suspeito[50];
    struct NoHash *prox;
};

// ------------------ FUNÇÕES DE SALAS ------------------

struct Sala* criarSala(const char *nome, const char *pista) {
    struct Sala *nova = (struct Sala*)malloc(sizeof(struct Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ------------------ FUNÇÕES DA ÁRVORE DE PISTAS ------------------

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

int buscarPista(struct NoPista *raiz, const char *pista) {
    if (raiz == NULL) return 0;
    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0) return 1;
    else if (cmp < 0) return buscarPista(raiz->esquerda, pista);
    else return buscarPista(raiz->direita, pista);
}

void exibirPistasEmOrdem(struct NoPista *raiz) {
    if (raiz != NULL) {
        exibirPistasEmOrdem(raiz->esquerda);
        printf(" - %s\n", raiz->pista);
        exibirPistasEmOrdem(raiz->direita);
    }
}

void liberarArvorePistas(struct NoPista *raiz) {
    if (raiz == NULL) return;
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

// ------------------ FUNÇÕES DE HASH ------------------

// Função hash simples baseada na soma ASCII
int funcaoHash(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

// Insere pista → suspeito na tabela hash
void inserirNaHash(struct NoHash *tabela[], const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);
    struct NoHash *novo = (struct NoHash*)malloc(sizeof(struct NoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

// Exibe todas as associações pista → suspeito
void exibirHash(struct NoHash *tabela[]) {
    printf("\n--- Relações Pista → Suspeito ---\n");
    for (int i = 0; i < TAM_HASH; i++) {
        struct NoHash *atual = tabela[i];
        while (atual != NULL) {
            printf("%s → %s\n", atual->pista, atual->suspeito);
            atual = atual->prox;
        }
    }
}

// Conta quantas vezes cada suspeito aparece
void contarSuspeitos(struct NoHash *tabela[]) {
    struct {
        char nome[50];
        int contagem;
    } suspeitos[20];
    int total = 0;

    for (int i = 0; i < TAM_HASH; i++) {
        struct NoHash *atual = tabela[i];
        while (atual != NULL) {
            int encontrado = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(suspeitos[j].nome, atual->suspeito) == 0) {
                    suspeitos[j].contagem++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(suspeitos[total].nome, atual->suspeito);
                suspeitos[total].contagem = 1;
                total++;
            }
            atual = atual->prox;
        }
    }

    if (total == 0) {
        printf("\nNenhum suspeito registrado.\n");
        return;
    }

    int max = 0;
    char principal[50];
    for (int i = 0; i < total; i++) {
        printf("%s: %d pista(s)\n", suspeitos[i].nome, suspeitos[i].contagem);
        if (suspeitos[i].contagem > max) {
            max = suspeitos[i].contagem;
            strcpy(principal, suspeitos[i].nome);
        }
    }

    printf("\n>>> Principal suspeito: %s <<<\n", principal);
}

// Libera memória da tabela hash
void liberarHash(struct NoHash *tabela[]) {
    for (int i = 0; i < TAM_HASH; i++) {
        struct NoHash *atual = tabela[i];
        while (atual != NULL) {
            struct NoHash *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
}

// ------------------ EXPLORAÇÃO DA MANSÃO ------------------

void explorarSalas(struct Sala *atual, struct NoPista **raizPistas, struct NoHash *tabela[]) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Se houver pista, coleta e associa a um suspeito
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            if (!buscarPista(*raizPistas, atual->pista)) {
                *raizPistas = inserirPista(*raizPistas, atual->pista);

                // Define suspeito para cada pista
                const char *suspeito;
                if (strcmp(atual->pista, "Chave Dourada") == 0)
                    suspeito = "Sr. Black";
                else if (strcmp(atual->pista, "Página Rasgada") == 0)
                    suspeito = "Sra. White";
                else if (strcmp(atual->pista, "Cálice Antigo") == 0)
                    suspeito = "Srta. Rose";
                else if (strcmp(atual->pista, "Rosa Azul") == 0)
                    suspeito = "Sr. Green";
                else
                    suspeito = "Desconhecido";

                inserirNaHash(tabela, atual->pista, suspeito);
                printf("Pista associada ao suspeito: %s\n", suspeito);
            } else {
                printf("Você já possui essa pista.\n");
            }
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Você chegou ao final da mansão.\n");
            return;
        }

        printf("\nEscolha seu caminho:\n");
        if (atual->esquerda != NULL)
            printf(" (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf(" (d) Ir para %s\n", atual->direita->nome);
        printf(" (p) Ver pistas\n");
        printf(" (h) Ver suspeitos\n");
        printf(" (s) Sair\n");
        printf("Digite sua opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL)
            atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita != NULL)
            atual = atual->direita;
        else if (escolha == 'p') {
            printf("\n--- Pistas Coletadas ---\n");
            if (*raizPistas == NULL)
                printf("Nenhuma pista ainda.\n");
            else
                exibirPistasEmOrdem(*raizPistas);
        }
        else if (escolha == 'h') {
            exibirHash(tabela);
        }
        else if (escolha == 's') {
            printf("Você decidiu sair da mansão.\n");
            return;
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}

// ------------------ FUNÇÃO PRINCIPAL ------------------

int main() {
    // Criação das salas
    struct Sala *hall = criarSala("Hall de Entrada", "Chave Dourada");
    struct Sala *biblioteca = criarSala("Biblioteca", "Página Rasgada");
    struct Sala *cozinha = criarSala("Cozinha", "");
    struct Sala *adega = criarSala("Adega", "Cálice Antigo");
    struct Sala *jardim = criarSala("Jardim", "Rosa Azul");
    struct Sala *salaJantar = criarSala("Sala de Jantar", "");

    // Conexões do mapa
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = salaJantar;
    biblioteca->direita = jardim;
    cozinha->esquerda = adega;

    struct NoPista *raizPistas = NULL;
    struct NoHash *tabela[TAM_HASH] = {NULL};

    printf("Bem-vindo ao Nível Mestre da Mansão das Pistas!\n");
    explorarSalas(hall, &raizPistas, tabela);

    printf("\n--- Relatório Final ---\n");
    printf("\nPistas encontradas:\n");
    if (raizPistas == NULL)
        printf("Nenhuma pista.\n");
    else
        exibirPistasEmOrdem(raizPistas);

    printf("\nAssociações de suspeitos:\n");
    exibirHash(tabela);

    printf("\nAnálise final dos suspeitos:\n");
    contarSuspeitos(tabela);

    liberarArvorePistas(raizPistas);
    liberarHash(tabela);
    printf("\nFim da investigação.\n");
    return 0;
}
