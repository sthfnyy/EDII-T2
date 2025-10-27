#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"


Artista *alocaArtista(infoArtista dados)
{
    Artista *novoNo = (Artista *) malloc(sizeof(Artista));
    if (novoNo != NULL)
    {
        novoNo->esq = NULL;         
        novoNo->dir = NULL;           
        novoNo->cor = VERMELHO;    
        novoNo->info = dados;       

        novoNo->info.numAlbuns = 0;    
        novoNo->info.albuns    = NULL; 
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
    dados.albuns = NULL;
    
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

Artista *rotacionaEsq(Artista *raiz)
{
    Artista *aux = (raiz)->dir;

    raiz->dir = aux->esq;
    aux->esq = raiz;
    aux->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return aux;
}

Artista *rotacionaDir(Artista *raiz)
{
    Artista *aux = raiz->esq;

    raiz->esq = aux->dir;
    aux->dir = raiz;
    aux->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return aux;
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
        *raiz = rotacionaEsq(*raiz);
    }

    if ((*raiz)->esq != NULL)
    {
        if (cor((*raiz)->esq) == VERMELHO &&
            (*raiz)->esq->esq != NULL &&
            cor((*raiz)->esq->esq) == VERMELHO)
        {
            *raiz = rotacionaDir(*raiz);
        }
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
        // imprime primeiro o nó atual (raiz do momento)
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

//remoção da árvore rubro-negra de artistas
Artista* move2EsqRed(Artista *raiz)
{
    Artista *ret = raiz;

    trocaCor(raiz);

    // só faz as rotações se houver dir e (dir->esq) vermelho
    if (raiz != NULL && raiz->dir != NULL)
    {
        if (cor(raiz->dir->esq) == VERMELHO)
        {
            raiz->dir = rotacionaDir(raiz->dir); // reanexa subárvore direita
            ret = rotacionaEsq(raiz);            // novo topo após rotação
            trocaCor(ret);
        }
    }

    return ret;
}


Artista* move2DirRed(Artista *raiz)
{
    Artista *ret = raiz;

    trocaCor(raiz);

    // só rota se existir esq e neto esquerdo vermelho
    if (raiz != NULL && raiz->esq != NULL)
    {
        if (raiz->esq->esq != NULL && cor(raiz->esq->esq) == VERMELHO)
        {
            ret = rotacionaDir(raiz); 
            trocaCor(ret);
        }
    }

    return ret;
}


Artista *removeMenor(Artista *raiz)
{
    Artista *resultado = raiz;

    if (raiz != NULL)
    {
        if (raiz->esq == NULL)
        {
            // libera TODA a árvore de álbuns (e depois as músicas)
            liberarArvoreAlbum(raiz->info.albuns);
            raiz->info.albuns = NULL;

            free(raiz);
            resultado = NULL;
        }
        else
        {
            Artista *esq = raiz->esq;
            Artista *esqEsq = NULL;

            if (esq != NULL)
                esqEsq = esq->esq;

            if (cor(esq) == PRETO && cor(esqEsq) == PRETO)
                raiz = move2EsqRed(raiz);

            raiz->esq = removeMenor(raiz->esq);
            resultado = raiz;
        }
    }
    return resultado;
}


Artista *procuraMenor(Artista *raiz)
{
    Artista *aux = raiz;

    while (aux->esq != NULL) 
    {
        aux = aux->esq;
    }
    return aux;
}

int removeArtista(Artista **raiz, const char *nomeArtista) 
{
    int removeu = 0;
    if (buscarArtista (*raiz, nomeArtista)) 
    {
        Artista *no = *raiz;
        *raiz = removeNo(*raiz, nomeArtista);
        if (*raiz != NULL) 
            (*raiz)->cor = PRETO;
        removeu = 1;
    }
    return removeu;
}

Artista* removeNo(Artista *raiz, const char *nomeArtista) 
{
    Artista *resultado = raiz;

    if (raiz != NULL)
    {
        int cmp = strcmp(nomeArtista, raiz->info.nome);

        if (cmp < 0)
        {
            if (raiz->esq && cor(raiz->esq) == PRETO &&
                (raiz->esq->esq == NULL || cor(raiz->esq->esq) == PRETO))
            {
                raiz = move2EsqRed(raiz);
            }
            raiz->esq = removeNo(raiz->esq, nomeArtista);
            resultado = raiz;
        }
        else
        {
            if (cor(raiz->esq) == VERMELHO)
                raiz = rotacionaDir(raiz);

            if (cmp == 0 && raiz->dir == NULL)
            {
                liberarArvoreAlbum(raiz->info.albuns);
                raiz->info.albuns = NULL;

                free(raiz);
                resultado = NULL;
            }
            else
            {
                if (cor(raiz->dir) == PRETO &&
                    (raiz->dir == NULL || cor(raiz->dir->esq) == PRETO))
                {
                    raiz = move2DirRed(raiz);
                }

                if (cmp == 0)
                {
                    Artista *menor = procuraMenor(raiz->dir);

                    liberarArvoreAlbum(raiz->info.albuns);

                    raiz->info = menor->info;
                    menor->info.albuns = NULL;

                    raiz->dir = removeMenor(raiz->dir);
                }
                else
                {
                    raiz->dir = removeNo(raiz->dir, nomeArtista);
                }

                resultado = raiz;
            }
        }

        if (resultado != NULL)
            balanceamento(&resultado);
    }

    return resultado;
}


void liberarArvore(Artista *raiz)
{
    if (raiz != NULL)
    {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        liberarArvoreAlbum(raiz->info.albuns); // libera árvore de álbuns
        free(raiz);
    }
}
