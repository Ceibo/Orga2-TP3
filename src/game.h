/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"

typedef enum direccion_e { ARR = 0x4, ABA = 0x7, DER = 0xA, IZQ = 0xD} direccion;

#define MAX_CANT_PIRATAS_VIVOS           8

#define INDICE_JUGADOR_A                         0
#define INDICE_JUGADOR_B                         1

#define MAPA_ANCHO                       80
#define MAPA_ALTO                        44


struct jugador_t;

typedef struct pirata_t
{
    uint32_t index;
    struct jugador_t *jugador;

    // id unica, posicion, tipo, reloj
} pirata_t;


typedef struct jugador_t
{
    uint32_t index;
    pirata_t piratas[MAX_CANT_PIRATAS_VIVOS];
    // coordenadas puerto, posiciones exploradas, mineros pendientes, etc
} jugador_t;


extern jugador_t jugadorA, jugadorB;

// ~ auxiliares dadas ~
uint32_t game_xy2lineal(uint32_t x, uint32_t y);
pirata_t* id_pirata2pirata(uint32_t id);

// ~ auxiliares sugeridas o requeridas (segun disponga enunciado) ~
void game_pirata_inicializar(pirata_t *pirata, jugador_t *jugador, uint32_t index, uint32_t id);
void game_pirata_erigir(pirata_t *pirata, jugador_t *j, uint32_t tipo);
void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int32_t x, int32_t y);
void game_pirata_exploto(uint32_t id);

void game_jugador_inicializar(jugador_t *j);
void game_jugador_lanzar_pirata(jugador_t *j, uint32_t tipo, int32_t x, int32_t y);
pirata_t* game_jugador_erigir_pirata(jugador_t *j, uint32_t tipo);
void game_jugador_anotar_punto(jugador_t *j);
void game_explorar_posicion(jugador_t *jugador, int32_t x, int32_t y);

uint32_t game_valor_tesoro(uint32_t x, uint32_t y);
void game_calcular_posiciones_vistas(int32_t *vistas_x, int32_t *vistas_y, int32_t x, int32_t y);
pirata_t* game_pirata_en_posicion(uint32_t x, uint32_t y);

uint32_t game_syscall_pirata_posicion(uint32_t id, int32_t idx);
uint32_t game_syscall_pirata_mover(uint32_t id, direccion key);
uint32_t game_syscall_manejar(uint32_t syscall, uint32_t param1);
void game_tick(uint32_t id_pirata);
void game_terminar_si_es_hora();
void game_atender_teclado(unsigned char tecla);


#endif  /* !__GAME_H__ */
