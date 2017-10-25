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
    static uint32_t reloj_global = 0;

    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_pintar(uint8_t c, uint8_t color, uint32_t fila, uint32_t columna)
{
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

uint8_t screen_valor_actual(uint32_t fila, uint32_t columna)
{
    return p[fila][columna].c;
}

void print(const char * text, uint32_t x, uint32_t y, uint16_t attr) {
    int32_t i;
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

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    int32_t i;
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

void print_dec(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    int32_t i;
    char letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        int32_t resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void screen_pintar_rect(uint8_t c, uint8_t color, int32_t fila, int32_t columna, int32_t alto, int32_t ancho) {
	uint32_t i, j;
	for (i = fila; i < fila + alto; i++) {
		for (j = columna; j < columna + ancho; j++) {
			screen_pintar(c, color, i, j);
		}
	}
}

void screen_pintar_linea_h(uint8_t c, uint8_t color, int32_t fila, int32_t columna, int32_t ancho) {
	while (ancho-- > 0) {
		screen_pintar(c, color, fila, columna++);
	}
}

void screen_pintar_linea_v(uint8_t c, uint8_t color, int32_t fila, int32_t columna, int32_t alto) {
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
uint8_t screen_color_jugador(jugador_t *j) {}
uint8_t screen_caracter_pirata(unsigned int32_t tipo) {}
void screen_pintar_pirata(jugador_t *j, pirata_t *pirata) {}
void screen_borrar_pirata(jugador_t *j, pirata_t *pirata) {}
void screen_pintar_reloj_pirata(jugador_t *j, pirata_t *pirata) {}
void screen_pintar_reloj_piratas(jugador_t *j) {}
void screen_pintar_relojes() {}
void screen_actualizar_posicion_mapa(uint32_t x, uint32_t y) {}
void screen_stop_game_show_winner(jugador_t *j) {}
int32_t ee_printf(const char *fmt, ...) {}*/

// Escribe el caracter nulo en todas las celdas de la pantalla y pone fondo negro
void clear_screen() {
	uint32_t fila, columna;
	for (fila = 0; fila < VIDEO_FILS; fila++) {
		for (columna = 0; columna < VIDEO_COLS; columna++) {
			screen_pintar(0, C_BG_BLACK, fila, columna);
		}
	}
}

void print_isr(int32_t i){

if(i == 0)
	print("Interrupcion 0 ocurrida: Divide Error.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 1)
	print("Interrupcion 1 ocurrida: Debug Exception.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 2)
	print("Interrupcion 2 ocurrida: NMI Interrupt.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 3)
	print("Interrupcion 3 ocurrida: Breakpoint.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 4)
	print("Interrupcion 4 ocurrida: Overflow.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 5)
	print("Interrupcion 5 ocurrida: BOUND Range Exceeded.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 6)
	print("Interrupcion 6 ocurrida: Invalid Opcode (Undefined Opcode).", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 7)
	print("Interrupcion 7 ocurrida: Device Not Available (No Math Coprocesor).", 0, 3, C_FG_LIGHT_GREY);

if(i == 8)
	print("Interrupcion 8 ocurrida: Double Fault.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 9)
	print("Interrupcion 9 ocurrida: Coprocessor Segment Overrun.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 10)
	print("Interrupcion 10 ocurrida: Invalid TSS.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 11)
	print("Interrupcion 11 ocurrida: Segment Not Present.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 12)
	print("Interrupcion 12 ocurrida: Stack-Segment Fault.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 13)
    print("Interrupcion 13 ocurrida: General Protection.", 0, 3, C_FG_LIGHT_GREY);
    
if(i == 14)
	print("Interrupcion 14 ocurrida: Page Fault.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 15)
	print("Interrupcion 15 ocurrida", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 16)
	print("Interrupcion 16 ocurrida: x87 FPU Floating-Point Error (Math Fault)", 0, 3, C_FG_LIGHT_GREY);

if(i == 17)
	print("Interrupcion 17 ocurrida: Alignment Check.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 18)
	print("Interrupcion 18 ocurrida: Machine Check.", 0, 3, C_FG_LIGHT_GREY);
	
if(i == 19)
	print("Interrupcion 19 ocurrida: SIMD Floating-Point Exception.", 0, 3, C_FG_LIGHT_GREY);
 
}


void imprimir_nombre_de_grupo(){
    print("El crew de Ariane 5", 61, 0, C_BG_MAGENTA+C_FG_LIGHT_GREEN);
}

