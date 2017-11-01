/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"


tss tss_inicial;
tss tss_idle;

tss tss_jugadorA[MAX_CANT_PIRATAS_VIVOS];
tss tss_jugadorB[MAX_CANT_PIRATAS_VIVOS];

void tss_inicializar() {
//Paso 1: Inicializo tarea inicial (valga la redundancia)
// el dpl queda en 0, no?
gdt[GDT_TAREA_INICIAL].limit_0_15 = 0x67;
gdt[GDT_TAREA_INICIAL].base_23_16 = (&tss_inicial) >> 16;
gdt[GDT_TAREA_INICIAL].base_31_24 = (&tss_inicial) >> 24;
gdt[GDT_TAREA_INICIAL].type = 0x9;
gdt[GDT_TAREA_INICIAL].s = 0x0;
gdt[GDT_TAREA_INICIAL].dpl = 0x0;
gdt[GDT_TAREA_INICIAL].p = 0x1;
gdt[GDT_TAREA_INICIAL].limit_16_19 = 0x0;
gdt[GDT_TAREA_INICIAL].base_0_15 = (&tss_inicial);
//Creo que podemos comentar los que tienen que estar en cero, usamos el constructor nulo.
gdt[GDT_IDLE].limit_0_15 = 0x67;
gdt[GDT_IDLE].base_23_16 = (&tss_inicial) >> 16;
gdt[GDT_IDLE].base_31_24 = (&tss_inicial) >> 24;
gdt[GDT_IDLE].type = 0x9;
gdt[GDT_IDLE].s = 0x0;
gdt[GDT_IDLE].dpl = 0x0;
gdt[GDT_IDLE].p = 0x1;
gdt[GDT_IDLE].limit_16_19 = 0x0;
gdt[GDT_IDLE].base_0_15 = (&tss_inicial);

//Paso 2: Inicializo ilde
gdt[GDT_IDLE].limit_0_15 = 0x67;


//Paso 3: Inicializo el resto de las 16 tareas que tenia
//Eran 8 por cada pirata, creo que eran "equivalentes"	
}


