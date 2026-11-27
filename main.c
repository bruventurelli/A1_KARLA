#include "fila_senhas.h"
#include <stdio.h>

int main(void) {
    SistemaSenhas sistema;
    inicializar_sistema(&sistema);

    int opcao;
    do {
        exibir_menu();
        opcao = ler_inteiro_intervalo("Escolha uma opcao: ", 0, 6);

        switch (opcao) {
            case 1:
                cadastrar_paciente(&sistema);
                break;
            case 2: {
                Paciente prox;
                if (consultar_proxima_senha(&sistema, &prox)) {
                    printf("Proxima senha a ser chamada: %s (%s)\n",
                           prox.codigo, nome_categoria(prox.categoria));
                } else {
                    printf("Fila vazia: nao ha proxima senha aguardando.\n");
                }
                break;
            }
            case 3:
                registrar_chamada(&sistema);
                exibir_painel(&sistema);
                break;
            case 4:
                if (sistema.ha_em_atendimento) {
                    printf("Senha atualmente em atendimento: %s (%s)\n",
                           sistema.senha_atual.codigo,
                           nome_categoria(sistema.senha_atual.categoria));
                } else {
                    printf("Nao ha nenhuma senha em atendimento no momento.\n");
                }
                break;
            case 5:
                finalizar_atendimento(&sistema);
                exibir_painel(&sistema);
                break;
            case 6:
                exibir_painel(&sistema);
                break;
            case 0:
                printf("Encerrando o sistema de gerenciamento de senhas.\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}