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


static void salvarListaDeMusicas(FILE *arquivo, Musica *listaDeMusicas) 
{
    Musica *musicaAtual;

    for (musicaAtual = listaDeMusicas; musicaAtual != NULL; musicaAtual = musicaAtual->proximo) 
    {
        fprintf(arquivo, "MUSICA;%s;%d\n",
                musicaAtual->info.titulo,
                musicaAtual->info.minutos);
    }
}


static void salvarArvoreDeAlbuns(FILE *arquivo, Album *raizAlbum) 
{

    int arvoreVazia;

    if (raizAlbum == NULL)
        arvoreVazia = 1;
    else
        arvoreVazia = 0;

    if (arvoreVazia != 1) 
    {
        // Percorre primeiro os álbuns da esquerda (menores)
        salvarArvoreDeAlbuns(arquivo, raizAlbum->esq);

        // Salva o álbum atual
        fprintf(arquivo, "ALBUM;%s;%d;%d\n",
                raizAlbum->info.titulo,
                raizAlbum->info.anoLancamento,
                raizAlbum->info.quantMusica);

        // Salva as músicas desse álbum
        salvarListaDeMusicas(arquivo, raizAlbum->info.musica);

        // Percorre os álbuns da direita (maiores)
        salvarArvoreDeAlbuns(arquivo, raizAlbum->dir);
    }

}

static void salvarArvoreDeArtistas(FILE *arquivo, Artista *raizArtista) 
{
    int arvoreVazia;

    if (raizArtista == NULL)
        arvoreVazia = 1;
    else
        arvoreVazia = 0;

    if (arvoreVazia != 1) 
    {
        salvarArvoreDeArtistas(arquivo, raizArtista->esq);

        fprintf(arquivo, "ARTISTA;%s;%s;%s\n",
                raizArtista->info.nome,
                raizArtista->info.estilo,
                raizArtista->info.tipo);

        salvarArvoreDeAlbuns(arquivo, raizArtista->info.albuns);
        salvarArvoreDeArtistas(arquivo, raizArtista->dir);
    }

}

int carregarBiblioteca(const char *caminho, Artista **raizArt)
{
    FILE   *arquivo = NULL;
    char    linha[512];
    Artista *artistaAtual = NULL;  // último ARTISTA lido (contexto)
    Album   *albumAtual   = NULL;  // último ALBUM lido (contexto)
    int     carregouComSucesso = 0;

    /* 1) Tenta abrir o arquivo */
    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        printf("Arquivo '%s' não encontrado. Iniciando vazio.\n", caminho);
        /* carregouComSucesso permanece 0 */
    } else {
        /* 2) Lê o arquivo linha a linha */
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            int linhaEhVazia, linhaEhComentario;
            removerQuebraLinha(linha);

            /* 2.1) Ignora linhas vazias e comentários */
            linhaEhVazia     = (linha[0] == '\0');
            linhaEhComentario = (!linhaEhVazia && linha[0] == '#');

            if (!linhaEhVazia && !linhaEhComentario) {

                /* 2.2) Seção de ARTISTA */
                if (strncmp(linha, "ARTISTA;", 8) == 0) {
                    char nome[50], estilo[50], tipo[50];

                    if (sscanf(linha, "ARTISTA;%49[^;];%49[^;];%49[^\n]",
                               nome, estilo, tipo) == 3)
                    {
                        infoArtista a;

                        /* Preenche struct de forma explícita */
                        strcpy(a.nome,   nome);
                        strcpy(a.estilo, estilo);
                        strcpy(a.tipo,   tipo);
                        a.numAlbuns = 0;
                        a.albuns    = NULL;

                        /* Insere na árvore RB de artistas */
                        insercao(raizArt, alocaArtista(a));

                        /* Atualiza o contexto de “último artista lido” */
                        artistaAtual = buscarArtista(*raizArt, a.nome);
                        albumAtual   = NULL; /* zera o contexto de álbum */
                    }

                /* 2.3) Seção de ALBUM (só faz sentido se houver um ARTISTA ativo) */
                } else if (strncmp(linha, "ALBUM;", 6) == 0) {
                    if (artistaAtual != NULL) {
                        char titulo[80];
                        int ano = 0, quant = 0;

                        if (sscanf(linha, "ALBUM;%79[^;];%d;%d",
                                   titulo, &ano, &quant) >= 2)
                        {
                            infoAlbum ab;

                            strcpy(ab.titulo, titulo);
                            ab.anoLancamento = ano;
                            ab.quantMusica   = quant;
                            ab.musica        = NULL;

                            /* Insere na árvore RB de álbuns desse artista */
                            insercaoAlbum(&artistaAtual->info.albuns, criarNoAlbum(ab));

                            /* Atualiza o contexto de “último álbum lido” */
                            albumAtual = BuscarNoPorTituloAlbum(artistaAtual->info.albuns, ab.titulo);

                            /* Atualiza contador do artista */
                            artistaAtual->info.numAlbuns++;
                        }
                    }

                /* 2.4) Seção de MUSICA (só faz sentido se houver um ALBUM ativo) */
                } else if (strncmp(linha, "MUSICA;", 7) == 0) {
                    if (albumAtual != NULL) {
                        char titulo[80];
                        int duracao = 0;

                        if (sscanf(linha, "MUSICA;%79[^;];%d", titulo, &duracao) >= 1) {
                            Musica *novaMusica = alocarNo();
                            strcpy(novaMusica->info.titulo, titulo);
                            novaMusica->info.minutos = duracao;

                            /* Insere no fim da lista/ordem; se duplicada, descarta */
                            if (!inserirMusica(&albumAtual->info.musica, novaMusica)) {
                                free(novaMusica);
                            }
                        }
                    }
                }

                /* (Demais tipos de linha são ignorados silenciosamente) */
            }
        }

        /* 3) Fechamento do arquivo e mensagem de sucesso */
        fclose(arquivo);
        arquivo = NULL;

        printf("Arquivo '%s' carregado com sucesso!\n", caminho);
        carregouComSucesso = 1;
    }

    return carregouComSucesso;
}

int salvarBiblioteca(const char *caminho, Artista *raizArtista)
{
    FILE *arquivo = NULL;
    int salvamentoRealizado = 0;  // 0 = falhou, 1 = sucesso

    /* 1) Tenta abrir o arquivo para escrita */
    arquivo = fopen(caminho, "w");

    /* 2) Verifica se a abertura foi bem-sucedida */
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo '%s' para escrita.\n", caminho);
    }
    else
    {
        /* 3) Escreve o cabeçalho informativo no início do arquivo */
        fprintf(arquivo, "# Biblioteca de Música\n");

        /* 4) Salva todos os artistas, álbuns e músicas */
        salvarArvoreDeArtistas(arquivo, raizArtista);

        /* 5) Fecha o arquivo após o salvamento */
        fclose(arquivo);
        arquivo = NULL;

        /* 6) Exibe mensagem de confirmação */
        printf("Dados salvos com sucesso em '%s'.\n", caminho);

        /* Marca o salvamento como realizado */
        salvamentoRealizado = 1;
    }

    /* 7) Retorno único indicando sucesso (1) ou falha (0) */
    return salvamentoRealizado;
}
