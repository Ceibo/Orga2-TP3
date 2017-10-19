
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
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/

/*
#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x00;                                                                  \
    idt[numero].attr = (unsigned short) 0x0000;                                                                  \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);
*/




void idt_inicializar() {
    /*IDT_ENTRY(0);
    IDT_ENTRY(1);
    IDT_ENTRY(2);
    IDT_ENTRY(3);
    IDT_ENTRY(4);
    IDT_ENTRY(5);
    IDT_ENTRY(6);
    IDT_ENTRY(7);
    IDT_ENTRY(8);
    IDT_ENTRY(9);
    IDT_ENTRY(10);
    IDT_ENTRY(11);
    IDT_ENTRY(12);
    IDT_ENTRY(13);
    IDT_ENTRY(14);
    IDT_ENTRY(15);
    IDT_ENTRY(16);
    IDT_ENTRY(17);
    IDT_ENTRY(18);
    IDT_ENTRY(19);*/
}

/*#define IDT_ENTRY(0)
    idt[0].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 0) & (unsigned int) 0xFFFF);
    idt[0].segsel = (unsigned short) 0x00;
    idt[0].attr = (unsigned short) 0x0000;
    idt[0].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 0) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(1)
    idt[1].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 1) & (unsigned int) 0xFFFF);
    idt[1].segsel = (unsigned short) 0x00;
    idt[1].attr = (unsigned short) 0x0000;
    idt[1].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 1) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(2)
    idt[2].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 2) & (unsigned int) 0xFFFF);
    idt[2].segsel = (unsigned short) 0x00;
    idt[2].attr = (unsigned short) 0x0000;
    idt[2].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 2) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(3)
    idt[3].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 3) & (unsigned int) 0xFFFF);
    idt[3].segsel = (unsigned short) 0x00;
    idt[3].attr = (unsigned short) 0x0000;
    idt[3].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 3) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(4)
    idt[4].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 4) & (unsigned int) 0xFFFF);
    idt[4].segsel = (unsigned short) 0x00;
    idt[4].attr = (unsigned short) 0x0000;
    idt[4].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 4) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(5)
    idt[5].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 5) & (unsigned int) 0xFFFF);
    idt[5].segsel = (unsigned short) 0x00;
    idt[5].attr = (unsigned short) 0x0000;
    idt[5].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 5) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(6)
    idt[6].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 6) & (unsigned int) 0xFFFF);
    idt[6].segsel = (unsigned short) 0x00;
    idt[6].attr = (unsigned short) 0x0000;
    idt[6].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 6) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(7)
    idt[7].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 7) & (unsigned int) 0xFFFF);
    idt[7].segsel = (unsigned short) 0x00;
    idt[7].attr = (unsigned short) 0x0000;
    idt[7].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 7) >> 16 & (unsigned int) 0xFFFF);


#define IDT_ENTRY(8)
    idt[8].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 8) & (unsigned int) 0xFFFF);
    idt[8].segsel = (unsigned short) 0x00;
    idt[8].attr = (unsigned short) 0x0000;
    idt[8].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 8) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(9)
    idt[9].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 9) & (unsigned int) 0xFFFF);
    idt[9].segsel = (unsigned short) 0x00;
    idt[9].attr = (unsigned short) 0x0000;
    idt[9].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 9) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(10)
    idt[10].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 10) & (unsigned int) 0xFFFF);
    idt[10].segsel = (unsigned short) 0x00;
    idt[10].attr = (unsigned short) 0x0000;
    idt[10].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 10) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(11)
    idt[11].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 11) & (unsigned int) 0xFFFF);
    idt[11].segsel = (unsigned short) 0x00;
    idt[11].attr = (unsigned short) 0x0000;
    idt[11].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 11) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(12)
    idt[12].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 12) & (unsigned int) 0xFFFF);
    idt[12].segsel = (unsigned short) 0x00;
    idt[12].attr = (unsigned short) 0x0000;
    idt[12].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 12) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(13)
    idt[13].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 13) & (unsigned int) 0xFFFF);
    idt[13].segsel = (unsigned short) 0x00;
    idt[13].attr = (unsigned short) 0x0000;
    idt[13].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 13) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(14)
    idt[14].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 14) & (unsigned int) 0xFFFF);
    idt[14].segsel = (unsigned short) 0x00;
    idt[14].attr = (unsigned short) 0x0000;
    idt[14].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 14) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(15)
    idt[15].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 15) & (unsigned int) 0xFFFF);
    idt[15].segsel = (unsigned short) 0x00;
    idt[15].attr = (unsigned short) 0x0000;
    idt[15].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 15) >> 16 & (unsigned int) 0xFFFF);


#define IDT_ENTRY(16)
    idt[16].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 16) & (unsigned int) 0xFFFF);
    idt[16].segsel = (unsigned short) 0x00;
    idt[16].attr = (unsigned short) 0x0000;
    idt[16].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 16) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(17)
    idt[17].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 17) & (unsigned int) 0xFFFF);
    idt[17].segsel = (unsigned short) 0x00;
    idt[17].attr = (unsigned short) 0x0000;
    idt[17].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 17) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(18)
    idt[18].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 18) & (unsigned int) 0xFFFF);
    idt[18].segsel = (unsigned short) 0x00;
    idt[18].attr = (unsigned short) 0x0000;
    idt[18].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 18) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY(19)
    idt[19].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## 19) & (unsigned int) 0xFFFF);
    idt[19].segsel = (unsigned short) 0x00;
    idt[19].attr = (unsigned short) 0x0000;
    idt[19].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## 19) >> 16 & (unsigned int) 0xFFFF);
*/