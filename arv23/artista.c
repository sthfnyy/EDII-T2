//VERSÃO OFICIAL

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "artista.h"


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

// Lê do teclado os campos de infoArtista e inicializa numAlbuns
infoArtista preencherArtista(void)
{
    infoArtista dados;
    printf("Digite o nome do artista: ");   scanf("%49s", dados.nome);
    printf("Digite o estilo do artista: "); scanf("%49s", dados.estilo);
    printf("Digite o tipo: ");              scanf("%49s", dados.tipo);
    dados.numAlbuns = 0;
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
            printf("   "); // espaçamento visual 
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

void liberarArv(Artista **raiz)
{
    if (*raiz) 
    {
        liberarArv(&((*raiz)->esq));
        liberarArv(&((*raiz)->cen));
        if ((*raiz)->Ninfos == 2)
        {
            liberarArv(&((*raiz)->dir));
        }
        free(*raiz);
        *raiz = NULL;
    }
}

void inserir23(Artista **novoNo, infoArtista dado, Artista *subArvInfo) 
{
    int cmp = strcmp(dado.nome, (*novoNo)->infoUm.nome); // compara nomes para ordenar
    
    if (cmp > 0) {
        // dado > infoUm  -> dado vai para infoDois (posição da direita)
        (*novoNo)->infoDois = dado;
        // como inserimos “à direita”, o filho direito do nó passa a ser 'subArvInfo'
        (*novoNo)->dir = subArvInfo;
    } else {
        // dado <= infoUm -> empurra infoUm para infoDois e coloca dado em infoUm
        (*novoNo)->infoDois = (*novoNo)->infoUm;
        (*novoNo)->infoUm = dado;
        // ao deslocar as infos, também deslocamos os filhos:
        // o antigo 'cen' vira 'dir', e 'subArvInfo' vira o novo 'cen'
        (*novoNo)->dir = (*novoNo)->cen;
        (*novoNo)->cen = subArvInfo;
    }
    (*novoNo)->Ninfos = 2; // agora o nó tem 2 infos
}

// Quebra (split) um nó que já está cheio (Ninfos == 2) para inserir 'dado'.
// Define em '*sobe' a info do meio (que vai subir para o pai)
// e retorna um novo nó (o "nó da direita" após o split).
// 'filhoDir' é o filho direito que acompanha a info promovida vinda de baixo.
Artista *quebrarNoArtista(Artista **no, infoArtista dado, infoArtista *sobe, Artista *filhoDir) 
{
    Artista *maior; // novo nó da direita que será retornado

    // Compara 'dado' com as duas infos do nó para identificar se é menor, meio ou maior
    int cmp1 = strcmp(dado.nome, (*no)->infoUm.nome);
    int cmp2 = strcmp(dado.nome, (*no)->infoDois.nome);


    if (cmp2 > 0) {
        // caso 1: dado é o MAIOR dos três, (sobe : infoDois)
        *sobe = (*no)->infoDois;        // sobe a info do meio (aqui o meio é infoDois antigo)
        maior = alocaArtista(dado, (*no)->dir, filhoDir);     // novo nó da direita fica com 'dado' (maior)
        // nó original reduz para ficar só com infoUm (menor)
        (*no)->Ninfos = 1;
        (*no)->dir = NULL; // opcional: por higiene, já que o nó volta a ter só esq/cen
    } else if (cmp1 > 0) {
        // caso 2: dado fica no MEIO
        *sobe = dado;                   // o próprio 'dado' sobe
        maior = alocaArtista((*no)->infoDois, filhoDir, (*no)->dir); // novo nó da direita recebe a maior info antiga
        // nó original permanece com infoUm (menor)
        (*no)->Ninfos = 1;
        (*no)->dir = NULL; // por higiene
    } else {
        // caso 3: dado é o MENOR dos três
        *sobe = (*no)->infoUm;            // sobe a menor info antiga (que vira a do meio global)
        maior = alocaArtista((*no)->infoDois, (*no)->cen, (*no)->dir); // novo nó da direita recebe a maior info antiga
        // no nó original, substitui infoUm por 'dado' (agora o menor),
        // e o filho do meio vira 'filhoDir' (direito do promovido)
        (*no)->infoUm = dado;
        (*no)->cen = filhoDir;
        (*no)->Ninfos = 1;
        (*no)->dir = NULL; // por higiene
    }

    return maior; // retorna o novo nó da direita
}

// Insere 'dado' na árvore 2–3.
// Retorna um ponteiro para um "novo nó da direita" caso haja split que precise ser encaixado pelo pai;
// caso contrário, retorna NULL.
//
// Parâmetros:
// - raiz: ponteiro para o ponteiro da subárvore atual
// - pai:  ponteiro para o nó pai (NULL se raiz da árvore)
// - dado: info a ser inserida
// - sobe: saída por referência com a info promovida quando acontece split
Artista *inserirArtista(Artista **raiz, Artista *pai, infoArtista dado, infoArtista *sobe)
{
    Artista *maiorNo = NULL; // "novo nó da direita" a ser devolvido ao pai, se houver split

    if (*raiz == NULL) 
    {
        // caso base: subárvore vazia -> cria nó folha com 'dado'
        *raiz = alocaArtista(dado, NULL, NULL);
    } 
    else if ((*raiz)->esq == NULL) 
    {
        // estamos em uma FOLHA
        if ((*raiz)->Ninfos == 1) 
        {
            // folha com espaço: insere aqui mesmo
            inserir23(raiz, dado, NULL);
        } else {
            // folha cheia: precisa quebrar
            maiorNo = quebrarNoArtista(raiz, dado, sobe, NULL);
            if (pai == NULL) 
            {
                *raiz = alocaArtista(*sobe, *raiz, maiorNo);
                maiorNo = NULL; // encaixado, nada a propagar
            }
            // se não é raiz, devolvemos 'maiorNo' e '*sobe' para o pai lidar
        }
    } else 
    {
        // estamos em um NÓ INTERNO: decidir por qual filho descer
        int cmp1 = strcmp(dado.nome, (*raiz)->infoUm.nome);
        int cmp2 = strcmp(dado.nome, (*raiz)->infoDois.nome);
        if (cmp1 < 0) 
        {
            // desce para a esquerda
            maiorNo = inserirArtista(&(*raiz)->esq, *raiz, dado, sobe);
        } else if ((*raiz)->Ninfos == 1 || cmp2 < 0) {
            // desce para o meio (se só 1 info, meio é o único “à direita do primeiro”)
            maiorNo = inserirArtista(&(*raiz)->cen, *raiz, dado, sobe);
        } else {
            // desce para a direita
            maiorNo = inserirArtista(&(*raiz)->dir, *raiz, dado, sobe);
        }
        // Ao voltar da recursão, pode ter vindo um split do filho (maiorNo != NULL)
        if (maiorNo != NULL) {
            if ((*raiz)->Ninfos == 1) {
                // nó atual tem espaço: insere a promovida e liga o 'maiorNo' como filho correto
                inserir23(raiz, *sobe, maiorNo);
                maiorNo = NULL; // encaixado, nada a propagar
            } else {
                // nó atual está cheio: precisa quebrar também
                maiorNo = quebrarNoArtista(raiz, *sobe, sobe, maiorNo);

                if (pai == NULL) {
                    // Quebra em RAIZ: criar nova raiz com a info promovida
                    *raiz = alocaArtista(*sobe, *raiz, maiorNo);
                    maiorNo = NULL; // já foi encaixado
                }
                // Se não é raiz, devolvemos 'maiorNo' + '*sobe' para o pai
            }
        }
    }

    return maiorNo; // se NULL, nada a propagar; se não, pai precisa encaixar
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


// int main(void)
// {
//     Artista *raiz = NULL;
//     infoArtista sobe;  // usado por inserirArtista

//     // Preenche com alguns artistas
//     infoArtista a = {"Chico", "MPB",  "Solo", 5};
//     infoArtista b = {"Ana",   "Pop",  "Banda",2};
//     infoArtista c = {"Zeca",  "Samba","Solo", 7};
//     infoArtista d = {"Beto",  "Rock", "Solo", 1};
//     infoArtista e = {"Lia",   "Indie","Solo", 3};
//     infoArtista f = {"Rafa",  "Jazz", "Banda",4};

//     inserirArtista(&raiz, NULL, a, &sobe);
//     inserirArtista(&raiz, NULL, b, &sobe);
//     inserirArtista(&raiz, NULL, c, &sobe);
//     inserirArtista(&raiz, NULL, d, &sobe);
//     inserirArtista(&raiz, NULL, e, &sobe);
//     inserirArtista(&raiz, NULL, f, &sobe);

//     // Mostra a árvore “deitada”
//     printf(">>> Estrutura da arvore (direita -> esquerda):\n");
//     if (raiz) imprimirArv(raiz, 0); else puts("(arvore vazia)");

//     // Testes de existencia (verificaDados)
//     const char *testes[] = {"Ana", "Zeca", "Xuxa", "Lia", "Beto"};
//     for (int i = 0; i < (int)(sizeof(testes)/sizeof(testes[0])); i++) {
//         printf("[VERIFICA] '%s': %s\n",
//                testes[i],
//                verificaDados(raiz, testes[i]) ? "existe" : "nao existe");
//     }

//     // Libera memória usando a função que você tem
//     liberarArv(&raiz);
//     return 0;
// }
