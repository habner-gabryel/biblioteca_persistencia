#include "sistema.h"
#include "Persistencia.h"
#include "entidades.h"
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

static int atualizarQuantidadeLivro(int id, int delta) {
    Livro livro;
    if (obterLivroPorId(id, &livro) != 0) {
        return -1;
    }

    livro.quantidade_disponivel += delta;
    if (livro.quantidade_disponivel < 0) {
        return -1;
    }

    return atualizarLivro(&livro);
}

static int lerUsuarioAtivo(int id, Usuario* usuario) {
    if (usuario == NULL) {
        return -1;
    }
    return obterUsuarioPorId(id, usuario);
}

static int lerLivroAtivo(int id, Livro* livro) {
    if (livro == NULL) {
        return -1;
    }
    return obterLivroPorId(id, livro);
}

static int validarDatasEmprestimo(const char* dataEmprestimo, const char* dataDevolucaoPrevista) {
    if (!dataValida(dataEmprestimo) || !dataValida(dataDevolucaoPrevista)) {
        return 0;
    }

    return dataMaiorOuIgual(dataDevolucaoPrevista, dataEmprestimo);
}

static int calcularStatusDevolucao(const char* dataPrevista, const char* dataReal) {
    int previsto = dataParaNumero(dataPrevista);
    int real = dataParaNumero(dataReal);
    if (previsto < 0 || real < 0) {
        return -1;
    }

    if (real > previsto) {
        return 2; // atrasado
    }
    return 1; // devolvido em dia ou no prazo
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

int registrarNovoEmprestimo(int id_usuario, int id_livro, const char* dataEmprestimo, const char* dataDevolucaoPrevista, int* idEmprestimo) {
    if (id_usuario < 1 || id_livro < 1 || dataEmprestimo == NULL || dataDevolucaoPrevista == NULL || idEmprestimo == NULL) {
        return -1;
    }

    if (!validarDatasEmprestimo(dataEmprestimo, dataDevolucaoPrevista)) {
        return -1;
    }

    Usuario usuario;
    if (lerUsuarioAtivo(id_usuario, &usuario) != 0) {
        return -1;
    }

    Livro livro;
    if (lerLivroAtivo(id_livro, &livro) != 0) {
        return -1;
    }

    if (livro.quantidade_disponivel <= 0) {
        return -1;
    }

    Livro livroAtualizado = livro;
    livroAtualizado.quantidade_disponivel -= 1;
    if (atualizarLivro(&livroAtualizado) != 0) {
        return -1;
    }

    Emprestimo emprestimo = {0};
    emprestimo.id_usuario = usuario.id;
    emprestimo.id_livro = livro.id;
    strncpy(emprestimo.data_emprestimo, dataEmprestimo, DATA_TAM - 1);
    emprestimo.data_emprestimo[DATA_TAM - 1] = '\0';
    strncpy(emprestimo.data_devolucao_prevista, dataDevolucaoPrevista, DATA_TAM - 1);
    emprestimo.data_devolucao_prevista[DATA_TAM - 1] = '\0';
    emprestimo.data_devolucao_real[0] = '\0';
    emprestimo.status = 0;
    emprestimo.ativo = 1;

    int result = criarEmprestimo(&emprestimo);
    if (result != 0) {
        Livro livroRestaurado = livro;
        livroRestaurado.quantidade_disponivel += 1;
        atualizarLivro(&livroRestaurado);
        return -1;
    }

    *idEmprestimo = emprestimo.id;
    return 0;
}

int registrarDevolucao(int idEmprestimo, const char* dataDevolucaoReal) {
    if (idEmprestimo < 1 || dataDevolucaoReal == NULL) {
        return -1;
    }

    if (!dataValida(dataDevolucaoReal)) {
        return -1;
    }

    Emprestimo emprestimo;
    if (obterEmprestimoPorId(idEmprestimo, &emprestimo) != 0) {
        return -1;
    }

    if (emprestimo.status != 0) {
        return -1;
    }

    int novoStatus = calcularStatusDevolucao(emprestimo.data_devolucao_prevista, dataDevolucaoReal);
    if (novoStatus < 0) {
        return -1;
    }

    strncpy(emprestimo.data_devolucao_real, dataDevolucaoReal, DATA_TAM - 1);
    emprestimo.data_devolucao_real[DATA_TAM - 1] = '\0';
    emprestimo.status = novoStatus;

    if (atualizarEmprestimo(&emprestimo) != 0) {
        return -1;
    }

    if (atualizarQuantidadeLivro(emprestimo.id_livro, 1) != 0) {
        return -1;
    }

    return 0;
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
