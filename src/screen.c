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
 

ca cartel[VIDEO_FILS][VIDEO_COLS];
const char reloj[] = "|/-\\";
#define reloj_size 4

struct {
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
  uint32_t esi;
  uint32_t edi;
  uint32_t ebp;
  uint32_t esp;
  uint32_t eip;
  uint16_t cs;
  uint16_t ds;
  uint16_t es;
  uint16_t fs;
  uint16_t gs;
  uint16_t ss;
  uint32_t eflags;
  uint32_t cr0;
  uint32_t cr2;
  uint32_t cr3;
  uint32_t cr4;
  uint32_t stack0;
  uint32_t stack1;
  uint32_t stack2;
  uint32_t stack3;
  uint32_t stack4;
} __attribute__((__packed__)) debugger;


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
uint8_t screen_color_actual(uint32_t fila, uint32_t columna)
{
    return p[fila][columna].a;
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

void screen_entorno_pirata(uint8_t color, int32_t fila, int32_t columna, int32_t alto, int32_t ancho) {
	//posiciOn no efectiva
	uint32_t i, j;uint8_t letra; uint8_t  color2;
	for (i = fila; i < fila + alto; i++) {
		for (j = columna; j < columna + ancho; j++) {
			if(i < VIDEO_FILS && j < VIDEO_COLS){
				//screen_actualizar_posicion_mapa(j-1,i);//pasar posiciOn efectiva de columna , fila
			    screen_actualizar_posicion_mapa(j,i-1);//pasar posiciOn efectiva de columna , fila

			    letra = screen_valor_actual(i, j);
			    if(letra == 0)//si no hay nada pisamos color anterior con color de jugador
					color2 = color;
				else
			        color2 = (screen_color_actual(i,j) << 4)>> 4;//obtenemos color caracter
			    screen_pintar(letra, color | color2, i, j);
			}
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
	screen_pintar_puntajes();
	 //slots
    int i;
    for (i = 1; i < 9; i++)
    {
        print_dec(i, 1, 2+2*i , 46, C_BW);
        screen_pintar('X', 0x04 , 48, 2+2*i);
        print_dec(i, 1, 57+2*i , 46, C_BW);
        screen_pintar('X', 0x01 , 48, 57+2*i);
    }

}

void screen_pintar_puntajes() {
	
	print_dec(jugadorA.puntos, 3, 35, 47, 0x4F);
    print_dec(jugadorB.puntos, 3, 42, 47, 0x1F);
	
	}

uint8_t screen_color_jugador(jugador_t *j) {
	 if (j == NULL)
        return C_FG_LIGHT_GREY;

    if (j->index == INDICE_JUGADOR_A)
        return C_FG_GREEN;
    else
        return C_FG_BLUE ;
	}
	
void screen_pintar_pirata(jugador_t *j, pirata_t *pirata) {
	uint8_t c     = screen_caracter_pirata(pirata->tipo);
    uint8_t color = C_MAKE_BG(screen_color_jugador(pirata->jugador)) | C_FG_WHITE;
    uint8_t color2 = C_MAKE_BG(screen_color_jugador(pirata->jugador));
    screen_entorno_pirata(color2,pirata->y, pirata->x-1,3,3);//y es fila - x es columna - posiciOn no efectiva  

  	screen_pintar(c, color, pirata->y+1, pirata->x);//y+1 porque en pantalla el mapa estA corrido +1 en y


	}
	
	
uint8_t screen_caracter_pirata(uint32_t tipo) {
	if (tipo == TIPO_EXPLORADOR) return 'E';
    if (tipo == TIPO_MINERO) return 'M';
    while(1){};
    return '?';
	}

 

void screen_actualizar_posicion_mapa(uint32_t x, uint32_t y) {
	//color background celda - recibe posiciOn efectiva - x es columna - y es fila
	uint8_t bg = screen_color_actual(y+1,x) >> 4;//recibe fila ,columna no efectiva

    uint8_t letra;
    uint32_t valor = game_valor_tesoro(x,y);//recibe posiciOn efectiva
    pirata_t *pirata = game_pirata_en_posicion(x, y);//recibe posiciOn efectiva
    if (pirata != NULL) {
		if(!pirata->libre)//si pirata activo entonces conservamos su caracter
        letra = screen_caracter_pirata(pirata->tipo);
    }  else if (valor > 0) {//si hay tesoro lo mostramos
        letra = screen_caracter_tesoro(valor);
    } else if ((jugadorA.x_puerto == x && jugadorA.y_puerto == y) || (jugadorB.x_puerto == x && jugadorB.y_puerto == y))
    {//si es puerto lo indicamos con 'x'
        letra = 'x';
    }
    
    
    else
    {
        letra = screen_valor_actual(y+1, x);//recibe fila, columna no efectiva
    }

    screen_pintar(letra, C_MAKE_BG(bg) | C_FG_BLACK, y+1, x);
	}
	
uint8_t screen_caracter_tesoro(uint32_t valor) {
    if (valor > 100)
        return '*';
    else
        return '$';
}
void screen_borrar_pirata(jugador_t *j, pirata_t *pirata) {
	uint8_t bg = screen_color_actual( pirata->y+1,pirata->x);
	screen_pintar('.', bg | C_FG_BLACK, pirata->y+1, pirata->x);
    screen_actualizar_posicion_mapa(pirata->x, pirata->y);//posiciOn efectiva
	}
	
void screen_actualizar_reloj_pirata (jugador_t *j, pirata_t *pirata) {
	pirata->reloj = (pirata->reloj+ 1) % reloj_size;

    if(j->index == INDICE_JUGADOR_A)
    {
        screen_pintar(reloj[pirata->reloj], C_BW, 48, 4+2*pirata->index);
    }
    else
    {
        screen_pintar(reloj[pirata->reloj], C_BW, 48, 59+2*pirata->index);
    }
}

/*
void screen_pintar_reloj_pirata(jugador_t *j, pirata_t *pirata) {}
void screen_pintar_reloj_piratas(jugador_t *j) {}
void screen_pintar_relojes() {}
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

if(i == 32)
	print("Interrupcion 32 ocurrida: Clock.", 0, 3, C_FG_LIGHT_GREY);
}


void imprimir_nombre_de_grupo(){
    print("El crew de Ariane 5", 61, 0, C_BG_MAGENTA+C_FG_LIGHT_GREEN);
}


void screen_pantalla_debug()
{
  uint32_t x = 25, y = 7;

  screen_pintar_rect(0, 0x00, y, x, 36, 32);
  screen_pintar_rect(0, 0x77, y+1, x+1, 34, 30);
  screen_pintar_rect(0, 0x44, y+1, x+1, 1, 30);


  print("eax", x+2, y+3 , 0x70); print_hex(debugger.eax, 8, x+6, y+3 , 0x7f);
  print("ebx", x+2, y+5 , 0x70); print_hex(debugger.ebx, 8, x+6, y+5 , 0x7f);
  print("ecx", x+2, y+7 , 0x70); print_hex(debugger.ecx, 8, x+6, y+7 , 0x7f);
  print("edx", x+2, y+9 , 0x70); print_hex(debugger.edx, 8, x+6, y+9 , 0x7f);
  print("esi", x+2, y+11, 0x70); print_hex(debugger.esi, 8, x+6, y+11, 0x7f);
  print("edi", x+2, y+13, 0x70); print_hex(debugger.edi, 8, x+6, y+13, 0x7f);
  print("ebp", x+2, y+15, 0x70); print_hex(debugger.ebp, 8, x+6, y+15, 0x7f);
  print("esp", x+2, y+17, 0x70); print_hex(debugger.esp, 8, x+6, y+17, 0x7f);
  print("eip", x+2, y+19, 0x70); print_hex(debugger.eip, 8, x+6, y+19, 0x7f);


  print("cs", x+3, y+21, 0x70); print_hex(debugger.cs, 4, x+6, y+21, 0x7f);
  print("ds", x+3, y+23, 0x70); print_hex(debugger.ds, 4, x+6, y+23, 0x7f);
  print("es", x+3, y+25, 0x70); print_hex(debugger.es, 4, x+6, y+25, 0x7f);
  print("fs", x+3, y+27, 0x70); print_hex(debugger.fs, 4, x+6, y+27, 0x7f);
  print("gs", x+3, y+29, 0x70); print_hex(debugger.gs, 4, x+6, y+29, 0x7f);
  print("ss", x+3, y+31, 0x70); print_hex(debugger.ss, 4, x+6, y+31, 0x7f);
  
  print("eflags", x+3, y+33, 0x70); print_hex(debugger.eflags, 8, x+9, y+33, 0x7f);
  
  print("cr0", x+16, y+3, 0x70); print_hex(debugger.cr0, 8, x+20, y+3, 0x7f);
  print("cr2", x+16, y+5, 0x70); print_hex(debugger.cr2, 8, x+20, y+5, 0x7f);
  print("cr3", x+16, y+7, 0x70); print_hex(debugger.cr3, 8, x+20, y+7, 0x7f);
  print("cr4", x+16, y+9, 0x70); print_hex(debugger.cr4, 8, x+20, y+9, 0x7f);

  print("stack", x+16, y+20, 0x70); 
  print_hex(debugger.stack0, 8, x+16, y+22, 0x7f);
  print_hex(debugger.stack1, 8, x+16, y+23, 0x7f);
  print_hex(debugger.stack2, 8, x+16, y+24, 0x7f);
  print_hex(debugger.stack3, 8, x+16, y+25, 0x7f);
  print_hex(debugger.stack4, 8, x+16, y+26, 0x7f);

}
void respaldo_pantalla_original()
{
  uint32_t i, j;
  for(i = 0; i<VIDEO_FILS; i++){
    for(j = 0; j<VIDEO_COLS; j++){
      cartel[i][j] = p[i][j];
    }
  }
}

void restaurar_pantalla_original()
{
  uint32_t i, j;
  for(i = 0; i<VIDEO_FILS; i++){
    for(j = 0; j<VIDEO_COLS; j++){
      p[i][j] = cartel[i][j];
    }
  }
}

void mostrar_info_debugger(){
	 respaldo_pantalla_original();
  screen_pantalla_debug();
  while(modo_debug_activado)
  {
    ;
  }
  restaurar_pantalla_original();
  modo_debug_activado = 1;
	
	}

