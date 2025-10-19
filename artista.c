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

void rotacionaEsq(Artista **raiz)
{
    Artista *aux = (*raiz)->dir;

    (*raiz)->dir = aux->esq;
    aux->esq = (*raiz);
    aux->cor = (*raiz)->cor;
    (*raiz)->cor = VERMELHO;
    (*raiz) = aux;
}

void rotacionaDir(Artista **raiz){
    Artista *aux = (*raiz)->esq;

    (*raiz)->esq = aux->dir;
    aux->dir = (*raiz);
    aux->cor = (*raiz)->cor;
    (*raiz)->cor = VERMELHO;
    (*raiz) = aux;
}

void trocaCor(Artista *raiz) {
    raiz->cor = !(raiz->cor); // troca a cor 
    if(raiz->esq != NULL)
        raiz->esq->cor = !(raiz->esq->cor);
    if(raiz->dir != NULL)
        raiz->dir->cor = !(raiz->dir->cor);
}

void balanceamento(Artista **raiz) 
{
    if (cor((*raiz)->esq) == PRETO && cor((*raiz)->dir) == VERMELHO)
    {
        rotacionaEsq(raiz);
    } 

    if ((cor((*raiz)->esq) == VERMELHO) && (cor((*raiz)->esq->esq) == VERMELHO))
    {
        rotacionaDir(raiz);
    } 

    if (cor((*raiz)->esq) == VERMELHO && cor((*raiz)->dir) == VERMELHO)
    {
        trocaCor(*raiz); 
    } 
}

int  insercao(Artista **raiz, Artista *novoNo) 
{
    int inseriu = inserirArtista(raiz, novoNo);

    if (inseriu && *raiz) 
        (*raiz)->cor = PRETO; //raiz sempre preta

    return inseriu;
}

int inserirArtista (Artista **raiz, Artista *novoNo)
{
    int inseriu = 1;

    if (*raiz == NULL)
    {
        *raiz = novoNo;
    }else
    {
        int cmp = strcmp(novoNo->info.nome, (*raiz)->info.nome);

        if (cmp < 0)
        {
            inseriu = inserirArtista(&((*raiz)->esq), novoNo);
        }
        else if (cmp > 0)
        {
            inseriu = inserirArtista(&((*raiz)->dir), novoNo);
        }
        else
        {
            inseriu = 0; // nome duplicado
        }

        if (*raiz && inseriu)
        {
            balanceamento(raiz);
        }   
    }    
    return inseriu;
}



// Busca por nome (iterativa)
Artista *buscarArtista(Artista *raiz, const char *nome)
{
    while (raiz != NULL && strcmp(nome, raiz->info.nome) != 0)
    {
        if (strcmp(nome, raiz->info.nome) < 0)
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
    }
    return raiz; // se não achar, raiz será NULL
}



// Impressão in-order
void mostrarArtistas(Artista *raiz)
{
    if (raiz != NULL)
    {
        mostrarArtistas(raiz->esq);

        char corChar;
        if (raiz->cor == PRETO)
            corChar = 'P';
        else
            corChar = 'V';

        printf("[%c] Nome: %s, Estilo: %s, Tipo: %s, Albuns Lancados: %d\n",
               corChar,
               raiz->info.nome,
               raiz->info.estilo,
               raiz->info.tipo,
               raiz->info.numAlbuns);

        mostrarArtistas(raiz->dir);
    }
}

// Impressão pre-order
void mostrarArtistasPreOrdem(Artista *raiz)
{
    
    if (raiz != NULL)
    {
        // imprime primeiro o nó atual (raiz local)
        char corChar;
        if (raiz->cor == PRETO)
            corChar = 'P';
        else
            corChar = 'V';
        
        printf("[%c] Nome: %s, Estilo: %s, Tipo: %s, Albuns Lancados: %d\n",
               corChar,
               raiz->info.nome,
               raiz->info.estilo,
               raiz->info.tipo,
               raiz->info.numAlbuns);

        // depois percorre os filhos
        mostrarArtistasPreOrdem(raiz->esq);
        mostrarArtistasPreOrdem(raiz->dir);
    }
}



void liberarArvore(Artista *raiz)
{
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}


/* ======== Teste rápido ======== */

/*
int main(void)
{
    Artista *raiz = NULL;

    infoArtista a = {"Chico", "MPB",  "Solo", 5};
    infoArtista b = {"Ana",   "Pop",  "Banda",2};
    infoArtista c = {"Zeca",  "Samba","Solo", 7};
    infoArtista d = {"Beto",  "Rock", "Solo", 1};

    insercao(&raiz, alocaArtista(a));
    insercao(&raiz, alocaArtista(b));
    insercao(&raiz, alocaArtista(c));
    insercao(&raiz, alocaArtista(d));

    printf("Lista em pre-ordem:\n");
    mostrarArtistasPreOrdem(raiz);

    Artista *encontrado = buscarArtista(raiz, "Ana");
    if (encontrado) printf("Achei %s\n", encontrado->info.nome);

    liberarArvore(raiz);
    return 0;
}

*/