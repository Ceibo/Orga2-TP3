/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "screen.h"

sched_t scheduler;

void sched_inicializar()
{
}


int sched_buscar_indice_tarea(uint32_t gdt_index) {
    return MAX_CANT_TAREAS_VIVAS;
}


int sched_buscar_tarea_libre()
{
	int i = 0;

    return i;
}



pirata_t* sched_tarea_actual()
{
    return scheduler.tasks[scheduler.current].pirata;
}

void sched_agregar_tarea(pirata_t *pirata)
{
}

void sched_remover_tarea(uint32_t gdt_index)
{
}


uint32_t sched_proxima_a_ejecutar()
{
    return MAX_CANT_TAREAS_VIVAS;
}


uint16_t sched_atender_tick()
{
    return scheduler.tasks[scheduler.current].gdt_index;
}



