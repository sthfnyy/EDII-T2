#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "album.h"


// Preenche a struct infoCidade com nome e população
void preencherCidade(infoCidade *cidade, char *nome_cidade, int populacaoC) {
    strncpy(cidade->nome_cidade, nome_cidade, TAM_STRING - 1);
    cidade->nome_cidade[TAM_STRING - 1] = '\0';
    cidade->populacao_c = populacaoC;
    cidade->avr_CEP = NULL;
}


Cidade *alocarCidade(infoCidade dado, Cidade *fEsq, Cidade *fCen) 
{
    Cidade *novo = (Cidade*) malloc(sizeof(Cidade));
    if (novo != NULL) {
        novo->info_um = dado;
        novo->qntd_info = 1;
        novo->esq = fEsq;
        novo->cen = fCen;
        novo->dir = NULL;
    }
    return novo;
}


void inserir_infoCidade(Cidade **novoNo, infoCidade dado, Cidade *subArvInfo) 
{
    if (strcmp(dado.nome_cidade, (*novoNo)->info_um.nome_cidade) > 0) {
        (*novoNo)->info_dois = dado;
        (*novoNo)->dir = subArvInfo;
    } else {
        (*novoNo)->info_dois = (*novoNo)->info_um;
        (*novoNo)->info_um = dado;
        (*novoNo)->dir = (*novoNo)->cen;
        (*novoNo)->cen = subArvInfo;
    }
    (*novoNo)->qntd_info = 2;
}

Cidade *quebrarNoCidade(Cidade **no, infoCidade dado, infoCidade *sobe, Cidade *filhoDir) 
{
    Cidade *maior;

    if (strcmp(dado.nome_cidade, (*no)->info_dois.nome_cidade) > 0) {
        *sobe = (*no)->info_dois;
        maior = alocarCidade(dado, (*no)->dir, filhoDir);
        (*no)->qntd_info = 1;
    } else if (strcmp(dado.nome_cidade, (*no)->info_um.nome_cidade) > 0) {
        *sobe = dado;
        maior = alocarCidade((*no)->info_dois, filhoDir, (*no)->dir);
        (*no)->qntd_info = 1;
    } else {
        *sobe = (*no)->info_um;
        maior = alocarCidade((*no)->info_dois, (*no)->cen, (*no)->dir);
        (*no)->info_um = dado;
        (*no)->cen = filhoDir;
        (*no)->qntd_info = 1;
    }

    return maior;
}

Cidade *inserirCidade(Cidade **raiz, Cidade *pai, infoCidade dado, infoCidade *sobe)
{
    Cidade *maiorNo = NULL;

    if (*raiz == NULL) {
        *raiz = alocarCidade(dado, NULL, NULL);
    } else if ((*raiz)->esq == NULL) {
        if ((*raiz)->qntd_info == 1) {
            inserir_infoCidade(raiz, dado, NULL);
        } else {
            maiorNo = quebrarNoCidade(raiz, dado, sobe, NULL);
            if (pai == NULL) {
                *raiz = alocarCidade(*sobe, *raiz, maiorNo);
                maiorNo = NULL;
            }
        }
    } else {
        if (strcmp(dado.nome_cidade, (*raiz)->info_um.nome_cidade) < 0) {
            maiorNo = inserirCidade(&(*raiz)->esq, *raiz, dado, sobe);
        } else if ((*raiz)->qntd_info == 1 || strcmp(dado.nome_cidade, (*raiz)->info_dois.nome_cidade) < 0) {
            maiorNo = inserirCidade(&(*raiz)->cen, *raiz, dado, sobe);
        } else {
            maiorNo = inserirCidade(&(*raiz)->dir, *raiz, dado, sobe);
        }

        if (maiorNo != NULL) {
            if ((*raiz)->qntd_info == 1) {
                inserir_infoCidade(raiz, *sobe, maiorNo);
                maiorNo = NULL;
            } else {
                maiorNo = quebrarNoCidade(raiz, *sobe, sobe, maiorNo);
                if (pai == NULL) {
                    *raiz = alocarCidade(*sobe, *raiz, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
    }

    return maiorNo;
}

void imprimirInOrdemCidade(Cidade *raiz) 
{
    if (raiz != NULL){

        // Percorre a esquerda
        imprimirInOrdemCidade(raiz->esq);

        // Visita o primeiro valor
        printf("1. nome:%s | n_populcao:%d\n", 
            raiz->info_um.nome_cidade,
            raiz->info_um.populacao_c);

        // Percorre o centro
        imprimirInOrdemCidade(raiz->cen);

        // Se for nó 2-info, visita segundo valor e percorre direita
        if (raiz->qntd_info == 2) {
            printf("2. nome:%s | n_populcao:%d\n", 
                raiz->info_dois.nome_cidade
                , raiz->info_dois.populacao_c);
            imprimirInOrdemCidade(raiz->dir);
        }
    }
}

void imprimirInOrdemCidadeComRaiz(Cidade *raiz) 
{
    printf("Valores em ordem: \n");
    imprimirInOrdemCidade(raiz);
    printf("\n");

    if (raiz) {
        printf("Raiz: ");
        if (raiz->qntd_info == 1)
            printf("[%s]\n", raiz->info_um.nome_cidade);
        else
            printf("[%s | %s]\n", raiz->info_um.nome_cidade, raiz->info_dois.nome_cidade);
    } else {
        printf("Árvore vazia.\n");
    }
}


Cidade* buscarCidade(Cidade *raiz, char *valor) 
{
    Cidade *resultado = NULL;

    if (raiz != NULL) {
        if (strcmp(raiz->info_um.nome_cidade, valor) == 0) {
            resultado = raiz;
        } else if (raiz->qntd_info == 2 && strcmp(raiz->info_dois.nome_cidade, valor) == 0) {
            resultado = raiz;
        } else if (strcmp(valor, raiz->info_um.nome_cidade) < 0) {
            resultado = buscarCidade(raiz->esq, valor);
        } else if (raiz->qntd_info == 1 || strcmp(valor, raiz->info_dois.nome_cidade) < 0) {
            resultado = buscarCidade(raiz->cen, valor);
        } else {
            resultado = buscarCidade(raiz->dir, valor);
        }
    }

    return resultado;
}

void liberar_arvore_cidades(Cidade **raiz) 
{
    if (*raiz) {
        liberar_arvore_cidades(&(*raiz)->esq);
        liberar_arvore_cidades(&(*raiz)->cen);
        if ((*raiz)->qntd_info == 2)
            liberar_arvore_cidades(&(*raiz)->dir);
        free(*raiz);
        *raiz = NULL;
    }
}


Cidade *buscarCidadePorCEP(Cidade *raiz, int valorCEP) {
    if (raiz == NULL)
        return NULL;

    if (buscarCEP(raiz->info_um.avr_CEP, valorCEP) != NULL) {
        return raiz;
    }

    if (raiz->qntd_info == 2 && buscarCEP(raiz->info_dois.avr_CEP, valorCEP) != NULL) {
        return raiz;
    }

    Cidade *resultado = buscarCidadePorCEP(raiz->esq, valorCEP);
    if (resultado != NULL) return resultado;

    resultado = buscarCidadePorCEP(raiz->cen, valorCEP);
    if (resultado != NULL) return resultado;

    if (raiz->qntd_info == 2) {
        resultado = buscarCidadePorCEP(raiz->dir, valorCEP);
        if (resultado != NULL) return resultado;
    }

    return NULL;
}

// Retorna a cidade mais populosa de um estado (exceto a capital)
infoCidade cidade_mais_populosa_de_um_estado_sem_ser_a_capital(Cidade *raiz, char *nome_capital) {
    infoCidade mais_populosa;
    mais_populosa.populacao_c = -1; // valor inicial impossível para garantir comparação

    if (raiz == NULL)
        return mais_populosa;

    // Verifica info_um

    if (strcmp(raiz->info_um.nome_cidade, nome_capital) != 0) {
        printf("AQUI");
        if (raiz->info_um.populacao_c > mais_populosa.populacao_c) {
            mais_populosa = raiz->info_um;
        }
    }

    // Verifica info_dois, se existir
    if (raiz->qntd_info == 2 && strcmp(raiz->info_dois.nome_cidade, nome_capital) != 0) {
        if (raiz->info_dois.populacao_c > mais_populosa.populacao_c) {
            mais_populosa = raiz->info_dois;
        }
    }

    // Recursão nas subárvores: esquerda, centro e direita
    infoCidade esq = cidade_mais_populosa_de_um_estado_sem_ser_a_capital(raiz->esq, nome_capital);
    if (esq.populacao_c > mais_populosa.populacao_c) {
        mais_populosa = esq;
    }

    infoCidade cen = cidade_mais_populosa_de_um_estado_sem_ser_a_capital(raiz->cen, nome_capital);
    if (cen.populacao_c > mais_populosa.populacao_c) {
        mais_populosa = cen;
    }

    infoCidade dir = cidade_mais_populosa_de_um_estado_sem_ser_a_capital(raiz->dir, nome_capital);
    if (dir.populacao_c > mais_populosa.populacao_c) {
        mais_populosa = dir;
    }

    return mais_populosa;
}