#ifndef OPEN_H
#define OPEN_H

#include <stdio.h>
#include <stdlib.h>

pDFile abrir(char arquivo[30], int tamanho) {
    return dOpen(arquivo, tamanho);
}

dFile* dOpen(const char* nomeArquivo, int tamanhoRegistro) {
    dFile* df = (dFile*)malloc(sizeof(dFile));
    if (df == NULL) {
        return NULL;
    }

    df->arquivo = fopen(nomeArquivo, "r+b");
    if (df->arquivo == NULL) {
        df->arquivo = fopen(nomeArquivo, "w+b");
    }

    if (df->arquivo == NULL) {
        free(df);
        return NULL;
    }

    df->tamanhoRegistro = tamanhoRegistro;
    return df;
}

#endif
