/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */


#include "colors.h"
#include "defines.h"
/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    uint8_t  c;
    uint8_t  a;
} ca;


struct pirata_t;
typedef struct pirata_t pirata_t;

struct jugador_t;
typedef struct jugador_t jugador_t;

int32_t ee_printf(const char *fmt, ...);


void screen_pintar(uint8_t  c, uint8_t  color, uint32_t fila, uint32_t columna);
void print(const char * text, uint32_t x, uint32_t y, uint16_t attr);
void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr);
void screen_pintar_rect(uint8_t  c, uint8_t  color, int32_t fila, int32_t columna, int32_t alto, int32_t ancho);
void screen_pintar_linea_h(uint8_t  c, uint8_t  color, int32_t fila, int32_t columna, int32_t ancho);
void screen_pintar_linea_v(uint8_t  c, uint8_t  color, int32_t fila, int32_t columna, int32_t alto);
void screen_inicializar();
void screen_pintar_puntajes();

void screen_actualizar_reloj_global();
void screen_actualizar_reloj_pirata (jugador_t *j, pirata_t *pirata);
uint8_t  screen_color_jugador(jugador_t *j);
uint8_t  screen_caracter_pirata(uint32_t tipo);
void screen_pintar_pirata(jugador_t *j, pirata_t *pirata);
void screen_borrar_pirata(jugador_t *j, pirata_t *pirata);
void screen_pintar_reloj_pirata(jugador_t *j, pirata_t *pirata);
void screen_pintar_reloj_piratas(jugador_t *j);
void screen_pintar_relojes();

void screen_actualizar_posicion_mapa(uint32_t x, uint32_t y);
void screen_stop_game_show_winner(jugador_t *j);

void clear_screen();

void print_isr(int32_t i);
void imprimir_nombre_de_grupo();
uint8_t screen_valor_actual(uint32_t fila, uint32_t columna);
//agregada
//pinta el entorno del pirata (9 lugares en total si es posible)
void screen_entorno_pirata(uint8_t color, int32_t fila, int32_t columna, int32_t alto, int32_t ancho);
uint8_t screen_color_actual(uint32_t fila, uint32_t columna);
//si hay mAs de 100 monedas entonces devuelve *, sino $.
uint8_t screen_caracter_tesoro(uint32_t valor);
void mostrar_info_debugger();
void screen_pantalla_debug();
void mostrar_info_debugger();
void restaurar_pantalla_original();

#endif  /* !__SCREEN_H__ */
