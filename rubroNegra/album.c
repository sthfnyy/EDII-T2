#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "tipo.h"
#include "album.h"
#include "musica.h"
#include "artista.h"

Album *criarNoAlbum(infoAlbum album)
{
    Album *novoNo = (Album*) malloc(sizeof(Album));
    if (novoNo != NULL)
    {
        novoNo->esq = NULL;
        novoNo->dir = NULL;
        novoNo->cor = VERMELHO;
        novoNo->info = album;
    }
    return novoNo;
}

infoAlbum preencherAlbum(void)
{
    infoAlbum dados;
    printf("Digite o titulo do album: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]", dados.titulo);
    printf("Digite o ano de lancamento do album: ");
    scanf("%d", &dados.anoLancamento);
    printf("Digite a quantidade de musica do album: ");
    scanf("%d", &dados.quantMusica);
    dados.musica = NULL;
    return dados;
}

int corAlbum(Album *raiz)
{
    int corNo = PRETO;
    if (raiz)
    {
        corNo = (raiz)->cor;
    }
    
    return corNo;
}

Album *rotacionaEsqAlbum(Album *raiz)
{
    Album *aux = (raiz)->dir;

    raiz->dir = aux->esq;
    aux->esq = raiz;
    aux->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return aux;
}

Album *rotacionaDirAlbum(Album *raiz)
{
    Album *aux = raiz->esq;

    raiz->esq = aux->dir;
    aux->dir = raiz;
    aux->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return aux;
}

void trocaCorAlbum(Album *raiz) 
{
    raiz->cor = !(raiz->cor); // troca a cor 
    if(raiz->esq != NULL)
        raiz->esq->cor = !(raiz->esq->cor);
    if(raiz->dir != NULL)
        raiz->dir->cor = !(raiz->dir->cor);
}

void balanceamentoAlbum(Album **raiz) 
{
    if (corAlbum((*raiz)->esq) == PRETO && corAlbum((*raiz)->dir) == VERMELHO)
    {
        *raiz = rotacionaEsqAlbum(*raiz);
    }

    if ((*raiz)->esq != NULL)
    {
        if (corAlbum((*raiz)->esq) == VERMELHO &&
            (*raiz)->esq->esq != NULL &&
            corAlbum((*raiz)->esq->esq) == VERMELHO)
        {
            *raiz = rotacionaDirAlbum(*raiz);
        }
    }

    if (corAlbum((*raiz)->esq) == VERMELHO && corAlbum((*raiz)->dir) == VERMELHO)
    {
        trocaCorAlbum(*raiz);
    }
    
}


int insereNoAlbum(Album **raiz, Album *novoNo)
{
    int inseriu = 1;

    if (*raiz == NULL)
    {
        *raiz = novoNo;
    }else{
        int cmp = strcmp(novoNo->info.titulo, (*raiz)->info.titulo);

        if (cmp < 0)
        {
            inseriu = insereNoAlbum(&((*raiz)->esq), novoNo);
        }
        else if (cmp > 0)
        {
            inseriu = insereNoAlbum(&((*raiz)->dir), novoNo);
        }
        else
        {
            inseriu = 0; //nome duplicado
        }
            
        if (*raiz && inseriu)
        {
            balanceamentoAlbum(raiz);
        }
    }    
    return inseriu;
}

int  insercaoAlbum(Album **raiz, Album *novoNo) 
{
    int inseriu = insereNoAlbum(raiz, novoNo);

    if (inseriu && *raiz) 
        (*raiz)->cor = PRETO;

    return inseriu;
}


Album* BuscarNoPorTituloAlbum(Album *raiz, const char* tituloBusca)
{
    Album *resultado = NULL;

    if (raiz != NULL)
    {
        int comparacao = strcmp(tituloBusca, raiz->info.titulo);

        if (comparacao == 0)
        {
            resultado = raiz;
        }
        else if (comparacao < 0)
        {
            resultado = BuscarNoPorTituloAlbum(raiz->esq, tituloBusca);
        }
        else
        {
            resultado = BuscarNoPorTituloAlbum(raiz->dir, tituloBusca);
        }
    }

    return resultado;
}


Album *move2EsqRedAlbum(Album *raiz)
{
    Album *retorna = raiz;
    trocaCorAlbum(raiz);

    if (raiz != NULL && raiz->dir != NULL)
    {
        if (raiz->dir->esq != NULL)
        {
            if (corAlbum(raiz->dir->esq) == VERMELHO)
            {
                raiz->dir = rotacionaDirAlbum(raiz->dir);
                retorna = rotacionaEsqAlbum(raiz);
                trocaCorAlbum(retorna);
            }
        }
    }
    return retorna;
}

Album *move2DirRedAlbum(Album *raiz)
{
    Album *retorna = raiz;
    trocaCorAlbum(raiz);

    if (raiz != NULL && raiz->esq != NULL)
    {
        if (raiz->esq->esq != NULL)
        {
            if (corAlbum(raiz->esq->esq) == VERMELHO)
            {
                retorna = rotacionaDirAlbum(raiz);
                trocaCorAlbum(retorna);
            }
        }
    }
    return retorna;
}

Album *removeMenorAlbum(Album *raiz)
{
    Album *resultado = raiz;

    if (raiz != NULL)
    {
        if (raiz->esq == NULL)
        {
            liberarListaMusicas(raiz->info.musica);
            raiz->info.musica = NULL;

            free(raiz);
            resultado = NULL;
        }
        else
        {
            Album *esq = raiz->esq;
            Album *esqEsq = NULL;

            if (esq != NULL)
                esqEsq = esq->esq;

            if (corAlbum(esq) == PRETO && corAlbum(esqEsq) == PRETO)
                raiz = move2EsqRedAlbum(raiz);

            raiz->esq = removeMenorAlbum(raiz->esq);
            resultado = raiz;
        }
    }

    return resultado;
}

Album *procuraMenorAlbum(Album *raiz)
{
    Album *aux = raiz;

    while (aux->esq != NULL) 
    {
        aux = aux->esq;
    }
    return aux;
}

int removeAlbum(Album **raiz, const char *titulo) 
{
    int removeu = 0;
    if (BuscarNoPorTituloAlbum (*raiz, titulo)) 
    {
        Album *no = *raiz;
        *raiz = removeNoAlbum(*raiz, titulo);
        if (*raiz != NULL) 
            (*raiz)->cor = PRETO;
        removeu = 1;
    }
    return removeu;
}

Album* removeNoAlbum(Album *raiz, const char *titulo) 
{
    Album *resultado = raiz;

    if (raiz != NULL)
    {
        int cmp = strcmp(titulo, raiz->info.titulo);

        if (cmp < 0) {
            if (raiz->esq && corAlbum(raiz->esq) == PRETO &&
                (raiz->esq->esq == NULL || corAlbum(raiz->esq->esq) == PRETO))
            {
                raiz = move2EsqRedAlbum(raiz);
            }
            raiz->esq = removeNoAlbum(raiz->esq, titulo);
            resultado = raiz;
        }
        else
        {
            if (corAlbum(raiz->esq) == VERMELHO)
                raiz = rotacionaDirAlbum(raiz);

            if (cmp == 0 && raiz->dir == NULL)
            {
                liberarListaMusicas(raiz->info.musica);
                raiz->info.musica = NULL;

                free(raiz);
                resultado = NULL;
            }
            else
            {
                if (corAlbum(raiz->dir) == PRETO &&
                    (raiz->dir == NULL || corAlbum(raiz->dir->esq) == PRETO))
                {
                    raiz = move2DirRedAlbum(raiz);
                }

                if (cmp == 0)
                {
                    Album *menor = procuraMenorAlbum(raiz->dir);

                    liberarListaMusicas(raiz->info.musica);

                    raiz->info = menor->info;

                    menor->info.musica = NULL;

                    raiz->dir = removeMenorAlbum(raiz->dir);
                }
                else
                {
                    raiz->dir = removeNoAlbum(raiz->dir, titulo);
                }
                resultado = raiz;
            }
        }

        if (resultado != NULL)
            balanceamentoAlbum(&resultado);
    }

    return resultado;
}


// Impressão pre-order
void mostrarAlbumPreOrdem(Album *raiz)
{
    
    if (raiz != NULL)
    {
        // imprime primeiro o nó atual (raiz local)
        char corChar;
        if (raiz->cor == PRETO)
            corChar = 'P';
        else
            corChar = 'V';
        
        printf("[%c] Titulo: %s, Ano Lancamento: %d, Quantidade Musica: %d\n",
               corChar,
               raiz->info.titulo,
               raiz->info.anoLancamento,
               raiz->info.quantMusica);

        // depois percorre os filhos
        mostrarAlbumPreOrdem(raiz->esq);
        mostrarAlbumPreOrdem(raiz->dir);
    }
}

void liberarArvoreAlbum(Album *raiz)
{
    if (raiz != NULL)
    {
        liberarArvoreAlbum(raiz->esq);
        liberarArvoreAlbum(raiz->dir);

        liberarListaMusicas(raiz->info.musica);

        free(raiz);
    }
}


// Percorre a RB de álbuns de UM artista (in-order) e procura por título 
int procurarAlbumPorTitulo(Album *raizDosAlbuns, const char *tituloBuscado, const char *nomeDoArtista) 
{
    int albumEncontrado = 0;

    if (raizDosAlbuns != NULL) {
        if (procurarAlbumPorTitulo(raizDosAlbuns->esq, tituloBuscado, nomeDoArtista)) {
            albumEncontrado = 1;
        }

        if (strcmp(raizDosAlbuns->info.titulo, tituloBuscado) == 0) {
            printf(">> Artista: %s | Álbum: %s | Ano: %d | Faixas (declaradas): %d\n",
                   nomeDoArtista,
                   raizDosAlbuns->info.titulo,
                   raizDosAlbuns->info.anoLancamento,
                   raizDosAlbuns->info.quantMusica);
            albumEncontrado = 1;
        }

        if (procurarAlbumPorTitulo(raizDosAlbuns->dir, tituloBuscado, nomeDoArtista)) 
        {
            albumEncontrado = 1;
        }
    }

    return albumEncontrado;
}

void percorrerArtistasEBuscarAlbum(Artista *raizDosArtistas, const char *tituloBuscado) 
{
    if (raizDosArtistas != NULL) {
        percorrerArtistasEBuscarAlbum(raizDosArtistas->esq, tituloBuscado);

        int albumEncontradoNesteArtista = procurarAlbumPorTitulo(raizDosArtistas->info.albuns, tituloBuscado, raizDosArtistas->info.nome);

        if (albumEncontradoNesteArtista) 
        {
            printf("------------------------------------------------------------\n");
        }

        percorrerArtistasEBuscarAlbum(raizDosArtistas->dir, tituloBuscado);
    }
}
