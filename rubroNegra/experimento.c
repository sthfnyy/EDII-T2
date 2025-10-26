#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "tipo.h"
#include "artista.h"

typedef struct {
    int    encontrado;       /* 1 = achou, 0 = não achou */
    int    nosVisitados;     /* quantidade de nós visitados */
    double tempo_ms;         /* duração da busca em milissegundos */
} ResultadoBusca;


static int compararNomesIgnorandoCaixa(const char *a, const char *b)
{
    int r = 0;
    r = strcasecmp(a, b);
    return r;
}

/* ======= Impressão de um nó RB (apenas nome) ======= */
static void imprimirNoArtistaRB(const Artista *no)
{
    if (no != NULL) {
        printf("-> [%s]\n", no->info.nome);
    }
}

static void caminhoRB_iniciar(CaminhoRB *c)
{
    c->nos = NULL;
    c->qtd = 0;
    c->cap = 0;
}

static void caminhoRB_adicionar(CaminhoRB *c, const Artista *no)
{
    if (c->qtd >= c->cap) {
        c->cap = (c->cap == 0 ? 64 : c->cap * 2);
        c->nos = (const Artista **)realloc(c->nos, (size_t)c->cap * sizeof(*c->nos));
    }
    c->nos[c->qtd++] = no;
}

static void caminhoRB_liberar(CaminhoRB *c)
{
    free((void*)c->nos);
    c->nos = NULL;
    c->qtd = 0;
    c->cap = 0;
}

/* ======= Busca cronometrada “limpa” na RB =======
   - Sem printf dentro do laço (tempo limpo);
   - Caminho é armazenado e impresso depois. */
static ResultadoBusca buscarArtistaComCaminho_RB_clean(Artista *raiz, const char *nomeBuscado, int imprimirCaminho)
{
    ResultadoBusca out;
    Artista *noAtual = raiz;
    CaminhoRB caminho;
    clock_t t0 = 0, t1 = 0;
    int continuar = 1;
    int visitados = 0;
    int achou = 0;
    double ms = 0.0;

    /* zera struct de saída */
    out.encontrado = 0;
    out.nosVisitados = 0;
    out.tempo_ms = 0.0;

    caminhoRB_iniciar(&caminho);

    /* ===== CRONÔMETRO INÍCIO ===== */
    t0 = clock();

    while (noAtual != NULL && continuar == 1)
    {
        int cmp = 0;
        visitados++;
        caminhoRB_adicionar(&caminho, noAtual); /* apenas registra o nó */

        cmp = compararNomesIgnorandoCaixa(nomeBuscado, noAtual->info.nome);
        if (cmp == 0) {
            achou = 1;
            continuar = 0;
        } else if (cmp < 0) {
            noAtual = noAtual->esq;
        } else {
            noAtual = noAtual->dir;
        }
    }

    t1 = clock();
    /* ===== CRONÔMETRO FIM ===== */

    ms = 1000.0 * (double)(t1 - t0) / (double)CLOCKS_PER_SEC;

    if (imprimirCaminho == 1)
    {
        int i = 0;
        printf("Caminho da busca por \"%s\":\n", nomeBuscado);
        for (i = 0; i < caminho.qtd; i++)
            imprimirNoArtistaRB(caminho.nos[i]);

        if (achou == 1)
            printf("Resultado: ENCONTRADO | nós visitados: %d | tempo: %.3f ms\n\n", visitados, ms);
        else
            printf("Resultado: NAO ENCONTRADO | nós visitados: %d | tempo: %.3f ms\n\n", visitados, ms);
    }

    out.encontrado   = achou;
    out.nosVisitados = visitados;
    out.tempo_ms     = ms;

    caminhoRB_liberar(&caminho);
    return out; /* único return */
}

/* ======= Coleta até 'maxNomes' nomes in-order (RB) ======= */
static void coletarNomesDeArtistas_RB(Artista *raiz,
                                      char ***listaNomes, int *quantNomes, int *capacidade,
                                      int maxNomes)
{
    int precisaAumentar = 0;
    int podeInserir = (raiz != NULL && (*quantNomes) < maxNomes);

    if (raiz != NULL && (*quantNomes) < maxNomes)
        coletarNomesDeArtistas_RB(raiz->esq, listaNomes, quantNomes, capacidade, maxNomes);

    if (podeInserir)
    {
        if (*capacidade == 0) {
            *capacidade = 16;
            *listaNomes = (char **)malloc((size_t)(*capacidade) * sizeof(char *));
        }
        precisaAumentar = (*quantNomes >= *capacidade);
        if (precisaAumentar) {
            *capacidade *= 2;
            *listaNomes = (char **)realloc(*listaNomes, (size_t)(*capacidade) * sizeof(char *));
        }

        (*listaNomes)[*quantNomes] = (char *)malloc(strlen(raiz->info.nome) + 1);
        strcpy((*listaNomes)[*quantNomes], raiz->info.nome);
        (*quantNomes)++;
    }

    if (raiz != NULL && (*quantNomes) < maxNomes)
        coletarNomesDeArtistas_RB(raiz->dir, listaNomes, quantNomes, capacidade, maxNomes);
}

/* ======= Gera nome inexistente ======= */
static void gerarNomeInexistente_RB(char *dest, size_t tam, const char *base, int idx)
{
    const char *b = (base != NULL ? base : "artista");
    snprintf(dest, tam, "%s__nao_existe_%d", b, idx);
}

/* ======= Ponto de entrada do experimento (RB, tempo limpo) ======= */
void executarExperimentoBuscas(Artista *raizArt)
{
    const int QUANTIDADE_BUSCAS = 30;
    char **listaNomes = NULL;
    int quantNomes = 0;
    int capacidade = 0;
    int encontrados = 0;
    double somaTempos_ms = 0.0;
    int somaNosVisitados = 0;
    int i = 0;
    ResultadoBusca r;

    if (raizArt == NULL) {
        printf("Arvore vazia: insira artistas antes do experimento.\n");
    } else {
        /* 1) Coleta até 20 nomes reais */
        coletarNomesDeArtistas_RB(raizArt, &listaNomes, &quantNomes, &capacidade, 20);

        /* 2) Completa até 30 com nomes inexistentes */
        for (i = quantNomes; i < QUANTIDADE_BUSCAS; i++)
        {
            char fake[128];
            const char *base = (quantNomes > 0 ? listaNomes[i % quantNomes] : "artista");

            gerarNomeInexistente_RB(fake, sizeof(fake), base, i);

            if (capacidade == 0) {
                capacidade = 16;
                listaNomes = (char **)malloc((size_t)capacidade * sizeof(char *));
            }
            if (i >= capacidade) {
                capacidade *= 2;
                listaNomes = (char **)realloc(listaNomes, (size_t)capacidade * sizeof(char *));
            }

            listaNomes[i] = (char *)malloc(strlen(fake) + 1);
            strcpy(listaNomes[i], fake);
        }

        printf("\n=== Experimento (RB, tempo limpo): %d buscas de ARTISTAS ===\n\n", QUANTIDADE_BUSCAS);

        /* 3) Executa as 30 buscas — imprime caminho só depois do clock */
        for (i = 0; i < QUANTIDADE_BUSCAS; i++)
        {
            r = buscarArtistaComCaminho_RB_clean(raizArt, listaNomes[i], 1 /* imprimir */);

            if (r.encontrado == 1) encontrados++;
            somaTempos_ms   += r.tempo_ms;
            somaNosVisitados += r.nosVisitados;
        }

        /* 4) Resumo */
        printf("=== Resumo (RB) ===\n");
        printf("- Encontrados: %d/%d\n", encontrados, QUANTIDADE_BUSCAS);
        printf("- Tempo total: %.3f ms\n", somaTempos_ms);
        printf("- Tempo medio: %.3f ms por busca\n", (double)(somaTempos_ms / QUANTIDADE_BUSCAS));
        printf("- Nos visitados (media): %.2f por busca\n\n", (double)somaNosVisitados / QUANTIDADE_BUSCAS);

        /* 5) Libera memória auxiliar */
        for (i = 0; i < QUANTIDADE_BUSCAS; i++)
            free(listaNomes[i]);
        free(listaNomes);
    }
}
