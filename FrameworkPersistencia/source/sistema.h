#ifndef SISTEMA_H
#define SISTEMA_H

#include "entidades.h"

#define ARQUIVO_USUARIOS "usuarios.dat"
#define ARQUIVO_LIVROS "livros.dat"
#define ARQUIVO_EMPRESTIMOS "emprestimos.dat"

/* Usuários */
int criarUsuario(Usuario* usuario);
int obterUsuarioPorId(int id, Usuario* usuario);
int atualizarUsuario(const Usuario* usuario);
int excluirUsuario(int id);
int listarUsuariosAtivos(Usuario** usuarios, int* quantidade);

/* Livros */
int criarLivro(Livro* livro);
int obterLivroPorId(int id, Livro* livro);
int atualizarLivro(const Livro* livro);
int excluirLivro(int id);
int listarLivrosDisponiveis(Livro** livros, int* quantidade);
int listarTodosLivros(Livro** livros, int* quantidade);

/* Empréstimos */
int criarEmprestimo(Emprestimo* emprestimo);
int obterEmprestimoPorId(int id, Emprestimo* emprestimo);
int atualizarEmprestimo(const Emprestimo* emprestimo);
int excluirEmprestimo(int id);
int listarEmprestimosAtivos(Emprestimo** emprestimos, int* quantidade);
int listarEmprestimosAtrasados(Emprestimo** emprestimos, int* quantidade);

void liberarLista(void* lista);

#endif // SISTEMA_H
