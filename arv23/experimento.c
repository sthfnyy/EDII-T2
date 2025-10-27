#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "artista.h"
#include "tipo.h"

#define LARGURA_NOME 100  /* largura máx. por nome (100) + terminador '\0' gerenciado pelos snprintf */

/* ============================================================
   EXPERIMENTO — ÁRVORE 2–3
   - Compara nomes case-insensitive
   - Mede tempo "limpo" (sem I/O dentro do laço)
   - Registra o caminho (nós visitados) e imprime depois
   - Usa buffer 2D fixo para evitar realocações durante o teste
   ============================================================ */

/* compara ignorando maiúsculas/minúsculas */
static int compararNomesIgnorandoCaixa(const char *nomeA, const char *nomeB)
{
    return strcasecmp(nomeA, nomeB); /* POSIX: comparação case-insensitive */
}

/* imprime a(s) chave(s) de um nó 2–3 (para depurar o caminho percorrido) */
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

/* inicializa o registrador de caminho (vetor dinâmico de nós visitados) */
static void inicializarCaminho(CaminhoBusca *caminho)
{
    caminho->nosVisitados = NULL;
    caminho->quantidade   = 0;
    caminho->capacidade   = 0;
}

/* adiciona um nó ao caminho; crescimento geométrico para reduzir realocações */
static void registrarNoCaminho(CaminhoBusca *caminho, const Artista *no)
{
    if (caminho->quantidade >= caminho->capacidade)
    {
        caminho->capacidade = (caminho->capacidade == 0 ? 64 : caminho->capacidade * 2);
        caminho->nosVisitados = (const Artista **)realloc(
            caminho->nosVisitados,
            (size_t)caminho->capacidade * sizeof(*caminho->nosVisitados)
        );
    }
    caminho->nosVisitados[caminho->quantidade++] = no; /* guarda ponteiro (não copia conteúdo) */
}

/* desaloca apenas o vetor do caminho (os nós pertencem à árvore) */
static void liberarCaminho(CaminhoBusca *caminho)
{
    free((void*)caminho->nosVisitados);
    caminho->nosVisitados = NULL;
    caminho->quantidade   = 0;
    caminho->capacidade   = 0;
}

/* ===== Busca cronometrada (tempo limpo) em árvore 2–3 =====
   - Sem printf dentro do laço
   - Caminho é impresso após medir o tempo
*/
static ResultadoBusca buscarArtistaComCaminho23_clean(Artista *raiz, const char *nomeBuscado, int imprimirCaminho)
{
    ResultadoBusca out = {0, 0, 0.0};
    Artista *noAtual = raiz;
    CaminhoBusca caminho;
    clock_t t0 = 0, t1 = 0;
    int visitados = 0;
    int achou = 0;
    double ms = 0.0;
    int continuar = 1; /* flag de controle do laço  */

    inicializarCaminho(&caminho);

    /* ======== CRONOMETRAGEM INÍCIO ======== */
    t0 = clock();

    while (noAtual != NULL && continuar)
    {
        int cmpUm = 0, cmpDois = 0;

        visitados++;                      /* conta o nó atual como visitado */
        registrarNoCaminho(&caminho, noAtual); /* registra para impressão posterior */

        /* compara com a primeira chave do nó */
        cmpUm = compararNomesIgnorandoCaixa(nomeBuscado, noAtual->infoUm.nome);
        if (cmpUm == 0) { 
            achou = 1; 
            continuar = 0;               /* sinaliza parada do laço */
        }

        /* se houver segunda chave, compara também */
        if (noAtual->Ninfos == 2) {
            cmpDois = compararNomesIgnorandoCaixa(nomeBuscado, noAtual->infoDois.nome);
            if (cmpDois == 0) {
                achou = 1; 
                continuar = 0;
            }
        }

        /* roteamento 2–3: esq | cen | dir, apenas se ainda não achou */
        if (continuar) {
            if (cmpUm < 0) {
                noAtual = noAtual->esq;
            } else if (noAtual->Ninfos == 1) {
                noAtual = noAtual->cen;
            } else if (cmpDois < 0) {
                noAtual = noAtual->cen;
            } else {
                noAtual = noAtual->dir;
            }
        }
        /* Nota: se 'continuar' ficou 0, não roteamos; o while encerra na próxima checagem */
    }

    t1 = clock();
    /* ======== CRONOMETRAGEM FIM ======== */

    ms = 1000.0 * (double)(t1 - t0) / (double)CLOCKS_PER_SEC; /* clocks -> ms */

    /* Impressão do caminho e do resultado (fora do tempo medido) */
    if (imprimirCaminho == 1)
    {
        printf("Caminho da busca por \"%s\":\n", nomeBuscado);
        for (int i = 0; i < caminho.quantidade; i++)   
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

/* ===== Coleta nomes (in-order 2–3): esq -> infoUm -> cen -> infoDois -> dir
   - Escreve no buffer 2D (100+1) com truncamento seguro               */
static void coletarNomesDeArtistas23(Artista *raiz, char (*lista)[100 + 1], int *qtd, int maxNomes)
{
    if (raiz && *qtd < maxNomes)
    {
        coletarNomesDeArtistas23(raiz->esq, lista, qtd, maxNomes);

        if (*qtd < maxNomes) {
            snprintf(lista[*qtd], 100 + 1, "%s", raiz->infoUm.nome); /* copia segura */
            (*qtd)++;
        }

        if (*qtd < maxNomes)
            coletarNomesDeArtistas23(raiz->cen, lista, qtd, maxNomes);

        if (raiz->Ninfos == 2 && *qtd < maxNomes) {
            snprintf(lista[*qtd], 100 + 1, "%s", raiz->infoDois.nome);
            (*qtd)++;
        }

        if (raiz->Ninfos == 2 && *qtd < maxNomes)
            coletarNomesDeArtistas23(raiz->dir, lista, qtd, maxNomes);
    }

}

/* gera um nome garantidamente ausente (para compor consultas negativas) */
static void gerarNomeInexistente23(char *destino, size_t tam, const char *base, int idx)
{
    const char *b = (base != NULL ? base : "artista");
    snprintf(destino, tam, "%s__nao_existe_%d", b, idx);
}

/* ===== Ponto de entrada do experimento (2–3, tempo limpo) =====
   - Prepara o lote de consultas (reais + inexistentes)
   - Executa as buscas e consolida métricas                     */
void executarExperimentoBuscas23(Artista *raizArt)
{
    enum { QUANT = 30 };

    /* buffer 2D: QUANT linhas de 101 chars — evita malloc/realloc por string no teste */
    char (*nomes)[100 + 1] = calloc(QUANT, sizeof *nomes);

    int qtd = 0;               /* quantos nomes reais foram coletados */
    int encontrados = 0;       /* buscas bem-sucedidas */
    double somaTempo = 0.0;    /* soma dos tempos das QUANT buscas (ms) */
    int somaVisitados = 0;     /* soma dos nós visitados nas QUANT buscas */
    ResultadoBusca r;

    /* só executa se árvore e alocação estão válidas */
    int podeExecutar = (raizArt != NULL && nomes != NULL);

    if (!podeExecutar) {
        if (raizArt == NULL) {
            printf("Arvore vazia: insira artistas antes do experimento.\n");
        } else {
            perror("calloc");
        }
    } else {
        /* 1) Coleta até 24 nomes reais (ou até QUANT), via percurso in-order */
        int limite = (24 < QUANT ? 24 : QUANT);
        coletarNomesDeArtistas23(raizArt, nomes, &qtd, limite);

        /* 2) Completa o lote com nomes inexistentes até totalizar QUANT */
        for (int i = qtd; i < QUANT; i++) {
            char fake[100 + 1];
            const char *base = (qtd > 0 ? nomes[i % qtd] : "artista");
            gerarNomeInexistente23(fake, sizeof(fake), base, i);
            snprintf(nomes[i], LARGURA_NOME + 1, "%s", fake);  /* grava com truncamento seguro */
        }

        printf("\n=== Experimento (2–3, tempo limpo): %d buscas de ARTISTAS ===\n\n", QUANT);

        /* 3) Executa as buscas; a função _clean mede o tempo internamente */
        for (int i = 0; i < QUANT; i++) {
            r = buscarArtistaComCaminho23_clean(raizArt, nomes[i], 1 /* imprimir caminho depois */);

            if (r.encontrado == 1)
                encontrados++;

            somaTempo     += r.tempo_ms;
            somaVisitados += r.nosVisitados;
        }

        /* 4) Resumo consolidado (tempo e nós visitados médios) */
        printf("=== Resumo (2–3) ===\n");
        printf("- Encontrados: %d/%d\n", encontrados, QUANT);
        printf("- Tempo total: %.3f ms\n", somaTempo);
        printf("- Tempo medio: %.3f ms por busca\n", (double)(somaTempo / QUANT));
        printf("- Nos visitados (media): %.2f por busca\n\n", (double)somaVisitados / QUANT);
    }

    free(nomes);
}
