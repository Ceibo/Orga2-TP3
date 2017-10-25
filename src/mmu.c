/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
/* Atributos paginas */
/* -------------------------------------------------------------------------- */


/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
void mmu_inicializar_dir_kernel() {
  unsigned int i;
  page_directory_kernel = (page_directory_entry*) KERNEL_PAGE_DIRECTORY_ADDRESS;
  page_table_kernel_0 = (page_table_entry*) KERNEL_PAGE_TABLE_ADDRESS_0;

  page_directory_entry pde;
  pde.p = 0;
  pde.rw = 0;
  pde.us = 0;
  pde.pwt = 0;
  pde.pcd = 0;
  pde.a = 0;
  pde.ignored = 0;
  pde.ps = 0;
  pde.g = 0;
  pde.avl = 0;
  pde.base = 0;

  for (i = 0; i < PAGE_ENTRIES_COUNT; i++) {
    page_directory_kernel[i] = pde;
  }
  page_directory_kernel[0].p = 1;
  page_directory_kernel[0].rw = 1;
  page_directory_kernel[0].base = KERNEL_PAGE_TABLE_ADDRESS_0 >> 12;

  page_table_entry pte;
  pte.p = 1;
  pte.rw = 1;
  pte.us = 0;
  pte.pwt = 0;
  pte.pcd = 0;
  pte.a = 0;
  pte.d = 0;
  pte.pat = 0;
  pte.g = 0;
  pte.avl = 0;

  for (i = 0x0; i < PAGE_ENTRIES_COUNT; i++) {
    page_table_kernel_0[i] = pte;
    page_table_kernel_0[i].base = i;
  }
}

void mmu_inicializar() {
  proxima_pagina_libre = (void*) INICIO_PAGINAS_LIBRES;
}

void* mmu_direccion_fisica_de_la_proxima_pagina_libre() {
  void* pagina_libre = proxima_pagina_libre;
  proxima_pagina_libre = (void*) ((unsigned int) proxima_pagina_libre) + PAGE_SIZE;
  return pagina_libre;
}

void mmu_inicializar_dir_pirata() {
  unsigned int i;
  page_directory_pirate = (page_directory_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
  page_table_pirate = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();

  page_directory_entry pde;
  pde.p = 0;
  pde.rw = 0;
  pde.us = 0;
  pde.pwt = 0;
  pde.pcd = 0;
  pde.a = 0;
  pde.ignored = 0;
  pde.ps = 0;
  pde.g = 0;
  pde.avl = 0;
  pde.base = 0;

  for (i = 0; i < PAGE_ENTRIES_COUNT; i++) {
    page_directory_pirate[i] = pde;
  }
  page_directory_pirate[0].p = 1;
  page_directory_pirate[0].rw = 1;
  page_directory_pirate[0].base = ((unsigned int) page_table_pirate) >> 12;

  page_table_entry pte;
  pte.p = 1;
  pte.rw = 1;
  pte.us = 0;
  pte.pwt = 0;
  pte.pcd = 0;
  pte.a = 0;
  pte.d = 0;
  pte.pat = 0;
  pte.g = 0;
  pte.avl = 0;

  for (i = 0x0; i < PAGE_ENTRIES_COUNT; i++) {
    page_table_pirate[i] = pte;
    // Manda la dir. lógica 0x400000 a la dir. física 0x500000:
    page_table_pirate[i].base = i + 0x100; // ((i << 12) + 0x100000) >> 12;
  }
}

// Funciones auxiliares