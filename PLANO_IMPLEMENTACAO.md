# Plano de desenvolvimento - Biblioteca de Persistência e Sistema de Biblioteca

## 1. Avaliação do README e do projeto atual

O README descreve um trabalho bem definido com dois objetivos claros:

1. Implementar uma biblioteca genérica de persistência em arquivo binário.
2. Construir um sistema com três entidades relacionadas: Usuário, Livro e Empréstimo.

O projeto existente já possui uma base inicial em [FrameworkPersistencia/source](FrameworkPersistencia/source), com arquivos de operações organizados em cabeçalhos e uma estrutura inicial para descritor de arquivo. No entanto, ainda há lacunas importantes em relação ao que o README exige:

- A API atual não está alinhada com o nome das funções esperadas pelo README.
- Funções importantes como `delete` e `persistAll` ainda estão incompletas.
- Não existe ainda um sistema de entidades para Usuário, Livro e Empréstimo.
- Não há menu interativo, nem validações de negócio para empréstimos.
- Não há arquivos binários de dados nem fluxo de CRUD completo para as entidades do sistema.

## 2. Objetivo principal

Implementar uma solução em linguagem C que:

- tenha uma biblioteca de persistência reutilizável;
- persista dados de Usuário, Livro e Empréstimo em arquivos binários separados;
- ofereça um menu interativo para CRUD e relatórios;
- aplique as regras de consistência no cadastro de empréstimos.

## 3. Plano de implementação

### Fase 1 - Padronizar a biblioteca de persistência

1. Definir a interface pública da biblioteca com os nomes esperados pelo README:
   - `dOpen`
   - `dClose`
   - `dCreate`
   - `dRetrieve`
   - `dUpdate`
   - `dDelete`
   - `dGetTotalRecords`
   - `dGetNextId`

2. Ajustar a estrutura do descritor de arquivo para manter:
   - `FILE* arquivo`
   - `int tamanhoRegistro`

3. Implementar corretamente as operações básicas:
   - abertura/criação do arquivo;
   - escrita no final do arquivo;
   - leitura de registros;
   - atualização de registros;
   - exclusão lógica (marcar `ativo = 0`);
   - contagem de registros;
   - cálculo do próximo ID.

4. Garantir que o código seja genérico e reutilizável para qualquer struct.

### Fase 2 - Definir as entidades do sistema

1. Criar as estruturas de dados para:
   - `Usuario`
   - `Livro`
   - `Emprestimo`

2. Incluir campos necessários para persistência e consistência:
   - `id`
   - `ativo`
   - `quantidade_disponivel` no livro
   - datas de empréstimo e devolução no empréstimo
   - status do empréstimo

3. Criar arquivos separados para organizar melhor o código, por exemplo:
   - `entidades.h`
   - `entidades.c`
   - `sistema.h`
   - `sistema.c`

### Fase 3 - Implementar o CRUD das entidades

#### Usuários
1. Cadastrar usuário.
2. Buscar usuário por ID.
3. Atualizar usuário.
4. Excluir logicamente usuário.
5. Listar usuários ativos.

#### Livros
1. Cadastrar livro.
2. Buscar livro por ID.
3. Atualizar livro.
4. Excluir logicamente livro.
5. Listar livros disponíveis.

#### Empréstimos
1. Cadastrar empréstimo.
2. Buscar empréstimo por ID.
3. Atualizar status de devolução.
4. Listar empréstimos ativos.
5. Listar empréstimos atrasados.

### Fase 4 - Implementar as regras de negócio

1. Validar se o usuário informado existe e está ativo.
2. Validar se o livro informado existe e está ativo.
3. Validar se há exemplares disponíveis do livro.
4. Validar se a data de devolução prevista é válida.
5. Caso a validação seja bem-sucedida:
   - diminuir `quantidade_disponivel` do livro;
   - atualizar o livro no arquivo;
   - gravar o empréstimo no arquivo.

6. Implementar a devolução de livro:
   - marcar o empréstimo como devolvido;
   - aumentar novamente a quantidade disponível.

### Fase 5 - Construir a interface de menu

1. Criar um menu principal com opções para:
   - gerenciamento de usuários;
   - gerenciamento de livros;
   - gerenciamento de empréstimos;
   - relatórios.

2. Implementar leitura de dados via terminal com tratamento adequado para buffer.

3. Exibir mensagens claras para sucesso e erro.

### Fase 6 - Testes e validação

1. Compilar o projeto e corrigir erros de compilação.
2. Testar os fluxos principais:
   - cadastro de usuário;
   - cadastro de livro;
   - cadastro de empréstimo válido;
   - cadastro de empréstimo com usuário inválido;
   - cadastro de empréstimo com livro sem estoque;
   - devolução de livro.
3. Validar se os arquivos binários estão sendo criados corretamente.
4. Ajustar detalhes de usabilidade e mensagens.

## 4. Entregáveis esperados

- Biblioteca de persistência funcional e reutilizável.
- Arquivos binários separados para usuário, livro e empréstimo.
- Menu interativo no terminal.
- CRUD completo para as três entidades.
- Regras de consistência aplicadas no empréstimo.
- Código organizado em módulos com responsabilidades claras.

## 5. Ordem recomendada de execução

1. Implementar a biblioteca de persistência.
2. Criar as estruturas de entidade.
3. Implementar CRUD de usuários.
4. Implementar CRUD de livros.
5. Implementar CRUD de empréstimos.
6. Adicionar validações de negócio.
7. Adicionar menu e relatórios.
8. Testar e corrigir problemas.
