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
page_table_entry* page_table;

#define SET_PAGE_TABLE_ENTRY(numero)  \
  page_table[numero].p = 1;        \
  page_table[numero].rw = 1;       \
  page_table[numero].us = 0;       \
  page_table[numero].pwt = 0;      \
  page_table[numero].pcd = 0;      \
  page_table[numero].a = 0;        \
  page_table[numero].d = 0;        \
  page_table[numero].pat = 0;      \
  page_table[numero].g = 0;        \
  page_table[numero].avl = 0;      \
  page_table[numero].base = 0;     \

void mmu_inicializar_dir_kernel() {
  page_directory = (page_directory_entry*) 0x27000;
  page_directory->p = 1;
  page_directory->rw = 1;
  page_directory->us = 0;
  page_directory->pwt = 0;
  page_directory->pcd = 0;
  page_directory->a = 0;
  page_directory->ignored = 0;
  page_directory->ps = 0;
  page_directory->g = 0;
  page_directory->avl = 0;
  page_directory->base = 0; 


  uint i;

  page_table = (page_table_entry*) 0x28000;
  for (i = 0; i < TABLE_SIZE; i++) {
    SET_PAGE_TABLE_ENTRY(i);
    page_table += 4;
  }

  page_table = (page_table_entry*) 0x29000;
  for (i = 0; i < TABLE_SIZE; i++) {
    SET_PAGE_TABLE_ENTRY(i);
    page_table += 4;
  }

  page_table = (page_table_entry*) 0x2A000;
  for (i = 0; i < TABLE_SIZE; i++) {
    SET_PAGE_TABLE_ENTRY(i);
    page_table += 4;
  }

  page_table = (page_table_entry*) 0x2B000;
  for (i = 0; i < TABLE_SIZE; i++) {
    SET_PAGE_TABLE_ENTRY(i);
    page_table += 4;
  }
}