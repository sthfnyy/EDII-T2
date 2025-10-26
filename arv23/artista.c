#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "artista.h"
#include "album.h"
#include "musica.h"

Artista *alocaArtista (infoArtista dados, Artista *fEsq, Artista *fCen)
{
    Artista *novo = (Artista *) malloc (sizeof(Artista));
    if (novo != NULL){
        novo->infoUm = dados;
        novo->Ninfos = 1;
        novo->esq = fEsq;
        novo->cen = fCen;
        novo->dir = NULL;
    }
    return novo;
}

infoArtista preencherArtista(void)
{
    infoArtista dados;
    printf("Digite o nome do artista: ");   scanf("%49s", dados.nome);
    printf("Digite o estilo do artista: "); scanf("%49s", dados.estilo);
    printf("Digite o tipo: ");              scanf("%49s", dados.tipo);
    dados.numAlbuns = 0;
    dados.albuns    = NULL;   
    return dados;
}

void imprimirArv(Artista *raiz, int nivel) 
{
    if (raiz != NULL)
    {
        // imprime da direita pra esquerda (como uma árvore "virada")
        imprimirArv(raiz->dir, nivel + 1);

        for (int i = 0; i < nivel; i++)
        {
            printf("   "); 
        }

        if (raiz->Ninfos == 1)
        {
            printf("[%s]\n", raiz->infoUm.nome);
        }
        else
        {
            printf("[%s|%s]\n", raiz->infoUm.nome, raiz->infoDois.nome);
        }

        imprimirArv(raiz->cen, nivel + 1);
        imprimirArv(raiz->esq, nivel + 1);
    }
}

void inserir23(Artista **novoNo, infoArtista dado, Artista *subArvInfo) 
{
    int cmp = strcmp(dado.nome, (*novoNo)->infoUm.nome); 
    
    if (cmp > 0) {
        (*novoNo)->infoDois = dado;
        (*novoNo)->dir = subArvInfo;
    } else {
        (*novoNo)->infoDois = (*novoNo)->infoUm;
        (*novoNo)->infoUm = dado;
        (*novoNo)->dir = (*novoNo)->cen;
        (*novoNo)->cen = subArvInfo;
    }
    (*novoNo)->Ninfos = 2; 
}


Artista *quebrarNoArtista(Artista **no, infoArtista dado, infoArtista *sobe, Artista *filhoDir) 
{
    Artista *maior; 

    int cmp1 = strcmp(dado.nome, (*no)->infoUm.nome);
    int cmp2 = strcmp(dado.nome, (*no)->infoDois.nome);


    if (cmp2 > 0) {
        *sobe = (*no)->infoDois;  
        maior = alocaArtista(dado, (*no)->dir, filhoDir);     
        (*no)->Ninfos = 1;
        (*no)->dir = NULL; 
    } else if (cmp1 > 0) {
        *sobe = dado;                   
        maior = alocaArtista((*no)->infoDois, filhoDir, (*no)->dir); 
        (*no)->Ninfos = 1;
        (*no)->dir = NULL; 
    } else {
        *sobe = (*no)->infoUm;           
        maior = alocaArtista((*no)->infoDois, (*no)->cen, (*no)->dir); 
        (*no)->infoUm = dado;
        (*no)->cen = filhoDir;
        (*no)->Ninfos = 1;
        (*no)->dir = NULL; 
    }

    return maior; 
}

Artista *inserirArtista(Artista **raiz, Artista *pai, infoArtista dado, infoArtista *sobe)
{
    Artista *maiorNo = NULL;

    if (*raiz == NULL)
    {
        *raiz = alocaArtista(dado, NULL, NULL);
    }
    else
    {
        if ((*raiz)->esq == NULL) // É folha
        {
            if ((*raiz)->Ninfos == 1)
            {
                inserir23(raiz, dado, NULL);
            }
            else
            {
                maiorNo = quebrarNoArtista(raiz, dado, sobe, NULL);
                if (pai == NULL)
                {
                    *raiz = alocaArtista(*sobe, *raiz, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
        else
        {
            if (strcmp(dado.nome, (*raiz)->infoUm.nome) < 0)
            {
                maiorNo = inserirArtista(&((*raiz)->esq), *raiz, dado, sobe);
            }
            else if ((*raiz)->Ninfos == 1 || strcmp(dado.nome, (*raiz)->infoDois.nome) < 0)
            {
                maiorNo = inserirArtista(&((*raiz)->cen), *raiz, dado, sobe);
            }
            else
            {
                maiorNo = inserirArtista(&((*raiz)->dir), *raiz, dado, sobe);
            }

            if (maiorNo != NULL)
            {
                if ((*raiz)->Ninfos == 1)
                {
                    inserir23(raiz, *sobe, maiorNo);
                    maiorNo = NULL;
                }
                else
                {
                    maiorNo = quebrarNoArtista(raiz, *sobe, sobe, maiorNo);
                    if (pai == NULL)
                    {
                        *raiz = alocaArtista(*sobe, *raiz, maiorNo);
                        maiorNo = NULL;
                    }
                }
            }
        }
    }

    return maiorNo;
}



int verificaDados(Artista *raiz, const char *nome) 
{
    int existe = 0;

    if (raiz != NULL)
    {
        int cmp1 = strcmp(nome, raiz->infoUm.nome);
        int cmp2 = strcmp(nome, raiz->infoDois.nome) ;

        if (cmp1 == 0 || (raiz->Ninfos == 2 && cmp2 == 0)) 
        {
            existe = 1; // encontrou o nome
        } 
        else if (cmp1 < 0)
        {
            existe = verificaDados(raiz->esq, nome);
        } 
        else if (raiz->Ninfos == 1 || cmp2 < 0) 
        {
            existe = verificaDados(raiz->cen, nome);
        } 
        else 
        {
            existe = verificaDados(raiz->dir, nome);
        }
    }

    return existe;
}

Artista *buscarArtista(Artista *raiz, const char *nome)
{
    Artista *resultado = NULL;

    if (raiz)
    {
        int cmp1 = strcmp(nome, raiz->infoUm.nome);

        if (cmp1 == 0){
            resultado = raiz;
        }
        else if (raiz->Ninfos == 2){
            int cmp2 = strcmp(nome, raiz->infoDois.nome);
            if (cmp2 == 0){
                resultado = raiz;
            }
            else if (cmp1 < 0){
                resultado = buscarArtista(raiz->esq, nome);
            }
            else if (cmp2 < 0){
                resultado = buscarArtista(raiz->cen, nome);
            }
            else{
                resultado = buscarArtista(raiz->dir, nome);
            }
        }
        else{
            if (cmp1 < 0)
                resultado = buscarArtista(raiz->esq, nome);
            else
                resultado = buscarArtista(raiz->cen, nome);
        }
    }
    return resultado;
}

infoArtista *buscarInfoArtista(Artista *raiz, const char *nome)
{
    Artista *no = buscarArtista(raiz, nome);
    infoArtista *resultado = NULL;

    if (no != NULL)
    {
        if (strcmp(no->infoUm.nome, nome) == 0)
        {
            resultado = &no->infoUm;
        }
        else if (no->Ninfos == 2 && strcmp(no->infoDois.nome, nome) == 0)
        {
            resultado = &no->infoDois;
        }
    }

    return resultado;
}


void liberarArv(Artista **ponteiroRaiz)
{

    Artista *noAtual = *ponteiroRaiz;

    // 1) Libera recursivamente as subárvores de artistas
    liberarArv(&noAtual->esq);
    liberarArv(&noAtual->cen);
    if (noAtual->Ninfos == 2)
        liberarArv(&noAtual->dir);

    // 2) Libera os álbuns (e músicas) ligados aos artistas deste nó
    if (noAtual->infoUm.albuns)
    {
        liberarArvAlbum(&noAtual->infoUm.albuns);
        noAtual->infoUm.albuns = NULL;
    }

    if (noAtual->Ninfos == 2 && noAtual->infoDois.albuns)
    {
        liberarArvAlbum(&noAtual->infoDois.albuns);
        noAtual->infoDois.albuns = NULL;
    }

    // 3) Libera o nó do artista e zera o ponteiro
    free(noAtual);
    *ponteiroRaiz = NULL;
}

