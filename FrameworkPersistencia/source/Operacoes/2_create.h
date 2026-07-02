#ifndef CREATE_H
#define CREATE_H

#include <stdio.h>

void createe(pDFile arq, void* dados) {
    (void)dCreate(arq, dados);
}

int dCreate(dFile* df, void* dado) {
    if (df == NULL || df->arquivo == NULL || dado == NULL) {
        return -1;
    }

    if (fseek(df->arquivo, 0, SEEK_END) != 0) {
        return -1;
    }

    size_t escrito = fwrite(dado, df->tamanhoRegistro, 1, df->arquivo);
    return (escrito == 1) ? 0 : -1;
}

#endif
