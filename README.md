# A1_KARLA
Prova regimental código em C modularizado
# Sistema de Gerenciamento de Senhas de Atendimento Clínico

Aplicação interativa desenvolvida em **linguagem C** para triagem, emissão sequencial de senhas por categoria, controle de filas FIFO com precedência e exibição de painel de chamada em clínicas de especialidades médicas.

## Objetivo do Sistema

Automatizar a organização das filas de atendimento clínico garantindo a identificação automática de prioridades, a ordem FIFO dentro de cada categoria e o controle claro do ciclo de chamada e encerramento de senhas.

## Funcionalidades

1. **Registro de pacientes e emissão de senhas (`Módulo 1`):** coleta gênero, idade e condições de prioridade (gestante quando aplicável, lactante, necessidade especial e obesidade) e gera códigos sequenciais independentes (`U001`, `P001`, `N001`).
2. **Classificação automática com precedência (`Módulo 2`):**
   - **Urgente (`Uxxx`):** idade superior a 80 anos.
   - **Preferencial (`Pxxx`):** gestantes, lactantes, pessoas com necessidade especial, pessoas com obesidade, mulheres com idade igual ou superior a 60 anos e homens com idade igual ou superior a 65 anos.
   - **Normal (`Nxxx`):** demais pacientes sem condições prioritárias cadastradas.
   - Ordem de chamada: **Urgente > Preferencial > Normal**, respeitando **FIFO** em cada fila.
3. **Controle de atendimento (`Módulo 3`):** consulta da próxima senha, registro de chamada, identificação da senha em atendimento, finalização do atendimento sem retorno à fila e atualização automática da fila.
4. **Painel textual (`Módulo 4`):** exibe senha em atendimento, categoria, próximas senhas na fila e contagem de pacientes aguardando por categoria.

## Procedimentos de Compilação e Execução

### Pré-requisitos
- Compilador `gcc` (suporte a C11) e `make` (opcional).

### Compilação
```bash
### Compilação
```bash
gcc -Wall -Wextra -std=c11 main.c triagem.c filas.c interface.c -o clinica_senhas
```

### Execução
```bash
./clinica_senhas
```

## Funções Implementadas

- `inicializar_sistema`: zera filas circulares, contadores sequenciais e estado de atendimento.
- `ler_inteiro_intervalo`, `ler_sim_nao`, `ler_genero`: validam entradas do usuário via `stdin` evitando falhas de leitura ou valores fora da faixa.
- `classificar_paciente`: determina `CAT_URGENTE`, `CAT_PREFERENCIAL` ou `CAT_NORMAL` conforme as regras da atividade.
- `gerar_codigo_senha`: formata os códigos `U001`, `P001`, `N001`, etc.
- `cadastrar_paciente`: coordena a coleta de dados, classificação e inserção na fila correspondente.
- `fila_vazia`, `fila_cheia`, `enfileirar`, `desenfileirar`, `contar_em_espera`: operam a estrutura FIFO de cada categoria.
- `consultar_proxima_senha`: identifica o próximo paciente respeitando a precedência sem removê-lo da fila.
- `registrar_chamada`: move o próximo paciente da fila de maior precedência para atendimento ativo.
- `finalizar_atendimento`: encerra o atendimento em curso sem reinserir a senha na fila.
- `exibir_painel` e `exibir_menu`: renderizam o painel textual de chamada e o menu interativo.

## Exemplos de Utilização

### Exemplo 1: Cadastro com precedência e chamada
1. Selecione `1` e cadastre um paciente com idade `85` -> gera `U001` (**Urgente**).
2. Selecione `1` e cadastre uma paciente gestante de `28` anos -> gera `P001` (**Preferencial**).
3. Selecione `1` e cadastre um paciente masculino de `30` anos sem condições -> gera `N001` (**Normal**).
4. Selecione `3` (**Registrar chamada**) -> chama `U001` primeiro, mesmo que `P001` ou `N001` tenham sido cadastrados antes dentro da mesma prioridade ou em fila inferior.
5. Selecione `5` (**Finalizar atendimento**) -> encerra `U001` e permite chamar `P001` em seguida.

### Exemplo 2: Tratamento de fila vazia e chamada dupla
- Escolher `3` com todas as filas vazias exibe mensagem informativa sem encerrar o programa.
- Escolher `3` novamente enquanto já existe uma senha em atendimento bloqueia a nova chamada até que a opção `5` finalize o atendimento atual.
