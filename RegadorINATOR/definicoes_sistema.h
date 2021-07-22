#ifndef DEFINICOES_SISTEMA_H_INCLUDED
#define DEFINICOES_SISTEMA_H_INCLUDED

#define true  1
#define false 0

#define MAX_TAREFAS 2

#define NUM_ESTADOS 5
#define NUM_EVENTOS 10

// ESTADOS
#define Idle           0
#define Emparelhamento 1
#define Rega           2
#define Fim_Rega       3
#define Falha_Rega     4

// EVENTOS
#define NENHUM_EVENTO         -1
#define emparelhando           0
#define umidade_acima          1
#define umidade_abaixo         2
#define umidade_manteve        3
#define timer_rega             4
#define cancelar               5
#define finalizar_falha        6
#define iniciar_emparelhamento 7
#define parametro_umid         8
#define parametro_tempo        9

// ACOES
#define NENHUMA_ACAO -1
#define A01  0  // Emparelhar
#define A02  1  // Desemparelhar
#define A03  2  // Acionar rega (umidade abaixo)
#define A04  3  // Finalizar rega (umidade acima)
#define A05  4  // Falha na rega (umidade manteve)
#define A06  5  // Terminou o timer de rega
#define A07  6  // Cancelar rega
#define A08  7  // Cancelar falha
#define A09  8  // Definir umidade
#define A10  9  // Definir tempo

#endif // DEFINICOES_SISTEMA_H_INCLUDED
