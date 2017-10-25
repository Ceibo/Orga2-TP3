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

#define PAGE_ENTRIES_COUNT 1024
#define INICIO_PAGINAS_LIBRES 0x100000

#define KERNEL_PAGE_DIRECTORY_ADDRESS 0x27000
#define KERNEL_PAGE_TABLE_ADDRESS_0 0x28000

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

typedef struct struct_page_table_entry {
    unsigned char p:1;
    unsigned char rw:1;
    unsigned char us:1;
    unsigned char pwt:1;
    unsigned char pcd:1;
    unsigned char a:1;
    unsigned char d:1;
    unsigned char pat:1;
    unsigned char g:1;
    unsigned char avl:3;
    unsigned int  base:20;
} __attribute__((__packed__, aligned (4))) page_table_entry;

page_directory_entry* page_directory_kernel;
page_table_entry* page_table_kernel_0;

page_directory_entry* page_directory_pirate;
page_table_entry* page_table_pirate;

void* proxima_pagina_libre;

// Funciones del enunciado

void mmu_inicializar_dir_kernel();
void mmu_inicializar_dir_pirata();
void* mmu_direccion_fisica_de_la_proxima_pagina_libre();
void mmu_inicializar();
void mmu_mapear_pagina(unsigned int direccion_virtual, unsigned int direccion_fisica, page_directory_entry* directorio_de_paginas);
void mmu_desmapear_pagina(unsigned int direccion_virtual, page_directory_entry* directorio_de_paginas);

unsigned int desplazamiento_para_funcion_de_mapeo(unsigned int direccion_virtual, unsigned int direccion_fisica);
page_table_entry crear_entrada_nula_de_tabla_de_paginas();
void llenar_tabla_de_paginas(page_table_entry* tabla, page_table_entry entrada, unsigned int desplazamiento);
void llenar_directorio_de_paginas(page_directory_entry* directorio, page_table_entry* primera_tabla);

#endif	/* !__MMU_H__ */