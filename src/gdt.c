 /* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"
#include "tss.h"

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

#define SEGMENTO_CODIGO_NIVEL_0 8
#define SEGMENTO_DATOS_NIVEL_0 9
#define SEGMENTO_CODIGO_NIVEL_3 10
#define SEGMENTO_DATOS_NIVEL_3 11
#define SEGMENTO_PANTALLA 12


/* Definicion de la GDT */
/* -------------------------------------------------------------------------- */

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (uint16_t)    0x0000,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x00,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x00,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },

    // [Ejercicio 1A] Agrego los 4 segmentos arrancando desde la posición 8
 
    // Segmento para código de nivel cero (kernel)
    //lImite modificado: 0x1F3FF tal que ((0x1F3FF + 0x1 )* 0x1000) - 0x1 = 500 megas
    //anterior lImite: 0x7cfff.
    [SEGMENTO_CODIGO_NIVEL_0] = (gdt_entry) {//8
        (uint16_t)    0xF3FF ,
        (uint16_t)    SEGMENT_BASE_ADDRESS_0_15,
        (uint8_t)     SEGMENT_BASE_ADDRESS_23_16,
        (uint8_t)     SEGMENT_TYPE___CODE_READ_EXECUTE,
        (uint8_t)     DESCRIPTOR_TYPE___CODE_OR_DATA,
        (uint8_t)     DESCRIPTOR_PRIVILEGE_LEVEL___ZERO,
        (uint8_t)     SEGMENT_PRESENT___YES,
        (uint8_t)     0x1,// hexa mAs significativo de lImite tal que lImite es 0x1F3FF,
        (uint8_t)     AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE,
        (uint8_t)     CODE_SEGMENT_64_BIT,
        (uint8_t)     DEFAULT_OPERATION_SIZE___32_BITS,
        (uint8_t)     GRANULARITY,
        (uint8_t)     SEGMENT_BASE_ADDRESS_31_24
    },
    
    // Segmento para datos de nivel cero (kernel)
    [SEGMENTO_DATOS_NIVEL_0] = (gdt_entry) {//9
        (uint16_t)    0xF3FF,
        (uint16_t)    SEGMENT_BASE_ADDRESS_0_15,
        (uint8_t)     SEGMENT_BASE_ADDRESS_23_16,
        (uint8_t)     SEGMENT_TYPE___DATA_READ_WRITE,
        (uint8_t)     DESCRIPTOR_TYPE___CODE_OR_DATA,
        (uint8_t)     DESCRIPTOR_PRIVILEGE_LEVEL___ZERO,
        (uint8_t)     SEGMENT_PRESENT___YES,
        (uint8_t)     0x1,// hexa mAs significativo de lImite tal que lImite es 0x1F3FF,,
        (uint8_t)     AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE,
        (uint8_t)     CODE_SEGMENT_64_BIT,
        (uint8_t)     DEFAULT_OPERATION_SIZE___32_BITS,
        (uint8_t)     GRANULARITY,
        (uint8_t)     SEGMENT_BASE_ADDRESS_31_24
    },

	// Segmento para código de nivel tres
    [SEGMENTO_CODIGO_NIVEL_3] = (gdt_entry) {//10
        (uint16_t)    0xF3FF,
        (uint16_t)    SEGMENT_BASE_ADDRESS_0_15,
        (uint8_t)     SEGMENT_BASE_ADDRESS_23_16,
        (uint8_t)     SEGMENT_TYPE___CODE_READ_EXECUTE,
        (uint8_t)     DESCRIPTOR_TYPE___CODE_OR_DATA,
        (uint8_t)     DESCRIPTOR_PRIVILEGE_LEVEL___THREE,
        (uint8_t)     SEGMENT_PRESENT___YES,
        (uint8_t)     0x1,// hexa mAs significativo de lImite tal que lImite es 0x1F3FF,
        (uint8_t)     AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE,
        (uint8_t)     CODE_SEGMENT_64_BIT,
        (uint8_t)     DEFAULT_OPERATION_SIZE___32_BITS,
        (uint8_t)     GRANULARITY,
        (uint8_t)     SEGMENT_BASE_ADDRESS_31_24
    },

	// Segmento para datos de nivel tres
    [SEGMENTO_DATOS_NIVEL_3] = (gdt_entry) {//11
        (uint16_t)    0xF3FF,//0x1F3FF tal que ((0x1F3FF + 0x1 )* 0x1000) - 0x1 = 500 megas
        (uint16_t)    SEGMENT_BASE_ADDRESS_0_15,
        (uint8_t)     SEGMENT_BASE_ADDRESS_23_16,
        (uint8_t)     SEGMENT_TYPE___DATA_READ_WRITE,
        (uint8_t)     DESCRIPTOR_TYPE___CODE_OR_DATA,
        (uint8_t)     DESCRIPTOR_PRIVILEGE_LEVEL___THREE,
        (uint8_t)     SEGMENT_PRESENT___YES,
        (uint8_t)     0x1,// hexa mAs significativo de lImite tal que lImite es 0x1F3FF,,
        (uint8_t)     AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE,
        (uint8_t)     CODE_SEGMENT_64_BIT,
        (uint8_t)     DEFAULT_OPERATION_SIZE___32_BITS,
        (uint8_t)     GRANULARITY,
        (uint8_t)     SEGMENT_BASE_ADDRESS_31_24
    },
    
    // Segmento para la pantalla utilizado solo por el kernel
    [SEGMENTO_PANTALLA] = (gdt_entry) {//12
        (uint16_t)    0x1f3f, // segment limit 0-15
        (uint16_t)    0x8000, // base address 0-15
        (uint8_t)     0x0B, // base address 16-23
        (uint8_t)     SEGMENT_TYPE___DATA_READ_WRITE,
        (uint8_t)     DESCRIPTOR_TYPE___CODE_OR_DATA,
        (uint8_t)     0x0,//(00)b: nivel 0 ,
        (uint8_t)     SEGMENT_PRESENT___YES,
        (uint8_t)     0x00, // segment limit 16-19
        (uint8_t)     AVAILABLE_FOR_USE_BY_SYSTEM_SOFTWARE,
        (uint8_t)     CODE_SEGMENT_64_BIT,
        (uint8_t)     DEFAULT_OPERATION_SIZE___32_BITS,
        (uint8_t)     0x00, // granulatity = 0
        (uint8_t)     0x00 // base address 24-31
    },
    
     //descriptor_tss  inicial en Indice 13
    //descriptor_tss idle en Indice 14
     
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};
