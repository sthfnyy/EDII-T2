#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_BLOCO (1024ULL * 1024ULL) // 1 MiB
#define MAX_CHAVES   4                    // até 4 chaves por nó (árvore 4-5)
#define MAX_FILHOS   5                    // até 5 filhos por nó

/* ======================= ESTRUTURAS ======================= */

typedef struct {
    char situacao;                         // 'L' = Livre, 'O' = Ocupado
    unsigned long long blocoInicial;       // chave usada na árvore
    unsigned long long blocoFinal;
    unsigned long long enderecoInicial;
    unsigned long long enderecoFinal;
} NoMemoria;

typedef struct NoArvore45 {
    int quantidadeChaves;                                   // quantas chaves estão em uso
    unsigned long long chave[MAX_CHAVES];                   // aqui salvamos o blocoInicial
    NoMemoria *intervalo[MAX_CHAVES];                       // dados associados a cada chave
    struct NoArvore45 *filho[MAX_FILHOS];                   // ponteiros para filhos
    int ehFolha;                                            // 1 = folha, 0 = interno
} NoArvore45;

typedef struct {
    NoArvore45 *raiz;
} Arvore45;

/* ======================= FUNÇÕES AUXILIARES ======================= */

unsigned long long enderecoParaBloco(unsigned long long endereco) {
    return endereco / TAMANHO_BLOCO;
}

NoMemoria *criarNoMemoria(char situacao,
                          unsigned long long enderecoInicial,
                          unsigned long long enderecoFinal) {
    NoMemoria *novo = (NoMemoria*) malloc(sizeof(NoMemoria));
    if (!novo) { perror("malloc"); exit(1); }

    novo->situacao = situacao;
    novo->enderecoInicial = enderecoInicial;
    novo->enderecoFinal   = enderecoFinal;
    novo->blocoInicial = enderecoParaBloco(enderecoInicial);
    novo->blocoFinal   = enderecoParaBloco(enderecoFinal);
    return novo;
}

/* ======================= ÁRVORE 4-5 (B-tree ordem 5) ======================= */

NoArvore45 *criarNoArvore(int ehFolha) {
    NoArvore45 *no = (NoArvore45*) malloc(sizeof(NoArvore45));
    if (!no) { perror("malloc"); exit(1); }
    no->quantidadeChaves = 0;
    no->ehFolha = ehFolha;
    for (int i = 0; i < MAX_CHAVES; i++) {
        no->chave[i] = 0;
        no->intervalo[i] = NULL;
    }
    for (int i = 0; i < MAX_FILHOS; i++) {
        no->filho[i] = NULL;
    }
    return no;
}

void iniciarArvore(Arvore45 *arvore) {
    arvore->raiz = criarNoArvore(1); // começa com raiz folha
}

/*
 * dividirFilho:
 * - Divide o filho Y do nó X na posição i.
 * - Em ordem 5 (MAX_CHAVES=4), Y cheio tem 4 chaves [k0,k1,k2,k3].
 *   Promovemos k2 para X; Y fica com [k0,k1]; novo nó Z fica com [k3].
 * - Filhos de Y são repartidos entre Y e Z se Y não for folha.
 */
void dividirFilho(NoArvore45 *noPai, int indiceFilho) {
    NoArvore45 *filhoEsquerdo = noPai->filho[indiceFilho];
    NoArvore45 *filhoDireito = criarNoArvore(filhoEsquerdo->ehFolha);

    // filhoDireito herda a última chave do filhoEsquerdo (k3)
    filhoDireito->quantidadeChaves = 1;
    filhoDireito->chave[0] = filhoEsquerdo->chave[3];
    filhoDireito->intervalo[0]  = filhoEsquerdo->intervalo[3];

    // Se não for folha, filhoDireito herda os dois últimos filhos do filhoEsquerdo
    if (!filhoEsquerdo->ehFolha) {
        filhoDireito->filho[0] = filhoEsquerdo->filho[3];
        filhoDireito->filho[1] = filhoEsquerdo->filho[4];
    }

    // filhoEsquerdo fica com as duas primeiras chaves (k0, k1)
    filhoEsquerdo->quantidadeChaves = 2;

    // Abre espaço no pai para inserir filhoDireito e a chave promovida
    for (int j = noPai->quantidadeChaves; j >= indiceFilho + 1; j--) {
        noPai->filho[j + 1] = noPai->filho[j];
    }
    noPai->filho[indiceFilho + 1] = filhoDireito;

    for (int j = noPai->quantidadeChaves - 1; j >= indiceFilho; j--) {
        noPai->chave[j + 1] = noPai->chave[j];
        noPai->intervalo[j + 1] = noPai->intervalo[j];
    }

    // Promove k2 do filhoEsquerdo para o pai
    noPai->chave[indiceFilho] = filhoEsquerdo->chave[2];
    noPai->intervalo[indiceFilho] = filhoEsquerdo->intervalo[2];
    noPai->quantidadeChaves += 1;

    // Limpa ponteiros não usados (opcional)
    filhoEsquerdo->chave[2] = filhoEsquerdo->chave[3] = 0;
    filhoEsquerdo->intervalo[2] = filhoEsquerdo->intervalo[3] = NULL;
    if (!filhoEsquerdo->ehFolha) {
        filhoEsquerdo->filho[3] = filhoEsquerdo->filho[4] = NULL;
    }
}

/*
 * inserirNaoCheio:
 * - Insere (chave k, valor val) em um nó que sabemos não estar cheio.
 * - Se for folha: insere deslocando.
 * - Se for interno: desce para o filho apropriado; se o filho estiver cheio, divide antes.
 */
void inserirNaoCheio(NoArvore45 *noAtual, unsigned long long chave, NoMemoria *valor) {
    int i = noAtual->quantidadeChaves - 1;

    if (noAtual->ehFolha) {
        // desloca para abrir espaço na posição correta
        while (i >= 0 && chave < noAtual->chave[i]) {
            noAtual->chave[i + 1] = noAtual->chave[i];
            noAtual->intervalo[i + 1] = noAtual->intervalo[i];
            i--;
        }
        noAtual->chave[i + 1] = chave;
        noAtual->intervalo[i + 1] = valor;
        noAtual->quantidadeChaves += 1;
    } else {
        // encontra o filho onde deve inserir
        while (i >= 0 && chave < noAtual->chave[i]) i--;
        i++; // filho i

        // se o filho está cheio, divide antes de descer
        if (noAtual->filho[i]->quantidadeChaves == MAX_CHAVES) {
            dividirFilho(noAtual, i);
            // após dividir, decidir para qual lado descer
            if (chave > noAtual->chave[i]) i++;
        }
        inserirNaoCheio(noAtual->filho[i], chave, valor);
    }
}

/*
 * inserirArvore:
 * - Se a raiz está cheia, criamos um novo nó S como raiz, dividimos a raiz antiga
 *   e então inserimos no filho apropriado. Caso contrário, inserimos direto na raiz.
 */
void inserirArvore(Arvore45 *arvore, unsigned long long chave, NoMemoria *valor) {
    NoArvore45 *raiz = arvore->raiz;
    if (raiz->quantidadeChaves == MAX_CHAVES) {
        NoArvore45 *novaRaiz = criarNoArvore(0); // novo nó raiz (interno)
        arvore->raiz = novaRaiz;
        novaRaiz->filho[0] = raiz;
        dividirFilho(novaRaiz, 0);
        int i = (chave > novaRaiz->chave[0]) ? 1 : 0;
        inserirNaoCheio(novaRaiz->filho[i], chave, valor);
    } else {
        inserirNaoCheio(raiz, chave, valor);
    }
}

/* Impressão em-ordem (ordenada por blocoInicial) */
void imprimirEmOrdem(NoArvore45 *no) {
    if (!no) return;
    for (int i = 0; i < no->quantidadeChaves; i++) {
        if (!no->ehFolha) imprimirEmOrdem(no->filho[i]);
        NoMemoria *info = no->intervalo[i];
        printf("[ %c | Blocos %llu..%llu | Enderecos %llu..%llu ]\n",
               info->situacao,
               info->blocoInicial, info->blocoFinal,
               info->enderecoInicial, info->enderecoFinal);
    }
    if (!no->ehFolha) imprimirEmOrdem(no->filho[no->quantidadeChaves]);
}

/* ======================= PROGRAMA PRINCIPAL ======================= */

int main(void) {
    Arvore45 arvore;
    iniciarArvore(&arvore);

    unsigned long long ultimoEnderecoMemoria;
    printf("Informe o ULTIMO endereco da memoria (em bytes): ");
    if (scanf("%llu", &ultimoEnderecoMemoria) != 1) {
        printf("Entrada invalida.\n");
        return 1;
    }

    // ===== Primeiro nó =====
    char situacaoInicial;
    unsigned long long enderecoInicial, enderecoFinal;

    printf("\nPrimeiro no:\n");
    printf("Situacao (L=Livre, O=Ocupado): ");
    scanf(" %c", &situacaoInicial);
    if (situacaoInicial == 'l') situacaoInicial = 'L';
    if (situacaoInicial == 'o') situacaoInicial = 'O';

    printf("Endereco INICIAL (bytes): ");
    if (scanf("%llu", &enderecoInicial) != 1) { printf("Entrada invalida.\n"); return 1; }

    printf("Endereco FINAL (bytes): ");
    if (scanf("%llu", &enderecoFinal) != 1) { printf("Entrada invalida.\n"); return 1; }

    if (enderecoFinal < enderecoInicial) {
        printf("Erro: final menor que inicial.\n");
        return 1;
    }

    NoMemoria *primeiro = criarNoMemoria(situacaoInicial, enderecoInicial, enderecoFinal);
    inserirArvore(&arvore, primeiro->blocoInicial, primeiro);

    unsigned long long fimAnterior = enderecoFinal;
    char situacaoAtual = situacaoInicial;

    // ===== Demais nós =====
    while (fimAnterior != ultimoEnderecoMemoria) {
        situacaoAtual = (situacaoAtual == 'L') ? 'O' : 'L';
        unsigned long long novoInicio = fimAnterior + 1;

        printf("\nProximo no:\n");
        printf("Situacao: %c (automatico)\n", situacaoAtual);
        printf("Endereco INICIAL: %llu (automatico)\n", novoInicio);
        printf("Informe apenas o ENDERECO FINAL (bytes): ");

        unsigned long long novoFim;
        if (scanf("%llu", &novoFim) != 1) { printf("Entrada invalida.\n"); return 1; }
        if (novoFim < novoInicio) { printf("Erro: final menor que inicial.\n"); return 1; }

        NoMemoria *novo = criarNoMemoria(situacaoAtual, novoInicio, novoFim);
        inserirArvore(&arvore, novo->blocoInicial, novo);

        fimAnterior = novoFim;
    }

    // ===== Saída =====
    printf("\n=== ARVORE 4-5 (ordenada por bloco inicial) ===\n");
    imprimirEmOrdem(arvore.raiz);

    printf("\nObservacoes:\n");
    printf("- Cada bloco tem 1 MiB. bloco = endereco / 1MiB.\n");
    printf("- A chave na arvore e o blocoInicial de cada intervalo.\n");
    printf("- Implementamos apenas INSERCAO (suficiente para o cadastro do enunciado).\n");

    return 0;
}
