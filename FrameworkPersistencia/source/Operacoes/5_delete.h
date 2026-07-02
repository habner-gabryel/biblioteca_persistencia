#ifndef DELETE_H
#define DELETE_H

#include <stdio.h>
#include <stdlib.h>

void deletee(pDFile arq, void* chave, FuncaoComparacao pfc) {
    (void)arq;
    (void)chave;
    (void)pfc;
}

int dDelete(dFile* df, int id) {
    if (df == NULL || df->arquivo == NULL || id < 1) {
        return -1;
    }

    long offset = (long)(id - 1) * (long)df->tamanhoRegistro;
    unsigned char* zeros = (unsigned char*)calloc(1, df->tamanhoRegistro);
    if (zeros == NULL) {
        return -1;
    }

    if (fseek(df->arquivo, offset, SEEK_SET) != 0) {
        free(zeros);
        return -1;
    }

    size_t written = fwrite(zeros, df->tamanhoRegistro, 1, df->arquivo);
    free(zeros);

    return (written == 1) ? 0 : -1;
}

#endif
