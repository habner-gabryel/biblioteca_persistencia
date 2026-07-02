#ifndef ENTIDADES_H
#define ENTIDADES_H

#define USUARIO_NOME_TAM 100
#define USUARIO_CPF_TAM 15
#define USUARIO_TELEFONE_TAM 20

#define LIVRO_TITULO_TAM 200
#define LIVRO_AUTOR_TAM 100

#define DATA_TAM 11

typedef struct {
    int id;
    char nome[USUARIO_NOME_TAM];
    char cpf[USUARIO_CPF_TAM];
    char telefone[USUARIO_TELEFONE_TAM];
    int ativo; // 1 = ativo, 0 = inativo
} Usuario;

typedef struct {
    int id;
    char titulo[LIVRO_TITULO_TAM];
    char autor[LIVRO_AUTOR_TAM];
    int quantidade_disponivel;
    int ativo;
} Livro;

typedef struct {
    int id;
    int id_usuario;
    int id_livro;
    char data_emprestimo[DATA_TAM];
    char data_devolucao_prevista[DATA_TAM];
    char data_devolucao_real[DATA_TAM];
    int status; // 0 = Ativo, 1 = Devolvido, 2 = Atrasado
    int ativo;
} Emprestimo;

int dataValida(const char* data);
int dataParaNumero(const char* data);
int dataMaiorOuIgual(const char* data1, const char* data2);

#endif // ENTIDADES_H
