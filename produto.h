#ifndef produto_h
#define produto_h
typedef struct {
    char nomep[50];
    int idp;
    float preco;
    int qtd;
    int promocao; 
    float precoPromocional; 
} Produto;

typedef struct {
    Produto produto;
    int quantidade;
} ItemCarrinho;

typedef struct {
    ItemCarrinho itens[100];
    int numItens;
} Carrinho;
extern Produto produtos[10]; 
void editar_produto();
void excluir_produto();
void cadastrar_produto();
void adicionar_ao_carrinho(Carrinho* carrinho);
void listar_produtos();
void ver_carrinho(Carrinho* carrinho);
void cadastrar_promocao();
void mostrar_promocoes();
void excluir_promocao();
void intercala(int e, int m, int d, Produto produtos[]);
void mergeSort(int i, int f, Produto produtos[]);
void mostrarProdutos();
void buscar();
#endif