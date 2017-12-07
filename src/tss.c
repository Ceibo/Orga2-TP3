/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"
 
//macro para definir descriptor de tss en gdt  
#define GDT_ENTRY(gdt_ind,estr_tss,nivel)                                                                  \
    gdt_ind.limit_0_15 = 0x67;                                                                        \
	gdt_ind.base_0_15 = (unsigned short) ((unsigned int)(&estr_tss) & (unsigned int) 0xFFFF);         \
	gdt_ind.base_23_16 = (unsigned short) ((unsigned int)(&estr_tss) >> 16 & (unsigned int) 0xFFFF);  \
	gdt_ind.type = 0x9;                                                                               \
	gdt_ind.s = 0x0;                                                                                  \
	gdt_ind.dpl = nivel;                                                                              \
	gdt_ind.p = 0x1;                                                                                  \
	gdt_ind.limit_16_19 = 0x0;                                                                        \
	gdt_ind.avl = 0x1;                                                                                \
	gdt_ind.l = 0x0 ;                                                                                 \
	gdt_ind.db = 0x0;                                                                                 \
	gdt_ind.g = 0x0;                                                                                  \
	gdt_ind.base_31_24 = (unsigned short) ((unsigned int)(&estr_tss) >> 24 & (unsigned int) 0xFFFF);  

//macro para llenar campos de tss
#define TSS_ENTRY(str,reg_ctrl_3,eip_reg,esp_seg,ebp_seg,es_seg,cs_seg,ss_seg,ds_seg,fs_seg,gs_seg,esp0_dir,ss0_seg)         \
    str.ptl = 0x0; \
    str.unused0 = 0x0;\
    str.esp0 = esp0_dir;\
    str.ss0 = ss0_seg;\
    str.unused1 = 0x0;\
    str.esp1 = 0x0;\
    str.ss1 = 0x0; \
    str.unused2 = 0x0;\
    str.esp2 = 0x0;\
    str.ss2 = 0x0;\
    str.unused3 = 0x0;\
    str.cr3 = reg_ctrl_3;\
    str.eip = eip_reg;\
    str.eflags = 0x00000202;\
    str.eax = 0x0;\
    str.ecx = 0x0;\
    str.edx = 0x0;\
    str.ebx = 0x0;\
    str.esp = esp_seg;\
    str.ebp = ebp_seg;\
    str.esi = 0x0;\
    str.edi = 0x0;\
    str.es = es_seg;\
    str.unused4 = 0x0;\
    str.cs = cs_seg;\
    str.unused5 = 0x0;\
    str.ss = ss_seg;\
    str.unused6 = 0x0;\
    str.ds = ds_seg;\
    str.unused7 = 0x0;\
    str.fs = fs_seg;\
    str.unused8 = 0x0;\
    str.gs = gs_seg;\
    str.unused9 = 0x0;\
    str.ldt = 0x0;\
    str.unused10 = 0x0;\
    str.dtrap = 0x0;\
    str.iomap = 0xffff;
    
tss tss_inicial;
tss tss_idle;

tss tss_jugadorA[MAX_CANT_PIRATAS_VIVOS];
tss tss_jugadorB[MAX_CANT_PIRATAS_VIVOS];

//inicializamos tss_idle
void tss_inicializar() {
//tss idle
    unsigned int aux = rcr3(); 
    mmu_mapear_pagina(0x16000,0x16000,(page_directory_entry* )aux,0x3);//tarea idle,1 pAgina - us = 1
    mmu_mapear_pagina(0x27000,0x27000,(page_directory_entry* )aux,0x1);//pila kernel- us = 0, rw = 1
    TSS_ENTRY(tss_idle,aux,0x16000,0x27000,0x27000,0x48,0x40,0x48,0x48,0x60,0x48,0x27000,0x48);    //mismos segmentos kernel     
 
};

//inicializamos descriptores
void inic_descriptor_tss(int i){
	 if(i == 13){
	   GDT_ENTRY(gdt[i], tss_inicial, 0x0);
	   return;void screen_pintar_pirata(jugador_t *j, pirata_t *pirata);

     }
     if(i == 14){
       GDT_ENTRY(gdt[i], tss_idle, 0x3);
       return;
     }
     /**/
     if(i >= 15 && i < 23){//descriptor de prueba para pirata
	   GDT_ENTRY(gdt[i], tss_jugadorA[i-15], 0x3);
       return;
	 }
	 
	 if(i >= 23 && i < 31){//descriptor de prueba para pirata
	   GDT_ENTRY(gdt[i], tss_jugadorB[i-23], 0x3);
       return;
	 }
	 /**/
     else 
	  return;
 }; 
 
 

//inicializamos tss pirata 
//argumentos: Indice en array de tss de jugador, jugador (ver game.h),   
// ubicaciOn en mapa, id de pirata
void tss_libre(int pirata, uint32_t jugador,   uint32_t dirFisDestinoCod, uint32_t id){
      
   //la selecciOn de tareas se hace en interrupciOn de teclado (lanzar tarea) de acuerdo 
   // a la tecla presionada. entonces se setea origen (tarea a-e, etc) y destino 
   //(jugador A = esquina arriba , jugador B = esquina abajo)
      
   //cr3: directorio pAginas pirata (pAgina nueva)
   //buscamos direcciOn de origen de cOdigo
   uint32_t origen = dir_code_x_tipo_pirata(id);
   
   unsigned int reg_ctrl_3 = (unsigned int)mmu_inicializar_dir_pirata(origen,dirFisDestinoCod,id);
   unsigned int pila_0 = (unsigned int ) mmu_direccion_fisica_de_la_proxima_pagina_libre();//nueva pAgina con identity mapping
   unsigned int  esp_0 = (unsigned int ) pila_0 + (PAGE_SIZE) ;//fin de pila 0
   //ss0 : selector de segmento de pila de kernel porque la pAgina pedida para la pila estA en 
   // el area libre y se mapea con identity mapping (ver enunciado en pAgina 4 antes de scheduler)
	 //eip: 0x400000, direcciOn virtual de cOdigo tarea
	 // ebp pila nivel 3: 0x401000, //direcciOn virtual base pAgina tarea 
	 // esp nivel 3:0x400ff4,//direcciOn virtual base pAgina tarea - 4*3 (argumentos y eip anterior)
	 // es,ss,ds,fs,gs : 0x005b (0000 0000 0101 1011)b , segmento datos nivel 3 en gdt
	 //cs : 0x0053 (0000 0000 0101 0011)b; segmento cOdigo nivel 3 en gdt
  if(jugador == INDICE_JUGADOR_A){
                   //TSS_ENTRY(str,reg_ctrl_3,eip_reg ,esp_seg ,ebp_seg ,es_seg,cs_seg,ss_seg,ds_seg,fs_seg,gs_seg,esp0_dir,ss0_seg)         
    TSS_ENTRY(tss_jugadorA[pirata],reg_ctrl_3,0x400000,0x400ff4,0x400ff4,0x005b,0x0053,0x005b,0x005b,0x005b,0x005b,esp_0,0x0048);    
 }else{
	 
    TSS_ENTRY(tss_jugadorB[pirata],reg_ctrl_3,0x400000,0x400ff4,0x400ff4,0x005b,0x0053,0x005b,0x005b,0x005b,0x005b,esp_0,0x0048);    
 }
};

