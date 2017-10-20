/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

// Formato de los define: A___B donde A es el nombre del campo y B es
// el valor (A y B están separados por tres guiones bajos)

// Falta decir que direccionen los primeros 500 MB de memoria. Qué significa y cómo se hace?

#define CODE_SEGMENT_64_BIT 0x00  // Sacado del ejemplo, confirmar

#define AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE 0x00

#define SEGMENT_BASE_ADDRESS_0_15 0x0000 // Sacado del ejemplo, confirmar
#define SEGMENT_BASE_ADDRESS_23_16 0x00 // Sacado del ejemplo, confirmar
#define SEGMENT_BASE_ADDRESS_31_24 0x00 // Sacado del ejemplo, confirmar

#define DEFAULT_OPERATION_SIZE___32_BITS 0x01

#define DESCRIPTOR_PRIVILEGE_LEVEL___ZERO 0x00
#define DESCRIPTOR_PRIVILEGE_LEVEL___THREE 0x03

// Direcciona los primeros 500MB
#define GRANULARITY 0x01 // divide el segment limit por 4kb
#define SEGMENT_LIMIT_0_15 0xF3FF
#define SEGMENT_LIMIT_19_16 0x01

#define SEGMENT_PRESENT___YES 0x01 // Sacado del ejemplo, confirmar. Por qué presente?

#define DESCRIPTOR_TYPE___CODE_OR_DATA 0x01

#define SEGMENT_TYPE___CODE_READ_EXECUTE 0x0A
#define SEGMENT_TYPE___DATA_READ_WRITE 0x02


/* Definicion de la GDT */
/* -------------------------------------------------------------------------- */

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    // [Ejercicio 1A] Agrego los 4 segmentos arrancando desde la posición 8
 
    // Segmento para código de nivel cero (Kerner)
    [8] = (gdt_entry) {
        (unsigned short)    SEGMENT_LIMIT_0_15,
        (unsigned short)    SEGMENT_BASE_ADDRESS_0_15,
        (unsigned char)     SEGMENT_BASE_ADDRESS_23_16,
        (unsigned char)     SEGMENT_TYPE___CODE_READ_EXECUTE,
        (unsigned char)     DESCRIPTOR_TYPE___CODE_OR_DATA,
        (unsigned char)     DESCRIPTOR_PRIVILEGE_LEVEL___ZERO,
        (unsigned char)     SEGMENT_PRESENT___YES,
        (unsigned char)     SEGMENT_LIMIT_19_16,
        (unsigned char)     AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE,
        (unsigned char)     CODE_SEGMENT_64_BIT,
        (unsigned char)     DEFAULT_OPERATION_SIZE___32_BITS,
        (unsigned char)     GRANULARITY,
        (unsigned char)     SEGMENT_BASE_ADDRESS_31_24
    },
    
    // Segmento para datos de nivel cero
    [9] = (gdt_entry) {
        (unsigned short)    SEGMENT_LIMIT_0_15,
        (unsigned short)    SEGMENT_BASE_ADDRESS_0_15,
        (unsigned char)     SEGMENT_BASE_ADDRESS_23_16,
        (unsigned char)     SEGMENT_TYPE___DATA_READ_WRITE,
        (unsigned char)     DESCRIPTOR_TYPE___CODE_OR_DATA,
        (unsigned char)     DESCRIPTOR_PRIVILEGE_LEVEL___ZERO,
        (unsigned char)     SEGMENT_PRESENT___YES,
        (unsigned char)     SEGMENT_LIMIT_19_16,
        (unsigned char)     AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE,
        (unsigned char)     CODE_SEGMENT_64_BIT,
        (unsigned char)     DEFAULT_OPERATION_SIZE___32_BITS,
        (unsigned char)     GRANULARITY,
        (unsigned char)     SEGMENT_BASE_ADDRESS_31_24
    },

	// Segmento para código de nivel tres
    [10] = (gdt_entry) {
        (unsigned short)    SEGMENT_LIMIT_0_15,
        (unsigned short)    SEGMENT_BASE_ADDRESS_0_15,
        (unsigned char)     SEGMENT_BASE_ADDRESS_23_16,
        (unsigned char)     SEGMENT_TYPE___CODE_READ_EXECUTE,
        (unsigned char)     DESCRIPTOR_TYPE___CODE_OR_DATA,
        (unsigned char)     DESCRIPTOR_PRIVILEGE_LEVEL___THREE,
        (unsigned char)     SEGMENT_PRESENT___YES,
        (unsigned char)     SEGMENT_LIMIT_19_16,
        (unsigned char)     AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE,
        (unsigned char)     CODE_SEGMENT_64_BIT,
        (unsigned char)     DEFAULT_OPERATION_SIZE___32_BITS,
        (unsigned char)     GRANULARITY,
        (unsigned char)     SEGMENT_BASE_ADDRESS_31_24
    },

	// Segmento para datos de nivel tres
    [11] = (gdt_entry) {
        (unsigned short)    SEGMENT_LIMIT_0_15,
        (unsigned short)    SEGMENT_BASE_ADDRESS_0_15,
        (unsigned char)     SEGMENT_BASE_ADDRESS_23_16,
        (unsigned char)     SEGMENT_TYPE___DATA_READ_WRITE,
        (unsigned char)     DESCRIPTOR_TYPE___CODE_OR_DATA,
        (unsigned char)     DESCRIPTOR_PRIVILEGE_LEVEL___THREE,
        (unsigned char)     SEGMENT_PRESENT___YES,
        (unsigned char)     SEGMENT_LIMIT_19_16,
        (unsigned char)     AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE,
        (unsigned char)     CODE_SEGMENT_64_BIT,
        (unsigned char)     DEFAULT_OPERATION_SIZE___32_BITS,
        (unsigned char)     GRANULARITY,
        (unsigned char)     SEGMENT_BASE_ADDRESS_31_24
    },
    
    // Segmento para la pantalla utilizado solo por el kernel
    [12] = (gdt_entry) {
        (unsigned short)    0x1F40, // segment limit 0-15
        (unsigned short)    0x8000, // base address 0-15
        (unsigned char)     0x0B, // base address 16-23
        (unsigned char)     SEGMENT_TYPE___DATA_READ_WRITE,
        (unsigned char)     DESCRIPTOR_TYPE___CODE_OR_DATA,
        (unsigned char)     DESCRIPTOR_PRIVILEGE_LEVEL___THREE,
        (unsigned char)     SEGMENT_PRESENT___YES,
        (unsigned char)     0x00, // segment limit 16-19
        (unsigned char)     AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE,
        (unsigned char)     CODE_SEGMENT_64_BIT,
        (unsigned char)     DEFAULT_OPERATION_SIZE___32_BITS,
        (unsigned char)     0x00, // granulatity = 0
        (unsigned char)     0x00 // base address 24-31
    }
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};
