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

void mmu_inicializar();


typedef struct struct_page_table_entry {
    unsigned char   p:1;
    unsigned char 	rw:1;
   	unsigned char	us:1;
   	unsigned char	pwt:1;
   	unsigned char	pcd:1;
   	unsigned char	a:1;
   	unsigned char	d:1;
   	unsigned char	pat:1;
   	unsigned char	g:1;
   	unsigned char	avl:3;
   	unsigned int	base:19;
} __attribute__((__packed__, aligned (4))) page_table_entry;



extern page_table_entry*;
extern page_table;	


#endif	/* !__MMU_H__ */
