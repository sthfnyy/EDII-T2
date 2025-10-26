#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "album.h"
#include "musica.h"


Album *criarNoAlbum(infoAlbum info, Album *fEsq, Album *fCen)
{
    Album *no;
    no = (Album*) malloc(sizeof(Album));
    if (no)
    {
        no->infoUm = info; 
        no->esq = fEsq;
        no->cen = fCen;
        no->dir = NULL;
        no->quantInfo = 1;
        no->pai = NULL; 
    }
    return no;
}

void adicionarInfoAlbum(Album **no, infoAlbum info, Album *subArvInfo)
{
    if (strcmp(info.titulo, (*no)->infoUm.titulo) > 0)
    {
        (*no)->infoDois = info;
        (*no)->dir = subArvInfo;
    }
    else
    {
        (*no)->infoDois = (*no)->infoUm;
        (*no)->infoUm = info;
        (*no)->dir = (*no)->cen;
        (*no)->cen = subArvInfo;
    }
    (*no)->quantInfo = 2;
}

Album *quebrarNoAlbum (Album **no, infoAlbum info, infoAlbum *sobe, Album *filhoDir)
{
    Album *maior;
    if (strcmp(info.titulo, (*no)->infoDois.titulo) > 0)
    {
        *sobe = (*no)->infoDois;
        maior = criarNoAlbum(info, (*no)->dir, filhoDir);
        (*no)->quantInfo = 1;
    }
    else if (strcmp(info.titulo, (*no)->infoUm.titulo) > 0)
    {
        *sobe = info;
        maior = criarNoAlbum((*no)->infoDois, filhoDir, (*no)->dir);
        (*no)->quantInfo = 1;
    }
    else
    {
        *sobe = (*no)->infoUm;
        maior = criarNoAlbum((*no)->infoDois, (*no)->cen, (*no)->dir);
        (*no)->infoUm = info;
        (*no)->cen = filhoDir;
        (*no)->quantInfo = 1;
    }
    
    if (maior->esq) maior->esq->pai = maior;
    if (maior->cen) maior->cen->pai = maior;
    if ((*no)->esq) (*no)->esq->pai = *no;
    if ((*no)->cen) (*no)->cen->pai = *no;

    return maior;
}

Album* inserirNoAlbum (Album **raiz, Album *pai, infoAlbum info, infoAlbum *sobe) 
{
    Album *maiorNo = NULL;
    if (*raiz == NULL)
    {
        *raiz = criarNoAlbum(info, NULL, NULL);
        (*raiz)->pai = pai;
    }
    else 
    {
        if ((*raiz)->esq == NULL) // É folha
        {
            if ((*raiz)->quantInfo == 1) 
            {
                adicionarInfoAlbum(raiz, info, NULL);
            } 
            else 
            {
                maiorNo = quebrarNoAlbum (raiz, info, sobe, NULL);
                if (pai == NULL)
                {
                    *raiz = criarNoAlbum(*sobe, *raiz, maiorNo);
                    // Atualiza o pai dos filhos
                    if ((*raiz)->esq) (*raiz)->esq->pai = *raiz;
                    if ((*raiz)->cen) (*raiz)->cen->pai = *raiz;
                    maiorNo = NULL;
                }
            }
        } 
        else 
        {
            if(strcmp(info.titulo, (*raiz)->infoUm.titulo) < 0)
            {
                maiorNo = inserirNoAlbum (&((*raiz)->esq), *raiz, info, sobe);
            }
            else if((*raiz)->quantInfo == 1 || strcmp(info.titulo, (*raiz)->infoDois.titulo) < 0)
            {
                maiorNo = inserirNoAlbum (&((*raiz)->cen), *raiz, info, sobe);
            }
            else
            {
                maiorNo = inserirNoAlbum (&((*raiz)->dir), *raiz, info, sobe);
            }
            
            if (maiorNo != NULL) 
            {
                if ((*raiz)->quantInfo == 1) 
                {
                    adicionarInfoAlbum(raiz, *sobe, maiorNo);
                    if (maiorNo) maiorNo->pai = *raiz;
                    maiorNo = NULL;
                } 
                else 
                {
                    maiorNo = quebrarNoAlbum (raiz, *sobe, sobe, maiorNo);
                    if (pai == NULL) 
                    {
                        *raiz = criarNoAlbum(*sobe, *raiz, maiorNo);
                        if ((*raiz)->esq) (*raiz)->esq->pai = *raiz;
                        if ((*raiz)->cen) (*raiz)->cen->pai = *raiz;
                        maiorNo = NULL;
                    }
                }
            }
        }
    }
    return maiorNo;
}

int verificaInfosAlbum(Album *raiz, const char* titulo) 
{
    int existe = 0;
    if (strcmp(raiz->infoUm.titulo, titulo) == 0) 
    {
        existe = 1;      
    }
    else if(raiz->quantInfo == 2)
    {   
        if(strcmp(raiz->infoDois.titulo, titulo) == 0)
        {
            existe = 1;
        }
    }
    return existe;
}

void imprimirArvAlbum (Album *raiz, int nivel) 
{
    if (raiz == NULL) return;
    
    imprimirArvAlbum (raiz->dir, nivel + 1);
    
    for (int i = 0; i < nivel; i++)
    {
        printf("    "); // Espaçamento
    }
    
    if (raiz->quantInfo == 1)
    {
        printf("[%s]\n", raiz->infoUm.titulo);
    }
    else
    {
        printf("[%s | %s]\n", raiz->infoUm.titulo, raiz->infoDois.titulo);
    }
    
    imprimirArvAlbum (raiz->cen, nivel + 1);
    imprimirArvAlbum (raiz->esq, nivel + 1);
}

void liberarArvAlbum(Album **ponteiroRaiz)
{
    Album *noAtual = NULL;

    if (ponteiroRaiz != NULL && *ponteiroRaiz != NULL)
    {
        noAtual = *ponteiroRaiz;

        if (noAtual->esq != NULL)
            liberarArvAlbum(&noAtual->esq);

        if (noAtual->cen != NULL)
            liberarArvAlbum(&noAtual->cen);

        if (noAtual->quantInfo == 2 && noAtual->dir != NULL)
            liberarArvAlbum(&noAtual->dir);

        if (noAtual->infoUm.musica != NULL)
            liberarListaMusicas(noAtual->infoUm.musica);

        if (noAtual->quantInfo == 2 && noAtual->infoDois.musica != NULL)
            liberarListaMusicas(noAtual->infoDois.musica);

        free(noAtual);
        *ponteiroRaiz = NULL;
    }
}

Album* buscarAlbum(Album *raiz, const char *titulo) {
    Album *resultado = NULL;

    if (raiz) {
        int cmpUm = strcmp(titulo, raiz->infoUm.titulo);

        if (cmpUm == 0) {
            resultado = raiz;
        } else if (raiz->quantInfo == 2) {
            int cmpDois = strcmp(titulo, raiz->infoDois.titulo);

            if (cmpDois == 0) {
                resultado = raiz;
            } else if (cmpUm < 0) {
                resultado = buscarAlbum(raiz->esq, titulo);
            } else if (cmpDois < 0) {
                resultado = buscarAlbum(raiz->cen, titulo);
            } else {
                resultado = buscarAlbum(raiz->dir, titulo);
            }
        } else {
            if (cmpUm < 0)
                resultado = buscarAlbum(raiz->esq, titulo);
            else
                resultado = buscarAlbum(raiz->cen, titulo);
        }
    }

    return resultado;
}

infoAlbum *buscarInfoAlbum(Album *raiz, const char *titulo)
{
    Album *no = buscarAlbum(raiz, titulo);
    infoAlbum *resultado = NULL;

    if (no != NULL)
    {
        if (strcmp(no->infoUm.titulo, titulo) == 0)
        {
            resultado = &no->infoUm;
        }
        else if (no->quantInfo == 2 && strcmp(no->infoDois.titulo, titulo) == 0)
        {
            resultado = &no->infoDois;
        }
    }

    return resultado;
}


/* In-order 2–3: esq -> infoUm -> cen -> infoDois -> dir */
int procurarAlbumPorTitulo23(Album *raizAlb, const char *tituloBuscado, const char *nomeDoArtista)
{
    int encontrado = 0;
    int achouAqui = 0;

    if (raizAlb != NULL)
    {
        /* esquerda */
        if (procurarAlbumPorTitulo23(raizAlb->esq, tituloBuscado, nomeDoArtista) == 1)
            encontrado = 1;

        /* infoUm */
        achouAqui = (strcmp(raizAlb->infoUm.titulo, tituloBuscado) == 0);
        if (achouAqui == 1)
        {
            printf("Artista: %s | Álbum: %s | Ano: %d | Faixas: %d\n",
                   nomeDoArtista,
                   raizAlb->infoUm.titulo,
                   raizAlb->infoUm.anoLancamento,
                   raizAlb->infoUm.quantMusica);
            encontrado = 1;
        }

        /* centro */
        if (procurarAlbumPorTitulo23(raizAlb->cen, tituloBuscado, nomeDoArtista) == 1)
            encontrado = 1;

        /* infoDois  */
        if (raizAlb->quantInfo == 2)
        {
            achouAqui = (strcmp(raizAlb->infoDois.titulo, tituloBuscado) == 0);
            if (achouAqui == 1)
            {
                printf("Artista: %s | Álbum: %s | Ano: %d | Faixas: %d\n",
                       nomeDoArtista,
                       raizAlb->infoDois.titulo,
                       raizAlb->infoDois.anoLancamento,
                       raizAlb->infoDois.quantMusica);
                encontrado = 1;
            }

            /* direita */
            if (procurarAlbumPorTitulo23(raizAlb->dir, tituloBuscado, nomeDoArtista) == 1)
                encontrado = 1;
        }
    }

    return encontrado;
}


int procurarMusicaNosAlbunsEmOrdem23(Album *raizAlb, const char *tituloBuscado, const char *nomeDoArtista)
{
    int achouEmAlgumAlbum = 0;
    int achouAqui = 0;

    if (raizAlb != NULL)
    {
        /* esquerda */
        if (procurarMusicaNosAlbunsEmOrdem23(raizAlb->esq, tituloBuscado, nomeDoArtista) == 1)
            achouEmAlgumAlbum = 1;

        /* infoUm */
        achouAqui = encontrarMusicaPeloTitulo(raizAlb->infoUm.musica, tituloBuscado);
        if (achouAqui == 1)
        {
            printf("  >> Artista: %s | Álbum: %s (%d)\n",
                   nomeDoArtista,
                   raizAlb->infoUm.titulo,
                   raizAlb->infoUm.anoLancamento);
            achouEmAlgumAlbum = 1;
        }

        /* centro */
        if (procurarMusicaNosAlbunsEmOrdem23(raizAlb->cen, tituloBuscado, nomeDoArtista) == 1)
            achouEmAlgumAlbum = 1;

        /* infoDois + direita, se existir */
        if (raizAlb->quantInfo == 2)
        {
            achouAqui = encontrarMusicaPeloTitulo(raizAlb->infoDois.musica, tituloBuscado);
            if (achouAqui == 1)
            {
                printf("  >> Artista: %s | Álbum: %s (%d)\n",
                       nomeDoArtista,
                       raizAlb->infoDois.titulo,
                       raizAlb->infoDois.anoLancamento);
                achouEmAlgumAlbum = 1;
            }

            if (procurarMusicaNosAlbunsEmOrdem23(raizAlb->dir, tituloBuscado, nomeDoArtista) == 1)
                achouEmAlgumAlbum = 1;
        }
    }

    return achouEmAlgumAlbum;
}




// int main() {
//     Album *raiz = NULL;
//     infoAlbum albumInfo;
//     infoAlbum sobe;
//     int opcao;
//     char tituloBusca[TAM_STRING];
//     Album *resultado;

//     do {
//         printf("\n--- MENU ÁRVORE 2-3 DE ÁLBUNS ---\n");
//         printf("1 - Inserir álbum\n");
//         printf("2 - Mostrar árvore\n");
//         printf("3 - Buscar álbum por título\n");
//         printf("0 - Sair\n");
//         printf("Escolha: ");
//         scanf("%d", &opcao);
//         getchar(); 

//         switch (opcao) {
//             case 1:
//                 printf("\nTítulo do álbum: ");
//                 fgets(albumInfo.titulo, TAM_STRING, stdin);
//                 albumInfo.titulo[strcspn(albumInfo.titulo, "\n")] = '\0';

//                 printf("Ano de lançamento: ");
//                 scanf("%d", &albumInfo.anoLancamento);

//                 printf("Quantidade de músicas: ");
//                 scanf("%d", &albumInfo.quantMusica);
//                 getchar();

//                 albumInfo.musica = NULL;

//                 inserirNoAlbum (&raiz, NULL, albumInfo, &sobe);
//                 printf("Álbum inserido com sucesso!\n");
//                 break;

//             case 2:
//                 if (raiz == NULL)
//                     printf("\nÁrvore vazia!\n");
//                 else {
//                     printf("\n=== Árvore 2-3 de Álbuns ===\n");
//                     imprimirArvAlbum (raiz, 0);
//                 }
//                 break;

//             case 3:
//                 printf("\nDigite o título do álbum a buscar: ");
//                 fgets(tituloBusca, TAM_STRING, stdin);
//                 tituloBusca[strcspn(tituloBusca, "\n")] = '\0';

//                 resultado = buscarAlbum(raiz, tituloBusca);

//                 if (resultado != NULL) {
//                     printf("\n Álbum encontrado!\n");
//                     if (strcmp(resultado->infoUm.titulo, tituloBusca) == 0) {
//                         printf("Título: %s\n", resultado->infoUm.titulo);
//                         printf("Ano: %d\n", resultado->infoUm.anoLancamento);
//                         printf("Músicas: %d\n", resultado->infoUm.quantMusica);
//                     } else {
//                         printf("Título: %s\n", resultado->infoDois.titulo);
//                         printf("Ano: %d\n", resultado->infoDois.anoLancamento);
//                         printf("Músicas: %d\n", resultado->infoDois.quantMusica);
//                     }
//                 } else {
//                     printf("\n❌ Álbum não encontrado!\n");
//                 }
//                 break;

//             case 0:
//                 printf("\nEncerrando...\n");
//                 break;

//             default:
//                 printf("\nOpção inválida!\n");
//         }

//     } while (opcao != 0);

//     liberarArvAlbum (&raiz);
//     return 0;
// }
