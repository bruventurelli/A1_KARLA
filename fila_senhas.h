#ifndef FILA_SENHAS_H
#define FILA_SENHAS_H

#include <stdbool.h>

#define MAX_FILA 100
#define TAM_SENHA 8

typedef enum {
    CAT_NORMAL = 0,
    CAT_PREFERENCIAL = 1,
    CAT_URGENTE = 2
} Categoria;

typedef struct {
    char codigo[TAM_SENHA];
    Categoria categoria;
    char genero;
    int idade;
    bool gestante;
    bool lactante;
    bool necessidade_especial;
    bool obesidade;
} Paciente;

typedef struct {
    Paciente itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
    int proximo_numero;
} FilaCategoria;

typedef struct {
    FilaCategoria filas[3]; 
    Paciente senha_atual;
    bool ha_em_atendimento;
} SistemaSenhas;

int ler_inteiro_intervalo(const char *mensagem, int min, int max);
bool ler_sim_nao(const char *mensagem);
char ler_genero(void);
const char *nome_categoria(Categoria cat);
void exibir_painel(const SistemaSenhas *sys);
void exibir_menu(void);

Categoria classificar_paciente(char genero, int idade, bool gestante, bool lactante,
                             bool necessidade_especial, bool obesidade);
void gerar_codigo_senha(Categoria cat, int numero, char destino[TAM_SENHA]);
bool cadastrar_paciente(SistemaSenhas *sys);

void inicializar_sistema(SistemaSenhas *sys);
bool fila_vazia(const FilaCategoria *fila);
bool fila_cheia(const FilaCategoria *fila);
bool enfileirar(FilaCategoria *fila, Paciente p);
bool desenfileirar(FilaCategoria *fila, Paciente *removido);
int contar_em_espera(const SistemaSenhas *sys, Categoria cat);
bool consultar_proxima_senha(const SistemaSenhas *sys, Paciente *proxima);
bool registrar_chamada(SistemaSenhas *sys);
bool finalizar_atendimento(SistemaSenhas *sys);

#endif