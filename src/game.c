/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"

#include <stdarg.h>


#define POS_INIT_A_X                      1
#define POS_INIT_A_Y                      1
#define POS_INIT_B_X         MAPA_ANCHO - 2
#define POS_INIT_B_Y          MAPA_ALTO - 2

#define CANT_POSICIONES_VISTAS            9
#define MAX_SIN_CAMBIOS                 999

#define BOTINES_CANTIDAD 8

uint32_t  botines[BOTINES_CANTIDAD][3] = { // TRIPLAS DE LA FORMA (X, Y, MONEDAS)
                                        {30,  3, 50}, {31, 38, 50}, {15, 21, 100}, {45, 21, 100} ,
                                        {49,  3, 50}, {48, 38, 50}, {64, 21, 100}, {34, 21, 100}
                                    };

jugador_t jugadorA;
jugador_t jugadorB;


void* error()
{
	__asm__ ("int3");
	return 0;
}

uint32_t  game_xy2lineal (uint32_t x, uint32_t y) {
	return (y * MAPA_ANCHO + x);
}

uint32_t  game_posicion_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < MAPA_ANCHO && y < MAPA_ALTO);
}

pirata_t* id_pirata2pirata(uint32_t  id_pirata)
{
    // ~ completar ~
	return NULL;
}

uint32_t  game_dir2xy(direccion dir, int *x, int *y)
{
	switch (dir)
	{
		case IZQ: *x = -1; *y =  0; break;
		case DER: *x =  1; *y =  0; break;
		case ABA: *x =  0; *y =  1; break;
		case ARR: *x =  0; *y = -1; break;
    	default: return -1;
	}

	return 0;
}

uint32_t  game_valor_tesoro(uint32_t  x, uint32_t  y)
{
	int i;
	for (i = 0; i < BOTINES_CANTIDAD; i++)
	{
		if (botines[i][0] == x && botines[i][1] == y)
			return botines[i][2];
	}
	return 0;
}

// dada una posicion (x,y) guarda las posiciones de alrededor en dos arreglos, uno para las x y otro para las y
void game_calcular_posiciones_vistas(int *vistas_x, int *vistas_y, int x, int y)
{
	int next = 0;
	int i, j;
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			vistas_x[next] = x + j;
			vistas_y[next] = y + i;
			next++;
		}
	}
}


void game_inicializar() {
  // Código de prueba, borrar después
  lcr3((uint32_t) mmu_inicializar_dir_pirata(0x10000, 0x500000 + game_xy2lineal(1, 1)*0x1000, INDICE_JUGADOR_A));
}

void game_jugador_inicializar_mapa(jugador_t *jug)
{
}

void game_jugador_inicializar(jugador_t *j)
{
	static int index = 0;

	j->index = index++;
    // ~ completar ~

}

void game_pirata_inicializar(pirata_t *pirata, jugador_t *j, uint32_t  index, uint32_t  id)
{
}

void game_tick(uint32_t  id_pirata) {
  screen_actualizar_reloj_global();
  // Falta actualizar el reloj del pirata
}


void game_pirata_relanzar(pirata_t *pirata, jugador_t *j, uint32_t  tipo)
{
}

pirata_t* game_jugador_erigir_pirata(jugador_t *j, uint32_t  tipo)
{
    // ~ completar ~

	return NULL;
}


void game_jugador_lanzar_pirata(jugador_t *j, uint32_t  tipo, int x, int y)
{
}

void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y)
{
}


void game_explorar_posicion(jugador_t *jugador, int c, int f)
{
}


uint32_t  game_syscall_pirata_mover(uint32_t  id, direccion dir)
{
    // ~ completar
    return 0;
}

uint32_t  game_syscall_cavar(uint32_t  id)
{
    // ~ completar ~

	return 0;
}

uint32_t  game_syscall_pirata_posicion(uint32_t  id, int idx)
{
    // ~ completar ~
    return 0;
}

uint32_t  game_syscall_manejar(uint32_t  syscall, uint32_t  param1)
{
    // ~ completar ~
    return 0;
}

void game_pirata_exploto(uint32_t  id)
{
}

pirata_t* game_pirata_en_posicion(uint32_t  x, uint32_t  y)
{
	return NULL;
}


void game_jugador_anotar_punto(jugador_t *j)
{
}



void game_terminar_si_es_hora()
{
}

#define KB_y        0x15 // 0x95
#define bc_y        0x95// break tecla y
#define KB_w        0x11 // 0x91
#define KB_s        0x1f // 0x9f
#define KB_a        0x1e // 0x9e
#define KB_d        0x20 // 0xa0
#define KB_e        0x12 // 0x92
#define KB_q        0x10 // 0x90
#define KB_i        0x17 // 0x97
#define KB_k        0x25 // 0xa5
#define KB_j        0x24 // 0xa4
#define KB_l        0x26 // 0xa6
#define KB_shiftL   0x2a // 0xaa
#define KB_shiftR   0x36 // 0xb6


void game_atender_teclado(uint8_t tecla)//(5)
{
	switch (tecla)
	{
		case KB_y: screen_pintar('y', C_FG_LIGHT_GREY, 30, 50);break;
		case bc_y: screen_pintar(219, C_FG_LIGHT_GREY, 30, 50);break;
  
		case KB_q: screen_pintar('q', C_FG_LIGHT_GREY, 30, 50);break;
 
		case KB_a:  screen_pintar('a', C_FG_LIGHT_GREY, 30, 50);break;
 
		case KB_k:   screen_pintar('k', C_FG_LIGHT_GREY, 30, 50);break;
  
		default: break;
	}
}
