#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "artista.h"

// Aloca e inicializa um nó 2–3 com 1 informação (infoUm = dados),
// sem filhos (esq/cen/dir = NULL) e Ninfos = 1.
Artista *alocaArtista(infoArtista dados)
{
    Artista *novoNo = (Artista *) malloc(sizeof(Artista)); // aloca memória
    if (novoNo != NULL)
    {
        novoNo->infoUm = dados;  // coloca a info no primeiro slot
        novoNo->Ninfos = 1;      // nó começa com 1 informação
        novoNo->esq = NULL;      // sem filhos
        novoNo->cen = NULL; 
        novoNo->dir = NULL;
    }
    return novoNo; // devolve o ponteiro (ou NULL se malloc falhou)
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

// Insere 'dado' em um nó que tem APENAS 1 informação (Ninfos == 1),
// ajustando a posição (infoUm/infoDois) e a ligação dos filhos.
// 'subArvInfo' é o filho direito associado ao elemento que está sendo inserido/promovido.
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
    int cmp2 = strcmp(dado.nome, (*no)->infoDois.nome);
    int cmp1 = strcmp(dado.nome, (*no)->infoUm.nome);

    if (cmp2 > 0) {
        // caso 1: dado é o MAIOR dos três
        *sobe = (*no)->infoDois;        // sobe a info do meio (aqui o meio é infoDois antigo)
        maior = alocaArtista(dado);     // novo nó da direita fica com 'dado' (maior)
        // filhos do novo nó: esq recebe o antigo dir do nó quebrado; cen recebe filhoDir (que veio da recursão)
        maior->esq = (*no)->dir;
        maior->cen = filhoDir;
        // nó original reduz para ficar só com infoUm (menor)
        (*no)->Ninfos = 1;
        (*no)->dir = NULL; // opcional: por higiene, já que o nó volta a ter só esq/cen
    } else if (cmp1 > 0) {
        // caso 2: dado fica no MEIO
        *sobe = dado;                   // o próprio 'dado' sobe
        maior = alocaArtista((*no)->infoDois); // novo nó da direita recebe a maior info antiga
        // o filho esquerdo do novo nó é 'filhoDir' (direito do promovido),
        // e o filho central é o antigo 'dir' do nó quebrado
        maior->esq = filhoDir;
        maior->cen = (*no)->dir;
        // nó original permanece com infoUm (menor)
        (*no)->Ninfos = 1;
        (*no)->dir = NULL; // por higiene
    } else {
        // caso 3: dado é o MENOR dos três
        *sobe = (*no)->infoUm;            // sobe a menor info antiga (que vira a do meio global)
        maior = alocaArtista((*no)->infoDois); // novo nó da direita recebe a maior info antiga
        // filhos do novo nó da direita: herdará os dois filhos mais à direita do nó original
        maior->esq = (*no)->cen;
        maior->cen = (*no)->dir;
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
        *raiz = alocaArtista(dado);
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
                // Quebra aconteceu na RAIZ: precisamos criar uma NOVA RAIZ.
                // CORREÇÃO: guardar a raiz antiga antes de sobrescrever.
                Artista *aux = *raiz;          // guarda a raiz antiga (após 'quebrar', *raiz aponta para o nó esquerdo)
                *raiz = alocaArtista(*sobe);       // nova raiz com a chave promovida
                (*raiz)->esq = aux;            // filho esquerdo da nova raiz = raiz antiga (menor)
                (*raiz)->cen = maiorNo;            // filho central = novo nó da direita (maior)
                maiorNo = NULL;                    // já foi encaixado
            }
            // se não é raiz, devolvemos 'maiorNo' e '*sobe' para o pai lidar
        }
    } else {
        // estamos em um NÓ INTERNO: decidir por qual filho descer
        int cmp = strcmp(dado.nome, (*raiz)->infoUm.nome);
        if (cmp < 0) {
            // desce para a esquerda
            maiorNo = inserirArtista(&(*raiz)->esq, *raiz, dado, sobe);
        } else if ((*raiz)->Ninfos == 1 || strcmp(dado.nome, (*raiz)->infoDois.nome) < 0) {
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
                    Artista *aux = *raiz;       // guarda o nó esquerdo pós-quebra
                    *raiz = alocaArtista(*sobe);    // nova raiz com a promovida
                    (*raiz)->esq = aux;         // filho esquerdo = nó esquerdo
                    (*raiz)->cen = maiorNo;         // filho central = novo nó da direita
                    maiorNo = NULL;                 // já foi encaixado
                }
                // Se não é raiz, devolvemos 'maiorNo' + '*sobe' para o pai
            }
        }
    }

    return maiorNo; // se NULL, nada a propagar; se não, pai precisa encaixar
}

void imprimirArtistas(Artista *raiz) 
{
    if (raiz != NULL) {
        // Percorre a subárvore esquerda
        imprimirArtistas(raiz->esq);

        // Imprime a primeira informação
        printf("Nome: %s | Estilo: %s | Tipo: %s | Nº Albuns: %d\n",
               raiz->infoUm.nome,
               raiz->infoUm.estilo,
               raiz->infoUm.tipo,
               raiz->infoUm.numAlbuns);

        // Percorre a subárvore central
        imprimirArtistas(raiz->cen);

        // Se houver uma segunda informação, imprime-a
        if (raiz->Ninfos == 2) {
            printf("Nome: %s | Estilo: %s | Tipo: %s | Nº Albuns: %d\n",
                   raiz->infoDois.nome,
                   raiz->infoDois.estilo,
                   raiz->infoDois.tipo,
                   raiz->infoDois.numAlbuns);

            // Percorre a subárvore direita
            imprimirArtistas(raiz->dir);
        }
    }
}

Artista *buscarArtista(Artista *raiz, const char *nome)
{
    Artista *resultado = NULL; // ponteiro que será retornado no final

    if (raiz != NULL)
    {
        int cmp1 = strcmp(nome, raiz->infoUm.nome);
        int cmp2 = 0; // só usamos se o nó tiver duas informações

        if (cmp1 == 0) {
            // Encontrou na primeira informação
            resultado = raiz;
        }
        else
        {
            if (raiz->Ninfos == 2)
                cmp2 = strcmp(nome, raiz->infoDois.nome);

            if (raiz->Ninfos == 2 && cmp2 == 0) {
                // Encontrou na segunda informação
                resultado = raiz;
            }
            else if (cmp1 < 0) {
                // Nome é menor que infoUm → vai pra esquerda
                resultado = buscarArtista(raiz->esq, nome);
            }
            else if (raiz->Ninfos == 1 || (raiz->Ninfos == 2 && cmp2 < 0)) {
                // Está entre infoUm e infoDois → vai pro centro
                resultado = buscarArtista(raiz->cen, nome);
            }
            else {
                // Nome é maior que infoDois → vai pra direita
                resultado = buscarArtista(raiz->dir, nome);
            }
        }
    }

    return resultado; // único ponto de retorno
}


void liberarArtistas(Artista *raiz) 
{
    if (raiz != NULL) {
        // Libera as subárvores
        liberarArtistas(raiz->esq);
        liberarArtistas(raiz->cen);
        liberarArtistas(raiz->dir);

        // Libera o nó atual
        free(raiz);
    }
}

// ==== MAIN DE TESTE ====
int main(void)
{
    Artista *raiz = NULL;
    infoArtista sobe;  // usado pela inserirArtista

    infoArtista a = {"Chico", "MPB",  "Solo", 5};
    infoArtista b = {"Ana",   "Pop",  "Banda",2};
    infoArtista c = {"Zeca",  "Samba","Solo", 7};
    infoArtista d = {"Beto",  "Rock", "Solo", 1};
    infoArtista e = {"Lia",   "Indie","Solo", 3};
    infoArtista f = {"Rafa",  "Jazz", "Banda",4};

    // Inserções (sem criar função nova)
    inserirArtista(&raiz, NULL, a, &sobe);
    inserirArtista(&raiz, NULL, b, &sobe);
    inserirArtista(&raiz, NULL, c, &sobe);
    inserirArtista(&raiz, NULL, d, &sobe);
    inserirArtista(&raiz, NULL, e, &sobe);
    inserirArtista(&raiz, NULL, f, &sobe);

    printf(">>> Em ordem:\n");
    imprimirArtistas(raiz);   // já é sua função

    // Busca (usando sua função)
    Artista *encontrado = buscarArtista(raiz, "Ana");
    if (encontrado)
        printf("\nAchei o artista: %s (%s)\n",
               // atenção: pode estar em infoUm ou infoDois
               (strcmp("Ana", encontrado->infoUm.nome) == 0) ? encontrado->infoUm.nome : encontrado->infoDois.nome,
               (strcmp("Ana", encontrado->infoUm.nome) == 0) ? encontrado->infoUm.estilo : encontrado->infoDois.estilo);
    else
        printf("\nArtista não encontrado.\n");

    // Libera memória (sua função)
    liberarArtistas(raiz);
    raiz = NULL;

    return 0;
}
