#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define preto 0
#define vermelho 1

typedef struct ArvRubNeg {
    int info;
    int cor;
    struct ArvRubNeg *esq, *dir;
} ArvRubNeg;

ArvRubNeg* criarNo(int dado) {
    ArvRubNeg *novoNo = (ArvRubNeg*) malloc(sizeof(ArvRubNeg));
    if (novoNo != NULL) {
        novoNo->info = dado;
        novoNo->esq = NULL;
        novoNo->dir = NULL;
        novoNo->cor = vermelho;
    }
    return novoNo;
}

void liberarArvore(ArvRubNeg *r) {
    if (r != NULL) {
        liberarArvore(r->esq);
        liberarArvore(r->dir);
        free(r);
    }
}

ArvRubNeg* buscaNo(ArvRubNeg *r, int valor) 
{
    ArvRubNeg *no;

    if (r->info == valor) 
        no = r;
    else if (r->info > valor)
        no = buscaNo(r->esq, valor);
    else if (r->info < valor)
        no = buscaNo(r->dir, valor);
    else 
        no = NULL;
    
    return no;
}

int cor(ArvRubNeg *r) {
    int cor = preto;

    if (r) {
        cor = (*r).cor;
    }

    return cor;
}

void rotEsq(ArvRubNeg **r) {
    ArvRubNeg *aux = (**r).dir;

    (**r).dir = (*aux).esq;
    (*aux).esq = *r;
    (*aux).cor = (**r).cor;
    (**r).cor = vermelho;
    (*r) = aux;
}

void rotDir(ArvRubNeg **r) {
    ArvRubNeg *aux = (**r).esq;

    (**r).esq = (*aux).dir;
    (*aux).dir = *r;
    (*aux).cor = (**r).cor;
    (**r).cor = vermelho;
    (*r) = aux;
}

void trocaCor(ArvRubNeg **r) {
    (**r).cor = !((**r).cor);
    (**r).esq->cor = !((**r).esq->cor);
    (**r).dir->cor = !((**r).dir->cor);
}

void balanceamento(ArvRubNeg **r) 
{
    if (cor((**r).esq) == preto && cor((**r).dir) == vermelho)
    {
        rotEsq(r);
    } 

    if (cor((**r).esq) == vermelho) if (cor((**r).esq->esq) == vermelho)
    {
        rotDir(r);
    } 

    if (cor((**r).esq) == vermelho && cor((**r).dir) == vermelho)
    {
        trocaCor(r); 
    } 
}

int insereNo(ArvRubNeg **r, ArvRubNeg *novoNo) {
    int inseriu = 1;

    if (*r == NULL) 
        *r = novoNo;
    else if ((**r).info > (*novoNo).info) 
        inseriu = insereNo(&((**r).esq), novoNo);
    else if ((**r).info < (*novoNo).info) 
        inseriu = insereNo(&((**r).dir), novoNo);
    else 
        inseriu = 0;
    
    if (*r && inseriu) balanceamento(r);

    return inseriu;
}

int  insercao(ArvRubNeg **r, ArvRubNeg *novoNo) {
    int inseriu = insereNo(r, novoNo);

    if (inseriu) (**r).cor = preto;

    return inseriu;
}

ArvRubNeg* moveTwoEsqRed(ArvRubNeg *r) {
    trocaCor(&r);

    if (cor((*r).dir->esq) == vermelho) {
        rotDir(&((*r).dir));
        rotEsq(&r);
        trocaCor(&r);
    }

    return r;
}

ArvRubNeg* moveTwoDirRed(ArvRubNeg *r) {
    trocaCor(&r);

    if (cor((*r).esq->esq) == vermelho) {
        rotDir(&r);
        trocaCor(&r);
    }

    return r;
}

ArvRubNeg *removeMenor(ArvRubNeg *r) {
    if ((*r).esq == NULL) {
        free (r);
        return NULL;
    }

    if (cor((*r).esq) == preto && cor((*r).esq->esq) == preto) r =  moveTwoEsqRed((*r).esq);

    (*r).esq = removeMenor((*r).esq);
    balanceamento(&r);

    return r;
}

ArvRubNeg* procuraMenor(ArvRubNeg *r) {
    ArvRubNeg *aux = r;

    while (aux->esq != NULL) aux = aux->esq;

    return aux;
}

ArvRubNeg* removeNo(ArvRubNeg *r, int valor) {
    if (valor < (*r).info) {
        if (cor((*r).esq) == preto && cor((*r).esq->esq) == preto) r = moveTwoEsqRed(r);
        
        (*r).esq = removeNo((*r).esq, valor);

    } else {
        if (cor((*r).esq) == vermelho) rotDir(&r);

        if (valor == (*r).info && ((*r).dir) == NULL) {
            free(r);
            return NULL;
        }

        if (cor((*r).dir) == preto && cor((*r).dir->esq) == preto) r = moveTwoDirRed(r);

        if (valor == (*r).info) {
            ArvRubNeg *menor = procuraMenor((*r).dir);
            (*r).info = (*menor).info;
            (*r).dir = removeMenor((*r).dir);
        } else 
            (*r).dir = removeNo((*r).dir, valor);
    }

    balanceamento(&r);

    return r;
} 

int main() {
    printf("inicializando a main...\n");

    ArvRubNeg *raiz = NULL;

    int valores[] = {1000, 300, 250, 200, 350, 2000, 3000, 3500, 3200, 1500, 1250, 1700, 1300, 100};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < 14; i++) {
        insercao(&raiz, criarNo(valores[i]));
    }

    printf("\nárvore antes da remoção:\n");
    imprimirArvore(raiz, 0);

    printf("\n");

    raiz = removeNo(raiz, 200);

    printf("\nárvore depois da remoção:\n");
    imprimirArvore(raiz, 0);

    printf("\n");

    liberarArvore(raiz);
    return 0;
}

