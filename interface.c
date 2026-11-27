#include "fila_senhas.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ler_inteiro_intervalo(const char *mensagem, int min, int max) {
    char linha[64];
    char *fim_ptr;
    long valor;

    while (true) {
        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            clearerr(stdin);
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }
        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0') {
            printf("Valor vazio. Informe um numero entre %d e %d.\n", min, max);
            continue;
        }
        valor = strtol(linha, &fim_ptr, 10);
        while (*fim_ptr != '\0' && isspace((unsigned char)*fim_ptr)) {
            fim_ptr++;
        }
        if (*fim_ptr != '\0' || valor < min || valor > max) {
            printf("Valor fora do intervalo [%d..%d]. Tente novamente.\n", min, max);
            continue;
        }
        return (int)valor;
    }
}

bool ler_sim_nao(const char *mensagem) {
    char linha[32];
    while (true) {
        printf("%s (S/N): ", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }
        for (size_t i = 0; linha[i] != '\0'; i++) {
            if (!isspace((unsigned char)linha[i])) {
                char ch = (char)toupper((unsigned char)linha[i]);
                if (ch == 'S') {
                    return true;
                }
                if (ch == 'N') {
                    return false;
                }
                break;
            }
        }
        printf("Resposta invalida. Digite 'S' para sim ou 'N' para nao.\n");
    }
}

char ler_genero(void) {
    char linha[32];
    while (true) {
        printf("Genero do paciente (F - Feminino / M - Masculino / O - Outro): ");
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }
        for (size_t i = 0; linha[i] != '\0'; i++) {
            if (!isspace((unsigned char)linha[i])) {
                char ch = (char)toupper((unsigned char)linha[i]);
                if (ch == 'F' || ch == 'M' || ch == 'O') {
                    return ch;
                }
                break;
            }
        }
        printf("Opcao invalida. Informe F, M ou O.\n");
    }
}

const char *nome_categoria(Categoria cat) {
    switch (cat) {
        case CAT_URGENTE:
            return "Urgente";
        case CAT_PREFERENCIAL:
            return "Preferencial";
        case CAT_NORMAL:
        default:
            return "Normal";
    }
}

void exibir_painel(const SistemaSenhas *sys) {
    printf("\n============================================================\n");
    printf("               PAINEL DE CHAMADA — CLINICA MEDICA           \n");
    printf("============================================================\n");

    if (sys->ha_em_atendimento) {
        printf("Senha em atendimento : %s\n", sys->senha_atual.codigo);
        printf("Categoria           : %s\n", nome_categoria(sys->senha_atual.categoria));
    } else {
        printf("Senha em atendimento : NENHUMA (aguardando chamada)\n");
        printf("Categoria           : -\n");
    }

    printf("------------------------------------------------------------\n");
    printf("Proximas senhas na fila (por precedencia):\n");

    const Categoria ordem[3] = {CAT_URGENTE, CAT_PREFERENCIAL, CAT_NORMAL};
    int exibidas = 0;
    for (int i = 0; i < 3; i++) {
        const FilaCategoria *f = &sys->filas[(int)ordem[i]];
        for (int j = 0; j < f->total && exibidas < 5; j++) {
            int idx = (f->inicio + j) % MAX_FILA;
            printf("  %d. %s [%s]\n", exibidas + 1, f->itens[idx].codigo,
                   nome_categoria(f->itens[idx].categoria));
            exibidas++;
        }
    }
    if (exibidas == 0) {
        printf("  (Nenhuma senha aguardando na fila)\n");
    }

    printf("------------------------------------------------------------\n");
    printf("Aguardando por categoria: Urgente: %d | Preferencial: %d | Normal: %d\n",
           contar_em_espera(sys, CAT_URGENTE),
           contar_em_espera(sys, CAT_PREFERENCIAL),
           contar_em_espera(sys, CAT_NORMAL));
    printf("============================================================\n");
}

void exibir_menu(void) {
    printf("\n--- Menu Principal ---\n");
    printf("1. Registrar novo paciente e emitir senha\n");
    printf("2. Consultar proxima senha a ser chamada\n");
    printf("3. Registrar chamada do paciente\n");
    printf("4. Identificar senha em atendimento\n");
    printf("5. Finalizar atendimento atual\n");
    printf("6. Exibir painel de chamada completo\n");
    printf("0. Encerrar sistema\n");
}