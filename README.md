# TRABALHO PRÁTICO - IMPLEMENTAÇÃO DE BIBLIOTECA DE PERSISTÊNCIA EM LINGUAGEM C E SISTEMA COM 3 ENTIDADES RELACIONADAS

Este trabalho prático engloba dois tópicos distintos:

1) a implementação de uma biblioteca de persistência de dados em arquivo binário em linguagem C, e

2) a implementação de um sistema com 3 entidades relacionadas, como por exemplo, Usuário, Livro e Empréstimo. O sistema deve usar a biblioteca de persistência para gravar e recuperar as informações gravadas em disco.

## 1) Biblioteca de Persistência

Este tópico do trabalho refere-se à implementação da biblioteca de persistência de dados, cujo objetivo é centralizar as operações mais comuns de gravação de dados em arquivos, tais como o CRUD (Create, Retrieve, Update, Delete).

A biblioteca já foi implementada quase na totalidade durante as aulas, basta implementar as operações que faltam.

Cada arquivo será representado por um descritor com a seguinte estrutura:

```c
struct dFile {
    FILE* arquivo;          // referência ao arquivo em disco
    int tamanhoRegistro;    // quantidade de bytes do tipo de dado (struct) a ser gravado/lido
};
```

Os tipos de dados e as operações da biblioteca de persistência estão descritos no quadro a seguir.

## 2) Sistema com 3 entidades relacionadas

O segundo tópico deste trabalho trata da implementação de um sistema contendo, no mínimo, 3 entidades relacionadas, conforme o exemplo de diagrama ilustrado na figura a seguir.

![Diagrama de exemplo com 3 entidades relacionadas](imagem_diagrama.png)

No caso do registro de novo empréstimo, é necessário fazer algumas consistências, como por exemplo:

- Validar se o ID do usuário informado está cadastrado, assim como o ID do livro.
- Validar se tem exemplar do livro disponível para empréstimo (verificando o atributo Quantidade_Disponível do arquivo de livros).
- Validar a data de devolução prevista para que não seja inferior a data do empréstimo.

### Operações da Biblioteca de Persistência (CRUD)

A tabela a seguir descreve as funções que devem estar presentes na biblioteca. Como mencionado, a maior parte já foi implementada em aula; foque nas operações que ainda não foram codificadas (tipicamente `update` e `delete` por posição, ou funções de busca).

| Função | Descrição |
| :--- | :--- |
| `dFile* dOpen(const char* nomeArquivo, int tamanhoRegistro)` | Abre (ou cria) o arquivo binário e retorna um descritor `dFile`. Deve armazenar o `tamanhoRegistro` para operações posteriores. |
| `void dClose(dFile* df)` | Fecha o arquivo e libera a memória do descritor. |
| `int dCreate(dFile* df, void* dado)` | Insere um novo registro no final do arquivo. Retorna 0 em caso de sucesso, ou -1 em caso de erro. |
| `int dRetrieve(dFile* df, int id, void* dado)` | Lê um registro de um determinado `id` (posição ou chave primária) e copia os dados para o ponteiro `dado`. Retorna 0 em sucesso, -1 se não encontrar. |
| `int dUpdate(dFile* df, int id, void* dado)` | Sobrescreve o registro na posição `id` com os novos dados fornecidos. Retorna 0 em sucesso, -1 em erro. |
| `int dDelete(dFile* df, int id)` | Remove logicamente (ou fisicamente) o registro de índice `id`. Uma abordagem comum é marcar o registro como inativo (ex: usando um campo `ativo` na struct). Retorna 0 em sucesso, -1 em erro. |
| `int dGetTotalRecords(dFile* df)` | Retorna o número total de registros (válidos ou não) presentes no arquivo. |
| `int dGetNextId(dFile* df)` | Retorna o próximo ID disponível (geralmente `totalRecords + 1` ou varredura de IDs livres) para ser usado na criação de um novo registro. |

> **Observação:** Como o arquivo é binário e de tamanho fixo (registros com tamanho definido), a localização de um registro pelo `id` (considerando `id` começando em 0 ou 1) é calculada por: `posição = (id - 1) * tamanhoRegistro`. Utilize `fseek()` e `fread()`/`fwrite()` para navegar e manipular os dados.

---

## Proposta de Estrutura de Dados para o Sistema (3 Entidades)

Para atender ao segundo tópico, você deve criar três structs inter-relacionadas. Segue um exemplo baseado no diagrama mencionado (Usuário, Livro e Empréstimo).

### 1. Entidade `Usuario`
Representa os usuários do sistema que podem pegar livros emprestados.

```c
typedef struct {
    int id;
    char nome[100];
    char cpf[15];
    char telefone[20];
    int ativo;          // 1 = ativo, 0 = inativo (para exclusão lógica)
} Usuario;
```

### 2. Entidade `Livro`
Representa os livros disponíveis na biblioteca.

```c
typedef struct {
    int id;
    char titulo[200];
    char autor[100];
    int quantidade_disponivel;  // Este campo é crucial para as consistências
    int ativo;
} Livro;
```

### 3. Entidade `Emprestimo`
Relaciona um `Usuario` a um `Livro`, registrando as datas de empréstimo e devolução.

```c
typedef struct {
    int id;
    int id_usuario;
    int id_livro;
    char data_emprestimo[11];      // Formato DD/MM/AAAA
    char data_devolucao_prevista[11]; // Formato DD/MM/AAAA
    char data_devolucao_real[11];   // Pode ser vazio enquanto não devolvido
    int status;                     // 0 = Ativo, 1 = Devolvido, 2 = Atrasado
    int ativo;
} Emprestimo;
```

---

## Regras de Negócio e Consistências (Registro de Empréstimo)

Conforme especificado, ao criar um novo empréstimo, as seguintes validações são obrigatórias:

1. **Validação do Usuário:**  
   - Utilizar a biblioteca de persistência para ler o arquivo de `Usuario` e verificar se o `id_usuario` informado existe e está `ativo = 1`.

2. **Validação do Livro:**  
   - Utilizar a biblioteca para ler o arquivo de `Livro` e verificar se o `id_livro` existe e está ativo.

3. **Disponibilidade de Exemplar:**  
   - Após encontrar o livro, verificar se o campo `quantidade_disponivel` é maior que 0.
   - Caso positivo, **diminuir** este campo em 1 unidade e atualizar o registro do livro no disco usando `dUpdate()`.

4. **Validação da Data de Devolução Prevista:**  
   - Converter as strings de data para um formato comparável (ex: `AAAAMMDD` usando `atoi` ou `strcmp`) e garantir que `data_devolucao_prevista` seja maior ou igual a `data_emprestimo`.

5. **Atualização do Empréstimo:**  
   - Se todas as validações forem bem-sucedidas, insira o novo registro de `Emprestimo` usando `dCreate()`.

---

## Estrutura de Menu Sugerida (Interface no `main()`)

Seu programa principal deve oferecer um menu interativo (via terminal) para testar todas as funcionalidades. Exemplo:

```
=== SISTEMA DE BIBLIOTECA ===
1. Gerenciar Usuários (CRUD)
2. Gerenciar Livros (CRUD)
3. Gerenciar Empréstimos
   3.1. Registrar novo empréstimo
   3.2. Registrar devolução (atualizar status e devolver quantidade)
   3.3. Listar empréstimos ativos
4. Relatórios
   4.1. Listar todos os usuários
   4.2. Listar todos os livros com quantidade disponível
   4.3. Listar empréstimos em atraso
0. Sair
```

---

## Dicas de Implementação

- **Exclusão Lógica:** Para facilitar e evitar a reorganização do arquivo binário, todos os `DELETE` devem apenas marcar o campo `ativo` como 0. O `dDelete` da biblioteca pode ser adaptado para isso, ou você pode chamar `dUpdate` sobrescrevendo o registro com `ativo = 0`.
- **Datas:** Utilize a função `strcmp` para comparar datas no formato `DD/MM/AAAA` (pois a ordem lexicográfica coincide com a cronológica se o ano vier primeiro, mas como estamos usando DD/MM/AAAA, você precisará converter para `AAAAMMDD` ou comparar ano, mês e dia separadamente para garantir a validação correta). Uma alternativa é usar a biblioteca `<time.h>`.
- **Gerenciamento de Memória:** Sempre abra e feche os arquivos adequadamente usando `dOpen` e `dClose` para cada operação, ou mantenha os descritores abertos durante a execução do programa (cuidado com o limite de arquivos abertos pelo sistema).
- **Leitura de Dados do Usuário:** Utilize `fgets()` e `scanf()` com cuidado para evitar estouro de buffer e limpar o buffer do teclado (`fflush(stdin)` ou `__fpurge` em Linux).

---

## Critérios de Avaliação (Esperados)

- A biblioteca de persistência deve funcionar de maneira genérica (usando `void*` e `tamanhoRegistro`), sendo reaproveitável para qualquer uma das três entidades.
- As operações de CRUD para as três entidades devem estar funcionais e salvar os dados em arquivos binários separados (ex: `usuarios.dat`, `livros.dat`, `emprestimos.dat`).
- As regras de consistência para o empréstimo devem ser rigorosamente aplicadas, com mensagens de erro claras para o usuário.
- O código deve ser bem estruturado em arquivos `.h` e `.c` (separando biblioteca, entidades e lógica do sistema).