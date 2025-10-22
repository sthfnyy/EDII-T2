#include <stdio.h>
#include <stdlib.h>

// CÓDIGO DO DIABO

typedef struct DoisTres 
{
    int infoUm, infoDois, quantInfo;
    struct DoisTres *esq, *cen, *dir;
}DoisTres;

DoisTres* criarNo(int info, DoisTres *fEsq, DoisTres *fCen)
{
    DoisTres *no;
    no = (DoisTres*) malloc(sizeof(DoisTres));

    if (no != NULL)
    {
        no->infoUm = info;
        no->esq = fEsq;
        no->cen = fCen;
        no->dir = NULL;
        no->quantInfo = 1;
    }
    return no;
}

void imprimirArv(DoisTres *raiz, int nivel) 
{
    if (raiz == NULL) return;
    // imprime da direita pra esquerda (como uma árvore "virada")
    imprimirArv(raiz->dir, nivel + 1);
    for (int i = 0; i < nivel; i++)
    {
        printf("   "); //espaçamento visual 
    }
    if (raiz->quantInfo == 1)
    {
        printf("[%d]\n", raiz->infoUm);
    }
    else
    {
        printf("[%d|%d]\n", raiz->infoUm, raiz->infoDois);
    }
    imprimirArv(raiz->cen, nivel + 1);
    imprimirArv(raiz->esq, nivel + 1);
}

void liberarArv(DoisTres **raiz)
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

void adicionarInfo(DoisTres **no, int info, DoisTres *subArvInfo)
{
    if (info > (*no)->infoUm)
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

DoisTres* quebrarNo(DoisTres **no, int info, int *sobe, DoisTres *filhoDir)
{
    DoisTres *maior;
    if(info > (*no)->infoDois)
    {
        *sobe = (*no)->infoDois;
        maior = criarNo(info, (*no)->dir, filhoDir);
        (*no)->quantInfo = 1;
    }
    else if(info > (*no)->infoUm) 
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
    return maior;
}

DoisTres* inserirNo(DoisTres **raiz, DoisTres *pai, int info, int *sobe) 
{
    DoisTres *maiorNo = NULL;
    if (*raiz == NULL)
    {
        *raiz = criarNo(info, NULL, NULL);
    }
    else 
    {
        if ((*raiz)->esq == NULL) 
        {
            if ((*raiz)->quantInfo == 1) 
            {
                adicionarInfo(raiz, info, NULL);
            } 
            else  
            {
                maiorNo = quebrarNo(raiz, info, sobe, NULL);
                if (pai == NULL) 
                {
                    *raiz = criarNo(*sobe, *raiz, maiorNo);
                    maiorNo = NULL;
                }
            }
        } 
        else 
        {
            if(info < (*raiz)->infoUm)
            {
                maiorNo = inserirNo(&((*raiz)->esq), *raiz, info, sobe);
            }
            else if((*raiz)->quantInfo == 1 || info < (*raiz)->infoDois)
            {
                maiorNo = inserirNo(&((*raiz)->cen), *raiz, info, sobe);
            }
            else
            {
                maiorNo = inserirNo(&((*raiz)->dir), *raiz, info, sobe);
            }
            if (maiorNo != NULL) 
            {
                if ((*raiz)->quantInfo == 1) 
                {
                    adicionarInfo(raiz, *sobe, maiorNo);
                    maiorNo = NULL;
                } 
                else 
                {
                    maiorNo = quebrarNo(raiz, *sobe, sobe, maiorNo);
                    if (pai == NULL) 
                    {
                        *raiz = criarNo(*sobe, *raiz, maiorNo);
                        maiorNo = NULL;
                    }
                }
            }
        }
    }
    return maiorNo;
}

int verificaInfos(DoisTres *raiz, int valor) 
{
    int existe = 0;
    if (raiz->infoUm == valor /*|| raiz->infoDois == valor*/) 
    {
        existe = 1;        
    }
    else if(raiz->quantInfo == 2)
    {   
        if(raiz->infoDois == valor)
        {
            existe = 1;
        }
    }
    return existe;
}

int remover(DoisTres **raiz, DoisTres *pai, int valor) 
{
    if(*raiz) 
    {
        if(verificaInfos(*raiz, valor)) //verifica se o valor esta no No
        {
            if((*raiz)->esq == NULL) //No folha
            {
                if((*raiz)->quantInfo == 2) //2 infos
                {
                    (*raiz)->quantInfo = 1; 
                    if ((*raiz)->infoUm == valor)
                    {
                        (*raiz)->infoUm = (*raiz)->infoDois;
                    }   
                } 
                else // folha com uma info
                { 
                    
                }
            } 
            else // não eh folha
            { 
                if((*raiz)->quantInfo == 2) //com duas infos
                {
                    if((*raiz)->infoDois == valor) //remover info 2
                    {
                        if((*raiz)->dir->quantInfo == 2) //confere a dir com 2 infos
                        {
                            (*raiz)->infoDois = (*raiz)->dir->infoUm;
                            (*raiz)->dir->infoUm = (*raiz)->dir->infoDois;
                            (*raiz)->dir->quantInfo = 1;
                        }
                        else if((*raiz)->dir->quantInfo == 1 && (*raiz)->cen->quantInfo == 2) //confere a dir com 1 info e o centro com 2 infos
                        {
                            (*raiz)->infoDois = (*raiz)->cen->infoDois;
                            (*raiz)->cen->quantInfo = 1;
                        }
                        else if((*raiz)->dir->quantInfo == 1 && (*raiz)->cen->quantInfo == 1) //confere a dir e o centro com 1 info 
                        {
                            (*raiz)->cen->infoDois = (*raiz)->dir->infoUm;
                            DoisTres *aux = (*raiz)->dir;
                            free(aux);
                            (*raiz)->dir = NULL;
                            (*raiz)->quantInfo = 1;
                            (*raiz)->cen->quantInfo = 2;
                        }
                    }
                    else //remover info 1
                    {
                        if((*raiz)->cen->quantInfo == 2)// confere centro com 2 info
                        {
                            (*raiz)->infoUm = (*raiz)->cen->infoUm;
                            (*raiz)->cen->infoUm = (*raiz)->cen->infoDois;
                            (*raiz)->cen->quantInfo = 1;
                        } 
                        else if((*raiz)->cen->quantInfo == 1 && (*raiz)->dir->quantInfo == 2) //centro com 1 e dir com 2
                        {
                            (*raiz)->infoUm = (*raiz)->cen->infoUm;
                            (*raiz)->cen->infoUm = (*raiz)->infoDois;
                            (*raiz)->infoDois = (*raiz)->dir->infoUm;
                            (*raiz)->dir->infoUm = (*raiz)->dir->infoDois;
                            (*raiz)->dir->quantInfo = 1;
                        }
                        else if((*raiz)->cen->quantInfo == 1 && (*raiz)->dir->quantInfo == 1) //centro e dir com 1 info
                        {
                            (*raiz)->infoUm = (*raiz)->cen->infoUm;
                            (*raiz)->cen->infoUm = (*raiz)->infoDois;
                            (*raiz)->cen->infoDois = (*raiz)->dir->infoUm;
                            (*raiz)->cen->quantInfo = 2;
                            (*raiz)->quantInfo = 1;
                            DoisTres *aux;
                            aux = (*raiz)->dir;
                            (*raiz)->dir = NULL;
                            free(aux);
                        }
                    }
                }
            }
        }
    }
}

int main() {
    printf("inicializando a main...\n");

    DoisTres *raiz = NULL;
    int sobe;

    int valores[] = {1000, 300, 250, 200, 350, 2000, 3000, 3500, 3200, 1500, 1250, 1100, 1200, 1700, 1300, 100};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        inserirNo(&raiz, NULL, valores[i], &sobe);
    }
    
    imprimirArv(raiz, 0);

    printf("\n");

    /* raiz = removeNo(raiz, 2000);

    printf("\nárvore depois da remoção:\n");
    imprimirArvore(raiz, 0);
    */

    printf("\n");

    liberarArv(&raiz);
    return 0;
}