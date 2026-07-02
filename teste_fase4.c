#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FrameworkPersistencia/source/entidades.h"
#include "FrameworkPersistencia/source/sistema.h"

int main(void) {
    remove("usuarios.dat");
    remove("livros.dat");
    remove("emprestimos.dat");

    Usuario usuario = {0, "Carlos Souza", "11122233344", "(11)98888-7777", 0};
    Livro livro = {0, "Aprendendo C", "Dennis Ritchie", 2, 0};

    if (criarUsuario(&usuario) != 0) {
        fprintf(stderr, "Erro criando usuario\n");
        return 1;
    }

    if (criarLivro(&livro) != 0) {
        fprintf(stderr, "Erro criando livro\n");
        return 2;
    }

    int idEmprestimo;
    if (registrarNovoEmprestimo(usuario.id, livro.id, "01/07/2026", "08/07/2026", &idEmprestimo) != 0) {
        fprintf(stderr, "Erro registrando novo emprestimo\n");
        return 3;
    }

    Livro livroAtual;
    if (obterLivroPorId(livro.id, &livroAtual) != 0) {
        fprintf(stderr, "Erro obtendo livro apos emprestimo\n");
        return 4;
    }

    if (livroAtual.quantidade_disponivel != 1) {
        fprintf(stderr, "Quantidade de livro incorreta apos emprestimo: %d\n", livroAtual.quantidade_disponivel);
        return 5;
    }

    if (registrarDevolucao(idEmprestimo, "08/07/2026") != 0) {
        fprintf(stderr, "Erro registrando devolucao\n");
        return 6;
    }

    Emprestimo emprestimo;
    if (obterEmprestimoPorId(idEmprestimo, &emprestimo) != 0) {
        fprintf(stderr, "Erro obtendo emprestimo apos devolucao\n");
        return 7;
    }

    if (emprestimo.status != 1) {
        fprintf(stderr, "Status esperado 1, encontrado %d\n", emprestimo.status);
        return 8;
    }

    if (strcmp(emprestimo.data_devolucao_real, "08/07/2026") != 0) {
        fprintf(stderr, "Data de devolucao real incorreta: %s\n", emprestimo.data_devolucao_real);
        return 9;
    }

    if (obterLivroPorId(livro.id, &livroAtual) != 0) {
        fprintf(stderr, "Erro obtendo livro apos devolucao\n");
        return 10;
    }

    if (livroAtual.quantidade_disponivel != 2) {
        fprintf(stderr, "Quantidade de livro incorreta apos devolucao: %d\n", livroAtual.quantidade_disponivel);
        return 11;
    }

    printf("OK\n");
    return 0;
}
