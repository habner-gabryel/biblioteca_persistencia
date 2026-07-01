#ifndef QUERY_H
#define QUERY_H

pDLista queryBy  (pDFile arq, FuncaoPredicado pfp){

   if (arq->arquivo == NULL){
       printf("Arquivo não foi aberto!");
       return NULL;
   }

   pDLista registros = criarLista();

    void* registro = malloc(arq->tamanhoRegistro);
    rewind(arq->arquivo);

    while(fread(registro, arq->tamanhoRegistro, 1 , arq->arquivo) != 0){
        if (pfp(registro) == 1){
            incluirInfo(registros, registro);
            registro = malloc(arq->tamanhoRegistro);
        }
    }

    return registros;
}

#endif

