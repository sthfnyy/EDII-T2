#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

// Essa funcao inicia faz com que os nos folhas seja pretos
int cor(Album *raiz)
{
    int corNo = PRETO;
    if (raiz)
    {
        corNo = (raiz)->cor;
    }
    
    return corNo;
}

void rotacionaEsq(Album *raiz)
{
    Album *aux = (raiz)->dir;

    raiz->dir = aux->esq;
    aux->esq = raiz;
    aux->cor = raiz->cor;
    raiz->cor = VERMELHO;
    raiz = aux;
}

void rotacionaDir(Album *raiz)
{
    Album *aux = (raiz)->esq;

    raiz->esq = aux->dir;
    aux->dir = raiz;
    aux->cor = raiz->cor;
    raiz->cor = VERMELHO;
    raiz = aux;
}

void trocaCor(Album *raiz) {
    raiz->cor = !(raiz->cor); // troca a cor 
    raiz->esq->cor = !(raiz->esq->cor);
    raiz->dir->cor = !(raiz->dir->cor);
}

void balanceamento(Album **raiz) 
{
    if (cor((**raiz).esq) == PRETO && cor((**raiz).dir) == VERMELHO)
    {
        rotacionaEsq(raiz);
    } 

    if ((cor((**raiz).esq) == VERMELHO) && (cor((**raiz).esq->esq) == VERMELHO))
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

    int cmp = strcmp(novoNo->info.titulo, (*raiz)->info.titulo);

    if (*raiz == NULL)
    {
        *raiz = novoNo;
    }
    //tem que comparar os titulos e ver a ordem
    else if (cmp > 0)
    {
        inseriu = insereNo(&((**raiz).esq), novoNo);
    }
    else if (cmp < 0)
    {
        inseriu = insereNo(&((**raiz).dir), novoNo);
    }
    else
        inseriu = 0; //nome duplicado

    if (*raiz && inseriu)
        balanceamento(raiz);
    
    return inseriu;
}

int  insercao(Album **raiz, Album *novoNo) 
{
    int inseriu = insereNo(raiz, novoNo);

    if (inseriu) (*raiz)->cor = PRETO;

    return inseriu;
}


Album* BuscarNoPorTitulo(Album *raiz, const char* tituloBusca)
{
    // 1. Cria um ponteiro para o nó que será retornado. Inicia com NULL.
    Album *noEncontrado = NULL;

    // 2. Cria um ponteiro para percorrer a árvore, começando pela raiz.
    Album *noAtual = raiz;

    // 3. O laço continua enquanto não chegamos ao fim de um galho (noAtual != NULL)
    //    e enquanto ainda não encontramos o nó (noEncontrado == NULL).
    while (noAtual != NULL && noEncontrado == NULL)
    {
        // Compara o título buscado com o título do nó atual
        int comparacao = strcmp(tituloBusca, noAtual->info.titulo);

        if (comparacao == 0)
        {
            // Encontrou! Atribui o nó atual à variável de resultado.
            noEncontrado = noAtual;
        }
        else if (comparacao < 0)
        {
            // O título buscado vem antes, então desce para a esquerda.
            noAtual = noAtual->esq;
        }
        else // (comparacao > 0)
        {
            // O título buscado vem depois, então desce para a direita.
            noAtual = noAtual->dir;
        }
    }

    // 4. Retorna a variável de resultado.
    //    Se o nó foi encontrado, ela conterá o endereço dele.
    //    Se o laço terminou sem encontrar, ela ainda terá o valor NULL.
    return noEncontrado;
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

Album *removerMenor(Album *raiz)
{
    if ((*raiz).esq == NULL)
    {
        free(raiz);
        return NULL;
    }

    if (cor(raiz->esq) == PRETO && cor(raiz->esq->esq) == PRETO)
        raiz = move2EsqRed(raiz);

    raiz->esq = removerMenor(raiz->esq);
    return (&raiz);
}

Album *procuraMenor(Album *raiz)
{
    Album *aux = raiz;

    while (aux->esq != NULL) 
    {
        aux = aux->esq;
    }
    return aux;
}

Album* removeNo(Album *raiz, char valor) {
    if (valor < (*raiz).info.titulo) 
    {
        if (cor((*raiz).esq) == PRETO && cor((*raiz).esq->esq) == PRETO) 
        {
            raiz = moveTwoEsqRed(raiz);
        }
        (*raiz).esq = removeNo((*raiz).esq, valor);

    } 
    else 
    {
        if (cor((*raiz).esq) == VERMELHO) 
        {
            rotDir(&raiz);
        }

        if (valor == (*raiz).info.titulo && ((*raiz).dir) == NULL) 
        {
            free(raiz);
            return NULL;
        }

        if (cor((*raiz).dir) == PRETO && cor((*raiz).dir->esq) == PRETO)
        {
            raiz = moveTwoDirRed(raiz);
        } 

        if (valor == (*raiz).info.titulo) 
        {
            Album *menor = procuraMenor((*raiz).dir);
            (*raiz).info = (*menor).info;
            (*raiz).dir = removeMenor((*raiz).dir);
        } 
        else 
            (*raiz).dir = removeNo((*raiz).dir, valor);
    }

    balanceamento(&raiz);

    return raiz;
} 
