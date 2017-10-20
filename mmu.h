/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "game.h"

#define CODIGO_BASE       0X400000

#define MAPA_BASE_FISICA  0x500000
#define MAPA_BASE_VIRTUAL 0x800000

#define PAGE_DIRECTORY_SIZE 1

typedef struct struct_page_directory_entry {
    unsigned char   p:1;       // Present(1)
    unsigned char   rw:1;      // Read/Write(1)
    unsigned char   us:1;      // User-Supervisor(0)
    unsigned char   pwt:1;     // Page-Level write-through(0)
    unsigned char   pcd:1;     // Page-Level cache disable(0)
    unsigned char   a:1;       // Accessed(0)
    unsigned char   ignored:1; // Ignored(0)
    unsigned char   ps:1;      // Page-Size (4Kb=0)
    unsigned char   g:1;       // Global (ignored=0)
    unsigned char   avl:3;     // Bits disponibles para uso del SO
    unsigned int    base:20;   // Page base address

} __attribute__((__packed__, aligned (4))) page_directory_entry;

void mmu_inicializar_dir_kernel();

#endif	/* !__MMU_H__ */
