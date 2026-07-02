#ifndef CLOSE_H
#define CLOSE_H

#include <stdio.h>
#include <stdlib.h>

void fechar(pDFile arq) {
    if (arq != NULL && arq->arquivo != NULL) {
        fclose(arq->arquivo);
        arq->arquivo = NULL;
    }
}

void dClose(dFile* df) {
    if (df == NULL) {
        return;
    }

    if (df->arquivo != NULL) {
        fclose(df->arquivo);
        df->arquivo = NULL;
    }

    free(df);
}

#endif
