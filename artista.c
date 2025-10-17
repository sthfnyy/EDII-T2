#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"


Artista *alocaArtista(infoArtista dados)
{
    Artista *novoNo = (Artista *) malloc(sizeof(Artista));
    if (novoNo != NULL)
    {
        novoNo->esq = NULL;         // sem filho à esquerda
        novoNo->dir = NULL;         // sem filho à direita
        novoNo->pai = NULL;         // sem pai
        novoNo->cor = VERMELHO;     // novo nó entra vermelho
        novoNo->info = dados;       // copia dados
    }
    return novoNo;
}

infoArtista preencherArtista(void)
{
    infoArtista dados;
    printf("Digite o nome do artista: ");   scanf("%49s", dados.nome);
    printf("Digite o estilo do artista: "); scanf("%49s", dados.estilo);
    printf("Digite o tipo: ");              scanf("%49s", dados.tipo);
    dados.numAlbuns = 0;
    return dados;
}

int cor (Artista *raiz)
{
    int corNo = PRETO;
    if (raiz)
    {
        corNo = (raiz)->cor;
    }
    return corNo;
}

void rotacionaEsq(Artista *raiz)
{
    Artista *aux = (raiz)->dir;

    raiz->dir = aux->esq;
    aux->esq = raiz;
    aux->cor = raiz->cor;
    raiz->cor = VERMELHO;
    raiz = aux;
}

void rotacionaDir(Artista *raiz){
    Artista *aux = (raiz)->esq;

    raiz->esq = aux->dir;
    aux->dir = raiz;
    aux->cor = raiz->cor;
    raiz->cor = VERMELHO;
    raiz = aux;
}

void trocaCor(Artista *raiz) {
    raiz->cor = !(raiz->cor); // troca a cor 
    raiz->esq->cor = !(raiz->esq->cor);
    raiz->dir->cor = !(raiz->dir->cor);
}

void balanceamento(Artista *raiz) 
{
    if (cor(raiz->esq) == PRETO && cor(raiz->dir) == VERMELHO)
    {
        rotacionaEsq(raiz);
    } 

    if ((cor(raiz->esq) == VERMELHO) && (cor(raiz->esq->esq) == VERMELHO))
    {
        rotacionaDir(raiz);
    } 

    if (cor(raiz->esq) == VERMELHO && cor(raiz->dir) == VERMELHO)
    {
        trocaCor(raiz); 
    } 
}


int inserirArtista (Artista **raiz, Artista *novoNo)
{
    int inseriu = 1;

    int cmp = strcmp(novoNo->info.nome, (*raiz)->info.nome);

    if (*raiz == NULL)
    {
        *raiz = novoNo;
    }
    else if (cmp < 0)
    {
        inseriu = insereNo(&((*raiz)->esq), novoNo);
    }
    else if (cmp > 0)
    {
        inseriu = insereNo(&((*raiz)->dir), novoNo);
    }
    else
    {
        inseriu = 0; // nome duplicado
    }

    if (*raiz && inseriu)
        balanceamento(raiz);
    
    return inseriu;
}

int  insercao(Artista **raiz, Artista *novoNo) 
{
    int inseriu = insereNo(raiz, novoNo);

    if (inseriu) (*raiz)->cor = PRETO;

    return inseriu;
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

void liberarArvore(Artista *raiz)
{
    if (raiz != NULL)
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
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
