#ifndef QUERYALL_H
#define QUERYALL_H

pDLista queryAll(pDFile arq){

   if (arq->arquivo == NULL){
       printf("Arquivo não foi aberto!");
       return NULL;
   }

   pDLista registros = criarLista();

    void* registro = malloc(arq->tamanhoRegistro);
    rewind(arq->arquivo);

    while(fread(registro, arq->tamanhoRegistro, 1 , arq->arquivo) != 0){
        incluirInfo(registros, registro);
        registro = malloc(arq->tamanhoRegistro);
    }

    return registros;

}

#endif
