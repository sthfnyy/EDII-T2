#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "album.h"


// Agora recebe uma struct 'infoAlbum' inteira
Album *criarNo(infoAlbum info, Album *fEsq, Album *fCen)
{
    Album *no;
    no = (Album*) malloc(sizeof(Album));
    if (no)
    {
        // Atribui a struct inteira
        no->infoUm = info; 
        no->esq = fEsq;
        no->cen = fCen;
        no->dir = NULL;
        no->quantInfo = 1;
        no->pai = NULL; // Inicializa o pai como NULL
    }
    return no;
}

// Agora recebe uma struct 'infoAlbum' inteira
void adicionarInfo(Album **no, infoAlbum info, Album *subArvInfo)
{
    // Compara usando strcmp com o título
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

// Agora recebe 'infoAlbum info' e 'infoAlbum *sobe'
Album *quebrarNo(Album **no, infoAlbum info, infoAlbum *sobe, Album *filhoDir)
{
    Album *maior;
    // Compara usando strcmp com o título
    if (strcmp(info.titulo, (*no)->infoDois.titulo) > 0)
    {
        *sobe = (*no)->infoDois;
        maior = criarNo(info, (*no)->dir, filhoDir);
        (*no)->quantInfo = 1;
    }
    // Compara usando strcmp com o título
    else if (strcmp(info.titulo, (*no)->infoUm.titulo) > 0)
    {
        *sobe = info;
        maior = criarNo((*no)->infoDois, filhoDir, (*no)->dir);
        (*no)->quantInfo = 1;
    }
    else
    {
        *sobe = (*no)->infoUm;
        maior = criarNo((*no)->infoDois, (*no)->cen, (*no)->dir);
        (*no)->infoUm = info;
        (*no)->cen = filhoDir;
        (*no)->quantInfo = 1;
    }
    
    // Atualiza os ponteiros de pai nos filhos
    if (maior->esq) maior->esq->pai = maior;
    if (maior->cen) maior->cen->pai = maior;
    if ((*no)->esq) (*no)->esq->pai = *no;
    if ((*no)->cen) (*no)->cen->pai = *no;

    return maior;
}

// Agora recebe 'infoAlbum info' e 'infoAlbum *sobe'
Album* inserirNo(Album **raiz, Album *pai, infoAlbum info, infoAlbum *sobe) 
{
    Album *maiorNo = NULL;
    if (*raiz == NULL)
    {
        *raiz = criarNo(info, NULL, NULL);
        (*raiz)->pai = pai;
    }
    else 
    {
        if ((*raiz)->esq == NULL) // É folha
        {
            if ((*raiz)->quantInfo == 1) 
            {
                adicionarInfo(raiz, info, NULL);
            } 
            else  // Folha cheia, precisa quebrar
            {
                maiorNo = quebrarNo(raiz, info, sobe, NULL);
                if (pai == NULL) // Se o pai é NULL, está quebrando a raiz
                {
                    *raiz = criarNo(*sobe, *raiz, maiorNo);
                    // Atualiza o pai dos filhos
                    if ((*raiz)->esq) (*raiz)->esq->pai = *raiz;
                    if ((*raiz)->cen) (*raiz)->cen->pai = *raiz;
                    maiorNo = NULL;
                }
            }
        } 
        else // Não é folha, desce recursivamente
        {
            // Compara usando strcmp com o título
            if(strcmp(info.titulo, (*raiz)->infoUm.titulo) < 0)
            {
                maiorNo = inserirNo(&((*raiz)->esq), *raiz, info, sobe);
            }
            // Compara usando strcmp com o título
            else if((*raiz)->quantInfo == 1 || strcmp(info.titulo, (*raiz)->infoDois.titulo) < 0)
            {
                maiorNo = inserirNo(&((*raiz)->cen), *raiz, info, sobe);
            }
            else
            {
                maiorNo = inserirNo(&((*raiz)->dir), *raiz, info, sobe);
            }
            
            if (maiorNo != NULL) // Um filho quebrou, 'sobe' tem info
            {
                if ((*raiz)->quantInfo == 1) // Nó atual tem espaço
                {
                    adicionarInfo(raiz, *sobe, maiorNo);
                    // Atualiza o pai do nó que subiu
                    if (maiorNo) maiorNo->pai = *raiz;
                    maiorNo = NULL;
                } 
                else // Nó atual também está cheio, quebra de novo
                {
                    maiorNo = quebrarNo(raiz, *sobe, sobe, maiorNo);
                    if (pai == NULL) // É a raiz, cria nova raiz
                    {
                        *raiz = criarNo(*sobe, *raiz, maiorNo);
                        // Atualiza o pai dos filhos
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

// Agora busca por um 'titulo' (string)
int verificaInfos(Album *raiz, const char* titulo) 
{
    int existe = 0;
    // Compara usando strcmp com o título
    if (strcmp(raiz->infoUm.titulo, titulo) == 0) 
    {
        existe = 1;      
    }
    else if(raiz->quantInfo == 2)
    {   
        // Compara usando strcmp com o título
        if(strcmp(raiz->infoDois.titulo, titulo) == 0)
        {
            existe = 1;
        }
    }
    return existe;
}

void imprimirArv(Album *raiz, int nivel) 
{
    if (raiz == NULL) return;
    
    imprimirArv(raiz->dir, nivel + 1);
    
    for (int i = 0; i < nivel; i++)
    {
        printf("    "); // Espaçamento
    }
    
    if (raiz->quantInfo == 1)
    {
        // Imprime o 'titulo'
        printf("[%s]\n", raiz->infoUm.titulo);
    }
    else
    {
        // Imprime os dois 'titulos'
        printf("[%s | %s]\n", raiz->infoUm.titulo, raiz->infoDois.titulo);
    }
    
    imprimirArv(raiz->cen, nivel + 1);
    imprimirArv(raiz->esq, nivel + 1);
}

void liberarArv(Album **raiz)
{
    if (*raiz) 
    {
        liberarArv(&((*raiz)->esq));
        liberarArv(&((*raiz)->cen));
        if ((*raiz)->quantInfo == 2)
        {
            liberarArv(&((*raiz)->dir));
        }
        free(*raiz);
        *raiz = NULL;
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

int main() {
    Album *raiz = NULL;
    infoAlbum albumInfo;
    infoAlbum sobe;
    int opcao;
    char tituloBusca[TAM_STRING];
    Album *resultado;

    do {
        printf("\n--- MENU ÁRVORE 2-3 DE ÁLBUNS ---\n");
        printf("1 - Inserir álbum\n");
        printf("2 - Mostrar árvore\n");
        printf("3 - Buscar álbum por título\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // remove \n pendente

        switch (opcao) {
            case 1:
                printf("\nTítulo do álbum: ");
                fgets(albumInfo.titulo, TAM_STRING, stdin);
                albumInfo.titulo[strcspn(albumInfo.titulo, "\n")] = '\0';

                printf("Ano de lançamento: ");
                scanf("%d", &albumInfo.anoLancamento);

                printf("Quantidade de músicas: ");
                scanf("%d", &albumInfo.quantMusica);
                getchar();

                albumInfo.musica = NULL;

                inserirNo(&raiz, NULL, albumInfo, &sobe);
                printf("✔ Álbum inserido com sucesso!\n");
                break;

            case 2:
                if (raiz == NULL)
                    printf("\nÁrvore vazia!\n");
                else {
                    printf("\n=== Árvore 2-3 de Álbuns ===\n");
                    imprimirArv(raiz, 0);
                }
                break;

            case 3:
                printf("\nDigite o título do álbum a buscar: ");
                fgets(tituloBusca, TAM_STRING, stdin);
                tituloBusca[strcspn(tituloBusca, "\n")] = '\0';

                resultado = buscarAlbum(raiz, tituloBusca);

                if (resultado != NULL) {
                    printf("\n📀 Álbum encontrado!\n");
                    if (strcmp(resultado->infoUm.titulo, tituloBusca) == 0) {
                        printf("Título: %s\n", resultado->infoUm.titulo);
                        printf("Ano: %d\n", resultado->infoUm.anoLancamento);
                        printf("Músicas: %d\n", resultado->infoUm.quantMusica);
                    } else {
                        printf("Título: %s\n", resultado->infoDois.titulo);
                        printf("Ano: %d\n", resultado->infoDois.anoLancamento);
                        printf("Músicas: %d\n", resultado->infoDois.quantMusica);
                    }
                } else {
                    printf("\n❌ Álbum não encontrado!\n");
                }
                break;

            case 0:
                printf("\nEncerrando...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    liberarArv(&raiz);
    return 0;
}
