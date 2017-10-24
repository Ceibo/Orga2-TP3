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
page_directory_entry* page_directory;
page_table_entry* page_table_0; 

void mmu_inicializar_dir_kernel() {
  uint i;
  page_directory = (page_directory_entry*) PAGE_DIRECTORY_ADDRESS;
  page_table_0 = (page_table_entry*) PAGE_TABLE_ADDRESS;

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

  for (i = 0; i < PAGE_DIRECTORIES_COUNT; i++) {
    page_directory[i] = pde;
  }
  page_directory[0].p = 1;
  page_directory[0].rw = 1;
  page_directory[0].base = PAGE_TABLE_ADDRESS >> 12;

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
  pte.base = 0;

  for (i = 0x0; i < PAGE_TABLES_COUNT; i++) {
    page_table_0[i] = pte;
    page_table_0[i].base = i;
  }
}