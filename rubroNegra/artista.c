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

        novoNo->info.numAlbuns = 0;    // se já não estiver
        novoNo->info.albuns    = NULL; // <-- inicialize aqui
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

//tive que modificar e tirar do tipo void, pois na remoção não dava pra usar void
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

    /* --- PROTEÇÃO AQUI --- */
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
            ret = rotacionaDir(raiz); // captura novo topo
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
            // libera TODA a árvore de álbuns (e consequentemente as músicas)
            liberarArvoreAlbum(raiz->info.albuns);
            raiz->info.albuns = NULL;

            free(raiz);
            resultado = NULL;
        }
        else
        {
            Artista *esq = raiz->esq;
            Artista *esqEsq = esq ? esq->esq : NULL;

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
                // liberar toda a árvore de álbuns ANTES de destruir o artista
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

                    // 1) libera os álbuns atuais do nó base (para não vazar)
                    liberarArvoreAlbum(raiz->info.albuns);

                    // 2) transfere info do sucessor
                    raiz->info = menor->info;

                    // 3) evitar double free: o sucessor será destruído em removeMenor,
                    //    então zere o ponteiro de álbuns que acabou de ser transferido
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


// int main(void)
// {
//     Artista *raiz = NULL;

//     infoArtista a = {"Chico", "MPB",  "Solo", 5};
//     infoArtista b = {"Ana",   "Pop",  "Banda",2};
//     infoArtista c = {"Zeca",  "Samba","Solo", 7};
//     infoArtista d = {"Beto",  "Rock", "Solo", 1};
//     infoArtista e = {"Lia",   "Indie","Solo", 3};
//     infoArtista f = {"Rafa",  "Jazz", "Banda",4};

//     // Inserções
//     insercao(&raiz, alocaArtista(a));
//     insercao(&raiz, alocaArtista(b));
//     insercao(&raiz, alocaArtista(c));
//     insercao(&raiz, alocaArtista(d));
//     insercao(&raiz, alocaArtista(e));
//     insercao(&raiz, alocaArtista(f));

//     printf(">>> Pre-ordem (inicial):\n");
//     mostrarArtistasPreOrdem(raiz);

//     // Busca simples
//     Artista *encontrado = buscarArtista(raiz, "Ana");
//     if (encontrado) printf("Achei %s\n\n", encontrado->info.nome);

//     // 1) Remover folha (escolha provável após balanceamentos)
//     printf(">>> Removendo 'Rafa' (folha?):\n");
//     if (removeArtista(&raiz, "Rafa"))
//         mostrarArtistasPreOrdem(raiz);
//     else
//         printf("Nao removido (nao encontrado)\n");
//     printf("\n");

//     // 2) Remover nó com 1 filho (depende do estado após rotações)
//     printf(">>> Removendo 'Beto' (possivel 1 filho):\n");
//     if (removeArtista(&raiz, "Beto"))
//         mostrarArtistasPreOrdem(raiz);
//     else
//         printf("Nao removido (nao encontrado)\n");
//     printf("\n");

//     // 3) Remover nó com 2 filhos (geralmente 'Chico' ou 'Zeca' acabam com 2)
//     printf(">>> Removendo 'Chico' (2 filhos?):\n");
//     if (removeArtista(&raiz, "Chico"))
//         mostrarArtistasPreOrdem(raiz);
//     else
//         printf("Nao removido (nao encontrado)\n");
//     printf("\n");

//     // 4) Remover a raiz atual (qualquer que seja após balanceamentos)
//     if (raiz) {
//         char raizNome[50];
//         strncpy(raizNome, raiz->info.nome, sizeof(raizNome));
//         raizNome[sizeof(raizNome)-1] = '\0';

//         printf(">>> Removendo a raiz atual ('%s'):\n", raizNome);
//         if (removeArtista(&raiz, raizNome))
//             mostrarArtistasPreOrdem(raiz);
//         else
//             printf("Nao removido (nao encontrado)\n");
//         printf("\n");
//     }

//     // 5) Tentar remover um nome inexistente
//     printf(">>> Removendo 'NaoExiste':\n");
//     if (removeArtista(&raiz, "NaoExiste"))
//         mostrarArtistasPreOrdem(raiz);
//     else
//         printf("Nao removido (nao encontrado)\n");
//     printf("\n");

//     liberarArvore(raiz);
//     return 0;
// }
