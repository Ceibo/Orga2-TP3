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
#define TSS_ENTRY(estructura,reg_ctrl_3,eip_reg,esp_seg,ebp_seg,es_seg,cs_seg,ss_seg,ds_seg,fs_seg,gs_seg,esp0_dir,ss0_seg)         \
    estructura.ptl = 0x0; \
    estructura.unused0 = 0x0;\
    estructura.esp0 = esp0_dir;\
    estructura.ss0 = ss0_seg;\
    estructura.unused1 = 0x0;\
    estructura.esp1 = 0x0;\
    estructura.ss1 = 0x0; \
    estructura.unused2 = 0x0;\
    estructura.esp2 = 0x0;\
    estructura.ss2 = 0x0;\
    estructura.unused3 = 0x0;\
    estructura.cr3 = reg_ctrl_3;\
    estructura.eip = eip_reg;\
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

//inicializamos tss_idle
void tss_inicializar() {
//tss idle
    unsigned int aux = rcr3(); 
    mmu_mapear_pagina(0x16000,0x16000,(page_directory_entry* )aux);//tarea idle, 1 pAgina
    mmu_mapear_pagina(0x27000,0x27000,(page_directory_entry* )aux);//pila kernel
    //mmu_mapear_pagina(0x26000,0x26000,(page_directory_entry* )aux);//pila kernel
    TSS_ENTRY(tss_idle,aux,0x00016000,0x00027000,0x00027000,0x0048,0x0040,0x0048,0x0048,0x0060,0x0048,0x0,0x0);    //mismos segmentos kernel     
    //TSS_ENTRY(tss_idle,aux,0x00016000,0x27000,0x27000,0x48,0x40,0x48,0x48,0x48,0x48);    //mismos segmentos kernel     

};

//inicializamos descriptores
void inic_descriptor_tss(int i){
	 if(i == 13){
	  GDT_ENTRY(GDT_TAREA_INICIAL, tss_inicial, 0x0);
	  return;
     }
     if(i == 14){
     GDT_ENTRY(GDT_IDLE, tss_idle, 0x0);
     }
     else
	  return;
 }; 
 
/* */

//inicializamos tss pirata 
//argumentos: Indice en array de tss de jugador, jugador (ver game.h), direcciOn fIsica origen tarea 
//y ubicaciOn en mapa.
void tss_libre(int pirata, uint32_t jugador, uint32_t origen, uint32_t dirFisDestinoCod){
      
     //cr3: directorio pAginas pirata (pAgina nueva)
	 unsigned int reg_ctrl_3 = (unsigned int)mmu_inicializar_dir_pirata(origen,dirFisDestinoCod,jugador);
	 void* pila_0 = mmu_direccion_fisica_de_la_proxima_pagina_libre();//nueva pAgina con identity mapping
	 unsigned int  esp_0 = (unsigned int ) pila_0 + (PAGE_SIZE -1) ;//fin de pila 0
	 //ss0 : selector de segmento de pila de kernel porque la pAgina pedida para la pila estA en 
	 // el area libre y se mapea con identity mapping (ver enunciado en pAgina 4 antes de scheduler)
	 //eip: 0x400000, direcciOn virtual de cOdigo tarea
	 // ebp pila nivel 3: 0x400fff, //direcciOn virtual base pAgina tarea 
	 // esp nivel 3:0x400ff3,//direcciOn virtual base pAgina tarea - 4*3 (argumentos y eip anterior)
	 // es,ss,ds,fs,gs : 0x005b (0000 0000 0101 1011)b , segmento datos nivel 3 en gdt
	 //cs : 0x0053 (0000 0000 0101 0011)b; segmento cOdigo nivel 3 en gdt
  if(jugador == INDICE_JUGADOR_A){
    TSS_ENTRY(tss_jugadorA[pirata],reg_ctrl_3,0x400000,0x400ff3,0x400fff,0x005b,0x0053,0x005b,0x005b,0x005b,0x005b,esp_0,0x0048);    
 }else{
    TSS_ENTRY(tss_jugadorB[pirata],reg_ctrl_3,0x400000,0x400ff3,0x400fff,0x005b,0x0053,0x005b,0x005b,0x005b,0x005b,esp_0,0x0048);    
 }
};

