/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "game.h"
#include "mmu.h"

#define MAX_CANT_TAREAS_VIVAS   (MAX_CANT_PIRATAS_VIVOS * 2)
/* MAX_CANT_TAREAS_VIVAS = 16 */

/*
================================================================================
                       ~~~ estructuras del scheduler ~~~
================================================================================
*/

// para el scheduler, lo importante de cada tarea es su indice en la gdt y
// el pirata al cual está asociada. Si el indice es null significará que está libre.

typedef struct sched_task_t
{
    uint16_t gdt_index;//selector de segmento de tarea
    //bits 3 a 15 de selector de segmento dado por index de pirata
    //completar con 011b como primeros 3 bits para completar el selector y asignar a gdt_index
	pirata_t *pirata;//cada pirata tiene asociado un jugador
	bool reservado_minero ;//si es 1 entonces scheduler debe inicializarlo como minero
	//por medio de su id de pirata podemos averiguar a que jugador pertenece
	bool reservado_explorador;//si es 1 entonces scheduler debe inicializarlo como explorador
	uint32_t posiciOn_x_tesoro;//por default en 100
	uint32_t posiciOn_y_tesoro;
 } sched_task_t;

// el scheduler posee un arreglo de tareas (cada una puede estar libre o asignada)
typedef struct sched_t
{
    sched_task_t tasks[MAX_CANT_TAREAS_VIVAS+1];
    uint16_t current;//Indice 0 a 16. 
    //sirve para indizar en array tasks (con 17 sched_task_t )
    //cada vez que scheduler deba cambiar de tarea se chequea si hay slot en tasks libre
    //(lo recorre una sola vez desde Indice actual). 
    //si hay slot debe chequear en posiciones_tesoros asociado a jugador correspondiente a slot
    //tal que si encuentra tupla con valores distinto de 100 entonces en ese slot
    // debe inicializar minero guardando en su pila de nivel 3 los valores de array. 
    //luego debe sobreescribir en esas posiciones de array posiciones_tesoros con 100
    uint32_t posiciones_tesoros_A[20][2];//array de 20 tuplas inicializadas en 100. jugadorA 
    uint16_t inicio_tesoros_A;//el array es cola fifo y este Indice indica primer lugar ocupado
    uint32_t posiciones_tesoros_B[20][2];//posiciones de monedas descubiertas por jugadorB 
    uint16_t inicio_tesoros_B;//por default es 0
    uint16_t prox;//Indice de prOxima tarea a switchear (si es 500 no debe usarse para switchear)
    int32_t tiempo_sin_cambios;//
} sched_t;

extern sched_t scheduler;

/*
================================================================================
                       ~~~ funciones del scheduler ~~~
================================================================================
*/

// actualiza prox si es posible 
void sched_agregar_tarea(uint16_t index_task);

// actualiza prox si es posible 
void sched_remover_tarea(uint16_t index_task);

// debe devolver el pirata correspondiente a la tarea que está corriendo actualmente
pirata_t* sched_tarea_actual();

//devuelve primer Indice de tarea libre en tasks de scheduler (0 - 16) despuEs de actual
//para un jugador especIfico. si retorna 1000 entonces no hay tarea libre
//int sched_buscar_tarea_libre(uint32_t index_j);

void sched_inicializar();

// debe avisar al juego que hubo un tick (para que haga cosas del juego) y luego configurarse
// para pasar a la siguiente tarea (devuelve su indice en la gdt)
uint16_t sched_atender_tick();

//busca lugar disponible en buffer de posiciones a cavar para almacenar posiciones de tesoro 
//para un jugador especIfico y almacena las posiciones. si no hay lugar descarta las posiciones.
void sched_guardar_prox_pos_en_buffer_tesoro(uint32_t index_j,uint32_t x, uint32_t y);

//recorremos posiciones_tesoros buscando si hay posiciOn almacenada. si no hay devuelve 500
//actualiza el primero en salir de array al siguiente (si es posible)
uint32_t consultar_buffer_pos_tesoro(uint32_t index_j);
//devuelve Indice de tarea activa o reservada de jugador especIfico. si no hay tarea
//se devuelve 500
uint16_t buscar_sig_tarea(uint32_t index_j);

//dado un selector de segmento devuelve su Indice en tasks
uint16_t buscar_tarea(uint16_t selector);
bool sched_descubierto(uint32_t index_j,uint32_t x, uint32_t y);
#endif	/* !__SCHED_H__ */
