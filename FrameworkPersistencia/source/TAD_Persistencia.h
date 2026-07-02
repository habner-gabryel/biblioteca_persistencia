#ifndef TAD_PERSISTENCIA_H
#define TAD_PERSISTENCIA_H

/* --------------------------
   Tipos de dados
   -------------------------- */
typedef struct dFile dFile;
typedef dFile* pDFile;

typedef int (*FuncaoComparacao)(void*, void*);
typedef void* (*FuncaoAloca)();
typedef void (*FuncaoImpressao)(void*);
typedef int (*FuncaoPredicado)(void*);
typedef int (*FuncaoPredicadoExt)(void*, void*);
typedef void* (*FuncaoAtualizacao)(void*, void*);

#endif

