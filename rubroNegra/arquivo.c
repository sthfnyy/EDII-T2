#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"
#include "album.h"
#include "musica.h"

// --- Função auxiliar ---
static void corta_newline(char *s) { if (s) s[strcspn(s, "\r\n")] = '\0'; }

static void salvarMusicas(FILE *f, Musica *lista) {
    for (Musica *p = lista; p; p = p->proximo)
        fprintf(f, "MUSICA;%s;%d\n", p->info.titulo, p->info.minutos);
}

static void salvarAlbuns(FILE *f, Album *raizAlb) {
    if (!raizAlb) return;
    salvarAlbuns(f, raizAlb->esq);
    fprintf(f, "ALBUM;%s;%d;%d\n", raizAlb->info.titulo,
            raizAlb->info.anoLancamento, raizAlb->info.quantMusica);
    salvarMusicas(f, raizAlb->info.musica);
    salvarAlbuns(f, raizAlb->dir);
}

static void salvarArtistas(FILE *f, Artista *raizArt) {
    if (!raizArt) return;
    salvarArtistas(f, raizArt->esq);
    fprintf(f, "ARTISTA;%s;%s;%s\n", raizArt->info.nome,
            raizArt->info.estilo, raizArt->info.tipo);
    salvarAlbuns(f, raizArt->info.albuns);
    salvarArtistas(f, raizArt->dir);
}

int carregarBiblioteca(const char *caminho, Artista **raizArt) {
    FILE *f = fopen(caminho, "r");
    if (!f) {
        printf("Arquivo '%s' não encontrado. Iniciando vazio.\n", caminho);
        return 0;
    }

    char linha[512];
    Artista *artAtual = NULL;
    Album *albAtual = NULL;

    while (fgets(linha, sizeof(linha), f)) {
        corta_newline(linha);
        if (!linha[0] || linha[0] == '#') continue;  // ignora linhas vazias ou comentários

        if (strncmp(linha, "ARTISTA;", 8) == 0) {
            char nome[50], estilo[50], tipo[50];
            if (sscanf(linha, "ARTISTA;%49[^;];%49[^;];%49[^\n]", nome, estilo, tipo) == 3) {
                infoArtista a;
                strcpy(a.nome, nome);
                strcpy(a.estilo, estilo);
                strcpy(a.tipo, tipo);
                a.numAlbuns = 0;
                a.albuns = NULL;
                insercao(raizArt, alocaArtista(a));
                artAtual = buscarArtista(*raizArt, a.nome);
                albAtual = NULL;
            }
        }
        else if (strncmp(linha, "ALBUM;", 6) == 0) {
            if (!artAtual) continue; // precisa de artista ativo
            char titulo[80];
            int ano, quant;
            if (sscanf(linha, "ALBUM;%79[^;];%d;%d", titulo, &ano, &quant) >= 2) {
                infoAlbum ab;
                strcpy(ab.titulo, titulo);
                ab.anoLancamento = ano;
                ab.quantMusica = quant;
                ab.musica = NULL;
                insercaoAlbum(&artAtual->info.albuns, criarNoAlbum(ab));
                albAtual = BuscarNoPorTituloAlbum(artAtual->info.albuns, ab.titulo);
                artAtual->info.numAlbuns++;
            }
        }
        else if (strncmp(linha, "MUSICA;", 7) == 0) {
            if (!albAtual) continue; // precisa de álbum ativo
            char titulo[80];
            int duracao;
            if (sscanf(linha, "MUSICA;%79[^;];%d", titulo, &duracao) >= 1) {
                Musica *m = alocarNo();
                strcpy(m->info.titulo, titulo);
                m->info.minutos = duracao;
                if (!inserirMusica(&albAtual->info.musica, m))
                    free(m);
            }
        }
    }

    fclose(f);
    printf("Arquivo '%s' carregado com sucesso!\n", caminho);
    return 1;
}

int salvarBiblioteca(const char *caminho, Artista *raizArt) {
    FILE *f = fopen(caminho, "w");
    if (!f) {
        printf("Erro ao abrir '%s' para escrita.\n", caminho);
        return 0;
    }
    fprintf(f, "# Biblioteca de Música\n");
    salvarArtistas(f, raizArt);
    fclose(f);
    printf("Dados salvos em '%s'.\n", caminho);
    return 1;
}
