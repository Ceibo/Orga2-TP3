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

  // Para que todos los piratas de cada jugador compartan el mismo mapa
  // y puedan así visualizar las áreas exploradas por los otros automáticamente
  tabla_de_paginas_del_mapa_del_jugador_A = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
  tabla_de_paginas_del_mapa_del_jugador_B = (page_table_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();
  page_table_entry entrada = crear_entrada_nula_de_tabla_de_paginas();
  llenar_tabla_de_paginas(tabla_de_paginas_del_mapa_del_jugador_A, entrada, FALSE);
  llenar_tabla_de_paginas(tabla_de_paginas_del_mapa_del_jugador_B, entrada, FALSE);
}

void mmu_inicializar_dir_kernel() {
  page_directory_kernel = (page_directory_entry*) KERNEL_PAGE_DIRECTORY_ADDRESS;
  page_table_kernel_0 = (page_table_entry*) KERNEL_PAGE_TABLE_ADDRESS_0;

  mapear_paginas_de_kernel(page_directory_kernel);
}

void mmu_prueba() {
  mmu_mapear_pagina(0x400000, 0x500000, (page_directory_entry*) rcr3());
}

page_directory_entry* mmu_inicializar_dir_pirata(uint32_t direccion_fisica_de_origen_del_codigo,
    uint32_t direccion_fisica_de_destino_del_codigo, uint32_t indice_del_jugador) {
  page_directory_entry* directorio = (page_directory_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();

  // En todos los directorios se mapea la zona del kernel:
  mapear_paginas_de_kernel(directorio);

  // Se mapean las 9 páginas correspondientes al área inicial de 3x3 del jugador en el mapa:
  page_table_entry* tabla_del_mapa = indice_del_jugador == INDICE_JUGADOR_A ?
      tabla_de_paginas_del_mapa_del_jugador_A : tabla_de_paginas_del_mapa_del_jugador_B;
  mapear_paginas_matricialmente_contiguas(directorio, tabla_del_mapa, direccion_fisica_de_destino_del_codigo);

  // Se mapea el inicio virtual del código a la posición inicial del jugador en el mapa:
  mmu_mapear_pagina(DIRECCION_VIRTUAL_DE_INICIO_DE_TAREAS,
      direccion_fisica_de_destino_del_codigo, directorio);

  // Se copia el código del origen a la posición inicial del jugador en el mapa:
  uint32_t* direccion_virtual_de_destino_del_codigo = (uint32_t*) DIRECCION_VIRTUAL_DE_INICIO_DE_TAREAS + 10 * PAGE_SIZE;
  page_directory_entry* directorio_actual = (page_directory_entry*) rcr3();
  mmu_mapear_pagina((uint32_t) direccion_virtual_de_destino_del_codigo,
      direccion_fisica_de_destino_del_codigo, directorio_actual);
  uint32_t* direccion_virtual_de_origen_del_codigo = (uint32_t*) direccion_fisica_de_origen_del_codigo; // Coinciden
  uint32_t i;
  for (i = 0; i < PAGE_SIZE; i++) {
    direccion_virtual_de_destino_del_codigo[i] = direccion_virtual_de_origen_del_codigo[i];
  }
  mmu_desmapear_pagina((uint32_t) direccion_virtual_de_destino_del_codigo, directorio_actual);

  return directorio;
}

void mapear_paginas_matricialmente_contiguas(page_directory_entry* directorio,
    page_table_entry* tabla_del_mapa, uint32_t direccion_fisica_de_la_posicion_actual_del_jugador) {
  // Calcula la fila y columna del mapa a partir de la dirección física
  uint32_t posicion_lineal_en_la_matriz_del_mapa =
    (direccion_fisica_de_la_posicion_actual_del_jugador - MAPA_BASE_FISICA) / PAGE_SIZE;
  uint32_t x = posicion_lineal_en_la_matriz_del_mapa % MAPA_ANCHO;
  uint32_t y = (posicion_lineal_en_la_matriz_del_mapa - x) / MAPA_ANCHO;

  int32_t i, j;
  uint32_t desplazamiento;
  for (i = -1; i <= 1; i++) {
    for (j = -1; j <= 1; j++) {
      desplazamiento = desplazamiento_para_calcular_la_direccion_de_la_pagina_en_el_mapa(x + i, y + j);
      mmu_mapear_pagina_especificando_direccion_de_tabla(
          MAPA_BASE_VIRTUAL + desplazamiento, MAPA_BASE_FISICA + desplazamiento, directorio, tabla_del_mapa);
    }
  }
}

uint32_t desplazamiento_para_calcular_la_direccion_de_la_pagina_en_el_mapa(uint32_t x, uint32_t y) {
  // La dirección base puede ser física o lógica, funciona con ambas.
  return game_xy2lineal(x, y) * PAGE_SIZE;
}

void* mmu_direccion_fisica_de_la_proxima_pagina_libre() {
  void* pagina_libre = proxima_pagina_libre;
  proxima_pagina_libre = (void*) ((unsigned int) proxima_pagina_libre) + PAGE_SIZE;
  return pagina_libre;
}

void llenar_tabla_de_paginas(page_table_entry* tabla, page_table_entry entrada, bool mapeo_kernel) {
  unsigned int i;
  if (mapeo_kernel) {
    for (i = 0; i < PAGE_ENTRIES_COUNT; i++) {
      tabla[i] = entrada;
      tabla[i].base = i;
    }
  } else {
    for (i = 0; i < PAGE_ENTRIES_COUNT; i++) {
      tabla[i] = entrada;
    }
  }
}

void llenar_directorio(page_directory_entry* directorio) {
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
}

void mapear_paginas_de_kernel(page_directory_entry* directorio) {
  llenar_directorio(directorio);

  directorio[0].p = 1;
  directorio[0].rw = 1;
  directorio[0].base = KERNEL_PAGE_TABLE_ADDRESS_0 >> 12;

  page_table_entry entrada = crear_entrada_nula_de_tabla_de_paginas();
  entrada.p = 1;
  entrada.rw = 1;
  llenar_tabla_de_paginas((page_table_entry*) KERNEL_PAGE_TABLE_ADDRESS_0, entrada, TRUE);
}

page_table_entry crear_entrada_nula_de_tabla_de_paginas() {
  page_table_entry entrada;
  entrada.p = 0;
  entrada.rw = 0;
  entrada.us = 0;
  entrada.pwt = 0;
  entrada.pcd = 0;
  entrada.a = 0;
  entrada.d = 0;
  entrada.pat = 0;
  entrada.g = 0;
  entrada.avl = 0;
  entrada.base = 0;
  return entrada;
}

void mmu_mapear_pagina(unsigned int direccion_virtual,
    unsigned int direccion_fisica, page_directory_entry* directorio_de_paginas) {
  // Ambas direcciones deberían ser múltiplos de 0x1000

  unsigned int nro_tabla = direccion_virtual >> 22; // índice del directorio de páginas
  unsigned int nro_pagina = (direccion_virtual << 10) >> 22; // índice de la tabla de páginas
  // unsigned int nro_linea = (direccion_virtual << 20) >> 20; // offset en la mem. física

  // Esta implementación asume que el directorio existe de acuerdo a la clase
  // de MMU de Lautaro Petaccio. Revisar.

  page_table_entry* tabla;
  page_table_entry entrada = crear_entrada_nula_de_tabla_de_paginas();

  if (!directorio_de_paginas[nro_tabla].p) {
    tabla = mmu_direccion_fisica_de_la_proxima_pagina_libre();
    llenar_tabla_de_paginas(tabla, entrada, FALSE);
    directorio_de_paginas[nro_tabla].base = ((unsigned int) tabla) >> 12;
    directorio_de_paginas[nro_tabla].p = 1; //pde presente
    directorio_de_paginas[nro_tabla].rw = 1;//r/w pde
  } else {
    tabla = (page_table_entry*) (directorio_de_paginas[nro_tabla].base << 12);
  }

  entrada.p = 1;
  entrada.rw = 1;
  entrada.base = direccion_fisica >> 12;
  tabla[nro_pagina] = entrada;

  tlbflush();
}

void mmu_mapear_pagina_especificando_direccion_de_tabla(unsigned int direccion_virtual,
    unsigned int direccion_fisica, page_directory_entry* directorio_de_paginas, page_table_entry* tabla) {
  // Ambas direcciones deberían ser múltiplos de 0x1000

  unsigned int nro_tabla = direccion_virtual >> 22; // índice del directorio de páginas
  unsigned int nro_pagina = (direccion_virtual << 10) >> 22; // índice de la tabla de páginas
  // unsigned int nro_linea = (direccion_virtual << 20) >> 20; // offset en la mem. física

  // Esta implementación asume que el directorio existe de acuerdo a la clase
  // de MMU de Lautaro Petaccio. Revisar.

  directorio_de_paginas[nro_tabla].base = ((unsigned int) tabla) >> 12;

  page_table_entry entrada = crear_entrada_nula_de_tabla_de_paginas();
  entrada.p = 1;
  entrada.rw = 1;
  entrada.base = direccion_fisica >> 12;
  tabla[nro_pagina] = entrada;

  tlbflush();
}

void mmu_desmapear_pagina(unsigned int direccion_virtual,
    page_directory_entry* directorio_de_paginas) {
  unsigned int nro_tabla = direccion_virtual >> 22; // índice del directorio de páginas
  unsigned int nro_pagina = (direccion_virtual << 10) >> 22; // índice de la tabla de páginas

  page_table_entry* tabla = (page_table_entry*) (directorio_de_paginas[nro_tabla].base << 12);
  tabla[nro_pagina].p = 0;
  
  tlbflush();
}