#include "fila_senhas.h"
#include <stdio.h>
#include <string.h>

Categoria classificar_paciente(char genero, int idade, bool gestante, bool lactante,
                             bool necessidade_especial, bool obesidade) {
    if (idade > 80) {
        return CAT_URGENTE;
    }

    bool prioridade_idade = (genero == 'F' && idade >= 60) || (genero == 'M' && idade >= 65);
    if (gestante || lactante || necessidade_especial || obesidade || prioridade_idade) {
        return CAT_PREFERENCIAL;
    }

    return CAT_NORMAL;
}

void gerar_codigo_senha(Categoria cat, int numero, char destino[TAM_SENHA]) {
    char prefixo = 'N';
    if (cat == CAT_URGENTE) {
        prefixo = 'U';
    } else if (cat == CAT_PREFERENCIAL) {
        prefixo = 'P';
    }
    snprintf(destino, TAM_SENHA, "%c%03d", prefixo, numero);
}

bool cadastrar_paciente(SistemaSenhas *sys) {
    Paciente p;
    memset(&p, 0, sizeof(Paciente));

    printf("\n--- Cadastro de Paciente e Emissao de Senha ---\n");
    p.genero = ler_genero();
    p.idade = ler_inteiro_intervalo("Idade do paciente (0 a 130): ", 0, 130);

    if (p.genero == 'F') {
        p.gestante = ler_sim_nao("A paciente esta gestante?");
    } else {
        p.gestante = false;
    }

    p.lactante = ler_sim_nao("O paciente e lactante?");
    p.necessidade_especial = ler_sim_nao("Apresenta necessidade especial?");
    p.obesidade = ler_sim_nao("Possui condicao de obesidade?");

    p.categoria = classificar_paciente(p.genero, p.idade, p.gestante, p.lactante,
                                       p.necessidade_especial, p.obesidade);

    FilaCategoria *fila_alvo = &sys->filas[(int)p.categoria];
    if (fila_cheia(fila_alvo)) {
        printf("Erro: fila da categoria %s atingiu a capacidade maxima (%d).\n",
               nome_categoria(p.categoria), MAX_FILA);
        return false;
    }

    gerar_codigo_senha(p.categoria, fila_alvo->proximo_numero, p.codigo);
    fila_alvo->proximo_numero++;

    if (!enfileirar(fila_alvo, p)) {
        printf("Erro ao adicionar senha na fila.\n");
        return false;
    }

    printf("Senha emitida: %s | Categoria: %s | Posicao na fila: %d\n",
           p.codigo, nome_categoria(p.categoria), fila_alvo->total);
    return true;
}