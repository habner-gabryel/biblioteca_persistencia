#include "sistema.h"
#include "Persistencia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int lerRegistro(const char* arquivo, int tamanhoRegistro, int id, void* registro) {
    dFile* df = dOpen(arquivo, tamanhoRegistro);
    if (df == NULL) {
        return -1;
    }

    int result = dRetrieve(df, id, registro);
    dClose(df);
    return result;
}

/* Usuários */
int criarUsuario(Usuario* usuario) {
    if (usuario == NULL) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_USUARIOS, sizeof(Usuario));
    if (df == NULL) {
        return -1;
    }

    usuario->id = dGetNextId(df);
    usuario->ativo = 1;
    int result = dCreate(df, usuario);
    dClose(df);
    return result;
}

int obterUsuarioPorId(int id, Usuario* usuario) {
    if (usuario == NULL || id < 1) {
        return -1;
    }

    Usuario temp;
    int result = lerRegistro(ARQUIVO_USUARIOS, sizeof(Usuario), id, &temp);
    if (result != 0 || temp.ativo != 1) {
        return -1;
    }

    *usuario = temp;
    return 0;
}

int atualizarUsuario(const Usuario* usuario) {
    if (usuario == NULL || usuario->id < 1) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_USUARIOS, sizeof(Usuario));
    if (df == NULL) {
        return -1;
    }

    int result = dUpdate(df, usuario->id, (void*)usuario);
    dClose(df);
    return result;
}

int excluirUsuario(int id) {
    if (id < 1) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_USUARIOS, sizeof(Usuario));
    if (df == NULL) {
        return -1;
    }

    Usuario usuario;
    int result = dRetrieve(df, id, &usuario);
    if (result != 0 || usuario.ativo != 1) {
        dClose(df);
        return -1;
    }

    usuario.ativo = 0;
    result = dUpdate(df, id, &usuario);
    dClose(df);
    return result;
}

int listarUsuariosAtivos(Usuario** usuarios, int* quantidade) {
    if (usuarios == NULL || quantidade == NULL) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_USUARIOS, sizeof(Usuario));
    if (df == NULL) {
        return -1;
    }

    int total = dGetTotalRecords(df);
    if (total < 0) {
        dClose(df);
        return -1;
    }

    Usuario* lista = (Usuario*)malloc(sizeof(Usuario) * total);
    if (lista == NULL) {
        dClose(df);
        return -1;
    }

    int count = 0;
    for (int id = 1; id <= total; ++id) {
        Usuario usuario;
        if (dRetrieve(df, id, &usuario) != 0) {
            continue;
        }

        if (usuario.ativo == 1) {
            lista[count++] = usuario;
        }
    }

    dClose(df);
    if (count == 0) {
        free(lista);
        *usuarios = NULL;
        *quantidade = 0;
        return 0;
    }

    *usuarios = (Usuario*)realloc(lista, sizeof(Usuario) * count);
    *quantidade = count;
    return 0;
}

/* Livros */
int criarLivro(Livro* livro) {
    if (livro == NULL) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_LIVROS, sizeof(Livro));
    if (df == NULL) {
        return -1;
    }

    livro->id = dGetNextId(df);
    livro->ativo = 1;
    int result = dCreate(df, livro);
    dClose(df);
    return result;
}

int obterLivroPorId(int id, Livro* livro) {
    if (livro == NULL || id < 1) {
        return -1;
    }

    Livro temp;
    int result = lerRegistro(ARQUIVO_LIVROS, sizeof(Livro), id, &temp);
    if (result != 0 || temp.ativo != 1) {
        return -1;
    }

    *livro = temp;
    return 0;
}

int atualizarLivro(const Livro* livro) {
    if (livro == NULL || livro->id < 1) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_LIVROS, sizeof(Livro));
    if (df == NULL) {
        return -1;
    }

    int result = dUpdate(df, livro->id, (void*)livro);
    dClose(df);
    return result;
}

int excluirLivro(int id) {
    if (id < 1) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_LIVROS, sizeof(Livro));
    if (df == NULL) {
        return -1;
    }

    Livro livro;
    int result = dRetrieve(df, id, &livro);
    if (result != 0 || livro.ativo != 1) {
        dClose(df);
        return -1;
    }

    livro.ativo = 0;
    result = dUpdate(df, id, &livro);
    dClose(df);
    return result;
}

int listarLivrosDisponiveis(Livro** livros, int* quantidade) {
    if (livros == NULL || quantidade == NULL) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_LIVROS, sizeof(Livro));
    if (df == NULL) {
        return -1;
    }

    int total = dGetTotalRecords(df);
    if (total < 0) {
        dClose(df);
        return -1;
    }

    Livro* lista = (Livro*)malloc(sizeof(Livro) * total);
    if (lista == NULL) {
        dClose(df);
        return -1;
    }

    int count = 0;
    for (int id = 1; id <= total; ++id) {
        Livro livro;
        if (dRetrieve(df, id, &livro) != 0) {
            continue;
        }

        if (livro.ativo == 1 && livro.quantidade_disponivel > 0) {
            lista[count++] = livro;
        }
    }

    dClose(df);
    if (count == 0) {
        free(lista);
        *livros = NULL;
        *quantidade = 0;
        return 0;
    }

    *livros = (Livro*)realloc(lista, sizeof(Livro) * count);
    *quantidade = count;
    return 0;
}

int listarTodosLivros(Livro** livros, int* quantidade) {
    if (livros == NULL || quantidade == NULL) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_LIVROS, sizeof(Livro));
    if (df == NULL) {
        return -1;
    }

    int total = dGetTotalRecords(df);
    if (total < 0) {
        dClose(df);
        return -1;
    }

    Livro* lista = (Livro*)malloc(sizeof(Livro) * total);
    if (lista == NULL) {
        dClose(df);
        return -1;
    }

    int count = 0;
    for (int id = 1; id <= total; ++id) {
        Livro livro;
        if (dRetrieve(df, id, &livro) != 0) {
            continue;
        }

        if (livro.ativo == 1) {
            lista[count++] = livro;
        }
    }

    dClose(df);
    if (count == 0) {
        free(lista);
        *livros = NULL;
        *quantidade = 0;
        return 0;
    }

    *livros = (Livro*)realloc(lista, sizeof(Livro) * count);
    *quantidade = count;
    return 0;
}

/* Empréstimos */
int criarEmprestimo(Emprestimo* emprestimo) {
    if (emprestimo == NULL) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_EMPRESTIMOS, sizeof(Emprestimo));
    if (df == NULL) {
        return -1;
    }

    emprestimo->id = dGetNextId(df);
    emprestimo->ativo = 1;
    int result = dCreate(df, emprestimo);
    dClose(df);
    return result;
}

int obterEmprestimoPorId(int id, Emprestimo* emprestimo) {
    if (emprestimo == NULL || id < 1) {
        return -1;
    }

    Emprestimo temp;
    int result = lerRegistro(ARQUIVO_EMPRESTIMOS, sizeof(Emprestimo), id, &temp);
    if (result != 0 || temp.ativo != 1) {
        return -1;
    }

    *emprestimo = temp;
    return 0;
}

int atualizarEmprestimo(const Emprestimo* emprestimo) {
    if (emprestimo == NULL || emprestimo->id < 1) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_EMPRESTIMOS, sizeof(Emprestimo));
    if (df == NULL) {
        return -1;
    }

    int result = dUpdate(df, emprestimo->id, (void*)emprestimo);
    dClose(df);
    return result;
}

int excluirEmprestimo(int id) {
    if (id < 1) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_EMPRESTIMOS, sizeof(Emprestimo));
    if (df == NULL) {
        return -1;
    }

    Emprestimo emprestimo;
    int result = dRetrieve(df, id, &emprestimo);
    if (result != 0 || emprestimo.ativo != 1) {
        dClose(df);
        return -1;
    }

    emprestimo.ativo = 0;
    result = dUpdate(df, id, &emprestimo);
    dClose(df);
    return result;
}

int listarEmprestimosAtivos(Emprestimo** emprestimos, int* quantidade) {
    if (emprestimos == NULL || quantidade == NULL) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_EMPRESTIMOS, sizeof(Emprestimo));
    if (df == NULL) {
        return -1;
    }

    int total = dGetTotalRecords(df);
    if (total < 0) {
        dClose(df);
        return -1;
    }

    Emprestimo* lista = (Emprestimo*)malloc(sizeof(Emprestimo) * total);
    if (lista == NULL) {
        dClose(df);
        return -1;
    }

    int count = 0;
    for (int id = 1; id <= total; ++id) {
        Emprestimo emprestimo;
        if (dRetrieve(df, id, &emprestimo) != 0) {
            continue;
        }

        if (emprestimo.ativo == 1 && emprestimo.status == 0) {
            lista[count++] = emprestimo;
        }
    }

    dClose(df);
    if (count == 0) {
        free(lista);
        *emprestimos = NULL;
        *quantidade = 0;
        return 0;
    }

    *emprestimos = (Emprestimo*)realloc(lista, sizeof(Emprestimo) * count);
    *quantidade = count;
    return 0;
}

int listarEmprestimosAtrasados(Emprestimo** emprestimos, int* quantidade) {
    if (emprestimos == NULL || quantidade == NULL) {
        return -1;
    }

    dFile* df = dOpen(ARQUIVO_EMPRESTIMOS, sizeof(Emprestimo));
    if (df == NULL) {
        return -1;
    }

    int total = dGetTotalRecords(df);
    if (total < 0) {
        dClose(df);
        return -1;
    }

    Emprestimo* lista = (Emprestimo*)malloc(sizeof(Emprestimo) * total);
    if (lista == NULL) {
        dClose(df);
        return -1;
    }

    int count = 0;
    for (int id = 1; id <= total; ++id) {
        Emprestimo emprestimo;
        if (dRetrieve(df, id, &emprestimo) != 0) {
            continue;
        }

        if (emprestimo.ativo == 1 && emprestimo.status == 2) {
            lista[count++] = emprestimo;
        }
    }

    dClose(df);
    if (count == 0) {
        free(lista);
        *emprestimos = NULL;
        *quantidade = 0;
        return 0;
    }

    *emprestimos = (Emprestimo*)realloc(lista, sizeof(Emprestimo) * count);
    *quantidade = count;
    return 0;
}

void liberarLista(void* lista) {
    free(lista);
}
