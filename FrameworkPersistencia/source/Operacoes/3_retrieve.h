#ifndef RETRIEVE_H
#define RETRIEVE_H

void* retrieve (pDFile arq, void* chave, FuncaoComparacao pfc){

   if (arq->arquivo == NULL){
       printf("Arquivo não foi aberto!");
       return NULL;
   }

   void* registro = malloc(arq->tamanhoRegistro);
    rewind(arq->arquivo);

    while(fread(registro, arq->tamanhoRegistro, 1 , arq->arquivo) != 0){
       if(pfc(chave, registro) == 0)
            return registro;
    }

    return NULL;
}


#endif
