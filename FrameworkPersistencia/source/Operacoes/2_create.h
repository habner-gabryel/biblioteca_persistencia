#ifndef CREATE_H
#define CREATE_H

void createe (pDFile arq, void* dados){

    if (arq->arquivo == NULL){
        printf("Arquivo não foi aberto!");
        return;
    }

    fseek(arq->arquivo, 0, SEEK_END);
    fwrite(dados, arq->tamanhoRegistro, 1 , arq->arquivo);
}


#endif
