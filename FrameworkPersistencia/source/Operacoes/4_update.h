#ifndef UPDATE_H
#define UPDATE_H

void update(pDFile arq, void* chave, void* dados, FuncaoComparacao pfc){

    if (arq->arquivo == NULL){
       printf("Arquivo não foi aberto!");
       return NULL;
   }

   void *registro = retrieve(arq, chave, pfc);
   if(registro != NULL)
   {
       fseek(arq->arquivo, - arq->tamanhoRegistro, SEEK_CUR);
       fwrite(dados, arq->tamanhoRegistro, 1, arq->arquivo);
   }


}

#endif
