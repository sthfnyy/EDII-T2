#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "artista.h"
#include "album.h"
#include "musica.h"


static void removerQuebraLinha(char *texto)
{
    if (texto != NULL)
        texto[strcspn(texto, "\r\n")] = '\0';
}


static void salvarListaDeMusicas23(FILE *arquivo, Musica *lista)
{
    Musica *m = lista;
    while (m != NULL)
    {
        fprintf(arquivo, "MUSICA;%s;%d\n", m->info.titulo, m->info.minutos);
        m = m->proximo;
    }
}

/* Árvore 2–3 de Álbuns (in-order para 2–3)
   Visita: ESQ -> infoUm -> CEN -> (infoDois se existir) -> DIR
   E para cada info, salva também a lista de músicas daquela info */
static void salvarArvoreDeAlbuns23(FILE *arquivo, Album *raizAlb)
{
    if (raizAlb != NULL)
    {
        /* ESQ */
        salvarArvoreDeAlbuns23(arquivo, raizAlb->esq);

        /* infoUm + músicas */
        fprintf(arquivo, "ALBUM;%s;%d;%d\n",
                raizAlb->infoUm.titulo,
                raizAlb->infoUm.anoLancamento,
                raizAlb->infoUm.quantMusica);
        salvarListaDeMusicas23(arquivo, raizAlb->infoUm.musica);

        /* CEN */
        salvarArvoreDeAlbuns23(arquivo, raizAlb->cen);

        /* infoDois (se existir) + músicas, então DIR */
        if (raizAlb->quantInfo == 2)
        {
            fprintf(arquivo, "ALBUM;%s;%d;%d\n",
                    raizAlb->infoDois.titulo,
                    raizAlb->infoDois.anoLancamento,
                    raizAlb->infoDois.quantMusica);
            salvarListaDeMusicas23(arquivo, raizAlb->infoDois.musica);

            salvarArvoreDeAlbuns23(arquivo, raizAlb->dir);
        }
    }
}

/* Árvore 2–3 de Artistas (in-order para 2–3)
   Visita: ESQ -> infoUm -> CEN -> (infoDois se existir) -> DIR
   E para cada info, salva também a árvore 2–3 de álbuns daquela info */
static void salvarArvoreDeArtistas23(FILE *arquivo, Artista *raizArt)
{
    if (raizArt != NULL)
    {
        /* ESQ */
        salvarArvoreDeArtistas23(arquivo, raizArt->esq);

        /* infoUm + toda árvore de álbuns daquela info */
        fprintf(arquivo, "ARTISTA;%s;%s;%s\n",
                raizArt->infoUm.nome,
                raizArt->infoUm.estilo,
                raizArt->infoUm.tipo);
        salvarArvoreDeAlbuns23(arquivo, raizArt->infoUm.albuns);

        /* CEN */
        salvarArvoreDeArtistas23(arquivo, raizArt->cen);

        /* infoDois (se existir) + álbuns + DIR */
        if (raizArt->Ninfos == 2)
        {
            fprintf(arquivo, "ARTISTA;%s;%s;%s\n",
                    raizArt->infoDois.nome,
                    raizArt->infoDois.estilo,
                    raizArt->infoDois.tipo);
            salvarArvoreDeAlbuns23(arquivo, raizArt->infoDois.albuns);

            salvarArvoreDeArtistas23(arquivo, raizArt->dir);
        }
    }
}


int carregarBiblioteca23(const char *caminho, Artista **raizArt)
{
    FILE *arquivo = NULL;
    char  linha[512];
    infoArtista *artistaAtualInfo = NULL;  /* contexto: info do artista corrente */
    infoAlbum   *albumAtualInfo   = NULL;  /* contexto: info do álbum corrente   */
    int carregou = 0;

    arquivo = fopen(caminho, "r");
    if (arquivo == NULL)
    {
        printf("Arquivo '%s' não encontrado. Iniciando vazio.\n", caminho);
    }
    else
    {
        while (fgets(linha, sizeof(linha), arquivo) != NULL)
        {
            int vazia;
            int comentario;
            removerQuebraLinha(linha);

            vazia      = (linha[0] == '\0');
            comentario = (!vazia && linha[0] == '#');

            if (!vazia && !comentario)
            {
                if (strncmp(linha, "ARTISTA;", 8) == 0)
                {
                    char nome[50], estilo[50], tipo[50];

                    if (sscanf(linha, "ARTISTA;%49[^;];%49[^;];%49[^\n]",
                               nome, estilo, tipo) == 3)
                    {
                        infoArtista a;
                        strcpy(a.nome,   nome);
                        strcpy(a.estilo, estilo);
                        strcpy(a.tipo,   tipo);
                        a.numAlbuns = 0;
                        a.albuns    = NULL;

                        /* insere na 2–3 de artistas */
                        {
                            infoArtista sobe;
                            inserirArtista(raizArt, NULL, a, &sobe);
                        }

                        /* atualiza contexto com a info (infoUm ou infoDois) onde o nome ficou */
                        artistaAtualInfo = buscarInfoArtista(*raizArt, a.nome);
                        albumAtualInfo   = NULL;
                    }
                }
                else if (strncmp(linha, "ALBUM;", 6) == 0)
                {
                    if (artistaAtualInfo != NULL)
                    {
                        char titulo[80];
                        int  ano = 0, quant = 0;

                        if (sscanf(linha, "ALBUM;%79[^;];%d;%d", titulo, &ano, &quant) >= 2)
                        {
                            infoAlbum ab;
                            strcpy(ab.titulo, titulo);
                            ab.anoLancamento = ano;
                            ab.quantMusica   = quant;
                            ab.musica        = NULL;

                            /* insere na 2–3 de álbuns daquela info de artista */
                            {
                                infoAlbum sobe;
                                inserirNoAlbum(&artistaAtualInfo->albuns, NULL, ab, &sobe);
                            }

                            /* atualiza contexto para a info do álbum recém inserido */
                            albumAtualInfo = buscarInfoAlbum(artistaAtualInfo->albuns, ab.titulo);

                            /* contador da info do artista */
                            artistaAtualInfo->numAlbuns++;
                        }
                    }
                }
                else if (strncmp(linha, "MUSICA;", 7) == 0)
                {
                    if (albumAtualInfo != NULL)
                    {
                        char titulo[80];
                        int  duracao = 0;

                        if (sscanf(linha, "MUSICA;%79[^;];%d", titulo, &duracao) >= 1)
                        {
                            Musica *nova = alocarNo();
                            if (nova != NULL)
                            {
                                strcpy(nova->info.titulo, titulo);
                                nova->info.minutos = duracao;
                                nova->proximo = NULL;

                                if (!inserirMusica(&albumAtualInfo->musica, nova))
                                {
                                    free(nova);
                                }
                            }
                        }
                    }
                }
               
            }
        }

        fclose(arquivo);
        arquivo = NULL;

        printf("Arquivo '%s' carregado com sucesso!\n", caminho);
        carregou = 1;
    }

    return carregou;
}

int salvarBiblioteca23(const char *caminho, Artista *raizArt)
{
    FILE *arquivo = NULL;
    int   ok = 0;

    arquivo = fopen(caminho, "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo '%s' para escrita.\n", caminho);
    }
    else
    {
        fprintf(arquivo, "# Biblioteca de Música (2-3)\n");
        salvarArvoreDeArtistas23(arquivo, raizArt);
        fclose(arquivo);
        arquivo = NULL;

        printf("Dados salvos com sucesso em '%s'.\n", caminho);
        ok = 1;
    }

    return ok;
}
