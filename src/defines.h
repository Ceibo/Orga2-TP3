/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Tipos basicos */
/* -------------------------------------------------------------------------- */
#define NULL                    0
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned char bool;
typedef int int32_t;

/* Constantes basicas */
/* -------------------------------------------------------------------------- */
#define PAGE_SIZE               0x00001000
#define TASK_SIZE               4096

#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 30

#define GDT_IDX_NULL_DESC           0
#define GDT_TAREA_INICIAL 13
#define GDT_IDLE 14

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)

/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */
/* Direcciones fIsicas del origen cOdigo tareas */
/* -------------------------------------------------------------------------- */
#define TAREA_A_E         0X10000

#define TAREA_A_M         0X11000

#define TAREA_B_E         0X12000

#define TAREA_B_M         0X13000

void* error();

#define ASSERT(x) while(!(x)) {};


#endif  /* !__DEFINES_H__ */
