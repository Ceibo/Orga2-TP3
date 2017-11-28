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

pirata_t* id_pirata2pirata(uint32_t  id)//completado por alumno
{
    
    for(int i = 0;i < MAX_CANT_PIRATAS_VIVOS; i++){//iteramos sobre los piratas de ambos jugadores
		if(jugadorA.piratas[i].id == id){
			return &(jugadorA.piratas[i]);
		}
		if(jugadorB.piratas[i].id == id){
			return &(jugadorB.piratas[i]);
		}
	}
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

// dada una posicion (x,y) guarda las posiciones de alrededor en dos arreglos, 
//uno para las x y otro para las y
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


void game_inicializar()//*************** agregado ************
{
	game_jugador_inicializar(&jugadorA);
	game_jugador_inicializar(&jugadorB);

    screen_pintar_puntajes();
}

void game_jugador_inicializar_mapa(jugador_t *jug)
{
if(jug == 0)
	return;
	
	 // Para que todos los piratas de cada jugador compartan el mismo mapa
  // y puedan así visualizar las áreas exploradas por los otros automáticamente
  //una tabla por cada Indice requerido para mapear todas las posiciones de mapa (Indice 2,3,4,5)
if(jug->index == INDICE_JUGADOR_A){
  tabla_de_paginas_del_mapa_del_jugador_A_2 = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
  tabla_de_paginas_del_mapa_del_jugador_A_3 = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
  tabla_de_paginas_del_mapa_del_jugador_A_4 = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
  tabla_de_paginas_del_mapa_del_jugador_A_5 = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
} else {
  tabla_de_paginas_del_mapa_del_jugador_B_2 = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
  tabla_de_paginas_del_mapa_del_jugador_B_3 = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
  tabla_de_paginas_del_mapa_del_jugador_B_4 = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
  tabla_de_paginas_del_mapa_del_jugador_B_5 = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
}
  page_table_entry entrada = crear_entrada_nula_de_tabla_de_paginas();
  //inicializamos las entradas de las tablas con valores nulos
if(jug->index == INDICE_JUGADOR_A){
  llenar_tabla_de_paginas(tabla_de_paginas_del_mapa_del_jugador_A_2, entrada, FALSE);
  llenar_tabla_de_paginas(tabla_de_paginas_del_mapa_del_jugador_A_3, entrada, FALSE);
  llenar_tabla_de_paginas(tabla_de_paginas_del_mapa_del_jugador_A_4, entrada, FALSE);
  llenar_tabla_de_paginas(tabla_de_paginas_del_mapa_del_jugador_A_5, entrada, FALSE);
}else{
  llenar_tabla_de_paginas(tabla_de_paginas_del_mapa_del_jugador_B_2, entrada, FALSE);
  llenar_tabla_de_paginas(tabla_de_paginas_del_mapa_del_jugador_B_3, entrada, FALSE);
  llenar_tabla_de_paginas(tabla_de_paginas_del_mapa_del_jugador_B_4, entrada, FALSE);
  llenar_tabla_de_paginas(tabla_de_paginas_del_mapa_del_jugador_B_5, entrada, FALSE);
}
}

void game_jugador_inicializar(jugador_t *j)
{
	static int index = 0;

	j->index = index++;
    
    //**************** agregado *************//

	if (j->index == INDICE_JUGADOR_A) {
		j->x_puerto = POS_INIT_A_X;
		j->y_puerto = POS_INIT_A_Y;

	} else {
		j->x_puerto = POS_INIT_B_X;
		j->y_puerto = POS_INIT_B_Y;
	}

	j->puntos = 0;


	int i;
	for (i = 0; i < MAX_CANT_PIRATAS_VIVOS; i++)
	{
		uint32_t gdt_index = 0; // CAMBIAR POR ALGO VALIDO
		game_pirata_inicializar(&j->piratas[i], j, i, gdt_index + i*8);
	}

}
// realiza inicialización básica de un pirata. El pirata aun no está vivo ni por lanzarse. Setea jugador, indice, etc
void game_pirata_inicializar(pirata_t *pirata, jugador_t *j, uint32_t  index, uint32_t  id)
{
	//***************** agregado *************************//
	pirata->id   = id;
    pirata->index = index;
    pirata->jugador = j;
	pirata->libre = TRUE;

}

void game_tick(uint32_t  id_pirata) {
  screen_actualizar_reloj_global();
  // Falta actualizar el reloj del pirata
}

// toma un pirata ya existente libre y lo recicla seteando x e y al puerto.
// luego inicializa su mapeo de memoria, tss, lo agrega al sched y pinta la pantalla

void game_pirata_relanzar(pirata_t *pirata, jugador_t *j, uint32_t  tipo)
{//******** hecho por alumno ***************
	

	pirata->x = j->x_puerto;
	pirata->y = j->y_puerto;
	pirata->tipo = tipo;
	pirata->libre = FALSE;

	// ahora debo llamar a rutinas que inicialicen un nuevo mapa de
	// memoria para el nuevo pirata, que carguen su tss correspondiente,
	// lo scheduleen y finalmente lo pinten en pantalla
	uint32_t dest = desplazamiento_para_calcular_la_direccion_de_la_pagina_en_el_mapa(pirata->x,pirata->y);
    //inicializamos tss de pirata
    tss_libre(pirata->index,j->index, dest, pirata->id);
    //inicializamos descriptor en gdt de tss (temporario)
	inic_descriptor_tss(15);
	//pintamos
	screen_pintar_pirata(j, pirata);

	//listo para usar (temporario)
}

pirata_t* game_jugador_erigir_pirata(jugador_t *j, uint32_t  tipo)
{
    // ~ completar ~

	return NULL;
}

//*********** agregado ************
// debe devolver el proximo pirata del arreglo que no esté siendo usado actualmente
pirata_t* game_jugador_dame_pirata_libre(jugador_t *j)
{
	int i;

	for (i = 0; i < MAX_CANT_PIRATAS_VIVOS; i++)
	{
		if (j->piratas[i].libre == TRUE)
			return &j->piratas[i];
	}

	return NULL;
}

void game_jugador_lanzar_pirata(jugador_t *j, uint32_t  tipo, int x, int y)
{ //********** hecho por alumno *************//
	if (game_pirata_en_posicion(x,y) != NULL)
		return;

	pirata_t *pirata = game_jugador_dame_pirata_libre(j);
	if (pirata == NULL)
		return;

	game_pirata_relanzar(pirata,j, tipo);
}

void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y)
{
}


void game_explorar_posicion(jugador_t *jugador, int c, int f)
{
}


uint32_t  game_syscall_pirata_mover(uint32_t  id, direccion dir)
{
	//si es posible copiar cOdigo de tarea a nueva posiciOn,si es explorador
	// mapear nueva posiciOn y alrededores. si hay botIn en nueva posiciOn crear minero
	//pasando posiciOn a tarea minero. si no hay lugar para nueva tarea setear indicador de
	//que hay minero esperando para avisar a scheduler.
	//chequear que no se salga del mapa y si es minero no mapear nada.
    //con el id ubico al pirata en estructura de jugador
    
    pirata_t * morgan = id_pirata2pirata(id); 
    jugador_t * j = morgan->jugador;
	int x,y;
	//calculamos incrementos de coordenadas
	uint32_t res =  game_dir2xy(dir, &x, &y); 
	//si res == 0 matar tarea
	if(res == 0)
		return res;
		
	//actualizamos coordenadas pirata
	int nuevo_x = morgan->x + x;//x e y tienen offset en mapa 
	int nuevo_y = morgan->y + y;
	//chequeamos  que la posiciOn nueva estE en mapa
	if(game_posicion_valida(nuevo_x,nuevo_y)){
		    //game_calcular_posiciones_vistas(morgan->vistas_x,morgan->vistas_y,nuevo_x,nuevo_y);
			//mover y verificar si hay botin
			//mapeamos todas las posiciones alrededor
  			//debemos actualizar posiciOn en estructura pirata antes de invocar mmu_mover_pirata ***
	     
	     //obtenemos anterior posiciOn de pirata
	     uint32_t viejo_x = morgan->x;
	     uint32_t viejo_y = morgan->y;
	     //actualizamos posiciOn de pirata
	     morgan->x = (uint32_t ) nuevo_x;
	     morgan->y = (uint32_t ) nuevo_y;
	     //copiamos a nueva posiciOn, mapeamos alrededor.
	     mmu_mover_pirata(morgan, viejo_x, viejo_y);
		 //pintamos en pantalla al pirata
		 screen_pintar_pirata(j,morgan);
	     
	}else{
			//matar tarea
			res = 0;
	}
		 
    return res;//si res == 0 se debe matar tarea (argumentos de syscall invAlidos)
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

//dada un id de pirata devuelve la direcciOn del origen de cOdigo del pirata
uint32_t dir_code_x_tipo_pirata(uint32_t id){
	 uint32_t res;
	 if(id_pirata2pirata(id)->tipo == TIPO_EXPLORADOR && 
	 id_pirata2pirata(id)->jugador->index == INDICE_JUGADOR_A){
		 res = TAREA_A_E;
	 }
	 if(id_pirata2pirata(id)->tipo == TIPO_MINERO && 
	 id_pirata2pirata(id)->jugador->index == INDICE_JUGADOR_A){
		 res = TAREA_A_M;
	 }
	 if(id_pirata2pirata(id)->tipo == TIPO_EXPLORADOR && 
	 id_pirata2pirata(id)->jugador->index == INDICE_JUGADOR_B){
		 res = TAREA_B_E;
	 }
	 if(id_pirata2pirata(id)->tipo == TIPO_MINERO && 
	 id_pirata2pirata(id)->jugador->index == INDICE_JUGADOR_B){
		 res = TAREA_B_M;
	 }
	 return res;
}

