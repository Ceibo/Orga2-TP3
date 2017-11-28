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

#define SEGMENTO_CODIGO_NIVEL_0 8
#define SEGMENTO_DATOS_NIVEL_0 9
#define SEGMENTO_CODIGO_NIVEL_3 10
#define SEGMENTO_DATOS_NIVEL_3 11
#define SEGMENTO_PANTALLA 12

//lImite modificado: 0x1F3FF tal que ((0x1F3FF + 0x1 )* 0x1000) - 0x1 = 500 megas
//anterior lImite: 0x7cfff.
#define SEGMENT_LIMIT 0x1F3FF

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
 
    // Segmento para código de nivel cero (kernel)
    [SEGMENTO_CODIGO_NIVEL_0] = (gdt_entry) {//8
        (uint16_t)    SEGMENT_LIMIT,
        (uint16_t)    0x0000,   // Base address 0:15
        (uint8_t)     0x00,     // Base address 16:23
        (uint8_t)     0x0A,     // Segment type = read-execute code
        (uint8_t)     0x01,     // Descriptor type = code or data
        (uint8_t)     0x00,     // DPL = 0
        (uint8_t)     0x01,     // Segment present
        (uint8_t)     (SEGMENT_LIMIT >> 16),
        (uint8_t)     0x00,     // Available for use by system software
        (uint8_t)     0x00,     // Code segment 64 bit
        (uint8_t)     0x01,     // Default operation size = 32 bits
        (uint8_t)     0x01,     // Granularidad. En 1 hace que el tamaño sea (límite+1)*4kb-1
        (uint8_t)     0x00,     // Base address 24:31
    },
    
    // Segmento para datos de nivel cero (kernel)
    [SEGMENTO_DATOS_NIVEL_0] = (gdt_entry) {//9
        (uint16_t)    SEGMENT_LIMIT,
        (uint16_t)    0x0000,   // Base address 0:15
        (uint8_t)     0x00,     // Base address 16:23
        (uint8_t)     0x02,     // Segment type = read-write data
        (uint8_t)     0x01,     // Descriptor type = code or data
        (uint8_t)     0x00,     // DPL = 0
        (uint8_t)     0x01,     // Segment present
        (uint8_t)     (SEGMENT_LIMIT >> 16),
        (uint8_t)     0x00,     // Available for use by system software
        (uint8_t)     0x00,     // Code segment 64 bit
        (uint8_t)     0x01,     // Default operation size = 32 bits
        (uint8_t)     0x01,     // Granularidad
        (uint8_t)     0x00,     // Base address 24:31
    },

	// Segmento para código de nivel tres
    [SEGMENTO_CODIGO_NIVEL_3] = (gdt_entry) {//10
        (uint16_t)    SEGMENT_LIMIT,
        (uint16_t)    0x0000,   // Base address 0:15
        (uint8_t)     0x00,     // Base address 16:23
        (uint8_t)     0x0A,     // Segment type = read-execute code
        (uint8_t)     0x01,     // Descriptor type = code or data
        (uint8_t)     0x03,     // DPL = 3
        (uint8_t)     0x01,     // Segment present
        (uint8_t)     (SEGMENT_LIMIT >> 16),
        (uint8_t)     0x00,     // Available for use by system software
        (uint8_t)     0x00,     // Code segment 64 bit
        (uint8_t)     0x01,     // Default operation size = 32 bits
        (uint8_t)     0x01,     // Granularidad
        (uint8_t)     0x00,     // Base address 24:31
    },

	// Segmento para datos de nivel tres
    [SEGMENTO_DATOS_NIVEL_3] = (gdt_entry) {//11
        (uint16_t)    SEGMENT_LIMIT,
        (uint16_t)    0x0000,   // Base address 0:15
        (uint8_t)     0x00,     // Base address 16:23
        (uint8_t)     0x02,     // Segment type = read-write data
        (uint8_t)     0x01,     // Descriptor type = code or data
        (uint8_t)     0x03,     // DPL = 3
        (uint8_t)     0x01,     // Segment present
        (uint8_t)     (SEGMENT_LIMIT >> 16),
        (uint8_t)     0x00,     // Available for use by system software
        (uint8_t)     0x00,     // Code segment 64 bit
        (uint8_t)     0x01,     // Default operation size = 32 bits
        (uint8_t)     0x01,     // Granularidad
        (uint8_t)     0x00,     // Base address 24:31
    },
    
    // Segmento para la pantalla utilizado solo por el kernel
    #define VIDEO_SEGMENT_LIMIT 0x01F3F           // 20 bits         
    #define VIDEO_SEGMENT_BASE_ADDRESS 0x000B8000 // 32 bits
    [SEGMENTO_PANTALLA] = (gdt_entry) {//12
        (uint16_t)    VIDEO_SEGMENT_LIMIT,                // segment limit 0-15
        (uint16_t)    VIDEO_SEGMENT_BASE_ADDRESS,         // base address 0-15
        (uint8_t)     (VIDEO_SEGMENT_BASE_ADDRESS >> 16), // base address 16-23
        (uint8_t)     0x02,     // Segment type = read-write data
        (uint8_t)     0x01,     // Descriptor type = code or data
        (uint8_t)     0x00,     // DPL = 0
        (uint8_t)     0x01,     // Segment present
        (uint8_t)     (VIDEO_SEGMENT_LIMIT >> 16), // segment limit 16-19
        (uint8_t)     0x00,     // Available for use by system software
        (uint8_t)     0x00,     // Code segment 64 bit
        (uint8_t)     0x01,     // Default operation size = 32 bits
        (uint8_t)     0x00,     // Granularidad
        (uint8_t)     (VIDEO_SEGMENT_BASE_ADDRESS >> 34) // base address 24-31
    },
    
     //descriptor_tss  inicial en Indice 13
    //descriptor_tss idle en Indice 14
     
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};
