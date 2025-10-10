#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"

/* ======== Funções auxiliares internas (RB) ======== */

// Rotação à esquerda tendo 'noPivo' como pivô
static void rotacaoEsq(Artista **raiz, Artista *noPivo)
{
    Artista *filhoDireito = noPivo->dir;

    noPivo->dir = filhoDireito->esq;
    if (filhoDireito->esq)
        filhoDireito->esq->pai = noPivo;

    filhoDireito->pai = noPivo->pai;
    if (!noPivo->pai)
        *raiz = filhoDireito;
    else if (noPivo == noPivo->pai->esq)
        noPivo->pai->esq = filhoDireito;
    else
        noPivo->pai->dir = filhoDireito;

    filhoDireito->esq = noPivo;
    noPivo->pai = filhoDireito;
}

// Rotação à direita tendo 'noPivo' como pivô
static void rotacaoDir(Artista **raiz, Artista *noPivo)
{
    Artista *filhoEsquerdo = noPivo->esq;

    noPivo->esq = filhoEsquerdo->dir;
    if (filhoEsquerdo->dir)
        filhoEsquerdo->dir->pai = noPivo;

    filhoEsquerdo->pai = noPivo->pai;
    if (!noPivo->pai)
        *raiz = filhoEsquerdo;
    else if (noPivo == noPivo->pai->esq)
        noPivo->pai->esq = filhoEsquerdo;
    else
        noPivo->pai->dir = filhoEsquerdo;

    filhoEsquerdo->dir = noPivo;
    noPivo->pai = filhoEsquerdo;
}

// Corrige propriedades RB após inserir 'noInserido' (que entrou VERMELHO)
static void corrigirInsercao(Artista **raiz, Artista *noInserido)
{
    while (noInserido->pai && noInserido->pai->cor == VERMELHO)
    {
        Artista *pai = noInserido->pai;
        Artista *avo = pai->pai;

        if (pai == avo->esq)
        {
            Artista *tio = avo->dir;

            if (tio && tio->cor == VERMELHO)
            {
                // Caso 1: tio vermelho -> recoloração
                pai->cor = PRETO;
                tio->cor = PRETO;
                avo->cor = VERMELHO;
                noInserido = avo;
            }
            else
            {
                if (noInserido == pai->dir)
                {
                    // Caso 2: triângulo (esq-dir) -> rotaciona para alinhar
                    noInserido = pai;
                    rotacaoEsq(raiz, noInserido);
                    pai = noInserido->pai;
                    avo = pai->pai;
                }
                // Caso 3: linha (esq-esq) -> rotação direita no avô
                pai->cor = PRETO;
                avo->cor = VERMELHO;
                rotacaoDir(raiz, avo);
            }
        }
        else
        {
            // Lado simétrico (pai é filho direito do avô)
            Artista *tio = avo->esq;

            if (tio && tio->cor == VERMELHO)
            {
                pai->cor = PRETO;
                tio->cor = PRETO;
                avo->cor = VERMELHO;
                noInserido = avo;
            }
            else
            {
                if (noInserido == pai->esq)
                {
                    noInserido = pai;
                    rotacaoDir(raiz, noInserido);
                    pai = noInserido->pai;
                    avo = pai->pai;
                }
                pai->cor = PRETO;
                avo->cor = VERMELHO;
                rotacaoEsq(raiz, avo);
            }
        }
    }
    if (*raiz) (*raiz)->cor = PRETO; // raiz sempre PRETA
}

/* ======== Implementações públicas ======== */

// Aloca e inicializa nó
Artista *alocaArtista(infoArtista dados)
{
    Artista *novoNo = (Artista *) malloc(sizeof(Artista));
    if (!novoNo)
    {
        printf("Erro de acesso a memoria.\n");
        exit(1);
    }
    novoNo->esq = NULL;         // sem filho à esquerda
    novoNo->dir = NULL;         // sem filho à direita
    novoNo->pai = NULL;         // sem pai
    novoNo->cor = VERMELHO;     // novo nó entra vermelho
    novoNo->info = dados;       // copia dados
    return novoNo;
}

// Entrada de dados via stdin
infoArtista preencherDadosArtista(void)
{
    infoArtista dados;
    printf("Digite o nome do artista: ");   scanf("%63s", dados.nome);
    printf("Digite o estilo do artista: "); scanf("%31s", dados.estilo);
    printf("Digite o tipo: ");              scanf("%31s", dados.tipo);
    dados.numAlbuns = 0;
    return dados;
}

// Insere como BST e corrige para RB
int inserirArtista(Artista **raiz, Artista *noParaInserir)
{
    // (1) Inserção BST iterativa para setar 'pai'
    Artista *noAtual = *raiz;
    Artista *possivelPai = NULL;

    while (noAtual != NULL)
    {
        int comparacao = strcmp(noParaInserir->info.nome, noAtual->info.nome);
        possivelPai = noAtual;

        if (comparacao == 0)
        {
            // Nome duplicado: não insere
            return 0;
        }
        else if (comparacao < 0)
        {
            noAtual = noAtual->esq;
        }
        else
        {
            noAtual = noAtual->dir;
        }
    }

    // Encaixa o nó no local correto
    noParaInserir->pai = possivelPai;
    if (possivelPai == NULL)
    {
        *raiz = noParaInserir; // árvore estava vazia
    }
    else if (strcmp(noParaInserir->info.nome, possivelPai->info.nome) < 0)
    {
        possivelPai->esq = noParaInserir;
    }
    else
    {
        possivelPai->dir = noParaInserir;
    }

    // (2) Corrige propriedades de RB
    corrigirInsercao(raiz, noParaInserir);
    return 1;
}

// Busca por nome (iterativa)
Artista *buscarArtista(Artista *raiz, const char *nome)
{
    Artista *noAtual = raiz;

    while (noAtual != NULL)
    {
        int comparacao = strcmp(nome, noAtual->info.nome);
        if (comparacao == 0)
            return noAtual;
        else if (comparacao < 0)
            noAtual = noAtual->esq;
        else
            noAtual = noAtual->dir;
    }
    return NULL;
}

// Impressão in-order
void mostrarArtistas(Artista *raiz)
{
    if (!raiz) return;

    mostrarArtistas(raiz->esq);
    printf("[%s] Nome: %s, Estilo: %s, Tipo: %s, Albuns Lancados: %d\n",
           (raiz->cor == PRETO ? "P" : "V"),
           raiz->info.nome, raiz->info.estilo, raiz->info.tipo, raiz->info.numAlbuns);
    mostrarArtistas(raiz->dir);
}

// Libera toda a árvore
void destruirArvore(Artista *raiz)
{
    if (!raiz) return;
    destruirArvore(raiz->esq);
    destruirArvore(raiz->dir);
    free(raiz);
}

/* ======== Exemplo rápido ======== */
/*
int main(void)
{
    Artista *raiz = NULL;

    infoArtista a = {"Chico", "MPB",  "Solo", 5};
    infoArtista b = {"Ana",   "Pop",  "Banda",2};
    infoArtista c = {"Zeca",  "Samba","Solo", 7};
    infoArtista d = {"Beto",  "Rock", "Solo", 1};

    inserirArtista(&raiz, alocaArtista(a));
    inserirArtista(&raiz, alocaArtista(b));
    inserirArtista(&raiz, alocaArtista(c));
    inserirArtista(&raiz, alocaArtista(d));

    mostrarArtistas(raiz);

    Artista *encontrado = buscarArtista(raiz, "Ana");
    if (encontrado) printf("Achei %s\n", encontrado->info.nome);

    destruirArvore(raiz);
    return 0;
}
*/
