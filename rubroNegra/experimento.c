#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "tipo.h"
#include "artista.h"

/* ------------------------------------------------------------------
   Arquivo de experimento para Árvore Rubro-Negra (RB).
   Objetivo: medir busca por nome com "tempo limpo" (sem I/O no laço),
   registrar o caminho percorrido, e emitir um resumo (tempo e nós).
   Observação: nomes são tratados de forma case-insensitive (strcasecmp).
   ------------------------------------------------------------------ */

static int compararNomesIgnorandoCaixa(const char *a, const char *b)
{
    /* strcasecmp é POSIX: compara strings ignorando maiúsc./minúsc. */
    int r = 0;
    r = strcasecmp(a, b);
    return r;
}

/* ======= Impressão de um nó RB  ======= */
static void imprimirNoArtistaRB(const Artista *no)
{
    /* Apenas exibe a "chave" (nome) do nó visitado no caminho. */
    if (no != NULL) {
        printf("-> [%s]\n", no->info.nome);
    }
}

static void caminhoRB_iniciar(CaminhoRB *c)
{
    /* Vetor dinâmico dos nós visitados: inicia vazio (qtd=0, cap=0). */
    c->nos = NULL;
    c->qtd = 0;
    c->cap = 0;
}

static void caminhoRB_adicionar(CaminhoRB *c, const Artista *no)
{
    /* Crescimento geométrico (cap=64 inicial; depois dobra) para eficiência. */
    if (c->qtd >= c->cap) {
        c->cap = (c->cap == 0 ? 64 : c->cap * 2);
        c->nos = (const Artista **)realloc(c->nos, (size_t)c->cap * sizeof(*c->nos));
    }
    /* Registra o ponteiro do nó visitado (não há cópia do conteúdo). */
    c->nos[c->qtd++] = no;
}

static void caminhoRB_liberar(CaminhoRB *c)
{
    /* Libera apenas o vetor; os nós pertencem à árvore e não são desalocados aqui. */
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
        /* Visita nó atual, registra no caminho e decide direção (BST). */
        int cmp = 0;
        visitados++;
        caminhoRB_adicionar(&caminho, noAtual); /* apenas registra o nó */

        /* Comparação case-insensitive com a chave do nó atual. */
        cmp = compararNomesIgnorandoCaixa(nomeBuscado, noAtual->info.nome);
        if (cmp == 0) {
            achou = 1;
            continuar = 0;          /* encontrou: interrompe laço */
        } else if (cmp < 0) {
            noAtual = noAtual->esq;  /* vai para a subárvore à esquerda */
        } else {
            noAtual = noAtual->dir;  /* vai para a subárvore à direita  */
        }
    }

    t1 = clock();
    /* ===== CRONÔMETRO FIM ===== */

    /* Conversão para milissegundos (double) a partir de clocks. */
    ms = 1000.0 * (double)(t1 - t0) / (double)CLOCKS_PER_SEC;

    if (imprimirCaminho == 1)
    {
        /* Impressão do caminho acontece fora da janela cronometrada. */
        int i = 0;
        printf("Caminho da busca por \"%s\":\n", nomeBuscado);
        for (i = 0; i < caminho.qtd; i++)
            imprimirNoArtistaRB(caminho.nos[i]);

        if (achou == 1)
            printf("Resultado: ENCONTRADO | nós visitados: %d | tempo: %.3f ms\n\n", visitados, ms);
        else
            printf("Resultado: NAO ENCONTRADO | nós visitados: %d | tempo: %.3f ms\n\n", visitados, ms);
    }

    /* Preenche a saída com métricas da busca. */
    out.encontrado   = achou;
    out.nosVisitados = visitados;
    out.tempo_ms     = ms;

    caminhoRB_liberar(&caminho);
    return out; 
}


/* ======= Coleta nomes reais da árvore (em ordem) ======= */
static void coletarNomesDeArtistas_RB(Artista *raiz,
                                      char (*listaNomes)[100 + 1],
                                      int *quantNomes,
                                      int maxNomes)
{
    /* único ponto de saída no final */
    if (raiz && *quantNomes < maxNomes)
    {
        coletarNomesDeArtistas_RB(raiz->esq, listaNomes, quantNomes, maxNomes);

        if (*quantNomes < maxNomes) {
            /* Cópia segura com truncamento a 100 caracteres (+ '\0'). */
            snprintf(listaNomes[*quantNomes], 100 + 1, "%s", raiz->info.nome);
            (*quantNomes)++;
        }

        if (*quantNomes < maxNomes)
            coletarNomesDeArtistas_RB(raiz->dir, listaNomes, quantNomes, maxNomes);
    }
}


/* ======= Gera nome inexistente ======= */
static void gerarNomeInexistente_RB(char *dest, size_t tam, const char *base, int idx)
{
    /* Monta um nome garantidamente ausente a partir de uma base e índice. */
    const char *b = (base != NULL ? base : "artista");
    snprintf(dest, tam, "%s__nao_existe_%d", b, idx);
}

/* ======= Ponto de entrada do experimento (RB, tempo limpo) ======= */
void executarExperimentoBuscas(Artista *raizArt)
{
    const int QUANTIDADE_BUSCAS = 30;

    /* Pré-aloca um buffer 2D: 30 linhas, cada uma com 100+1 chars.
       Vantagem: sem malloc/realloc por string durante a medição. */
    char (*listaNomes)[100 + 1] = calloc(QUANTIDADE_BUSCAS, sizeof *listaNomes);

    int quantNomes = 0;        /* quantos nomes reais foram coletados da árvore */
    int encontrados = 0;       /* contagem de buscas que retornaram sucesso */
    double somaTempos_ms = 0.0;/* soma dos tempos das 30 buscas (ms) */
    int somaNosVisitados = 0;  /* soma dos nós visitados nas 30 buscas */
    ResultadoBusca r;

    /* Sinaliza se é possível executar (árvore não vazia e alocação OK). */
    int podeExecutar = (raizArt != NULL && listaNomes != NULL);

    if (!podeExecutar) {
        if (raizArt == NULL) {
            printf("Arvore vazia: insira artistas antes do experimento.\n");
        } else {
            perror("calloc");  /* falha de alocação do buffer */
        }
    } else {
        /* 1) Coleta até 24 nomes reais (ou o que houver), via in-order. */
        int limiteColeta = (24 < QUANTIDADE_BUSCAS ? 24 : QUANTIDADE_BUSCAS);
        coletarNomesDeArtistas_RB(raizArt, listaNomes, &quantNomes, limiteColeta);

        /* 2) Preenche o restante com nomes inexistentes para totalizar 30. */
        for (int i = quantNomes; i < QUANTIDADE_BUSCAS; i++) {
            char fake[100 + 1];
            const char *base = (quantNomes > 0 ? listaNomes[i % quantNomes] : "artista");
            gerarNomeInexistente_RB(fake, sizeof(fake), base, i);
            /* grava já truncando para 100 chars, se necessário */
            snprintf(listaNomes[i], 100 + 1, "%s", fake);
        }

        printf("\n=== Experimento (RB, tempo limpo): %d buscas de ARTISTAS ===\n\n",
               QUANTIDADE_BUSCAS);

        /* 3) Executa as 30 buscas, medindo tempo limpo e acumulando métricas. */
        for (int i = 0; i < QUANTIDADE_BUSCAS; i++) {
            r = buscarArtistaComCaminho_RB_clean(raizArt, listaNomes[i], 1 /* imprimir */);
            if (r.encontrado == 1) encontrados++;
            somaTempos_ms   += r.tempo_ms;
            somaNosVisitados += r.nosVisitados;
        }

        /* 4) Emite o resumo consolidado do experimento. */
        printf("=== Resumo (RB) ===\n");
        printf("- Encontrados: %d/%d\n", encontrados, QUANTIDADE_BUSCAS);
        printf("- Tempo total: %.3f ms\n", somaTempos_ms);
        printf("- Tempo medio: %.3f ms por busca\n",
               (double)(somaTempos_ms / QUANTIDADE_BUSCAS));
        printf("- Nos visitados (media): %.2f por busca\n\n",
               (double)somaNosVisitados / QUANTIDADE_BUSCAS);
    }

    /* 5) Libera o buffer 2D (apenas um bloco). */
    free(listaNomes);
    return;
}
