#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FrameworkPersistencia/source/entidades.h"
#include "FrameworkPersistencia/source/sistema.h"

int main(void) {
    Usuario usuario = {0, "Maria Silva", "12345678901", "(11)99999-0000", 0};
    Livro livro = {0, "C Programando em C", "Brian Kernighan", 3, 0};
    Emprestimo emprestimo = {0, 0, 0, "01/07/2026", "08/07/2026", "", 0, 0};

    if (criarUsuario(&usuario) != 0) {
        fprintf(stderr, "Erro criando usuário\n");
        return 1;
    }

    if (criarLivro(&livro) != 0) {
        fprintf(stderr, "Erro criando livro\n");
        return 2;
    }

    printf("Usuario criado id=%d\n", usuario.id);
    printf("Livro criado id=%d\n", livro.id);

    if (atualizarUsuario(&usuario) != 0) {
        fprintf(stderr, "Erro atualizando usuário\n");
        return 3;
    }

    emprestimo.id_usuario = usuario.id;
    emprestimo.id_livro = livro.id;
    if (criarEmprestimo(&emprestimo) != 0) {
        fprintf(stderr, "Erro criando empréstimo\n");
        return 4;
    }

    printf("Emprestimo criado id=%d\n", emprestimo.id);

    Usuario* usuarios = NULL;
    int quantidadeUsuarios = 0;
    if (listarUsuariosAtivos(&usuarios, &quantidadeUsuarios) != 0) {
        fprintf(stderr, "Erro listando usuarios\n");
        return 5;
    }

    printf("Usuarios ativos: %d\n", quantidadeUsuarios);
    liberarLista(usuarios);

    Livro* livros = NULL;
    int quantidadeLivros = 0;
    if (listarLivrosDisponiveis(&livros, &quantidadeLivros) != 0) {
        fprintf(stderr, "Erro listando livros\n");
        return 6;
    }

    printf("Livros disponiveis: %d\n", quantidadeLivros);
    liberarLista(livros);

    Emprestimo* emprestimos = NULL;
    int quantidadeEmprestimos = 0;
    if (listarEmprestimosAtivos(&emprestimos, &quantidadeEmprestimos) != 0) {
        fprintf(stderr, "Erro listando emprestimos\n");
        return 7;
    }

    printf("Emprestimos ativos: %d\n", quantidadeEmprestimos);
    liberarLista(emprestimos);

    return 0;
}
