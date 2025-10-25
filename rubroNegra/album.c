#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tipo.h"
#include "album.h"

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


// Essa funcao inicia faz com que os nos folhas seja pretos
int corAlbum(Album *raiz)
{
    int corNo = PRETO;
    if (raiz)
    {
        corNo = (raiz)->cor;
    }
    
    return corNo;
}

//tive que modificar e tirar do tipo void, pois na remoção não dava pra usar void
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

    /* --- PROTEÇÃO AQUI --- */
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
    // 1. Cria um ponteiro para o nó que será retornado. Inicia com NULL.
    Album *noEncontrado = NULL;

    // 2. Cria um ponteiro para percorrer a árvore, começando pela raiz.
    Album *noAtual = raiz;

    // 3. O laço continua enquanto não chegamos ao fim de um galho (noAtual != NULL)
    //    e enquanto ainda não encontramos o nó (noEncontrado == NULL).
    while (noAtual != NULL && noEncontrado == NULL)
    {
        // Compara o título buscado com o título do nó atual
        int comparacao = strcmp(tituloBusca, noAtual->info.titulo);

        if (comparacao == 0)
        {
            // Encontrou! Atribui o nó atual à variável de resultado.
            noEncontrado = noAtual;
        }
        else if (comparacao < 0)
        {
            // O título buscado vem antes, então desce para a esquerda.
            noAtual = noAtual->esq;
        }
        else // (comparacao > 0)
        {
            // O título buscado vem depois, então desce para a direita.
            noAtual = noAtual->dir;
        }
    }

    // 4. Retorna a variável de resultado.
    //    Se o nó foi encontrado, ela conterá o endereço dele.
    //    Se o laço terminou sem encontrar, ela ainda terá o valor NULL.
    return noEncontrado;
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
            // libera músicas do álbum que será destruído
            liberarListaMusicas(raiz->info.musica);
            raiz->info.musica = NULL;

            free(raiz);
            resultado = NULL;
        }
        else
        {
            Album *esq = raiz->esq;
            Album *esqEsq = esq ? esq->esq : NULL;

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
                // liberar músicas deste álbum ANTES de destruir
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

                    // 1) liberar músicas atuais do nó base (para não vazar)
                    liberarListaMusicas(raiz->info.musica);

                    // 2) transferir info do sucessor
                    raiz->info = menor->info;

                    // 3) evitar double free: o sucessor será destruído em removeMenorAlbum,
                    //    então zeramos seu ponteiro de músicas que acabamos de transferir
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

        // LIBERA músicas desse álbum ANTES de liberar o nó do álbum
        liberarListaMusicas(raiz->info.musica);

        free(raiz);
    }
}



// /* ======== Teste rápido ======== */

// int main(void)
// {
//     Album *raiz = NULL;

//     infoAlbum a = {"Epitafio",   2020, 5};
//     infoAlbum b = {"Chuva",      2018, 2};
//     infoAlbum c = {"Cavaquinho", 2025, 7};
//     infoAlbum d = {"Guitarra",   2022, 1};
//     infoAlbum e = {"Acordes",    2015, 3};  // extra pra gerar mais casos
//     infoAlbum f = {"Violino",    2021, 4};  // extra

//     // Inserções
//     insercao(&raiz, criarNo(a));
//     insercao(&raiz, criarNo(b));
//     insercao(&raiz, criarNo(c));
//     insercao(&raiz, criarNo(d));
//     insercao(&raiz, criarNo(e));
//     insercao(&raiz, criarNo(f));

//     printf(">>> Pre-ordem (inicial):\n");
//     mostrarAlbumPreOrdem(raiz);

//     // Busca simples
//     Album *encontrado = BuscarNoPorTitulo(raiz, "Chuva");
//     if (encontrado) printf("Achei %s\n\n", encontrado->info.titulo);

//     // 1) Remover folha (escolha um que esteja folha após balancear; “Violino” costuma virar folha)
//     printf(">>> Removendo 'Violino' (folha):\n");
//     if (removeAlbum(&raiz, "Violino"))
//         mostrarAlbumPreOrdem(raiz);
//     else
//         printf("Nao removido (nao encontrado)\n");
//     printf("\n");

//     // 2) Remover nó com 1 filho (dependendo do balanceamento, “Guitarra” ou “Acordes” podem cair nesse caso)
//     printf(">>> Removendo 'Guitarra' (possivel 1 filho):\n");
//     if (removeAlbum(&raiz, "Guitarra"))
//         mostrarAlbumPreOrdem(raiz);
//     else
//         printf("Nao removido (nao encontrado)\n");
//     printf("\n");

//     // 3) Remover nó com 2 filhos (normalmente “Epitafio” ou “Cavaquinho” terão 2 filhos após inserções)
//     printf(">>> Removendo 'Epitafio' (2 filhos):\n");
//     if (removeAlbum(&raiz, "Epitafio"))
//         mostrarAlbumPreOrdem(raiz);
//     else
//         printf("Nao removido (nao encontrado)\n");
//     printf("\n");

//     // 4) Remover a raiz atual (qualquer que seja após balanceamentos)
//     //    Pegue o título da raiz atual (se quiser garantir que está testando a raiz):
//     if (raiz) {
//         char raizTitulo[80];
//         strncpy(raizTitulo, raiz->info.titulo, sizeof(raizTitulo));
//         raizTitulo[sizeof(raizTitulo)-1] = '\0';

//         printf(">>> Removendo a raiz atual ('%s'):\n", raizTitulo);
//         if (removeAlbum(&raiz, raizTitulo))
//             mostrarAlbumPreOrdem(raiz);
//         else
//             printf("Nao removido (nao encontrado)\n");
//         printf("\n");
//     }

//     // 5) Tentar remover um titulo inexistente
//     printf(">>> Removendo 'NaoExiste':\n");
//     if (removeAlbum(&raiz, "NaoExiste"))
//         mostrarAlbumPreOrdem(raiz);
//     else
//         printf("Nao removido (nao encontrado)\n");
//     printf("\n");

//     liberarArvore(raiz);
//     return 0;
// }

