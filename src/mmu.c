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
void mmu_inicializar() {
  proxima_pagina_libre = (void*) INICIO_PAGINAS_LIBRES;
}

void mmu_inicializar_dir_kernel() {
  page_directory_kernel = (page_directory_entry*) KERNEL_PAGE_DIRECTORY_ADDRESS;
  page_table_kernel_0 = (page_table_entry*) KERNEL_PAGE_TABLE_ADDRESS_0;

  llenar_directorio_de_paginas(page_directory_kernel, page_table_kernel_0);

  page_table_entry pte = crear_entrada_nula_de_tabla_de_paginas();
  pte.p = 1;
  pte.rw = 1;
  llenar_tabla_de_paginas(page_table_kernel_0, pte, 0);
}

void mmu_inicializar_dir_pirata() {
  page_directory_pirate = (page_directory_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
  page_table_pirate = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();

  llenar_directorio_de_paginas(page_directory_pirate, page_table_pirate);

  page_table_entry pte = crear_entrada_nula_de_tabla_de_paginas();
  pte.p = 1;
  pte.rw = 1;
  unsigned int desplazamiento = desplazamiento_para_funcion_de_mapeo(0x400000, 0x500000);
  
  // Manda la dir. lógica 0x400000 a la dir. física 0x500000:
  llenar_tabla_de_paginas(page_table_pirate, pte, desplazamiento);
}

void* mmu_direccion_fisica_de_la_proxima_pagina_libre() {
  void* pagina_libre = proxima_pagina_libre;
  proxima_pagina_libre = (void*) ((unsigned int) proxima_pagina_libre) + PAGE_SIZE;
  return pagina_libre;
}

unsigned int desplazamiento_para_funcion_de_mapeo(
    unsigned int direccion_virtual, unsigned int direccion_fisica) {
  long desplazamiento = direccion_fisica - direccion_virtual;
  return (unsigned int) desplazamiento >> 12;
}

void llenar_tabla_de_paginas(page_table_entry* tabla, page_table_entry entrada, unsigned int desplazamiento) {
  unsigned int i;
  for (i = 0; i < PAGE_ENTRIES_COUNT; i++) {
    tabla[i] = entrada;
    tabla[i].base = i + desplazamiento;
  }
}

void llenar_directorio_de_paginas(page_directory_entry* directorio, page_table_entry* primera_tabla) {
  page_directory_entry entrada;
  entrada.p = 0;
  entrada.rw = 0;
  entrada.us = 0;
  entrada.pwt = 0;
  entrada.pcd = 0;
  entrada.a = 0;
  entrada.ignored = 0;
  entrada.ps = 0;
  entrada.g = 0;
  entrada.avl = 0;
  entrada.base = 0;

  unsigned int i;
  for (i = 0; i < PAGE_ENTRIES_COUNT; i++) {
    directorio[i] = entrada;
  }

  directorio[0].p = 1;
  directorio[0].rw = 1;
  directorio[0].base = ((unsigned int) primera_tabla) >> 12;
}

page_table_entry crear_entrada_nula_de_tabla_de_paginas() {
  page_table_entry pte;
  pte.p = 0;
  pte.rw = 0;
  pte.us = 0;
  pte.pwt = 0;
  pte.pcd = 0;
  pte.a = 0;
  pte.d = 0;
  pte.pat = 0;
  pte.g = 0;
  pte.avl = 0;
  pte.base = 0;
  return pte;
}

void mmu_mapear_pagina(unsigned int direccion_virtual,
    unsigned int direccion_fisica, page_directory_entry* directorio_de_paginas) {
  // Ambas direcciones deberían ser múltiplos de 0x1000

  unsigned int nro_tabla = direccion_virtual >> 22; // índice del directorio de páginas
  unsigned int nro_pagina = (direccion_virtual << 10) >> 12; // índice de la tabla de páginas
  // unsigned int nro_linea = (direccion_virtual << 20) >> 20; // offset en la mem. física

  // Esta implementación asume que el directorio existe de acuerdo a la clase
  // de MMU de Lautaro Petaccio. Revisar.

  page_table_entry* tabla;
  page_table_entry pte = crear_entrada_nula_de_tabla_de_paginas();

  if (!directorio_de_paginas[nro_tabla].p) {
    tabla = mmu_direccion_fisica_de_la_proxima_pagina_libre();
    llenar_tabla_de_paginas(tabla, pte, 0);
    directorio_de_paginas[nro_tabla].base = ((unsigned int) tabla) >> 12;
  } else {
    tabla = (page_table_entry*) (directorio_de_paginas[nro_tabla].base << 12);
  }

  tabla[nro_pagina] = pte;
  tabla[nro_pagina].p = 1;
  tabla[nro_pagina].rw = 1;
  tabla[nro_pagina].base = desplazamiento_para_funcion_de_mapeo(direccion_virtual, direccion_fisica);

  tlbflush();
}

void mmu_desmapear_pagina(unsigned int direccion_virtual,
    page_directory_entry* directorio_de_paginas) {
  unsigned int nro_tabla = direccion_virtual >> 22; // índice del directorio de páginas
  unsigned int nro_pagina = (direccion_virtual << 10) >> 12; // índice de la tabla de páginas

  page_table_entry* tabla = (page_table_entry*) (directorio_de_paginas[nro_tabla].base << 12);
  tabla[nro_pagina].p = 0;
  
  tlbflush();
}