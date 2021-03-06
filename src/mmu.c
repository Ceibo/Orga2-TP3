/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "game.h"
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

  mapear_paginas_de_kernel(page_directory_kernel);
}

page_directory_entry* mmu_inicializar_dir_pirata(uint32_t direccion_fisica_de_origen_del_codigo,
    uint32_t direccion_fisica_de_destino_del_codigo, uint32_t id) {
  page_directory_entry* directorio = (page_directory_entry*) mmu_direccion_fisica_de_la_proxima_pagina_libre();

  // En todos los directorios se mapea la zona del kernel:
  mapear_paginas_de_kernel(directorio);
 
  
  mapear_paginas_matricialmente_contiguas(directorio, direccion_fisica_de_destino_del_codigo,id);
  
  
  // Se mapea el inicio virtual del código a la posición inicial del jugador en el mapa:
  mmu_mapear_pagina(DIRECCION_VIRTUAL_DE_INICIO_DE_TAREAS,
      direccion_fisica_de_destino_del_codigo, directorio,0x3);//0x3 == r/w (bit 0), u (bit 1)

  // Se copia el código del origen a la posición inicial del jugador en el mapa:
  uint32_t* direccion_virtual_de_destino_del_codigo = (uint32_t*) DIRECCION_VIRTUAL_DE_INICIO_DE_TAREAS + 10 * PAGE_SIZE;
  page_directory_entry* directorio_actual = (page_directory_entry*) rcr3();
  mmu_mapear_pagina((uint32_t) direccion_virtual_de_destino_del_codigo,
      direccion_fisica_de_destino_del_codigo, directorio_actual,0x3);
  uint32_t* direccion_virtual_de_origen_del_codigo = (uint32_t*) direccion_fisica_de_origen_del_codigo; // Coinciden
  uint32_t i;
  const uint32_t cant_a_copiar = PAGE_SIZE >> 2; // si copiáramos con uint8_t no haría falta dividir por 4
  for (i = 0; i < cant_a_copiar; i++) {
    direccion_virtual_de_destino_del_codigo[i] = direccion_virtual_de_origen_del_codigo[i];
  }
  
  mmu_desmapear_pagina((uint32_t) direccion_virtual_de_destino_del_codigo, directorio_actual);
	 
  return directorio;
}

void mapear_paginas_matricialmente_contiguas(page_directory_entry* directorio,
                 uint32_t direccion_fisica_de_la_posicion_actual_del_jugador,uint32_t id) {
  // Calcula la fila y columna del mapa a partir de la dirección física
  uint32_t posicion_lineal_en_la_matriz_del_mapa =
    (direccion_fisica_de_la_posicion_actual_del_jugador - MAPA_BASE_FISICA) / PAGE_SIZE;
  uint32_t x = posicion_lineal_en_la_matriz_del_mapa % MAPA_ANCHO;
  uint32_t y = (posicion_lineal_en_la_matriz_del_mapa - x) / MAPA_ANCHO;
  uint32_t index_jugador = id_pirata2pirata(id)->jugador->index;
  int32_t i, j;
  uint32_t desplazamiento;
  for (i = -1; i <= 1; i++) {
    for (j = -1; j <= 1; j++) {
		int a = x + i;  int b = y + j;
		if(game_posicion_valida(a,b) ){//chequeamos que estEn dentro de mapa
		  
          desplazamiento = desplazamiento_para_calcular_la_direccion_de_la_pagina_en_el_mapa(x + i, y + j);
          mmu_mapear_pagina_especificando_direccion_de_tabla(
          MAPA_BASE_VIRTUAL + desplazamiento, MAPA_BASE_FISICA + desplazamiento, directorio, index_jugador);
       }
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
  page_directory_entry entrada = crear_entrada_nula_de_directorio_de_paginas();
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

page_directory_entry crear_entrada_nula_de_directorio_de_paginas() {
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
  return entrada;
}

void mmu_mapear_pagina(unsigned int direccion_virtual,
    unsigned int direccion_fisica, page_directory_entry* directorio_de_paginas, uint32_t att) {
  // Ambas direcciones deberían ser múltiplos de 0x1000

  unsigned int nro_tabla = direccion_virtual >> 22; // índice del directorio de páginas
  unsigned int nro_pagina = (direccion_virtual << 10) >> 22; // índice de la tabla de páginas
  // unsigned int nro_linea = (direccion_virtual << 20) >> 20; // offset en la mem. física
 

  page_table_entry* tabla;
  page_table_entry entrada = crear_entrada_nula_de_tabla_de_paginas();

  if (!directorio_de_paginas[nro_tabla].p) {
    tabla = mmu_direccion_fisica_de_la_proxima_pagina_libre();
    llenar_tabla_de_paginas(tabla, entrada, FALSE);
    directorio_de_paginas[nro_tabla].base = ((unsigned int) tabla) >> 12;
    directorio_de_paginas[nro_tabla].p = 1; //pde presente
    
    directorio_de_paginas[nro_tabla].rw = att;
    directorio_de_paginas[nro_tabla].us = att >>1;
  } else { 
	  //si ya estaba mapeada la pAgina se obtiene la direcciOn de esta
    tabla = (page_table_entry*) (directorio_de_paginas[nro_tabla].base << 12);
  }

  entrada.p = 1;
  entrada.rw = att;
  entrada.us = att >> 1;
  entrada.base = direccion_fisica >> 12;
  //se pisa la anterior entrada en caso de misma direcciOn de origen (mover cOdigo)
  tabla[nro_pagina] = entrada;

  tlbflush();
}

void mmu_mapear_pagina_especificando_direccion_de_tabla(unsigned int direccion_virtual,
    unsigned int direccion_fisica, page_directory_entry* directorio_de_paginas, 
    uint32_t index_jugador) {
  // Ambas direcciones deberían ser múltiplos de 0x1000

  unsigned int nro_tabla = direccion_virtual >> 22; // índice del directorio de páginas
  unsigned int nro_pagina = (direccion_virtual << 10) >> 22; // índice de la tabla de páginas
  // unsigned int nro_linea = (direccion_virtual << 20) >> 20; // offset en la mem. física
  page_table_entry * tabla;
  // Esta implementación asume que el directorio existe
  // de acuerdo al Indice en el directorio y al index jugador usamos las pAginas para mapa read only
  if(index_jugador == INDICE_JUGADOR_A){
	  if(nro_tabla == 2)
		   tabla = tabla_de_paginas_del_mapa_del_jugador_A_2;
	  if(nro_tabla == 3)
	       tabla = tabla_de_paginas_del_mapa_del_jugador_A_3;
      if(nro_tabla == 4)
	       tabla = tabla_de_paginas_del_mapa_del_jugador_A_4;
	  if(nro_tabla == 5)
	       tabla = tabla_de_paginas_del_mapa_del_jugador_A_5;
  }
   if(index_jugador == INDICE_JUGADOR_B){
	  if(nro_tabla == 2)
		   tabla = tabla_de_paginas_del_mapa_del_jugador_B_2;
	  if(nro_tabla == 3)
	       tabla = tabla_de_paginas_del_mapa_del_jugador_B_3;
      if(nro_tabla == 4)
	       tabla = tabla_de_paginas_del_mapa_del_jugador_B_4;
	  if(nro_tabla == 5)
	       tabla = tabla_de_paginas_del_mapa_del_jugador_B_5;
  }
	
  page_directory_entry entrada_al_directorio = crear_entrada_nula_de_directorio_de_paginas();
  entrada_al_directorio.base = ((unsigned int) tabla) >> 12;
  entrada_al_directorio.p = 1;
  entrada_al_directorio.rw = 0;
  entrada_al_directorio.us = 1; //user
  directorio_de_paginas[nro_tabla] = entrada_al_directorio;

  page_table_entry entrada_a_la_tabla = crear_entrada_nula_de_tabla_de_paginas();
  entrada_a_la_tabla.p = 1;
  entrada_a_la_tabla.rw = 0; // 0 = read-only; 
  entrada_a_la_tabla.us = 1; //user
  entrada_a_la_tabla.base = direccion_fisica >> 12;
  tabla[nro_pagina] = entrada_a_la_tabla;

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
 // debe remapear y copiar el codigo
void mmu_mover_pirata(pirata_t *pirata, int viejo_x, int viejo_y){
	//necesitamos viejo_x y viejo_y para trasladar el cOdigo y pila de anterior posiciOn
	
	// Se mapea el inicio virtual del código a la posición del jugador en el mapa:
	
  uint32_t nuevo_x = pirata->x;
  uint32_t nuevo_y = pirata->y;
  uint32_t id = pirata->id;
  uint32_t desp = desplazamiento_para_calcular_la_direccion_de_la_pagina_en_el_mapa(nuevo_x,nuevo_y);
  //obtenemos dir fIsica de nueva ubicaciOn en el mapa 
  unsigned int direccion_fisica_de_destino_del_codigo = MAPA_BASE_FISICA + desp;
  //obtenemos directorio actual
  page_directory_entry*  directorio = (page_directory_entry* ) rcr3(); 
  //mapeamos a nueva posiciOn en mapa como user y rw:
  mmu_mapear_pagina(DIRECCION_VIRTUAL_DE_INICIO_DE_TAREAS,
      direccion_fisica_de_destino_del_codigo, directorio,0x3);//0x3 == r/w (bit 0), u (bit 1)
  
  // Se copia el código del origen a la posición inicial del jugador en el mapa:
 
  //mapeamos a anterior posiciOn sobre pAginas no usadas para no pisar nuevo mapeo y
  //acceder a cOdigo y pila de anterior posiciOn
  uint32_t* direccion_virtual_de_origen_del_codigo = (uint32_t*) DIRECCION_VIRTUAL_DE_INICIO_DE_TAREAS + 10 * PAGE_SIZE;
  //calculamos antigua direcciOn fIsica de cOdigo y pila
  desp = desplazamiento_para_calcular_la_direccion_de_la_pagina_en_el_mapa(viejo_x,viejo_y);
  unsigned int direccion_fisica_de_origen_del_codigo = MAPA_BASE_FISICA + desp;
  //mapeamos pAgina temporal a antigua direcciOn fIsica de cOdigo y pila
  mmu_mapear_pagina((uint32_t) direccion_virtual_de_origen_del_codigo,
      direccion_fisica_de_origen_del_codigo, directorio,0x3);
  //calculamos direcciOn virtual de destino (nueva posiciOn, ya mapeada)
    uint32_t* direccion_virtual_de_destino_del_codigo = (uint32_t*) DIRECCION_VIRTUAL_DE_INICIO_DE_TAREAS;
  //copiamos a nueva posiciOn usando pAgina temporal (direccion_virtual_de_destino_del_codigo)
   uint32_t i;
  const uint32_t cant_a_copiar = PAGE_SIZE >> 2; //se copian de a 4 bytes, dividir por 4
  for (i = 0; i < cant_a_copiar; i++) {
    direccion_virtual_de_destino_del_codigo[i] = direccion_virtual_de_origen_del_codigo[i];
  }
  mmu_desmapear_pagina((uint32_t) direccion_virtual_de_origen_del_codigo, directorio);
  
  //mapeamos posiciOn junto a posiciones de alrededor como read only si es posible (sOlo explorador):
  if(pirata->tipo == TIPO_EXPLORADOR)
     mapear_paginas_matricialmente_contiguas(directorio, direccion_fisica_de_destino_del_codigo,id);
  else if (pirata->tipo == TIPO_MINERO){//chequeamos que posiciOn a donde avanza estE mapeada
     uint32_t  desplazamiento = desplazamiento_para_calcular_la_direccion_de_la_pagina_en_el_mapa(pirata->x, pirata->y);
     unsigned int direccion_virtual = MAPA_BASE_VIRTUAL + desplazamiento;
     unsigned int nro_tabla = direccion_virtual >> 22; // índice del directorio de páginas
     unsigned int nro_pagina = (direccion_virtual << 10) >> 22; // índice de la tabla de páginas
     if (!directorio[nro_tabla].p)
		error();
	 else{
		 page_table_entry* tabla = (page_table_entry*) (directorio[nro_tabla].base << 12);
		 if(!tabla[nro_pagina].p)
			error();
	}  
   }
   
}

