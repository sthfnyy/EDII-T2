#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "artista.h"
#include "heads.h"


/* compara ignorando maiúsculas/minúsculas */
static int compararNomesIgnorandoCaixa(const char *nomeA, const char *nomeB)
{
    int resultado = 0;
    resultado = strcasecmp(nomeA, nomeB);
    return resultado;
}

/* imprimir a "chave" de um nó 2–3 */
static void imprimirNoArtista23(const Artista *no)
{
    if (no != NULL)
    {
        if (no->Ninfos == 1)
            printf("-> [%s]\n", no->infoUm.nome);
        else
            printf("-> [%s | %s]\n", no->infoUm.nome, no->infoDois.nome);
    }
}


/* inicializa a estrutura de caminho */
static void inicializarCaminho(CaminhoBusca *caminho)
{
    caminho->nosVisitados = NULL;
    caminho->quantidade = 0;
    caminho->capacidade = 0;
}

/* adiciona um nó visitado ao vetor, realocando se necessário */
static void registrarNoCaminho(CaminhoBusca *caminho, const Artista *no)
{
    if (caminho->quantidade >= caminho->capacidade)
    {
        caminho->capacidade = (caminho->capacidade == 0 ? 64 : caminho->capacidade * 2);
        caminho->nosVisitados = (const Artista **) realloc(
            caminho->nosVisitados,
            (size_t)caminho->capacidade * sizeof(*caminho->nosVisitados)
        );
    }

    caminho->nosVisitados[caminho->quantidade++] = no;
}

/* libera a memória usada para armazenar o caminho */
static void liberarCaminho(CaminhoBusca *caminho)
{
    free((void*) caminho->nosVisitados);
    caminho->nosVisitados = NULL;
    caminho->quantidade = 0;
    caminho->capacidade = 0;
}


/* ===== Busca cronometrada  =====
   - Mede tempo sem printf dentro do laço.
   - Guarda nós visitados em 'caminho' e imprime depois (fora do tempo).
*/
static ResultadoBusca buscarArtistaComCaminho23_clean(Artista *raiz, const char *nomeBuscado, int imprimirCaminho)
{
    ResultadoBusca out;
    Artista *noAtual = raiz;
    CaminhoBusca caminho;
    clock_t t0 = 0, t1 = 0;
    int continuar = 1;
    int achou = 0;
    int visitados = 0;
    double ms = 0.0;

    inicializarCaminho(&caminho);

    /* ======== CRONOMETRAGEM INÍCIO ======== */
    t0 = clock();

    while (noAtual != NULL && continuar == 1)
    {
        int cmpUm = 0;
        int cmpDois = 0;

        visitados++;
        registrarNoCaminho(&caminho, noAtual); /* apenas guarda o nó; não imprime */

        /* compara com infoUm */
        cmpUm = compararNomesIgnorandoCaixa(nomeBuscado, noAtual->infoUm.nome);
        if (cmpUm == 0)
        {
            achou = 1;
            continuar = 0;
        }
        else
        {
            if (noAtual->Ninfos == 2)
            {
                cmpDois = compararNomesIgnorandoCaixa(nomeBuscado, noAtual->infoDois.nome);
                if (cmpDois == 0)
                {
                    achou = 1;
                    continuar = 0;
                }
            }

            if (continuar == 1)
            {
                if (cmpUm < 0)
                    noAtual = noAtual->esq;
                else if (noAtual->Ninfos == 1 || compararNomesIgnorandoCaixa(nomeBuscado, noAtual->infoDois.nome) < 0)
                    noAtual = noAtual->cen;
                else
                    noAtual = noAtual->dir;
            }
        }
    }

    t1 = clock();
    /* ======== CRONOMETRAGEM FIM ======== */

    ms = 1000.0 * (double)(t1 - t0) / (double)CLOCKS_PER_SEC;

    /* Impressão do caminho e do resultado  */
    if (imprimirCaminho == 1)
    {
        int i = 0;
        printf("Caminho da busca por \"%s\":\n", nomeBuscado);
        for (i = 0; i < caminho.capacidade; i++)
            imprimirNoArtista23(caminho.nosVisitados[i]);

        if (achou == 1)
            printf("Resultado: ENCONTRADO | nós visitados: %d | tempo: %.3f ms\n\n", visitados, ms);
        else
            printf("Resultado: NAO ENCONTRADO | nós visitados: %d | tempo: %.3f ms\n\n", visitados, ms);
    }

    out.encontrado   = achou;
    out.nosVisitados = visitados;
    out.tempo_ms     = ms;

    liberarCaminho(&caminho);
    return out;
}

/* ===== Coleta nomes de artistas (in-order 2–3): esq -> infoUm -> cen -> infoDois -> dir ===== */
static void coletarNomesDeArtistas23(Artista *raiz, char ***lista, int *qtd, int *cap, int maxNomes)
{
    int pode;

    if (raiz != NULL && *qtd < maxNomes)
        coletarNomesDeArtistas23(raiz->esq, lista, qtd, cap, maxNomes);

    if (raiz != NULL && *qtd < maxNomes)
    {
        if (*cap == 0)
        {
            *cap = 16;
            *lista = (char**) malloc((size_t)(*cap) * sizeof(char*));
        }
        if (*qtd >= *cap)
        {
            *cap *= 2;
            *lista = (char**) realloc(*lista, (size_t)(*cap) * sizeof(char*));
        }
        (*lista)[*qtd] = (char*) malloc(strlen(raiz->infoUm.nome) + 1);
        strcpy((*lista)[*qtd], raiz->infoUm.nome);
        (*qtd)++;
    }

    if (raiz != NULL && *qtd < maxNomes)
        coletarNomesDeArtistas23(raiz->cen, lista, qtd, cap, maxNomes);

    if (raiz != NULL && raiz->Ninfos == 2 && *qtd < maxNomes)
    {
        if (*cap == 0)
        {
            *cap = 16;
            *lista = (char**) malloc((size_t)(*cap) * sizeof(char*));
        }
        if (*qtd >= *cap)
        {
            *cap *= 2;
            *lista = (char**) realloc(*lista, (size_t)(*cap) * sizeof(char*));
        }
        (*lista)[*qtd] = (char*) malloc(strlen(raiz->infoDois.nome) + 1);
        strcpy((*lista)[*qtd], raiz->infoDois.nome);
        (*qtd)++;
    }

    pode = (raiz != NULL && raiz->Ninfos == 2 && *qtd < maxNomes);
    if (pode)
        coletarNomesDeArtistas23(raiz->dir, lista, qtd, cap, maxNomes);
}

/* gera nome inexistente a partir de base */
static void gerarNomeInexistente23(char *destino, size_t tam, const char *base, int idx)
{
    const char *b = (base != NULL ? base : "artista");
    snprintf(destino, tam, "%s__nao_existe_%d", b, idx);
}

/* ===== Ponto de entrada do experimento (2–3, tempo limpo) ===== */
void executarExperimentoBuscas23(Artista *raizArt)
{
    const int QUANT = 30;
    char **nomes = NULL;
    int qtd = 0;
    int cap = 0;
    int encontrados = 0;
    double somaTempo = 0.0;
    int somaVisitados = 0;
    int i = 0;
    ResultadoBusca r;
    int arvVazia = (raizArt == NULL ? 1 : 0);

    if (arvVazia == 1)
    {
        printf("Arvore vazia: insira artistas antes do experimento.\n");
    }
    else
    {
        /* 1) Coleta até 20 nomes reais */
        coletarNomesDeArtistas23(raizArt, &nomes, &qtd, &cap, 20);

        /* 2) Completa até 30 com nomes inexistentes */
        for (i = qtd; i < QUANT; i++)
        {
            char fake[128];
            const char *base = (qtd > 0 ? nomes[i % qtd] : "artista");

            gerarNomeInexistente23(fake, sizeof(fake), base, i);

            if (cap == 0)
            {
                cap = 16;
                nomes = (char**) malloc((size_t)cap * sizeof(char*));
            }
            if (i >= cap)
            {
                cap *= 2;
                nomes = (char**) realloc(nomes, (size_t)cap * sizeof(char*));
            }

            nomes[i] = (char*) malloc(strlen(fake) + 1);
            strcpy(nomes[i], fake);
        }

        printf("\n=== Experimento (2–3, tempo limpo): %d buscas de ARTISTAS ===\n\n", QUANT);

        /* 3) Executa as 30 buscas: mede tempo limpo e imprime o caminho depois */
        for (i = 0; i < QUANT; i++)
        {
            r = buscarArtistaComCaminho23_clean(raizArt, nomes[i], 1 /* imprimir caminho depois */);

            if (r.encontrado == 1)
                encontrados++;

            somaTempo    += r.tempo_ms;
            somaVisitados += r.nosVisitados;
        }

        /* 4) Resumo */
        printf("=== Resumo (2–3) ===\n");
        printf("- Encontrados: %d/%d\n", encontrados, QUANT);
        printf("- Tempo total: %.3f ms\n", somaTempo);
        printf("- Tempo medio: %.3f ms por busca\n", (double)(somaTempo / QUANT));
        printf("- Nos visitados (media): %.2f por busca\n\n", (double)somaVisitados / QUANT);

        /* 5) Libera nomes */
        for (i = 0; i < QUANT; i++)
            free(nomes[i]);
        free(nomes);
    }
}
