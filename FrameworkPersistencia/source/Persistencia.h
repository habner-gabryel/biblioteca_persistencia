#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <stdio.h>
#include <stdlib.h>

#include "TAD_Persistencia.h"

/* API pública da biblioteca de persistência */
dFile* dOpen(const char* nomeArquivo, int tamanhoRegistro);
void dClose(dFile* df);
int dCreate(dFile* df, void* dado);
int dRetrieve(dFile* df, int id, void* dado);
int dUpdate(dFile* df, int id, void* dado);
int dDelete(dFile* df, int id);
int dGetTotalRecords(dFile* df);
int dGetNextId(dFile* df);

#include "Operacoes/0_structs.h"
#include "Operacoes/1_open.h"
#include "Operacoes/2_create.h"
#include "Operacoes/3_retrieve.h"
#include "Operacoes/4_update.h"
#include "Operacoes/5_delete.h"
#include "Operacoes/6_close.h"
#include "Operacoes/7_queryAll.h"
#include "Operacoes/8_query.h"
#include "Operacoes/9_persistAll.h"

#endif


