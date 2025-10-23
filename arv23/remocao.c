//TESTE PARA VER SE CONSIGO FAZER A REMOÇÃO

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
    int cmp1 = strcmp(dado.nome, (*no)->infoUm.nome);
    int cmp2 = strcmp(dado.nome, (*no)->infoDois.nome);


    if (cmp2 > 0) {
        // caso 1: dado é o MAIOR dos três, (sobe : infoDois)
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
                Artista *aux = *raiz;             // guarda a raiz antiga (após 'quebrar', *raiz aponta para o nó esquerdo)
                *raiz = alocaArtista(*sobe);       // nova raiz com a chave promovida
                (*raiz)->esq = aux;                // filho esquerdo da nova raiz = raiz antiga (menor)
                (*raiz)->cen = maiorNo;            // filho central = novo nó da direita (maior)
                maiorNo = NULL;                    // já foi encaixado
            }
            // se não é raiz, devolvemos 'maiorNo' e '*sobe' para o pai lidar
        }
    } else {
        // estamos em um NÓ INTERNO: decidir por qual filho descer
        int cmp = strcmp(dado.nome, (*raiz)->infoUm.nome);
        int cmp2 = strcmp(dado.nome, (*raiz)->infoDois.nome);
        if (cmp < 0) {
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

void imprimirArtistasInOrdem(Artista *raiz) 
{
    if (raiz != NULL) {
        // Percorre a subárvore esquerda
        imprimirArtistasInOrdem(raiz->esq);

        // Imprime a primeira informação
        printf("Nome: %s | Estilo: %s | Tipo: %s | Nº Albuns: %d\n",
               raiz->infoUm.nome,
               raiz->infoUm.estilo,
               raiz->infoUm.tipo,
               raiz->infoUm.numAlbuns);

        // Percorre a subárvore central
        imprimirArtistasInOrdem(raiz->cen);

        // Se houver uma segunda informação, imprime-a
        if (raiz->Ninfos == 2) {
            printf("Nome: %s | Estilo: %s | Tipo: %s | Nº Albuns: %d\n",
                   raiz->infoDois.nome,
                   raiz->infoDois.estilo,
                   raiz->infoDois.tipo,
                   raiz->infoDois.numAlbuns);

            // Percorre a subárvore direita
            imprimirArtistasInOrdem(raiz->dir);
        }
    }
}

void imprimirInOrdemArtistaComRaiz(Artista *raiz) 
{
    printf("Valores em ordem: \n");
    imprimirArtistasInOrdem(raiz);
    printf("\n");

    if (raiz) {
        printf("Raiz: ");
        if (raiz->Ninfos == 1)
            printf("[%s]\n", raiz->infoUm.nome);
        else
            printf("[%s | %s]\n", raiz->infoUm.nome, raiz->infoDois.nome);
    } else {
        printf("Árvore vazia.\n");
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

//=====DAQUI PRA BAIXO COMEÇA BAGUNÇA=====
//=====REMOÇÃO=======


// ---------- Função auxiliar: verifica se nó é folha ----------
int ehFolha(Artista *no) {
    return (no && !no->esq && !no->cen && !no->dir);
}

// ---------- Busca o predecessor (maior à esquerda) ----------
infoArtista obterPredecessorArtista(Artista* no) {
    Artista *u = no;
    while (u->dir || u->cen) {
        if (u->dir) u = u->dir;
        else u = u->cen;
    }
    return (u->Ninfos == 2) ? u->infoDois : u->infoUm;
}

// ---------- Redistribuição e Fusão ----------

// 1️⃣ Empresta do irmão esquerdo
void redistribuiDaEsquerda(Artista *pai, int caso) {
    // caso==1 → filho do meio em underflow
    // caso==2 → filho direito em underflow
    if (caso == 1) {
        Artista *esq = pai->esq;
        Artista *cen = pai->cen;
        // desce infoUm pro cen
        cen->infoDois = cen->infoUm;
        cen->infoUm = pai->infoUm;
        cen->Ninfos = 2;
        // sobe infoDois do irmão esquerdo
        pai->infoUm = esq->infoDois;
        esq->Ninfos = 1;
        // reorganiza filhos se internos
        if (!ehFolha(esq)) {
            cen->dir = cen->cen;
            cen->cen = cen->esq;
            cen->esq = esq->dir;
            esq->dir = NULL;
        }
    } else { // caso==2, filho direito em underflow
        Artista *cen = pai->cen;
        Artista *dir = pai->dir;
        dir->infoDois = dir->infoUm;
        dir->infoUm = pai->infoDois;
        dir->Ninfos = 2;
        pai->infoDois = cen->infoDois;
        cen->Ninfos = 1;
        if (!ehFolha(cen)) {
            dir->dir = dir->cen;
            dir->cen = dir->esq;
            dir->esq = cen->dir;
            cen->dir = NULL;
        }
    }
}

// 2️⃣ Empresta do irmão direito
void redistribuiDaDireita(Artista *pai, int caso) {
    // caso==0 → filho esquerdo em underflow
    // caso==1 → filho do meio em underflow
    if (caso == 0) {
        Artista *esq = pai->esq;
        Artista *cen = pai->cen;
        esq->infoDois = pai->infoUm;
        esq->Ninfos = 2;
        pai->infoUm = cen->infoUm;
        cen->infoUm = cen->infoDois;
        cen->Ninfos = 1;
        if (!ehFolha(esq)) {
            esq->dir = cen->esq;
            cen->esq = cen->cen;
            cen->cen = cen->dir;
            cen->dir = NULL;
        }
    } else { // filho do meio em underflow e tem irmão direito
        Artista *cen = pai->cen;
        Artista *dir = pai->dir;
        cen->infoDois = pai->infoDois;
        cen->Ninfos = 2;
        pai->infoDois = dir->infoUm;
        dir->infoUm = dir->infoDois;
        dir->Ninfos = 1;
        if (!ehFolha(cen)) {
            cen->dir = dir->esq;
            dir->esq = dir->cen;
            dir->cen = dir->dir;
            dir->dir = NULL;
        }
    }
}

// 3️⃣ Fusão (merge)
void fundir(Artista *pai, int caso) {
    // 0=esq underflow, 1=cen, 2=dir
    if (caso == 0) {
        // funde esq(NULL) + infoUm + cen → cen = [infoUm, cen.infoUm]
        Artista *cen = pai->cen;
        cen->infoDois = cen->infoUm;
        cen->infoUm = pai->infoUm;
        cen->Ninfos = 2;
        pai->esq = cen;
        pai->cen = pai->dir;
        pai->dir = NULL;
        if (pai->Ninfos == 2) {
            pai->infoUm = pai->infoDois;
            pai->Ninfos = 1;
        } else pai->Ninfos = 0;
    } else if (caso == 1) {
        // funde esq + infoUm + cen(NULL)
        Artista *esq = pai->esq;
        esq->infoDois = pai->infoUm;
        esq->Ninfos = 2;
        if (pai->Ninfos == 2) {
            pai->infoUm = pai->infoDois;
            pai->cen = pai->dir;
            pai->dir = NULL;
            pai->Ninfos = 1;
        } else pai->Ninfos = 0;
    } else if (caso == 2) {
        // funde cen + infoDois + dir(NULL)
        Artista *cen = pai->cen;
        cen->infoDois = pai->infoDois;
        cen->Ninfos = 2;
        pai->dir = NULL;
        pai->Ninfos = 1;
    }
}

// ---------- Função recursiva principal ----------
// ---------- Função recursiva principal ----------
Artista* removerArtista(Artista *raiz, const char *nome, int *diminuiu) {
    if (diminuiu) *diminuiu = 0; 
    if (!raiz) return NULL;

    int cmp1 = strcmp(nome, raiz->infoUm.nome);
    int tem2 = (raiz->Ninfos == 2);
    int cmp2 = tem2 ? strcmp(nome, raiz->infoDois.nome) : 1;

    // ----- CASO FOLHA -----
    if (ehFolha(raiz)) {
        if (cmp1 == 0) {
            if (tem2) {
                raiz->infoUm = raiz->infoDois;
                raiz->Ninfos = 1;
                return raiz;
            } else {
                free(raiz);
                *diminuiu = 1;
                return NULL;
            }
        } else if (tem2 && cmp2 == 0) {
            raiz->Ninfos = 1;
            return raiz;
        } else {
            return raiz; // não encontrou
        }
    }

    // ----- CASO INTERNO -----
    if (cmp1 == 0) {
        // chave está em infoUm → trocar por predecessor e remover na ESQ
        infoArtista pred = obterPredecessorArtista(raiz->esq);
        raiz->infoUm = pred;
        raiz->esq = removerArtista(raiz->esq, pred.nome, diminuiu);

        // ✅ AJUSTE #1: underflow foi no FILHO ESQUERDO
        if (*diminuiu) {
            if (raiz->cen && raiz->cen->Ninfos == 2) {
                // empresta do CEN para o ESQ
                redistribuiDaDireita(raiz, 0);
                *diminuiu = 0;
            } else {
                // funde ESQ(NULL) + [infoUm] + CEN
                fundir(raiz, 0);
                *diminuiu = (raiz->Ninfos == 0);
            }
        }
        return raiz;
    }

    if (tem2 && cmp2 == 0) {
        // chave está em infoDois → trocar por predecessor da CEN e remover na CEN
        infoArtista pred = obterPredecessorArtista(raiz->cen);
        raiz->infoDois = pred;
        raiz->cen = removerArtista(raiz->cen, pred.nome, diminuiu);

        // ✅ AJUSTE #2: underflow foi no FILHO CENTRAL
        if (*diminuiu) {
            if (raiz->esq && raiz->esq->Ninfos == 2) {
                // empresta do ESQ para o CEN
                redistribuiDaEsquerda(raiz, 1);
                *diminuiu = 0;
            } else if (raiz->dir && raiz->dir->Ninfos == 2) {
                // empresta do DIR para o CEN
                redistribuiDaDireita(raiz, 1);
                *diminuiu = 0;
            } else {
                // funde ESQ + [infoUm] + CEN(NULL)
                fundir(raiz, 1);
                *diminuiu = (raiz->Ninfos == 0);
            }
        }
        return raiz;
    }

    // ----- DESCIDA NORMAL -----
    if (cmp1 < 0) {
        // desce à ESQ
        raiz->esq = removerArtista(raiz->esq, nome, diminuiu);
        if (*diminuiu) {
            if (raiz->cen && raiz->cen->Ninfos == 2) {
                redistribuiDaDireita(raiz, 0); // empresta do CEN p/ ESQ
                *diminuiu = 0;
            } else {
                fundir(raiz, 0);               // funde ESQ(NULL) + [infoUm] + CEN
                *diminuiu = (raiz->Ninfos == 0);
            }
        }
    } else if (!tem2 || cmp2 < 0) {
        // desce ao CEN
        raiz->cen = removerArtista(raiz->cen, nome, diminuiu);

        // ✅ AJUSTE #3: underflow no CEN
        if (*diminuiu) {
            if (raiz->esq && raiz->esq->Ninfos == 2) {
                redistribuiDaEsquerda(raiz, 1); // empresta do ESQ p/ CEN
                *diminuiu = 0;
            } else if (raiz->dir && raiz->dir->Ninfos == 2) {
                redistribuiDaDireita(raiz, 1);  // empresta do DIR p/ CEN
                *diminuiu = 0;
            } else {
                fundir(raiz, 1);                // funde ESQ + [infoUm] + CEN(NULL)
                *diminuiu = (raiz->Ninfos == 0);
            }
        }
    } else {
        // ✅ NOVO RAMO: desce à DIR
        raiz->dir = removerArtista(raiz->dir, nome, diminuiu);
        if (*diminuiu) {
            if (raiz->cen && raiz->cen->Ninfos == 2) {
                redistribuiDaEsquerda(raiz, 2); // empresta do CEN p/ DIR
                *diminuiu = 0;
            } else {
                fundir(raiz, 2);                // funde CEN + [infoDois] + DIR(NULL)
                *diminuiu = (raiz->Ninfos == 0);
            }
        }
    }

    // ----- COLAPSO DA RAIZ -----
    if (raiz->Ninfos == 0) {
        Artista *nova = raiz->esq ? raiz->esq : raiz->cen;
        free(raiz);
        *diminuiu = 1; // altura diminuiu
        return nova;
    }

    return raiz;
}




/*
Artista* removerArtista(Artista* raiz, const char* nome, int* diminuiu) {
    if (!raiz) return NULL;

    int cmp1 = strcmp(nome, raiz->infoUm.nome);
    int cmp2 = raiz->Ninfos == 2 ? strcmp(nome, raiz->infoDois.nome) : 1;

    // Caso 1: nome está em infoUm
    if (cmp1 == 0) {
        if (!raiz->esq) {
            // É folha
            if (raiz->Ninfos == 2) {
                raiz->infoUm = raiz->infoDois;
                raiz->Ninfos = 1;
            } else {
                free(raiz);
                *diminuiu = 1;
                return NULL;
            }
        } else {
            // Substituir por sucessor
            Artista* sucessor = raiz->cen;
            while (sucessor->esq)
                sucessor = sucessor->esq;
            raiz->infoUm = sucessor->infoUm;
            raiz->cen = removerArtista(raiz->cen, sucessor->infoUm.nome, diminuiu);
        }
    }

    // Caso 2: nome está em infoDois
    else if (cmp2 == 0 && raiz->Ninfos == 2) {
        if (!raiz->esq) {
            raiz->Ninfos = 1;
        } else {
            Artista* sucessor = raiz->dir;
            while (sucessor->esq)
                sucessor = sucessor->esq;
            raiz->infoDois = sucessor->infoUm;
            raiz->dir = removerArtista(raiz->dir, sucessor->infoUm.nome, diminuiu);
        }
    }

    // Caso 3: nome está em subárvore
    else {
        if (cmp1 < 0)
            raiz->esq = removerArtista(raiz->esq, nome, diminuiu);
        else if (raiz->Ninfos == 1 || cmp2 < 0)
            raiz->cen = removerArtista(raiz->cen, nome, diminuiu);
        else
            raiz->dir = removerArtista(raiz->dir, nome, diminuiu);
    }

    // Verifica se algum filho ficou vazio e faz fusão
    if (raiz->esq && raiz->esq->Ninfos == 0) {
        Artista* novo = raiz->cen;
        novo->infoUm = raiz->infoUm;
        novo->Ninfos = 2;
        novo->esq = raiz->esq;
        raiz = novo;
        *diminuiu = 1;
    } else if (raiz->cen && raiz->cen->Ninfos == 0) {
        Artista* novo = raiz->esq;
        novo->infoDois = raiz->infoUm;
        novo->Ninfos = 2;
        novo->dir = raiz->cen;
        raiz = novo;
        *diminuiu = 1;
    } else if (raiz->dir && raiz->dir->Ninfos == 0 && raiz->Ninfos == 2) {
        Artista* novo = raiz->cen;
        novo->infoDois = raiz->infoDois;
        novo->Ninfos = 2;
        novo->dir = raiz->dir;
        raiz = novo;
        *diminuiu = 1;
    }

    return raiz;
}

*/


int alturaFolha = -1;

int verificarValidadeArvore(Artista* no, int nivel) {
    if (!no) return 1;

    // Verifica número de informações
    if (no->Ninfos < 1 || no->Ninfos > 2)
        return 0;

    // Verifica filhos
    int numFilhos = (no->esq ? 1 : 0) + (no->cen ? 1 : 0) + (no->dir ? 1 : 0);
    if (no->Ninfos == 1 && numFilhos != 0 && numFilhos != 2)
        return 0;
    if (no->Ninfos == 2 && numFilhos != 0 && numFilhos != 3)
        return 0;

    // Verifica se folhas estão no mesmo nível
    if (!no->esq && !no->cen && !no->dir) {
        if (alturaFolha == -1)
            alturaFolha = nivel;
        else if (alturaFolha != nivel)
            return 0;
    }

    return verificarValidadeArvore(no->esq, nivel + 1) &&
           verificarValidadeArvore(no->cen, nivel + 1) &&
           verificarValidadeArvore(no->dir, nivel + 1);
}


/*
int main() {
    Artista* raiz = NULL;
    int opcao, diminuiu;
    char nome[100], estilo[100];

    do {
        printf("\n--- MENU ÁRVORE 2–3 DE ARTISTAS ---\n");
        printf("1. Inserir artista\n");
        printf("2. Remover artista\n");
        printf("3. Buscar artista\n");
        printf("4. Imprimir árvore\n");
        printf("5. Verificar validade\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // limpar buffer

        switch (opcao) {
            case 1:
                printf("Nome do artista: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Estilo musical: ");
                fgets(estilo, sizeof(estilo), stdin);
                estilo[strcspn(estilo, "\n")] = 0;
                raiz = inserirArtista(raiz, nome, estilo);
                break;

            case 2:
                printf("Nome do artista a remover: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                diminuiu = 0;
                raiz = removerArtista(raiz, nome, &diminuiu);
                break;

            case 3:
                printf("Nome do artista a buscar: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                buscarArtista(raiz, nome);
                break;

            case 4:
                imprimirInOrdemArtistaComRaiz(raiz);
                break;

            case 5:
                alturaFolha = -1;
                if (verificarValidadeArvore(raiz, 0))
                    printf("✅ Árvore válida!\n");
                else
                    printf("❌ Árvore inválida!\n");
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    liberarArvoreArtista(raiz);
    return 0;
}

*/


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

    printf(">>> Em ordem com raiz:\n");
    imprimirInOrdemArtistaComRaiz(raiz);   // já é sua função

    // Busca (usando sua função)
    Artista *encontrado = buscarArtista(raiz, "Rafa");
    if (encontrado)
        printf("\nAchei o artista: %s (%s)\n",
               // atenção: pode estar em infoUm ou infoDois
               (strcmp("Rafa", encontrado->infoUm.nome) == 0) ? encontrado->infoUm.nome : encontrado->infoDois.nome,
               (strcmp("Rafa", encontrado->infoUm.nome) == 0) ? encontrado->infoUm.estilo : encontrado->infoDois.estilo);
    else
        printf("\nArtista não encontrado.\n");

    

    
    int diminuiu = 0;
    raiz = removerArtista(raiz, "Rafa", &diminuiu);
    printf("\n>>> Após remover 'Rafa':\n");
    imprimirInOrdemArtistaComRaiz(raiz);

    

    // Libera memória (sua função)
    liberarArtistas(raiz);
    raiz = NULL;

    return 0;
}
