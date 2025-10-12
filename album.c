#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tipo.h"
#include "album.h"

Album *criarNo(infoAlbum album)
{
    Album *novoNo = (Album*) malloc(sizeof(Album));
    if (novoNo != NULL)
    {
        novoNo->info = album;
        novoNo->esq = NULL;
        novoNo->dir = NULL;
        novoNo->cor = VERMELHO;
    }
    return novoNo;
}

infoAlbum preencherAlbum()
{
    infoAlbum dados;
    printf("Digite o titulo do album: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]", dados.titulo);
    printf("Digite o ano de lancamento do album: ");
    scanf("%d", dados.anoLancamento);
    printf("Digite a quantidade de musica do album: ");
    scanf("%d", dados.quantMusica);
    dados.musica = NULL;
    return dados;
}

void liberarArvore(Album *raiz)
{
    if (raiz != NULL)
    {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

//Busca o no pelo nome do titulo
Album *BuscarNo(Album *raiz, int valor)
{
    Album *no;

    if (raiz->info.titulo == valor)
    {
        no = raiz;
    }
    else if(raiz->info.titulo > valor)
    {
        no = BuscarNo(raiz->esq, valor);
    }
    else if(raiz->info.titulo < valor)
    {
        no = BuscarNo(raiz->dir, valor);
    }
    else
        no = NULL;

    return no;
}

// Essa funcao inicia faz com que os nos folhas seja pretos
int cor(Album *raiz)
{
    int corNo = PRETO;
    if (raiz)
    {
        corNo = (*raiz).cor;
    }
    
    return corNo;
}

void rotacionaEsq(Album **raiz)
{
    Album *aux = (**raiz).dir;

    (**raiz).dir = (*aux).esq;
    (*aux).esq = *raiz;
    (*aux).cor = (**raiz).cor;
    (**raiz).cor = VERMELHO;
    (*raiz) = aux;
}

void rotacionaDir(Album **raiz)
{
    Album *aux = (**raiz).esq;

    (**raiz).esq = (*aux).dir;
    (*aux).dir = *raiz;
    (*aux).cor = (**raiz).cor;
    (**raiz).cor = VERMELHO;
    (*raiz) = aux;
}

void trocaCor(Album **raiz) {
    (**raiz).cor = !((**raiz).cor);
    (**raiz).esq->cor = !((**raiz).esq->cor);
    (**raiz).dir->cor = !((**raiz).dir->cor);
}

void balanceamento(Album **raiz) 
{
    if (cor((**raiz).esq) == PRETO && cor((**raiz).dir) == VERMELHO)
    {
        rotacionaEsq(raiz);
    } 

    if (cor((**raiz).esq) == VERMELHO) if (cor((**raiz).esq->esq) == VERMELHO)
    {
        rotacionaDir(raiz);
    } 

    if (cor((**raiz).esq) == VERMELHO && cor((**raiz).dir) == VERMELHO)
    {
        trocaCor(raiz); 
    } 
}
int insereNo(Album **raiz, Album *novoNo)
{
    int inseriu = 1;

    if (*raiz == NULL)
    {
        *raiz = novoNo;
    }
    else if ((**raiz).info > (*novoNo).info)
    {
        inseriu = insereNo(&((**raiz).esq), novoNo);
    }
    else if ((**raiz).info < (*novoNo).info)
    {
        inseriu = insereNo(&((**raiz).dir), novoNo);
    }
    else
        inseriu = 0;

    if (*raiz && inseriu)
        balanceamento(raiz);
    
    return inseriu;
}

int  insercao(Album **raiz, Album *novoNo) {
    int inseriu = insereNo(raiz, novoNo);

    if (inseriu) (**raiz).cor = PRETO;

    return inseriu;
}

Album *move2EsqRed(Album *raiz)
{
    trocaCor(&raiz);
    if (cor((*raiz).dir->esq) == VERMELHO)
    {
        rotacionaDir(&((*raiz).dir));
        rotacionaEsq(&raiz);
        trocaCor(&raiz);
    }
    return raiz;
}

Album *move2DirRed(Album *raiz)
{
    trocaCor(&raiz);
    if (cor((*raiz).esq->esq) == VERMELHO)
    {
        rotacionaDir(&raiz);
        trocaCor(&raiz);
    }
    return raiz;
}



