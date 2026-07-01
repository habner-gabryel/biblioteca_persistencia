/* --------------------------
   Tipos de dados
   -------------------------- */
typedef struct dFile DFile;
typedef DFile*       pDFile;

typedef int   (*FuncaoComparacao) (void *, void *);
typedef void* (*FuncaoAloca)      ();
typedef void  (*FuncaoImpressao)  (void *);
typedef int   (*FuncaoPredicado)  (void *);

// Uma função de predicato estendida recebe os dados e um seletor para avaliar
// o predicado.
// Por exemplo, quando aplicado ao cadastro de pessoa, pode-se filtrar as
// pessoas que fazem aniversário em um determinado mês, sendo que o
// primeiro parâmetro são os dados da pessoa e o segundo parâmetro é o mês.
typedef int   (*FuncaoPredicadoExt)  (void*, void*);

// Toda função de atualização precisa definir dois parâmetros:
//   1) o primeiro deles é os dados a serem atualizados (em geral a struct), e
//   2) o segundo parâmetro é o valor que será utilizado para atualizar
// A função deve retornar os dados (da struct) atualizados.
typedef void* (*FuncaoAtualizacao)(void *, void *);

/* --------------------------
   Operações CRUD
               Create
               Retrieve
               Update
               Delete
   -------------------------- */
pDFile  abrir     (char[30], int);
void    createe   (pDFile, void*);                          // dados
void*   retrieve  (pDFile, void*, FuncaoComparacao);        // chave
void    update    (pDFile, void*, void*, FuncaoComparacao); // chave e os dados
void    deletee   (pDFile, void*, FuncaoComparacao);        // chave

void    fechar     (pDFile);

pDLista queryAll  (pDFile);
pDLista queryBy   (pDFile, FuncaoPredicado);

void    persistAll (pDFile, pDLista);




