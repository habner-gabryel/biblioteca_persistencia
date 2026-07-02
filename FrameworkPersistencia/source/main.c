#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sistema.h"
#include "entidades.h"

static void lerString(const char* prompt, char* buffer, size_t tamanho) {
    printf("%s", prompt);
    if (fgets(buffer, (int)tamanho, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

static int lerInteiro(const char* prompt) {
    char buffer[32];
    lerString(prompt, buffer, sizeof(buffer));
    return (int)strtol(buffer, NULL, 10);
}

static void aguardarEnter(void) {
    printf("\nPressione Enter para continuar...");
    fflush(stdout);
    while (getchar() != '\n') {
        if (feof(stdin)) {
            break;
        }
    }
}

static void imprimirUsuario(const Usuario* usuario) {
    if (usuario == NULL) {
        return;
    }
    printf("ID: %d\n", usuario->id);
    printf("Nome: %s\n", usuario->nome);
    printf("CPF: %s\n", usuario->cpf);
    printf("Telefone: %s\n", usuario->telefone);
    printf("Status: %s\n", usuario->ativo == 1 ? "Ativo" : "Inativo");
    printf("---------------------------\n");
}

static void imprimirLivro(const Livro* livro) {
    if (livro == NULL) {
        return;
    }
    printf("ID: %d\n", livro->id);
    printf("Título: %s\n", livro->titulo);
    printf("Autor: %s\n", livro->autor);
    printf("Quantidade disponível: %d\n", livro->quantidade_disponivel);
    printf("Status: %s\n", livro->ativo == 1 ? "Ativo" : "Inativo");
    printf("---------------------------\n");
}

static const char* statusEmprestimo(int status) {
    switch (status) {
        case 0: return "Ativo";
        case 1: return "Devolvido";
        case 2: return "Atrasado";
        default: return "Desconhecido";
    }
}

static void imprimirEmprestimo(const Emprestimo* emprestimo) {
    if (emprestimo == NULL) {
        return;
    }
    printf("ID: %d\n", emprestimo->id);
    printf("ID Usuário: %d\n", emprestimo->id_usuario);
    printf("ID Livro: %d\n", emprestimo->id_livro);
    printf("Data empréstimo: %s\n", emprestimo->data_emprestimo);
    printf("Data previsão: %s\n", emprestimo->data_devolucao_prevista);
    printf("Data real: %s\n", emprestimo->data_devolucao_real[0] ? emprestimo->data_devolucao_real : "Ainda não devolvido");
    printf("Status: %s\n", statusEmprestimo(emprestimo->status));
    printf("---------------------------\n");
}

static void cadastrarUsuario(void) {
    Usuario usuario = {0};
    lerString("Nome: ", usuario.nome, sizeof(usuario.nome));
    lerString("CPF: ", usuario.cpf, sizeof(usuario.cpf));
    lerString("Telefone: ", usuario.telefone, sizeof(usuario.telefone));

    if (criarUsuario(&usuario) != 0) {
        printf("Erro ao cadastrar usuário.\n");
    } else {
        printf("Usuário cadastrado com ID %d.\n", usuario.id);
    }
    aguardarEnter();
}

static void listarUsuarios(void) {
    Usuario* usuarios = NULL;
    int quantidade = 0;
    if (listarUsuariosAtivos(&usuarios, &quantidade) != 0) {
        printf("Erro ao listar usuários.\n");
        aguardarEnter();
        return;
    }

    if (quantidade == 0) {
        printf("Nenhum usuário ativo encontrado.\n");
    } else {
        printf("Usuários ativos (%d):\n", quantidade);
        printf("---------------------------\n");
        for (int i = 0; i < quantidade; ++i) {
            imprimirUsuario(&usuarios[i]);
        }
    }
    liberarLista(usuarios);
    aguardarEnter();
}

static void alterarUsuario(void) {
    int id = lerInteiro("ID do usuário: ");
    Usuario usuario;
    if (obterUsuarioPorId(id, &usuario) != 0) {
        printf("Usuário não encontrado ou inativo.\n");
        aguardarEnter();
        return;
    }

    printf("Atualize os dados (deixe em branco para manter)\n");
    char buffer[USUARIO_NOME_TAM];

    lerString("Novo nome: ", buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(usuario.nome, buffer, sizeof(usuario.nome) - 1);
        usuario.nome[sizeof(usuario.nome) - 1] = '\0';
    }

    lerString("Novo CPF: ", buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(usuario.cpf, buffer, sizeof(usuario.cpf) - 1);
        usuario.cpf[sizeof(usuario.cpf) - 1] = '\0';
    }

    lerString("Novo telefone: ", buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(usuario.telefone, buffer, sizeof(usuario.telefone) - 1);
        usuario.telefone[sizeof(usuario.telefone) - 1] = '\0';
    }

    if (atualizarUsuario(&usuario) != 0) {
        printf("Erro ao atualizar usuário.\n");
    } else {
        printf("Usuário atualizado com sucesso.\n");
    }
    aguardarEnter();
}

static void excluirUsuarioMenu(void) {
    int id = lerInteiro("ID do usuário: ");
    if (excluirUsuario(id) != 0) {
        printf("Erro ao excluir usuário ou usuário inexistente.\n");
    } else {
        printf("Usuário excluído logicamente com sucesso.\n");
    }
    aguardarEnter();
}

static void gerenciarUsuarios(void) {
    int opcao;
    do {
        printf("\n=== Gerenciar Usuários ===\n");
        printf("1. Cadastrar usuário\n");
        printf("2. Listar usuários ativos\n");
        printf("3. Alterar usuário\n");
        printf("4. Excluir usuário\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
            case 1: cadastrarUsuario(); break;
            case 2: listarUsuarios(); break;
            case 3: alterarUsuario(); break;
            case 4: excluirUsuarioMenu(); break;
            case 0: break;
            default: printf("Opção inválida.\n"); aguardarEnter(); break;
        }
    } while (opcao != 0);
}

static void cadastrarLivro(void) {
    Livro livro = {0};
    lerString("Título: ", livro.titulo, sizeof(livro.titulo));
    lerString("Autor: ", livro.autor, sizeof(livro.autor));
    livro.quantidade_disponivel = lerInteiro("Quantidade disponível: ");

    if (criarLivro(&livro) != 0) {
        printf("Erro ao cadastrar livro.\n");
    } else {
        printf("Livro cadastrado com ID %d.\n", livro.id);
    }
    aguardarEnter();
}

static void listarLivros(void) {
    Livro* livros = NULL;
    int quantidade = 0;
    if (listarTodosLivros(&livros, &quantidade) != 0) {
        printf("Erro ao listar livros.\n");
        aguardarEnter();
        return;
    }

    if (quantidade == 0) {
        printf("Nenhum livro ativo encontrado.\n");
    } else {
        printf("Livros ativos (%d):\n", quantidade);
        printf("---------------------------\n");
        for (int i = 0; i < quantidade; ++i) {
            imprimirLivro(&livros[i]);
        }
    }
    liberarLista(livros);
    aguardarEnter();
}

static void alterarLivro(void) {
    int id = lerInteiro("ID do livro: ");
    Livro livro;
    if (obterLivroPorId(id, &livro) != 0) {
        printf("Livro não encontrado ou inativo.\n");
        aguardarEnter();
        return;
    }

    printf("Atualize os dados (deixe em branco para manter)\n");
    char buffer[LIVRO_TITULO_TAM];

    lerString("Novo título: ", buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(livro.titulo, buffer, sizeof(livro.titulo) - 1);
        livro.titulo[sizeof(livro.titulo) - 1] = '\0';
    }

    lerString("Novo autor: ", buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(livro.autor, buffer, sizeof(livro.autor) - 1);
        livro.autor[sizeof(livro.autor) - 1] = '\0';
    }

    char quantidadeBuffer[16];
    lerString("Nova quantidade disponível: ", quantidadeBuffer, sizeof(quantidadeBuffer));
    if (quantidadeBuffer[0] != '\0') {
        livro.quantidade_disponivel = (int)strtol(quantidadeBuffer, NULL, 10);
    }

    if (atualizarLivro(&livro) != 0) {
        printf("Erro ao atualizar livro.\n");
    } else {
        printf("Livro atualizado com sucesso.\n");
    }
    aguardarEnter();
}

static void excluirLivroMenu(void) {
    int id = lerInteiro("ID do livro: ");
    if (excluirLivro(id) != 0) {
        printf("Erro ao excluir livro ou livro inexistente.\n");
    } else {
        printf("Livro excluído logicamente com sucesso.\n");
    }
    aguardarEnter();
}

static void gerenciarLivros(void) {
    int opcao;
    do {
        printf("\n=== Gerenciar Livros ===\n");
        printf("1. Cadastrar livro\n");
        printf("2. Listar livros\n");
        printf("3. Alterar livro\n");
        printf("4. Excluir livro\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
            case 1: cadastrarLivro(); break;
            case 2: listarLivros(); break;
            case 3: alterarLivro(); break;
            case 4: excluirLivroMenu(); break;
            case 0: break;
            default: printf("Opção inválida.\n"); aguardarEnter(); break;
        }
    } while (opcao != 0);
}

static void registrarEmprestimo(void) {
    int idUsuario = lerInteiro("ID do usuário: ");
    int idLivro = lerInteiro("ID do livro: ");
    char dataEmprestimo[DATA_TAM];
    char dataPrevista[DATA_TAM];

    lerString("Data de empréstimo (DD/MM/AAAA): ", dataEmprestimo, sizeof(dataEmprestimo));
    lerString("Data de devolução prevista (DD/MM/AAAA): ", dataPrevista, sizeof(dataPrevista));

    int idEmprestimo = 0;
    if (registrarNovoEmprestimo(idUsuario, idLivro, dataEmprestimo, dataPrevista, &idEmprestimo) != 0) {
        printf("Erro ao registrar novo empréstimo. Verifique os dados e disponibilidade.\n");
    } else {
        printf("Empréstimo registrado com ID %d.\n", idEmprestimo);
    }
    aguardarEnter();
}

static void registrarDevolucaoMenu(void) {
    int idEmprestimo = lerInteiro("ID do empréstimo: ");
    char dataReal[DATA_TAM];
    lerString("Data de devolução (DD/MM/AAAA): ", dataReal, sizeof(dataReal));

    if (registrarDevolucao(idEmprestimo, dataReal) != 0) {
        printf("Erro ao registrar devolução. Verifique o empréstimo e a data.\n");
    } else {
        printf("Devolução registrada com sucesso.\n");
    }
    aguardarEnter();
}

static void listarEmprestimosAtivosMenu(void) {
    Emprestimo* emprestimos = NULL;
    int quantidade = 0;
    if (listarEmprestimosAtivos(&emprestimos, &quantidade) != 0) {
        printf("Erro ao listar empréstimos ativos.\n");
        aguardarEnter();
        return;
    }

    if (quantidade == 0) {
        printf("Nenhum empréstimo ativo encontrado.\n");
    } else {
        printf("Empréstimos ativos (%d):\n", quantidade);
        printf("---------------------------\n");
        for (int i = 0; i < quantidade; ++i) {
            imprimirEmprestimo(&emprestimos[i]);
        }
    }
    liberarLista(emprestimos);
    aguardarEnter();
}

static void gerenciarEmprestimos(void) {
    int opcao;
    do {
        printf("\n=== Gerenciar Empréstimos ===\n");
        printf("1. Registrar novo empréstimo\n");
        printf("2. Registrar devolução\n");
        printf("3. Listar empréstimos ativos\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
            case 1: registrarEmprestimo(); break;
            case 2: registrarDevolucaoMenu(); break;
            case 3: listarEmprestimosAtivosMenu(); break;
            case 0: break;
            default: printf("Opção inválida.\n"); aguardarEnter(); break;
        }
    } while (opcao != 0);
}

static void relatorios(void) {
    int opcao;
    do {
        printf("\n=== Relatórios ===\n");
        printf("1. Listar todos os usuários ativos\n");
        printf("2. Listar todos os livros com quantidade disponível\n");
        printf("3. Listar empréstimos em atraso\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
            case 1: listarUsuarios(); break;
            case 2: {
                Livro* livros = NULL;
                int quantidade = 0;
                if (listarLivrosDisponiveis(&livros, &quantidade) != 0) {
                    printf("Erro ao listar livros disponíveis.\n");
                    aguardarEnter();
                } else if (quantidade == 0) {
                    printf("Nenhum livro disponível encontrado.\n");
                    aguardarEnter();
                } else {
                    printf("Livros disponíveis (%d):\n", quantidade);
                    printf("---------------------------\n");
                    for (int i = 0; i < quantidade; ++i) {
                        imprimirLivro(&livros[i]);
                    }
                    liberarLista(livros);
                    aguardarEnter();
                }
                break;
            }
            case 3: {
                Emprestimo* emprestimos = NULL;
                int quantidade = 0;
                if (listarEmprestimosAtrasados(&emprestimos, &quantidade) != 0) {
                    printf("Erro ao listar empréstimos em atraso.\n");
                    aguardarEnter();
                } else if (quantidade == 0) {
                    printf("Nenhum empréstimo em atraso encontrado.\n");
                    aguardarEnter();
                } else {
                    printf("Empréstimos em atraso (%d):\n", quantidade);
                    printf("---------------------------\n");
                    for (int i = 0; i < quantidade; ++i) {
                        imprimirEmprestimo(&emprestimos[i]);
                    }
                    liberarLista(emprestimos);
                    aguardarEnter();
                }
                break;
            }
            case 0: break;
            default: printf("Opção inválida.\n"); aguardarEnter(); break;
        }
    } while (opcao != 0);
}

int main(void) {
    int opcao;
    do {
        printf("\n=== SISTEMA DE BIBLIOTECA ===\n");
        printf("1. Gerenciar Usuários\n");
        printf("2. Gerenciar Livros\n");
        printf("3. Gerenciar Empréstimos\n");
        printf("4. Relatórios\n");
        printf("0. Sair\n");
        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
            case 1: gerenciarUsuarios(); break;
            case 2: gerenciarLivros(); break;
            case 3: gerenciarEmprestimos(); break;
            case 4: relatorios(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida.\n"); aguardarEnter(); break;
        }
    } while (opcao != 0);

    return 0;
}
