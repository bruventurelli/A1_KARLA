#include "fila_senhas.h"
#include <stdio.h>
#include <string.h>

void inicializar_sistema(SistemaSenhas *sys) {
    for (int i = 0; i < 3; i++) {
        sys->filas[i].inicio = 0;
        sys->filas[i].fim = 0;
        sys->filas[i].total = 0;
        sys->filas[i].proximo_numero = 1;
    }
    memset(&sys->senha_atual, 0, sizeof(Paciente));
    sys->ha_em_atendimento = false;
}

bool fila_vazia(const FilaCategoria *fila) {
    return fila->total == 0;
}

bool fila_cheia(const FilaCategoria *fila) {
    return fila->total >= MAX_FILA;
}

bool enfileirar(FilaCategoria *fila, Paciente p) {
    if (fila_cheia(fila)) {
        return false;
    }
    fila->itens[fila->fim] = p;
    fila->fim = (fila->fim + 1) % MAX_FILA;
    fila->total++;
    return true;
}

bool desenfileirar(FilaCategoria *fila, Paciente *removido) {
    if (fila_vazia(fila)) {
        return false;
    }
    if (removido != NULL) {
        *removido = fila->itens[fila->inicio];
    }
    fila->inicio = (fila->inicio + 1) % MAX_FILA;
    fila->total--;
    return true;
}

int contar_em_espera(const SistemaSenhas *sys, Categoria cat) {
    return sys->filas[(int)cat].total;
}

bool consultar_proxima_senha(const SistemaSenhas *sys, Paciente *proxima) {
    const Categoria ordem[3] = {CAT_URGENTE, CAT_PREFERENCIAL, CAT_NORMAL};
    for (int i = 0; i < 3; i++) {
        const FilaCategoria *f = &sys->filas[(int)ordem[i]];
        if (!fila_vazia(f)) {
            if (proxima != NULL) {
                *proxima = f->itens[f->inicio];
            }
            return true;
        }
    }
    return false;
}

bool registrar_chamada(SistemaSenhas *sys) {
    if (sys->ha_em_atendimento) {
        printf("Aviso: ja existe a senha %s (%s) em atendimento. Finalize antes de chamar a proxima.\n",
               sys->senha_atual.codigo, nome_categoria(sys->senha_atual.categoria));
        return false;
    }

    const Categoria ordem[3] = {CAT_URGENTE, CAT_PREFERENCIAL, CAT_NORMAL};
    for (int i = 0; i < 3; i++) {
        FilaCategoria *f = &sys->filas[(int)ordem[i]];
        if (!fila_vazia(f)) {
            desenfileirar(f, &sys->senha_atual);
            sys->ha_em_atendimento = true;
            printf("Chamada registrada: senha %s — Categoria %s.\n",
                   sys->senha_atual.codigo, nome_categoria(sys->senha_atual.categoria));
            return true;
        }
    }

    printf("Nao ha pacientes aguardando nas filas para chamada.\n");
    return false;
}

bool finalizar_atendimento(SistemaSenhas *sys) {
    if (!sys->ha_em_atendimento) {
        printf("Nao ha nenhum atendimento ativo no momento para finalizar.\n");
        return false;
    }

    printf("Atendimento da senha %s (%s) finalizado. Senha encerrada e removida do fluxo.\n",
           sys->senha_atual.codigo, nome_categoria(sys->senha_atual.categoria));
    memset(&sys->senha_atual, 0, sizeof(Paciente));
    sys->ha_em_atendimento = false;
    return true;
}