#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usuario.h"
#include "produto.h"
void menu_administrativo() {
    if (!autenticar_admin()) {
        return; 
    }

    int op;
    do {
        printf("\n1 - Cadastrar Produto\n");
        printf("2 - Cadastrar Promoção\n");
        printf("3 - Editar Produto\n");
        printf("4 - Excluir Produto\n");
        printf("5 - excluir promoção\n");
        printf("6 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &op);

        if (op ==1){
                cadastrar_produto(); 
        }
            else if(op == 2){
                cadastrar_promocao(); 
            }
            else if( op == 3){
                editar_produto();
            }
                else if(op ==4){
                    excluir_produto();
                }
                else if(op == 5){
                    excluir_promocao();
                }
            else {
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (op != 6);
}


int menu(Carrinho* carrinho) {
    int op;
    do {
        printf("1 - Abrir Loja\n");
        printf("2 - Ver Carrinho\n");
        printf("3 - Adicionar Produto ao Carrinho\n");
        printf("4 - Promoções do Dia\n");
        printf("5 - mostrar Produtos pelo preço\n");
        printf("6 - buscar\n");
        printf("7 - Sair\n");
        scanf("%d", &op);

        if (op == 1) {
            listar_produtos();
        } else if (op == 2) {
            ver_carrinho(carrinho);
        } else if (op == 3) {
            adicionar_ao_carrinho(carrinho);
        } else if (op == 4) {
            mostrar_promocoes();
        } 
        else if (op == 5) {
            mostrarProdutos();
        }
        //o buscar não consegui fazer da certo de jeito nenhum    
        else if (op == 6) {
                buscar();
                
        }else if (op == 7) {
            printf("Saindo do menu...\n");
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (op != 7);
    return 0;
}
int main() {
    int opcao;
    Carrinho carrinho = { .numItens = 0 };

    do {
        printf("1. Registrar\n");
        printf("2. Login Cliente\n");
        printf("3. Login Adm\n");
        printf("4. sair\n");
        printf("Escolha uma opção:\n");
        scanf("%d", &opcao);

        if (opcao == 1) {
            registrar_usuario();
        } else if (opcao == 2) {
            if (login()) {
                printf("Acesso ao sistema permitido.\n");
                menu(&carrinho);
            }
        } else if (opcao == 3) {
            menu_administrativo();
        }
        
    } while (opcao != 4);

    return 0;
}