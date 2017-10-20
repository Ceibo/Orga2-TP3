
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

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        idt_entry(0);
        ...
        idt_entry(19);

        ...
    }
*/

#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x40;                                                                  \
    idt[numero].attr = (unsigned short) 0x8E00;                                                                  \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

//Mando el segmento de codigo al kernel, que era el subindice 8, que es 0x40 por que está shifteado 3 bits.
// 1000 1110 0000 0000(o cualquier cosa en estos ultimos) Pasar esto a hexa para el atributo.
// 0x8E00

void print_isr(int i){
    print("Codigo de error " ,2,2,0x7);
    //IMPRIMIR EL ERROR Y VER COMO HACERLO

}

void idt_inicializar() {
    idt_entry(0);
    idt_entry(1);
    idt_entry(2);
    idt_entry(3);
    idt_entry(4);
    idt_entry(5);
    idt_entry(6);
    idt_entry(7);
    idt_entry(8);
    idt_entry(9);
    idt_entry(10);
    idt_entry(11);
    idt_entry(12);
    idt_entry(13);
    idt_entry(14);
    idt_entry(15);
    idt_entry(16);
    idt_entry(17);
    idt_entry(18);
    idt_entry(19);
}
