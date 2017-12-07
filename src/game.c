/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
 
#include "screen.h"

#include <stdarg.h>
#include "sched.h"


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
void game_calcular_posiciones_vistas(int* vistas_x, int* vistas_y, int x, int y)
{
	int next = 0;
	int i, j;
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			//breakpoint();
			vistas_x[next] = x + j;
			vistas_y[next] = y + i;
			next++;
		}
	}
}


void game_inicializar()
{
	 //breakpoint(); 
	game_jugador_inicializar(&jugadorA);
	game_jugador_inicializar(&jugadorB);

    screen_pintar_puntajes();
    
    
    //inicializamos scheduler
	sched_inicializar();
	
	//modo debug
	modo_debug = 0;
    pantalla_debug_activada = 0;
	 
}

void game_jugador_inicializar_mapa(jugador_t *jug)
{
if(jug == NULL)
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
	//breakpoint();
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

	j->index = index;
	index++;
	
    
    //**************** agregado *************//

	if (j->index == INDICE_JUGADOR_A) {
		j->x_puerto = POS_INIT_A_X;
		j->y_puerto = POS_INIT_A_Y;

	} else {
		j->x_puerto = POS_INIT_B_X;
		j->y_puerto = POS_INIT_B_Y;
	}

	j->puntos = 0;

    uint16_t gdt_index;
    if(j->index == INDICE_JUGADOR_A)//asociamos a cada pirata con el Indice en gdt de su tss descriptor
		    gdt_index = 15;  //primero van los 8 de jugadorA (Indices 15 a 22)
	else{
		
			gdt_index = 23;//los de jugadorB (Indices 23 a 30)
		}
	uint32_t i;
	for (i = 0; i < MAX_CANT_PIRATAS_VIVOS; i++)
	{
		 
		game_pirata_inicializar(&j->piratas[i], j, i, gdt_index + i);
	}
	
	//inicializamos mapa
	game_jugador_inicializar_mapa(j);
	

}
//inicializa pirata libre. El pirata aun no está vivo ni por lanzarse. Setea jugador, indice, tss_desc
void game_pirata_inicializar(pirata_t *pirata, jugador_t *j, uint32_t  index, uint16_t  id)
{
	//***************** agregado *************************//
	pirata->id   = id;
    pirata->index = index;
    pirata->jugador = j;
	pirata->libre = 1;
	pirata->reloj = 0;
	//inicializamos descriptor en gdt de tss  
    static int gdt_A = 15;
    static int gdt_B = 23;
    if(j->index == INDICE_JUGADOR_A){
       inic_descriptor_tss(gdt_A);
       gdt_A++;
    }else{
	   
	   inic_descriptor_tss(gdt_B);
	   gdt_B++;
	}

}

void game_tick(pirata_t* pirata) {
  screen_actualizar_reloj_global();
  if(pirata != NULL && !pirata->libre){
	scheduler.tiempo_sin_cambios++;
	screen_actualizar_reloj_pirata(pirata->jugador,pirata);
  }
  if(scheduler.tiempo_sin_cambios == 10000){
	  print("JUEGO TERMINADO",25,0,0x0f0f);
	  while(1){}
  }
	
   //si se cumple un tiempo dado sin cambios en puntaje se debe terminar juego
}

// toma un pirata ya existente libre y lo recicla seteando x e y al puerto.
// luego inicializa su mapeo de memoria, tss, lo agrega al sched y pinta la pantalla
//sOlo se invoca desde scheduler en caso de reservas 

void game_pirata_relanzar(pirata_t *pirata, jugador_t *j, uint32_t  tipo)
{//******** hecho por alumno ***************
	

	pirata->x = j->x_puerto;
	pirata->y = j->y_puerto;
	pirata->tipo = tipo;
	pirata->libre = 0;

	// ahora debo llamar a rutinas que inicialicen un nuevo mapa de
	// memoria para el nuevo pirata, que carguen su tss correspondiente,
	// lo scheduleen y finalmente lo pinten en pantalla
     
 
 	//inicializamos tss
 	 
	uint32_t dirFisDestinoCod = dir_Fis_DestinoCod(pirata->x,pirata->y); 
	tss_libre(pirata->index,j->index,dirFisDestinoCod , pirata->id);
	
	//pintamos
	screen_pintar_pirata(j, pirata);

	 
}

//viene de game_atender_teclado - busca slot libre para reservar

pirata_t* game_jugador_erigir_pirata(jugador_t *j, uint32_t  tipo)
{   
     return NULL;
}

 
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

//viene de game_atender_teclado
//reserva pirata en puerto de jugador para que scheduler lo active
void game_jugador_lanzar_pirata(jugador_t *j, uint32_t  tipo, int x, int y)
{ 
	
	/************** version en que no se puede lanzar pirata si hay uno en posicion *****

	//si hay un pirata perteneciente a jugador j en posiciOn no lanzamos 
	if (game_pirata_en_posicion(x,y) != NULL)
		return;
     **********************************************************************************/
	//si no hay slot en piratas de jugador no lanzamos
	pirata_t *pirata = game_jugador_dame_pirata_libre(j);
	if (pirata == NULL)
		return;
    //breakpoint();
	//si hay lugar seteamos valores de pirata sin inicializar tss (se hace al switchear)

	uint16_t selector = ((pirata->id)<<3 )| 0x0;
	uint16_t index_tasks = buscar_tarea(selector);//buscamos su Indice en tasks de scheduler
	//seteamos como reservado
	scheduler.tasks[index_tasks].reservado_explorador = 1;
	//seteamos como slot ocupado
	pirata->libre = 0;
	//seteamos posiciones iniciales
	pirata->x = j->x_puerto;
	pirata->y = j->y_puerto;
	//seteamos tipo
	pirata->tipo = TIPO_EXPLORADOR;
	//actualizamos prox de scheduler si es necesario
	//breakpoint();
	sched_agregar_tarea(index_tasks);
	
}

void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y)
{
}


void game_explorar_posicion(jugador_t *jugador, int x, int y)
{
		 
}


uint32_t  game_syscall_pirata_mover(uint32_t  id, direccion dir)
{
	 
    
    pirata_t * morgan = id_pirata2pirata(id);
    if (!morgan) error();
    
    jugador_t * j = morgan->jugador;
	int x,y;
	
	//calculamos incrementos de coordenadas
	uint32_t res =  game_dir2xy(dir, &x, &y);  
	
	//si res == -1  (argumento invAlido) matar tarea
	if(res == -1){
 		error();
		return res;
	 }
	//obtenemos nuevas coordenadas pirata
	int nuevo_x = morgan->x + x;//x e y tienen offset en mapa 
	int nuevo_y = morgan->y + y;
	
	//chequeamos  que la posiciOn nueva estE en mapa
	if(game_posicion_valida(nuevo_x,nuevo_y)){
		 
		game_calcular_pos_nuevas(j->vistas_x,j->vistas_y,nuevo_x,nuevo_y,x,y);
			 
	     //modificamos pantalla en anterior posiciOn	
         screen_borrar_pirata(j, morgan);
	 
	     //obtenemos anterior posiciOn de pirata:
	     uint32_t viejo_x = morgan->x;
	     uint32_t viejo_y = morgan->y;
	     
	     //actualizamos posiciOn de pirata:
	     morgan->x = (uint32_t ) nuevo_x;
	     morgan->y = (uint32_t ) nuevo_y; 
	     
 		 //copiamos cOdigo a nueva posiciOn, mapeamos alrededor si es explorador:
	     mmu_mover_pirata(morgan, viejo_x, viejo_y);
	     
		//pintamos en pantalla al pirata:
		screen_pintar_pirata(j,morgan);
		
	     //si pirata es explorador chequear si hay tesoros en nueva posiciOn
		 
		 if(morgan->tipo == TIPO_EXPLORADOR){
			 int k;
			 for(k = 0; k < 9; k++){
				  
				if(game_valor_tesoro(j->vistas_x[k],j->vistas_y[k])){
				 //breakpoint();
				     
				    pirata_t* nuevo = game_jugador_dame_pirata_libre(j);
				    
				    //libre es Indice en tasks de scheduler. si hay libre guardar allI posiciones
				     
					if(nuevo != NULL){	
						 
						uint16_t selector = (nuevo->id )<<3 | 0x0;;
						uint16_t libre = buscar_tarea(selector);
 						scheduler.tasks[libre].posiciOn_x_tesoro = j->vistas_x[k];
						scheduler.tasks[libre].posiciOn_y_tesoro = j->vistas_y[k];
						scheduler.tasks[libre].reservado_minero = 1; //reservamos slot
						uint16_t selector2 = scheduler.tasks[libre].gdt_index;
	                    uint16_t index_tasks = buscar_tarea(selector2);//buscamos su Indice en tasks de scheduler
	
						sched_agregar_tarea(index_tasks);
					}else{
						 
					//si no hay libre guardar posiciones en array posiciones de scheduler (si es posible)
					    sched_guardar_prox_pos_en_buffer_tesoro(j->index,j->vistas_x[k],j->vistas_y[k]);	
					}
					break;
				}
			}
		 } 
 	     
	}else{
			//posiciOn invAlida - matar tarea
			error();
			res = -1;
	}
	 
    return res; 
}

uint32_t  game_syscall_cavar(uint32_t  id)
{
	uint32_t res = -1;
	
	pirata_t * pir = id_pirata2pirata(id);
    if (!pir) error();
    if (pir->tipo != TIPO_MINERO) error();

	 
    uint32_t x = pir->x; 
    uint32_t y = pir->y; 

    uint32_t i;
    for (i = 0; i < BOTINES_CANTIDAD; i++)
    {
        if (x == botines[i][0] && y == botines[i][1])
        {
            if (botines[i][2] > 0) 
            {
                (pir->jugador)->puntos++;
                scheduler.tiempo_sin_cambios = 0;
                screen_pintar_puntajes();
                return --botines[i][2];
            }
            else res = -1;
        }
    }

    if (res == -1)
		error();

	return 0;
}

uint32_t  game_syscall_pirata_posicion(uint32_t  id, int idx)
{//retorna  :  000.00yyyyyyyxxxxxxb == 8 bits menos significativos son x, los siguientes son y
    if (idx < -1 || idx > 7) {
		error();
		return 0;
	}
	
	pirata_t * pir = id_pirata2pirata(id);
    if (!pir){ 
		error();
		return 0;
		}
    
    if(idx == -1){
		 uint32_t pos_y = id_pirata2pirata(id)->y;
		 return ((pos_y << 8) | id_pirata2pirata(id)->x);
	}else{
		//jugador_t * dueño = id_pirata2pirata(id)->jugador;
		uint32_t pos_y = id_pirata2pirata(id)->jugador->piratas[idx].y;
		return ((pos_y << 8) | id_pirata2pirata(id)->jugador->piratas[idx].x);
	}
	
}

uint32_t  game_syscall_manejar(uint32_t  syscall, uint32_t  param1)
{
    // ~ completar ~
    return 0;
}

void game_pirata_exploto(uint32_t  id)
{
	pirata_t* morgan = id_pirata2pirata(id);
	//permitimos que slot de pirata estE disponible
	 
		morgan->libre = 1;
		//borramos pirata de pantalla
		screen_borrar_pirata(morgan->jugador,morgan);
		//actualizamos prox si es necesario
		uint16_t selector = ((morgan->id)<<3 )| 0x0;
		sched_remover_tarea(buscar_tarea(selector));
	
		if(morgan->jugador->index == INDICE_JUGADOR_A) screen_pintar('X', 0x04, 48, 4+2*morgan->index);
		else screen_pintar('X', 0x01, 48, 59+2*morgan->index);
     
     if(modo_debug)
    {
        pantalla_debug_activada = 1;
        mostrar_info_debugger(); 
        pantalla_debug_activada = 0;
    }
    
     
}

pirata_t* game_pirata_en_posicion(uint32_t  x, uint32_t  y)
{
	int i;
	for (i = 0; i < MAX_CANT_PIRATAS_VIVOS; i++)
	{
		if (!jugadorA.piratas[i].libre && jugadorA.piratas[i].x == x && jugadorA.piratas[i].y == y)
			return &jugadorA.piratas[i];
		if (!jugadorB.piratas[i].libre && jugadorB.piratas[i].x == x && jugadorB.piratas[i].y == y)
			return &jugadorB.piratas[i];
	}
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
#define bc_shiftL   0xaa // break tecla shiftl
#define bc_shiftR   0xb6 // break tecla shiftr


void game_atender_teclado(uint8_t tecla)//(5)
{
	switch (tecla)
	{
		 
		case KB_shiftL:  game_jugador_lanzar_pirata(&jugadorA,TIPO_EXPLORADOR,jugadorA.x_puerto,jugadorA.y_puerto);break;

 		 
		case KB_shiftR:  game_jugador_lanzar_pirata(&jugadorB,TIPO_EXPLORADOR,jugadorB.x_puerto,jugadorB.y_puerto);break; 
		 
        case KB_y  :
    
         
        if(modo_debug)
			modo_debug = 0;
		else
		    modo_debug = 1;
		    
        if(modo_debug){
			print("MODO DEBUG ACTIVADO",25,0,0x0f0f);
		} else{
			screen_pintar_rect(0, 0x00, 0, 25, 1, 23);
		}
        
    

		default: break;
	}
}

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

//dada posiciOn actual devuelve direcciOn fIsica en mapa 
 uint32_t dir_Fis_DestinoCod(uint32_t x, uint32_t y){
	 uint32_t desp = desplazamiento_para_calcular_la_direccion_de_la_pagina_en_el_mapa(x,y);
	 uint32_t res = MAPA_BASE_FISICA + desp;
	 return res;
 }
 
uint16_t game_id_pirata_actual(){
	pirata_t * morgan = sched_tarea_actual();
	if(morgan != NULL)
		return morgan->id;
	else
		return 0;
}

void game_calcular_pos_nuevas(int32_t * vistas_x,int32_t * vistas_y,int32_t nuevo_x, int32_t nuevo_y,int32_t x,int32_t y){
	if(x == 1 || x == -1){
			vistas_x[0] = nuevo_x + x;
			vistas_y[0] = nuevo_y;
			vistas_x[1] = nuevo_x + x;
			vistas_y[1] = nuevo_y+1;
			vistas_x[2] = nuevo_x + x;
			vistas_y[2] = nuevo_y-1;
			int k;
			for(k = 3; k < 9;k++){
				vistas_x[k] = 500;
			    vistas_y[k] = 500;
			}
		
	}else if(y == -1 || y == 1){
			vistas_x[0] = nuevo_x;
			vistas_y[0] = nuevo_y + y;
			vistas_x[1] = nuevo_x + 1;
			vistas_y[1] = nuevo_y+ y;
			vistas_x[2] = nuevo_x -1;
			vistas_y[2] = nuevo_y+y;
			int k;
			for(k = 3; k < 9;k++){
				vistas_x[k] = 500;
			    vistas_y[k] = 500;
			}
		
		}
	}
	
uint32_t debugger_activo()
{
  if(pantalla_debug_activada) return 1;
  else return 0;
}

