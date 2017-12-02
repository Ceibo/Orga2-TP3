/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "game.h"

#define MAX_CANT_TAREAS_VIVAS   (MAX_CANT_PIRATAS_VIVOS * 2)


/*
================================================================================
                       ~~~ estructuras del scheduler ~~~
================================================================================
*/

// para el scheduler, lo importante de cada tarea es su indice en la gdt y
// el pirata al cual está asociada. Si el indice es null significará que está libre.

typedef struct sched_task_t
{
    uint32_t gdt_index;
	pirata_t *pirata;//cada pirata tiene asociado un jugador
	bool reservado_minero ;//si es 1 entonces scheduler debe inicializarlo como minero
	//por medio de su id de pirata podemos averiguar a que jugador pertenece
	bool reservado_explorador;//si es 1 entonces scheduler debe inicializarlo como explorador
	uint32_t posiciOn_x_tesoro;
	uint32_t posiciOn_y_tesoro;
 } sched_task_t;

// el scheduler posee un arreglo de tareas (cada una puede estar libre o asignada)
typedef struct sched_t
{
    sched_task_t tasks[MAX_CANT_TAREAS_VIVAS+1];
    uint16_t current;//sirve para indizar en array tasks (con 17 sched_task_t )
    //cada vez que scheduler deba cambiar de tarea se chequea si hay slot en tasks libre
    //(lo recorre una sola vez desde Indice actual). 
    //si hay slot debe chequear en posiciones_tesoros asociado a jugador correspondiente a slot
    //tal que si encuentra tupla con valores distinto de 100 entonces en ese slot
    // debe inicializar minero guardando en su pila de nivel 3 los valores de array. 
    //luego debe sobreescribir en esas posiciones de array posiciones_tesoros con 100
    uint32_t posiciones_tesoros_A[50][2];//array de 50 tuplas inicializadas en 100.  
} sched_t;

extern sched_t scheduler;

/*
================================================================================
                       ~~~ funciones del scheduler ~~~
================================================================================
*/

// debe agregar una tarea al scheduler, buscando un slot libre y asociando indice de gdt y pirata 
void sched_agregar_tarea(pirata_t *pirata);

// debe remover tareas del scheduler
void sched_remover_tarea(uint32_t gdt_index);

// debe devolver el pirata correspondiente a la tarea que está corriendo actualmente
pirata_t* sched_tarea_actual();

//devuelve Indice de tarea libre en tasks de scheduler (0 - 16). 
//si retorna 1000 entonces no hay tarea libre
int sched_buscar_tarea_libre();

// debe avisar al juego que hubo un tick (para que haga cosas del juego) y luego configurarse
// para pasar a la siguiente tarea (devuelve su indice en la gdt)
uint16_t sched_atender_tick();

#endif	/* !__SCHED_H__ */
