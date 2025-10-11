#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tipo.h"
#include "album.h"

Album *criarNo(infoAlbum album)
{
    Album *novoNo = (Album*) malloc(sizeof(Album));
    if (novoNo != NULL)
    {
        novoNo->info = album;
        novoNo->esq = NULL;
        novoNo->dir = NULL;
        novoNo->cor = VERMELHO;
    }
    return novoNo;
}

infoAlbum preencherAlbum()
{
    infoAlbum dados;
    printf("Digite o titulo do album: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]", dados.titulo);
    printf("Digite o ano de lancamento do album: ");
    scanf("%d", dados.anoLancamento);
    printf("Digite a quantidade de musica do album: ");
    scanf("%d", dados.quantMusica);
    dados.musica = NULL;
    return dados;
}

