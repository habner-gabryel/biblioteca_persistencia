#ifndef QUERYALL_H
#define QUERYALL_H

#include <stdio.h>
#include <stdlib.h>

int dGetTotalRecords(dFile* df) {
    if (df == NULL || df->arquivo == NULL) {
        return -1;
    }

    long posicaoAtual = ftell(df->arquivo);
    fseek(df->arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(df->arquivo);
    fseek(df->arquivo, posicaoAtual, SEEK_SET);

    if (df->tamanhoRegistro <= 0) {
        return -1;
    }

    return (int)(tamanhoArquivo / df->tamanhoRegistro);
}

int dGetNextId(dFile* df) {
    int total = dGetTotalRecords(df);
    return (total < 0) ? 1 : total + 1;
}

#endif
