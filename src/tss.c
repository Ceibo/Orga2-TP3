/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"
 
//macro para definir descriptor de tss en gdt
#define GDT_ENTRY(indice,estr_tss,nivel)                                                                  \
    gdt[indice].limit_0_15 = 0x67;                                                                        \
	gdt[indice].base_0_15 = (unsigned short) ((unsigned int)(&estr_tss) & (unsigned int) 0xFFFF);         \
	gdt[indice].base_23_16 = (unsigned short) ((unsigned int)(&estr_tss) >> 16 & (unsigned int) 0xFFFF);  \
	gdt[indice].type = 0x9;                                                                               \
	gdt[indice].s = 0x0;                                                                                  \
	gdt[indice].dpl = nivel;                                                                              \
	gdt[indice].p = 0x1;                                                                                  \
	gdt[indice].limit_16_19 = 0x0;                                                                        \
	gdt[indice].avl = 0x1;                                                                                \
	gdt[indice].l = 0x0 ;                                                                                 \
	gdt[indice].db = 0x0;                                                                                 \
	gdt[indice].g = 0x0;                                                                                  \
	gdt[indice].base_31_24 = (unsigned short) ((unsigned int)(&estr_tss) >> 24 & (unsigned int) 0xFFFF);  

//macro para llenar campos de tss
#define TSS_ENTRY(estructura,registro_control_3,instruction_pointer,esp_seg,ebp_seg,es_seg,cs_seg,ss_seg,ds_seg,fs_seg,gs_seg)         \
    estructura.ptl = 0x0; \
    estructura.unused0 = 0x0;\
    estructura.esp0 = 0x0;\
    estructura.ss0 = 0x0;\
    estructura.unused1 = 0x0;\
    estructura.esp1 = 0x0;\
    estructura.ss1 = 0x0; \
    estructura.unused2 = 0x0;\
    estructura.esp2 = 0x0;\
    estructura.ss2 = 0x0;\
    estructura.unused3 = 0x0;\
    estructura.cr3 = registro_control_3;\
    estructura.eip = instruction_pointer;\
    estructura.eflags = 0x00000202;\
    estructura.eax = 0x0;\
    estructura.ecx = 0x0;\
    estructura.edx = 0x0;\
    estructura.ebx = 0x0;\
    estructura.esp = esp_seg;\
    estructura.ebp = ebp_seg;\
    estructura.esi = 0x0;\
    estructura.edi = 0x0;\
    estructura.es = es_seg;\
    estructura.unused4 = 0x0;\
    estructura.cs = cs_seg;\
    estructura.unused5 = 0x0;\
    estructura.ss = ss_seg;\
    estructura.unused6 = 0x0;\
    estructura.ds = ds_seg;\
    estructura.unused7 = 0x0;\
    estructura.fs = fs_seg;\
    estructura.unused8 = 0x0;\
    estructura.gs = gs_seg;\
    estructura.unused9 = 0x0;\
    estructura.ldt = 0x0;\
    estructura.unused10 = 0x0;\
    estructura.dtrap = 0x0;\
    estructura.iomap = 0xffff;
    
tss tss_inicial;
tss tss_idle;

tss tss_jugadorA[MAX_CANT_PIRATAS_VIVOS];
tss tss_jugadorB[MAX_CANT_PIRATAS_VIVOS];

//inicializamos tss
void tss_inicializar() {
//tss idle
    unsigned int aux = rcr3();
    TSS_ENTRY(tss_idle,aux,0x00016000,0x27000,0x27000,0x48,0x40,0x48,0x48,0x60,0x48);    //mismos segmentos kernel     
 
}

//inicializamos descriptores
void inic_descriptor_tss(int i){
	 if(i == 13){
	  GDT_ENTRY(GDT_TAREA_INICIAL, tss_inicial, 0);
	  return;
     }
     if(i == 14){
     GDT_ENTRY(GDT_IDLE, tss_idle, 0);
     }
     else
	  return;
 }; 
