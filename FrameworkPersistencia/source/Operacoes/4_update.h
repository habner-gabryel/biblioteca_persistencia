#ifndef UPDATE_H
#define UPDATE_H

#include <stdio.h>

void update(pDFile arq, void* chave, void* dados, FuncaoComparacao pfc) {
    (void)arq;
    (void)chave;
    (void)dados;
    (void)pfc;
}

int dUpdate(dFile* df, int id, void* dado) {
    if (df == NULL || df->arquivo == NULL || dado == NULL || id < 1) {
        return -1;
    }

    long offset = (long)(id - 1) * (long)df->tamanhoRegistro;
    if (fseek(df->arquivo, offset, SEEK_SET) != 0) {
        return -1;
    }

    size_t escrito = fwrite(dado, df->tamanhoRegistro, 1, df->arquivo);
    return (escrito == 1) ? 0 : -1;
}

#endif
