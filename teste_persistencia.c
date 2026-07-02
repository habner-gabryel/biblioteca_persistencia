#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FrameworkPersistencia/source/Persistencia.h"

typedef struct {
    int id;
    char nome[20];
    int ativo;
} Pessoa;

int main(void) {
    const char* caminho = "teste_persistencia.bin";
    remove(caminho);

    dFile* df = dOpen(caminho, sizeof(Pessoa));
    if (df == NULL) {
        fprintf(stderr, "Falha ao abrir o arquivo\n");
        return 1;
    }

    Pessoa p1 = {0, "Ana", 1};
    Pessoa p2 = {0, "Bruno", 1};

    if (dCreate(df, &p1) != 0) {
        fprintf(stderr, "Falha ao criar primeiro registro\n");
        dClose(df);
        return 2;
    }

    if (dCreate(df, &p2) != 0) {
        fprintf(stderr, "Falha ao criar segundo registro\n");
        dClose(df);
        return 3;
    }

    Pessoa leitura;
    if (dRetrieve(df, 1, &leitura) != 0) {
        fprintf(stderr, "Falha ao recuperar registro\n");
        dClose(df);
        return 4;
    }

    if (strcmp(leitura.nome, "Ana") != 0) {
        fprintf(stderr, "Dados lidos incorretos\n");
        dClose(df);
        return 5;
    }

    Pessoa atualizada = {0, "Ana Silvia", 1};
    if (dUpdate(df, 1, &atualizada) != 0) {
        fprintf(stderr, "Falha ao atualizar registro\n");
        dClose(df);
        return 6;
    }

    if (dGetTotalRecords(df) != 2) {
        fprintf(stderr, "Total de registros incorreto\n");
        dClose(df);
        return 7;
    }

    if (dGetNextId(df) != 3) {
        fprintf(stderr, "Próximo ID incorreto\n");
        dClose(df);
        return 8;
    }

    if (dDelete(df, 2) != 0) {
        fprintf(stderr, "Falha ao excluir registro\n");
        dClose(df);
        return 9;
    }

    dClose(df);
    remove(caminho);

    puts("OK");
    return 0;
}
