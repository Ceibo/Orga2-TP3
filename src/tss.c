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
//Mejor no la inicializo, no hace falta. Lo unico que necesita es algo para guardar, si guarda basura no 
//hay problema, total nunca va a llamar a esa tarea más tarde.

//Paso 2: Inicializo idle
gdt[GDT_IDLE].limit_0_15 = 0x67;
gdt[GDT_IDLE].base_23_16 = (&tss_inicial) >> 16;
gdt[GDT_IDLE].base_31_24 = (&tss_inicial) >> 24;
gdt[GDT_IDLE].type = 0x9;
gdt[GDT_IDLE].s = 0x0;
gdt[GDT_IDLE].dpl = 0x0;
gdt[GDT_IDLE].p = 0x1;
gdt[GDT_IDLE].limit_16_19 = 0x0;
gdt[GDT_IDLE].base_0_15 = (&tss_inicial);


//Paso 3: Inicializo el resto de las 16 tareas que tenia
//Eran 8 por cada pirata, creo que eran "equivalentes"	
}


void tss_inicializar_idle(){
    tss_idle.ptl = 0x0;
    tss_idle.unused0 = 0x0;
    tss_idle.esp0 = 0x0;
    tss_idle.ss0 = 0x0;
    tss_idle.unused1 = 0x0;
    tss_idle.esp1 = 0x0;
    tss_idle.ss1 = 0x0;
    tss_idle.unused2 = 0x0;
    tss_idle.esp2 = 0x0;
    tss_idle.ss2 = 0x0;
    tss_idle.unused3 = 0x0;
    tss_idle.cr3 = 0x0;
	tss_idle.eip = 0x00016000;//Como dice en el dibujito de la consigna
    tss_idle.eflags = 0x00000202;// EFLAGS con interrupciones habilitadas
    tss_idle.eax = 0x0;
    tss_idle.ecx = 0x0;
    tss_idle.edx = 0x0;
    tss_idle.ebx = 0x0;
    tss_idle.esp = 0x0;
    tss_idle.ebp = 0x0;
    tss_idle.esi = 0x0;
    tss_idle.edi = 0x0;
    tss_idle.es = 0x0;
    tss_idle.unused4 = ;
    tss_idle.cs = 0x0;
    tss_idle.unused5 = ;
    tss_idle.ds = 0x0; 
    tss_idle.unused6 = ;
    tss_idle.gs = 0x0;
    tss_idle.unused7 = ;
    tss_idle.ss = 0x0; 
    tss_idle.unused8 = ;
    tss_idle.fs = 0x0;
    tss_idle.iomap = 0xFFFF;//Los ultimos bits, como vemos en la clade de tareas
    tss_idle.unused9 = 0x0;
    tss_idle.ldt = 0x0;
    tss_idle.unused10 = 0x0;
    tss_idle.dtrap = 0x0;
}