/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "screen.h"
#include "game.h"


extern jugador_t jugadorA, jugadorB;


static ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

const char reloj[] = "|/-\\";
#define reloj_size 4


void screen_actualizar_reloj_global()
{
    static uint reloj_global = 0;

    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_pintar(uchar c, uchar color, uint fila, uint columna)
{
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

uchar screen_valor_actual(uint fila, uint columna)
{
    return p[fila][columna].c;
}

void print(const char * text, uint x, uint y, unsigned short attr) {
    int i;
    for (i = 0; text[i] != 0; i++)
     {
        screen_pintar(text[i], attr, y, x);

        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_dec(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        int resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void screen_pintar_rect(unsigned char c, unsigned char color, int fila, int columna, int alto, int ancho) {
	uint i, j;
	for (i = fila; i < fila + alto; i++) {
		for (j = columna; j < columna + ancho; j++) {
			screen_pintar(c, color, i, j);
		}
	}
}

void screen_pintar_linea_h(unsigned char c, unsigned char color, int fila, int columna, int ancho) {
	while (ancho-- > 0) {
		screen_pintar(c, color, fila, columna++);
	}
}

void screen_pintar_linea_v(unsigned char c, unsigned char color, int fila, int columna, int alto) {
	while (alto-- > 0) {
		screen_pintar(c, color, fila++, columna);
	}
}

void screen_inicializar() {
	screen_pintar_rect(0, C_BG_LIGHT_GREY, 1, 0, 44, 80); // mapa con fondo gris
	screen_pintar_rect(0, C_BG_RED, 45, 33, 5, 7); // barra de jugador rojo
	screen_pintar_rect(0, C_BG_BLUE, 45, 40, 5, 7); // barra de jugador azul
}

/*void screen_pintar_puntajes() {}
void screen_actualizar_reloj_pirata (jugador_t *j, pirata_t *pirata) {}
unsigned char screen_color_jugador(jugador_t *j) {}
unsigned char screen_caracter_pirata(unsigned int tipo) {}
void screen_pintar_pirata(jugador_t *j, pirata_t *pirata) {}
void screen_borrar_pirata(jugador_t *j, pirata_t *pirata) {}
void screen_pintar_reloj_pirata(jugador_t *j, pirata_t *pirata) {}
void screen_pintar_reloj_piratas(jugador_t *j) {}
void screen_pintar_relojes() {}
void screen_actualizar_posicion_mapa(uint x, uint y) {}
void screen_stop_game_show_winner(jugador_t *j) {}
int ee_printf(const char *fmt, ...) {}*/

// Escribe el caracter nulo en todas las celdas de la pantalla y pone fondo negro
void clear_screen() {
	uint fila, columna;
	for (fila = 0; fila < VIDEO_FILS; fila++) {
		for (columna = 0; columna < VIDEO_COLS; columna++) {
			screen_pintar(0, C_BG_BLACK, fila, columna);
		}
	}
}

void print_isr(){
  print("Interrupcion ha sido atendida.", 0, 3, C_FG_LIGHT_GREY);
}

void imprimir_nombre_de_grupo(){
    print("El crew de Ariane 5", 61, 0, C_BG_MAGENTA+C_FG_LIGHT_GREEN);
}

