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

/* ---------- Comparação de nomes (ignorando maiúsculas/minúsculas) ---------- */
static int compararNomesIgnorandoCaixa(const char *nomeA, const char *nomeB)
{
    int resultadoComparacao = 0;

    resultadoComparacao = strcasecmp(nomeA, nomeB);

    return resultadoComparacao; 
}

/* ---------- Imprime a “chave” do nó visitado ---------- */
static void imprimirNoArtista(const Artista *noAtual)
{
    if (noAtual != NULL)
    {
        printf("-> [%s]\n", noAtual->info.nome);
    }

}

/* ---------- Busca um ARTISTA na RB imprimindo o caminho e medindo tempo ---------- */
static ResultadoBusca buscarArtistaComCaminho(Artista *raiz, const char *nomeBuscado)
{
    ResultadoBusca resultado;
    Artista *noAtual = NULL;
    int continuar = 1;
    clock_t instanteInicio = 0;
    clock_t instanteFim = 0;
    int comparacao = 0;

    /* inicializa struct de retorno */
    resultado.encontrado = 0;
    resultado.nosVisitados = 0;
    resultado.tempo_ms = 0.0;

    printf("Caminho da busca por \"%s\":\n", nomeBuscado);

    instanteInicio = clock();
    noAtual = raiz;

    while (noAtual != NULL && continuar == 1)
    {
        resultado.nosVisitados++;
        imprimirNoArtista(noAtual);

        comparacao = compararNomesIgnorandoCaixa(nomeBuscado, noAtual->info.nome);

        if (comparacao == 0)
        {
            resultado.encontrado = 1;
            continuar = 0; /* achou: encerra laço */
        }
        else
        {
            if (comparacao < 0)
                noAtual = noAtual->esq;
            else
                noAtual = noAtual->dir;
        }
    }

    instanteFim = clock();
    resultado.tempo_ms = 1000.0 * (double)(instanteFim - instanteInicio) / (double)CLOCKS_PER_SEC;

    if (resultado.encontrado == 1)
        printf("Resultado: ENCONTRADO | nós visitados: %d | tempo: %.3f ms\n\n", resultado.nosVisitados, resultado.tempo_ms);
    else
        printf("Resultado: NAO ENCONTRADO | nós visitados: %d | tempo: %.3f ms\n\n", resultado.nosVisitados, resultado.tempo_ms);

    return resultado; 
}

/* ---------- Coleta até 'maxNomes' nomes em ordem (in-order) ---------- */
static void coletarNomesDeArtistas(Artista *raiz,
                                   char ***listaNomes, int *quantNomes, int *capacidade,
                                   int maxNomes)
{
    int podePercorrer = 0;
    int precisaAumentar = 0;

    /* visita subárvore esquerda */
    if (raiz != NULL && (*quantNomes) < maxNomes)
    {
        coletarNomesDeArtistas(raiz->esq, listaNomes, quantNomes, capacidade, maxNomes);
    }

    /* prepara vetor dinâmico e insere o nome do nó atual */
    podePercorrer = (raiz != NULL && (*quantNomes) < maxNomes);
    if (podePercorrer)
    {
        if (*capacidade == 0)
        {
            *capacidade = 16;
            *listaNomes = (char **) malloc((*capacidade) * sizeof(char *));
        }

        precisaAumentar = (*quantNomes >= *capacidade);
        if (precisaAumentar)
        {
            *capacidade *= 2;
            *listaNomes = (char **) realloc(*listaNomes, (*capacidade) * sizeof(char *));
        }

        (*listaNomes)[*quantNomes] = (char *) malloc(strlen(raiz->info.nome) + 1);
        strcpy((*listaNomes)[*quantNomes], raiz->info.nome);
        (*quantNomes)++;
    }

    /* visita subárvore direita */
    if (raiz != NULL && (*quantNomes) < maxNomes)
    {
        coletarNomesDeArtistas(raiz->dir, listaNomes, quantNomes, capacidade, maxNomes);
    }

}

/* ---------- Gera um nome inexistente a partir de uma base ---------- */
static void gerarNomeInexistente(char *destino, size_t tamanhoDestino, const char *base, int indice)
{
    const char *baseSegura = NULL;

    baseSegura = (base != NULL ? base : "artista");
    snprintf(destino, tamanhoDestino, "%s__nao_existe_%d", baseSegura, indice);

}

/* ================== PONTO DE ENTRADA DO EXPERIMENTO (30 BUSCAS) ================== */
void executarExperimentoBuscas(Artista *raizArt)
{
    const int QUANTIDADE_BUSCAS = 30;
    int arvoreEstaVazia = 0;
    char **listaNomes = NULL;
    int quantNomes = 0;
    int capacidade = 0;
    int indice = 0;
    int encontrados = 0;
    double somaTempos_ms = 0.0;
    int somaNosVisitados = 0;
    int i = 0;
    ResultadoBusca resultado;

    arvoreEstaVazia = (raizArt == NULL ? 1 : 0);

    if (arvoreEstaVazia == 1)
    {
        printf("Arvore vazia: insira artistas antes do experimento.\n");
    }
    else
    {
        /* 1) Coleta até 20 nomes reais */
        coletarNomesDeArtistas(raizArt, &listaNomes, &quantNomes, &capacidade, 20);

        /* 2) Completa até 30 com nomes inexistentes */
        for (indice = quantNomes; indice < QUANTIDADE_BUSCAS; indice++)
        {
            char nomeFalso[128];
            const char *base = (quantNomes > 0 ? listaNomes[indice % quantNomes] : "artista");

            gerarNomeInexistente(nomeFalso, sizeof(nomeFalso), base, indice);

            if (capacidade == 0)
            {
                capacidade = 16;
                listaNomes = (char **) malloc(capacidade * sizeof(char *));
            }
            if (indice >= capacidade)
            {
                capacidade *= 2;
                listaNomes = (char **) realloc(listaNomes, capacidade * sizeof(char *));
            }

            listaNomes[indice] = (char *) malloc(strlen(nomeFalso) + 1);
            strcpy(listaNomes[indice], nomeFalso);
        }

        printf("\n=== Experimento: %d buscas de ARTISTAS ===\n\n", QUANTIDADE_BUSCAS);

        /* 3) Executa as 30 buscas e acumula estatísticas */
        for (i = 0; i < QUANTIDADE_BUSCAS; i++)
        {
            resultado = buscarArtistaComCaminho(raizArt, listaNomes[i]);

            if (resultado.encontrado == 1)
                encontrados++;

            somaTempos_ms += resultado.tempo_ms;
            somaNosVisitados += resultado.nosVisitados;
        }

        /* 4) Resumo final */
        printf("=== Resumo ===\n");
        printf("- Encontrados: %d/%d\n", encontrados, QUANTIDADE_BUSCAS);
        printf("- Tempo total: %.3f ms\n", somaTempos_ms);
        printf("- Tempo medio: %.3f ms por busca\n", (double)(somaTempos_ms / QUANTIDADE_BUSCAS));
        printf("- Nos visitados (media): %.2f por busca\n\n", (double)somaNosVisitados / QUANTIDADE_BUSCAS);

        /* 5) Libera memória */
        for (i = 0; i < QUANTIDADE_BUSCAS; i++)
        {
            free(listaNomes[i]);
        }
        free(listaNomes);
    }

}
