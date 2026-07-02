#include "entidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parseData(const char* data, int* dia, int* mes, int* ano) {
    if (data == NULL || dia == NULL || mes == NULL || ano == NULL) {
        return 0;
    }

    if (sscanf(data, "%2d/%2d/%4d", dia, mes, ano) != 3) {
        return 0;
    }

    if (*ano < 1 || *mes < 1 || *mes > 12 || *dia < 1) {
        return 0;
    }

    int diasNoMes[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int bissexto = (*ano % 4 == 0 && (*ano % 100 != 0 || *ano % 400 == 0));

    if (*mes == 2 && bissexto) {
        diasNoMes[2] = 29;
    }

    if (*dia > diasNoMes[*mes]) {
        return 0;
    }

    return 1;
}

int dataValida(const char* data) {
    return parseData(data, &(int){0}, &(int){0}, &(int){0});
}

int dataParaNumero(const char* data) {
    int dia, mes, ano;
    if (!parseData(data, &dia, &mes, &ano)) {
        return -1;
    }
    return ano * 10000 + mes * 100 + dia;
}

int dataMaiorOuIgual(const char* data1, const char* data2) {
    int numero1 = dataParaNumero(data1);
    int numero2 = dataParaNumero(data2);

    if (numero1 < 0 || numero2 < 0) {
        return 0;
    }

    return numero1 >= numero2;
}
