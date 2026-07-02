#ifndef RETRIEVE_H
#define RETRIEVE_H

#include <stdio.h>
#include <stdlib.h>

void* retrieve(pDFile arq, void* chave, FuncaoComparacao pfc) {
    (void)arq;
    (void)chave;
    (void)pfc;
    return NULL;
}

int dRetrieve(dFile* df, int id, void* dado) {
    if (df == NULL || df->arquivo == NULL || dado == NULL || id < 1) {
        return -1;
    }

    long offset = (long)(id - 1) * (long)df->tamanhoRegistro;
    if (fseek(df->arquivo, offset, SEEK_SET) != 0) {
        return -1;
    }

    size_t lido = fread(dado, df->tamanhoRegistro, 1, df->arquivo);
    return (lido == 1) ? 0 : -1;
}

#endif
