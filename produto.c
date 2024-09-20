#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usuario.h"
#include "produto.h"
void editar_produto() {

    FILE *file = fopen("produtos.b", "r+b");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    Produto produto;
    int id;
    float novo_preco;

    printf("Digite o ID do produto que deseja editar: ");
    scanf("%d", &id);


    int encontrado = 0;
    while (fread(&produto, sizeof(Produto), 1, file)) {
        if (produto.idp == id) {
            printf("Produto encontrado: %s, Preço atual: R$%.2f\n", produto.nomep, produto.preco);
            printf("Digite o novo preço: ");
            scanf("%f", &novo_preco);


            produto.preco = novo_preco;


            fseek(file, -sizeof(Produto), SEEK_CUR);
            fwrite(&produto, sizeof(Produto), 1, file);
            fflush(file);

            printf("Produto atualizado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    fclose(file);

    if (!encontrado) {
        printf("Produto não encontrado.\n");
    }
}
void excluir_produto() {
    FILE *file = fopen("produtos.b", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    FILE *tempFile = fopen("temp.b", "wb");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
        fclose(file);
        return;
    }

    Produto produto;
    int id;
    int encontrado = 0;

    printf("Digite o ID do produto que deseja excluir: ");
    scanf("%d", &id);

    while (fread(&produto, sizeof(Produto), 1, file)) {
        if (produto.idp == id) {
            printf("Produto encontrado e excluído: %s, ID: %d\n", produto.nomep, produto.idp);
            encontrado = 1;
            continue; // Pula a gravação deste produto no arquivo temporário
        }
        fwrite(&produto, sizeof(Produto), 1, tempFile); // Copia os outros produtos para o arquivo temporário
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado) {
        remove("produtos.b");          // Remove o arquivo antigo
        rename("temp.b", "produtos.b"); // Renomeia o arquivo temporário para o nome original
        printf("Produto excluído com sucesso!\n");
    } else {
        remove("temp.b"); // Remove o arquivo temporário se o produto não foi encontrado
        printf("Produto não encontrado.\n");
    }
}
void cadastrar_produto() {
    FILE *file = fopen("produtos.b", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    Produto novo_produto;
    printf("Digite o nome do produto: ");
    scanf("%s", novo_produto.nomep);
    fflush(stdin);
    printf("Digite o ID do produto: ");
    scanf("%d", &novo_produto.idp);

    printf("Digite o preço do produto: ");
    scanf("%f", &novo_produto.preco);

    printf("Digite a quantidade do produto: ");
    scanf("%d", &novo_produto.qtd);


    fwrite(&novo_produto, sizeof(Produto), 1, file);
    fclose(file);

    printf("Produto cadastrado com sucesso!\n");
}
void adicionar_ao_carrinho(Carrinho* carrinho) {
    FILE *file = fopen("produtos.b", "r+b");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    Produto produto;
    int id, quantidade;
    int found = 0;

    printf("Digite o ID do produto que deseja adicionar ao carrinho: ");
    scanf("%d", &id);


    while (fread(&produto, sizeof(Produto), 1, file)) {
        if (produto.idp == id) {
            printf("Produto encontrado: %s, Preço: R$%.2f, Quantidade disponível: %d\n", 
                   produto.nomep, produto.promocao ? produto.precoPromocional : produto.preco, produto.qtd);

            printf("Digite a quantidade que deseja adicionar ao carrinho: ");
            scanf("%d", &quantidade);

            if (quantidade > produto.qtd) {
                printf("Quantidade indisponível.\n");
                fclose(file);
                return;
            }


            produto.qtd -= quantidade;


            fseek(file, -sizeof(Produto), SEEK_CUR);
            fwrite(&produto, sizeof(Produto), 1, file);
            fflush(file);


            carrinho->itens[carrinho->numItens].produto = produto;
            carrinho->itens[carrinho->numItens].quantidade = quantidade;
            carrinho->numItens++;

            printf("Produto adicionado ao carrinho com sucesso!\n");
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Produto não encontrado.\n");
    }
}
void listar_produtos() {
    FILE *file = fopen("produtos.b", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    Produto produto;
    printf("Produtos disponíveis na loja:\n");
    while (fread(&produto, sizeof(Produto), 1, file)) {
        if (!produto.promocao) { 
            printf("ID: %d\n", produto.idp);
            printf("Nome: %s\n", produto.nomep);
            printf("Preço: R$%.2f\n", produto.preco);
            printf("Quantidade disponível: %d\n", produto.qtd);
            printf("-----------------------------------\n");
        }
    }

    fclose(file);
}

void ver_carrinho(Carrinho* carrinho) {
    if (carrinho->numItens == 0) {
        printf("O carrinho está vazio.\n");
        return;
    }

    printf("Itens no carrinho:\n");
    float total = 0.0;
    for (int i = 0; i < carrinho->numItens; i++) {
        Produto produto = carrinho->itens[i].produto;
        int quantidade = carrinho->itens[i].quantidade;


        float preco = produto.promocao ? produto.precoPromocional : produto.preco;

        printf("Produto: %s\n", produto.nomep);
        printf("Quantidade: %d\n", quantidade);
        printf("Preço unitário: R$%.2f\n", preco);
        printf("Subtotal: R$%.2f\n", preco * quantidade);
        printf("-----------------------------------\n");

        total += preco * quantidade;
    }
    printf("Total do carrinho: R$%.2f\n", total);
}
void cadastrar_promocao() {
    FILE *file = fopen("produtos.b", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    Produto produto;
    int id;
    float desconto;

    printf("Digite o ID do produto que deseja colocar em promoção: ");
    scanf("%d", &id);
    printf("Digite a porcentagem de desconto (ex: 10 para 10%%): ");
    scanf("%f", &desconto);

    while (fread(&produto, sizeof(Produto), 1, file)) {
        if (produto.idp == id) {
            produto.promocao = 1;
            produto.precoPromocional = produto.preco * (1 - desconto / 100);


            fseek(file, -sizeof(Produto), SEEK_CUR);
            fwrite(&produto, sizeof(Produto), 1, file);

            printf("produto %s, novo preço: R$%.2f\n", produto.nomep, produto.precoPromocional);
            fclose(file);
            return;
        }
    }

    fclose(file);
}
void mostrar_promocoes() {
    FILE *file = fopen("produtos.b", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }


    Produto produto;
    printf("Promoções do dia:\n");
    while (fread(&produto, sizeof(Produto), 1, file)) {
        if (produto.promocao) {  
            printf("ID: %d\n", produto.idp);
            printf("Nome: %s\n", produto.nomep);
            printf("Preço Promocional: R$%.2f\n", produto.precoPromocional);
            printf("Quantidade disponível: %d\n", produto.qtd);
            printf("-----------------------------------\n");
        }
    }

    fclose(file);
}
void excluir_promocao() {
    FILE *file = fopen("produtos.b", "r+b");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    Produto produto;
    int id;

    printf("Digite o ID do produto cuja promoção deseja excluir: ");
    scanf("%d", &id);
    int encontrado = 0;
    while (fread(&produto, sizeof(Produto), 1, file)) {
        if (produto.idp == id) {
            if (produto.promocao) {
                printf("Produto com promoção encontrada: %s, Preço promocional: R$%.2f\n", produto.nomep, produto.precoPromocional);


                produto.promocao = 0; 
                produto.precoPromocional = 0.0; 


                fseek(file, -sizeof(Produto), SEEK_CUR);
                fwrite(&produto, sizeof(Produto), 1, file);
                fflush(file);

                printf("Promoção excluída com sucesso!\n");
                encontrado = 1;
                break;
            } else {
                printf("O produto não está em promoção.\n");
                encontrado = 1;
                break;
            }
        }
    }
    fclose(file);

    if (!encontrado) {
        printf("Produto não encontrado.\n");
    }
}

void intercala1(int e, int m, int d, Produto v[])
{
  int n1 = (m - e) + 1;
  int n2 = (d - m);
  Produto E[n1];
  Produto D[n2];
  int i, j;

  for (i = 0; i < n1; i++)
  {
    E[i] = v[e + i];
  }
  for (j = 0; j < n2; j++)
  {
    D[j] = v[m + 1 + j];
  }

  i = 0;
  j = 0;
  int k = e;
  while (i < n1 && j < n2)
  {
    if (E[i].idp < D[j].idp)
    {
      v[k] = E[i];
      i++;
    }
    else
    {
      v[k] = D[j];
      j++;
    }
    k++;
  }

  while (i < n1)
  {
    v[k] = E[i];
    i++;
    k++;
  }
  while (j < n2)
  {
    v[k] = D[j];
    j++;
    k++;
  }
}


void intercala(int e, int m, int d, Produto produtos[])
{

    int n1 = (m - e) + 1;
    int n2 = (d - m);
    Produto E[n1], D[n2];
    int i, j;

    // Copia os elementos para os vetores temporários
    for (i = 0; i < n1; i++)
        E[i] = produtos[e + i];
    for (j = 0; j < n2; j++)
        D[j] = produtos[m + 1 + j];

    i = 0;
    j = 0;
    int k = e;

    // Intercala os dois subvetores, ordenando por preço (considerando promoções)
    while (i < n1 && j < n2)
    {
        float precoE = (E[i].promocao) ? E[i].precoPromocional : E[i].preco;
        float precoD = (D[j].promocao) ? D[j].precoPromocional : D[j].preco;

        if (precoE < precoD)
        {
            produtos[k] = E[i];
            i++;
        }
        else
        {
            produtos[k] = D[j];
            j++;
        }
        k++;
    }

    // Copia os elementos restantes
    while (i < n1)
    {
        produtos[k] = E[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        produtos[k] = D[j];
        j++;
        k++;
    }
    
}

// Função mergeSort para ordenar o array de produtos
    void mergeSort1(int i, int f, Produto produtos[])
    {
        if (i < f)
        {
            int meio = (i + f) / 2;
            mergeSort(i, meio, produtos);
            mergeSort(meio + 1, f, produtos);
            intercala1(i, meio, f, produtos);
        }

    }
void mergeSort(int i, int f, Produto produtos[])
{
    if (i < f)
    {
        int meio = (i + f) / 2;
        mergeSort(i, meio, produtos);
        mergeSort(meio + 1, f, produtos);
        intercala(i, meio, f, produtos);
    }
 
}

// Função para exibir os produtos cadastrados
// Função para exibir os produtos cadastrados
// Função para carregar todos os produtos do arquivo binário
int carregarProdutos(char *filename, Produto **produtos) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return 0;
    }

    // Calcula o número de produtos no arquivo
    fseek(file, 0, SEEK_END);
    int n = ftell(file) / sizeof(Produto);
    fseek(file, 0, SEEK_SET);

    // Aloca espaço para armazenar os produtos
    *produtos = (Produto *)malloc(n * sizeof(Produto));

    // Lê todos os produtos do arquivo
    fread(*produtos, sizeof(Produto), n, file);

    fclose(file);
    return n;
}

// Função para exibir os produtos cadastrados e ordenados por preço
void mostrarProdutos() {
    Produto *produtos = NULL;

    // Carrega os produtos do arquivo
    int numProdutos = carregarProdutos("produtos.b", &produtos);
    if (numProdutos == 0) {
        return;  // Nenhum produto foi carregado
    }

    // Ordena os produtos pelo preço (considerando promoções) em ordem crescente
    mergeSort(0, numProdutos - 1, produtos);

    // Exibe os produtos ordenados
    printf("\nLista de Produtos Cadastrados (Ordenados por Preço):\n");
    for (int i = 0; i < numProdutos; i++) {
        float preco = (produtos[i].promocao) ? produtos[i].precoPromocional : produtos[i].preco;
        printf("ID: %d, Nome: %s, Preço: R$%.2f\n", produtos[i].idp, produtos[i].nomep, preco);
    }

    printf("\n");

    // Libera a memória alocada
    free(produtos);
}
int qeDeP()
{
  FILE *file = fopen("produtos.b", "rb");
  Produto a;
  int cont = 0;
  while (fread(&a, sizeof(Produto), 1, file))
  {
    cont++;
  }
  fclose(file);
  return cont;
}
int buscaBinariaRecursiva(int cod, Produto v[], int inicio, int fina)
{
  if (inicio > fina)
  {
    return -1;
  }
  int meio = (inicio + fina) / 2;
  if (v[meio].idp == cod)
  {
    return meio;
  }
  if (v[meio].idp < cod)
  {
    return buscaBinariaRecursiva(cod, v, meio + 1, fina);
  }
  return buscaBinariaRecursiva(cod, v, inicio, meio - 1);
}

void buscar() {
    int cod;
    printf("Digite o código do produto que deseja buscar: ");
    scanf("%d", &cod);

    // Quantidade de produtos
    int n = qeDeP();
    if (n == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    // Alocando array para os produtos
    Produto *v = (Produto *)malloc(n * sizeof(Produto));

    // Abrindo o arquivo para leitura
    FILE *file = fopen("produtos.b", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        free(v);
        return;
    }

    // Lendo todos os produtos do arquivo
    for (int i = 0; i < n; i++) {
        fread(&v[i], sizeof(Produto), 1, file);
    }
    fclose(file);

    // Ordenando os produtos pelo campo idp usando mergeSort
    mergeSort1(0, n - 1, v);

    // Realizando a busca binária
    int pos = buscaBinariaRecursiva(cod, v, 0, n - 1);

    // Verificando se o produto foi encontrado
    if (pos == -1) {
        printf("Produto não encontrado.\n");
    } else {
        printf("\nProduto encontrado:\n");
        printf("Nome: %s\n", v[pos].nomep);
        printf("Código: %d\n", v[pos].idp);
        printf("Preço: R$%.2f\n", v[pos].preco);
        printf("Quantidade em estoque: %d\n", v[pos].qtd);
    }

    // Liberando a memória alocada
    free(v);
}
