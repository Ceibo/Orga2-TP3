
/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

#define IDT_ENTRY(numero,dpl)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x40;                                                                  \
    idt[numero].attr = (unsigned short) 0x8E00 | (((unsigned short)(dpl & 0x3)) << 13)  ;                                                                      \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

//Mando el segmento de codigo al kernel, que era el subindice 8, que es 0x40 por que está shifteado 3 bits.
// 1000 1110 0000 0000(o cualquier cosa en estos ultimos) Pasar esto a hexa para el atributo.
// 0x8E00
 
void idt_inicializar() {
    IDT_ENTRY(0,0);
    IDT_ENTRY(1,0);
    IDT_ENTRY(2,0);
    IDT_ENTRY(3,0);
    IDT_ENTRY(4,0);
    IDT_ENTRY(5,0);
    IDT_ENTRY(6,0);
    IDT_ENTRY(7,0);
    IDT_ENTRY(8,0);
    IDT_ENTRY(9,0);
    IDT_ENTRY(10,0);
    IDT_ENTRY(11,0);
    IDT_ENTRY(12,0);
    IDT_ENTRY(13,0);
    IDT_ENTRY(14,0);
    IDT_ENTRY(15,0);
    IDT_ENTRY(16,0);
    IDT_ENTRY(17,0);
    IDT_ENTRY(18,0);
    IDT_ENTRY(19,0);
    IDT_ENTRY(32,0); // gate reloj
    IDT_ENTRY(33,0); // gate teclado
    IDT_ENTRY(70,3); //gate syscall (nivel 3 para poder ser accedido por tareas *chequeado*)
}
