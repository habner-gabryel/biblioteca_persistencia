#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FrameworkPersistencia/source/entidades.h"
#include "FrameworkPersistencia/source/sistema.h"

static void limparArquivos(void) {
    remove("usuarios.dat");
    remove("livros.dat");
    remove("emprestimos.dat");
}

static int arquivoExiste(const char* nome) {
    FILE* f = fopen(nome, "rb");
    if (f == NULL) return 0;
    fclose(f);
    return 1;
}

int main(void) {
    limparArquivos();

    if (arquivoExiste("usuarios.dat") || arquivoExiste("livros.dat") || arquivoExiste("emprestimos.dat")) {
        fprintf(stderr, "Erro: arquivos antigos ainda existem\n");
        return 1;
    }

    Usuario usuario1 = {0, "Ana Costa", "12345678901", "(11)99999-0001", 0};
    Usuario usuario2 = {0, "Bruno Lima", "10987654321", "(11)99999-0002", 0};
    Livro livro1 = {0, "C para Iniciantes", "Autor Exemplo", 1, 0};
    int idEmprestimo1 = 0;

    if (criarUsuario(&usuario1) != 0) {
        fprintf(stderr, "Falha ao criar usuário 1\n");
        return 2;
    }

    if (criarUsuario(&usuario2) != 0) {
        fprintf(stderr, "Falha ao criar usuário 2\n");
        return 3;
    }

    if (criarLivro(&livro1) != 0) {
        fprintf(stderr, "Falha ao criar livro\n");
        return 4;
    }

    if (!arquivoExiste("usuarios.dat") || !arquivoExiste("livros.dat")) {
        fprintf(stderr, "Erro: arquivos de usuário ou livro não foram criados corretamente\n");
        return 5;
    }

    if (arquivoExiste("emprestimos.dat")) {
        fprintf(stderr, "Erro: arquivo de empréstimos não deveria existir antes do primeiro empréstimo\n");
        return 6;
    }

    if (registrarNovoEmprestimo(usuario1.id, livro1.id, "01/07/2026", "08/07/2026", &idEmprestimo1) != 0) {
        fprintf(stderr, "Falha ao registrar empréstimo válido\n");
        return 6;
    }

    Livro livroAtual;
    if (obterLivroPorId(livro1.id, &livroAtual) != 0) {
        fprintf(stderr, "Falha ao obter livro após empréstimo\n");
        return 7;
    }

    if (livroAtual.quantidade_disponivel != 0) {
        fprintf(stderr, "Quantidade incorreta após empréstimo: esperado 0, obtido %d\n", livroAtual.quantidade_disponivel);
        return 8;
    }

    if (registrarNovoEmprestimo(999, livro1.id, "01/07/2026", "08/07/2026", &idEmprestimo1) == 0) {
        fprintf(stderr, "Erro: empréstimo com usuário inválido foi aceito\n");
        return 9;
    }

    if (registrarNovoEmprestimo(usuario1.id, 999, "01/07/2026", "08/07/2026", &idEmprestimo1) == 0) {
        fprintf(stderr, "Erro: empréstimo com livro inválido foi aceito\n");
        return 10;
    }

    if (registrarNovoEmprestimo(usuario2.id, livro1.id, "01/07/2026", "08/07/2026", &idEmprestimo1) == 0) {
        fprintf(stderr, "Erro: empréstimo com livro sem estoque foi aceito\n");
        return 11;
    }

    if (registrarDevolucao(idEmprestimo1, "08/07/2026") != 0) {
        fprintf(stderr, "Falha ao registrar devolução dentro do prazo\n");
        return 12;
    }

    Emprestimo emprestimo;
    if (obterEmprestimoPorId(idEmprestimo1, &emprestimo) != 0) {
        fprintf(stderr, "Falha ao obter empréstimo após devolução\n");
        return 13;
    }

    if (emprestimo.status != 1) {
        fprintf(stderr, "Status incorreto após devolução: esperado 1, obtido %d\n", emprestimo.status);
        return 14;
    }

    if (strcmp(emprestimo.data_devolucao_real, "08/07/2026") != 0) {
        fprintf(stderr, "Data de devolução real incorreta: %s\n", emprestimo.data_devolucao_real);
        return 15;
    }

    if (obterLivroPorId(livro1.id, &livroAtual) != 0) {
        fprintf(stderr, "Falha ao obter livro após devolução\n");
        return 16;
    }

    if (livroAtual.quantidade_disponivel != 1) {
        fprintf(stderr, "Quantidade incorreta após devolução: esperado 1, obtido %d\n", livroAtual.quantidade_disponivel);
        return 17;
    }

    printf("OK\n");
    return 0;
}
