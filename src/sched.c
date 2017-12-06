/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "screen.h"

sched_t scheduler;

void sched_inicializar()
{
	//llenamos array tasks:
	
	//el primer Indice es para la tarea idle. sOlo indicamos su Indice en gdt, lo demAs en 0
	scheduler.tasks[0].gdt_index = ((14 << 3) | 0x0);//0x70;
	scheduler.tasks[0].pirata = NULL;
	scheduler.tasks[0].reservado_explorador = 0;
	scheduler.tasks[0].reservado_minero = 0;
	scheduler.tasks[0].posiciOn_x_tesoro = 100;
	scheduler.tasks[0].posiciOn_y_tesoro = 100;
	
	int i;
	
	//de Indice 1 a 8 se almacenarAn estructuras asociadas a piratas de jugadorA
	for(i = 1; i < MAX_CANT_TAREAS_VIVAS - 7 ; i++){// 1 =< i =< 8
		pirata_t * morgan = &jugadorA.piratas[i-1];
		scheduler.tasks[i].pirata = morgan;
		scheduler.tasks[i].gdt_index = (morgan->id << 3) | 0x0;
		scheduler.tasks[i].reservado_explorador = 0;
		scheduler.tasks[i].reservado_minero = 0;
		scheduler.tasks[i].posiciOn_x_tesoro = 100;
		scheduler.tasks[i].posiciOn_y_tesoro = 100;
	}
		
	//de Indice 9 a 16 se almacenarAn estructuras asociadas a piratas de jugadorB
	for(i = 9; i < MAX_CANT_TAREAS_VIVAS +1 ; i++){// 9 =< i =< 16
		pirata_t * morgan = &jugadorB.piratas[i-9];
		scheduler.tasks[i].pirata = morgan;
		scheduler.tasks[i].gdt_index = (morgan->id << 3) | 0x0;
		scheduler.tasks[i].reservado_explorador = 0;
		scheduler.tasks[i].reservado_minero = 0;
		scheduler.tasks[i].posiciOn_x_tesoro = 100;
		scheduler.tasks[i].posiciOn_y_tesoro = 100;
	}
	
	//seteamos a current apuntando a la tarea idle:
	
	scheduler.current = 0;
	
	//seteamos arrays posiciones_tesoros_A y posiciones_tesoros_B:
	
	for(i = 0; i < 20; i++){
		scheduler.posiciones_tesoros_A[i][0] = 100;
		scheduler.posiciones_tesoros_A[i][1] = 100;
		scheduler.inicio_tesoros_A = 0;
		scheduler.posiciones_tesoros_B[i][0] = 100;
		scheduler.posiciones_tesoros_B[i][1] = 100;
		scheduler.inicio_tesoros_B = 0;
	}
	
	//seteamos prox como invAlido (500 indica idle es Unica activa y no se debe saltar a otra tarea)
	scheduler.prox = 500;
 
}


int sched_buscar_indice_tarea(uint32_t gdt_index) {
    return MAX_CANT_TAREAS_VIVAS;
}


int sched_buscar_tarea_libre(uint32_t index_j)
{
	int i, indice = 1000;
	int j = (int) scheduler.current;
    for(i = 0; i < MAX_CANT_TAREAS_VIVAS+1; i++){
	  indice = (j+i)% (MAX_CANT_TAREAS_VIVAS+1); 
	  if(scheduler.tasks[indice].pirata != NULL && scheduler.tasks[indice].pirata->libre && 
	     scheduler.tasks[indice].pirata->jugador->index == index_j)
	    break;
    }
    return indice;
}

void sched_guardar_prox_pos_en_buffer_tesoro(uint32_t index_j,uint32_t x, uint32_t y){
	uint16_t i,k;
	if(index_j == INDICE_JUGADOR_A){
		//empezamos desde primer Indice usado
		k = scheduler.inicio_tesoros_A;
		for(i = 0; i < 20; i++){
			uint16_t u = (k+i)%20;
			if(scheduler.posiciones_tesoros_A[u][0] == 100){ 
				scheduler.posiciones_tesoros_A[u][0] = x;
				scheduler.posiciones_tesoros_A[u][1] = y;
				break;
			}
		}
	}else{
		k = scheduler.inicio_tesoros_B;
		for(i = 0; i < 20; i++){
			uint16_t u = (k+i)%20;
			if(scheduler.posiciones_tesoros_B[u][0] == 100){ 
				scheduler.posiciones_tesoros_B[u][0] = x;
				scheduler.posiciones_tesoros_B[u][1] = y;
				break;
			}
		}
	}
}


pirata_t* sched_tarea_actual()
{
	if(scheduler.current != 0)
		return scheduler.tasks[scheduler.current].pirata;
	else 
		return NULL;
}

void sched_agregar_tarea(uint16_t index_task)
{
	/*
	//si prox == 500 entonces pisamos con Indice de nuevo pirata a prox y current
	if(scheduler.prox == 500){
		scheduler.prox = index_task;
		scheduler.current = index_task;
	} 
	*/ //else{
	if( scheduler.prox != 500 ){
		
		//chequeamos si estamos saltando a tarea de mismo jugador
		if(sched_tarea_actual()->jugador->index == scheduler.tasks[scheduler.prox].pirata->jugador->index){
			//breakpoint();
			//si dueño de pirata actual no es dueño de pirata a agregar pisamos prox
			if(sched_tarea_actual()->jugador->index != scheduler.tasks[index_task].pirata->jugador->index){
				scheduler.prox = index_task;//si es asI entonces actualizamos prox con nueva tarea
				 
			}
			/*else if(scheduler.tasks[scheduler.current].gdt_index == 
			scheduler.tasks[scheduler.prox].gdt_index){
				//breakpoint();
				//si hay Unica tarea pisamos prox
				scheduler.prox = index_task;
			}*/
			 
		 }
	} 
	//breakpoint();
}

void sched_remover_tarea(uint16_t index_task)
{	
	//consultamos si la tarea es Unica activa (prox apunta a esta)
	if(index_task == scheduler.prox)
		scheduler.prox = 500;//restauramos prox a default
}


uint32_t sched_proxima_a_ejecutar()
{
    return MAX_CANT_TAREAS_VIVAS;
}


uint16_t sched_atender_tick()
{
  
    uint16_t id = game_id_pirata_actual();
    
    //actualizamos relojes y si se cumpliO tiempo de juego se debe terminar la partida
	game_tick(id);
	 
	//chequeamos si hay slots libres
	uint32_t index_A = INDICE_JUGADOR_A;
	uint32_t index_B = INDICE_JUGADOR_B;
	int j;
	//primero iteramos sOlo sobre los 8 piratas de jugadorA
	for(j = 0; j < 8; j++){
		int libre = sched_buscar_tarea_libre( index_A);//si libre == 1000 no hay slot libre
		if(libre != 1000){
			 uint32_t index_pos = consultar_buffer_pos_tesoro(index_A);
			 if (index_pos != 500){
				 //si hay slot libre y posiciOn en buffer reservamos minero y seteamos posiciOn tesoro
				 scheduler.tasks[libre].posiciOn_x_tesoro = scheduler.
				 posiciones_tesoros_A[index_pos][0];
				 scheduler.tasks[libre].posiciOn_y_tesoro = scheduler.
				 posiciones_tesoros_A[index_pos][1];
				 //seteamos lugar de buffer de posiciones monedas como disponible
				 scheduler.posiciones_tesoros_A[index_pos][0] = 100;
				 scheduler.posiciones_tesoros_A[index_pos][1] = 100;
				 //reservamos  minero
				 scheduler.tasks[libre].reservado_minero = 1;
			 }else
				break;//no hay posiciones guardadas de A
			 
		}else
			break;//si  no hay slot libre  debemos abortar bUsqueda
    }
    //luego iteramos sOlo sobre los 8 piratas de jugadorB
	for(j = 0; j < 8; j++){
		int libre = sched_buscar_tarea_libre( index_B);//si libre == 1000 no hay slot libre
		if(libre != 1000){
			 uint32_t index_pos = consultar_buffer_pos_tesoro(index_B);
			 if (index_pos != 500){
				 //si hay slot libre y posiciOn en buffer reservamos minero y seteamos posiciOn tesoro
				 scheduler.tasks[libre].posiciOn_x_tesoro = scheduler.
				 posiciones_tesoros_B[index_pos][0];
				 scheduler.tasks[libre].posiciOn_y_tesoro = scheduler.
				 posiciones_tesoros_B[index_pos][1];
				 //seteamos lugar de buffer de posiciones monedas como disponible
				 scheduler.posiciones_tesoros_B[index_pos][0] = 100;
				 scheduler.posiciones_tesoros_B[index_pos][1] = 100;
				 //reservamos minero
				 scheduler.tasks[libre].reservado_minero = 1;
			 }else
				break;//no hay posiciones guardadas de B
			 
		}else
			break;//si resultado es 1000 no hay slot libre y debemos abortar bUsqueda
    }
    
    //chequeamos si hay prOxima tarea:
   // breakpoint();
    /*
    //si no jugadores activos devolvemos selector de gdt de actual (no se actualiza current)
    if(scheduler.prox == 500)//idle es Unica activa//breakpoint();
 		return scheduler.tasks[scheduler.current].gdt_index;
	*/
	
    //si hay jugadores activos actualizamos prox y current con tarea a la que switcheamos:
    
	//current es tarea de algUn jugador y prox es a la que estamos switcheando
	//actualizamos prOxima tarea (debe ser de mismo jugador que la activa reciente)
	//if(scheduler.current == 0)
		//return scheduler.tasks[scheduler.current].gdt_index;
	//else{
	//uint16_t aux = scheduler.prox;//backup prox (lo necesitamos para current)
	uint16_t index_task;
	if(scheduler.tasks[scheduler.current].pirata != NULL){
		uint32_t index_jug = scheduler.tasks[scheduler.current].pirata->jugador->index;
		index_task = buscar_sig_tarea(index_jug );
    }else{
		
		index_task = buscar_sig_tarea(INDICE_JUGADOR_A);
		//breakpoint();
 		if(index_task == 500){
			
			index_task = buscar_sig_tarea(INDICE_JUGADOR_B);
		}
	} 
	//si hay siguiente tarea de jugador dueño de la tarea activa reciente seteamos prox
	//}
	
	if(index_task != 500){
		//breakpoint();
		scheduler.prox = index_task;
	}else{
		//breakpoint();
		return scheduler.tasks[scheduler.current].gdt_index;
		
		/*
		breakpoint();
		//si no hay tarea de jugador reciente buscamos de jugador contrario a tarea activa reciente
		//en caso de no haber tarea tampoco en este caso prox queda seteado por default (500)
		scheduler.current = aux; //para que empiece a buscar en pos siguiente a prox 
		if(index_jug == INDICE_JUGADOR_A)
			scheduler.prox = buscar_sig_tarea( INDICE_JUGADOR_B);
		else
			scheduler.prox = buscar_sig_tarea(INDICE_JUGADOR_A);
		*/	
	}
	//breakpoint();
	// actualizamos current (el valor que se retorna)
	scheduler.current = scheduler.prox;//en caso de no entrar en rama else
	
	//chequeamos si tarea dada por current actualizado estA activa o reservada 
	if(scheduler.tasks[scheduler.current].reservado_minero){
		//breakpoint();
		//inicializamos tss, seteamos como activa , pintamos , mapeamos y copiamos cOdigo
		pirata_t * morgan = scheduler.tasks[scheduler.current].pirata;
		game_pirata_relanzar(morgan,morgan->jugador,TIPO_MINERO);
		//restauramos reserva a default
		scheduler.tasks[scheduler.current].reservado_minero = 0;
		
		//pasamos las posiciones de tesoro a minero. usamos cr3 actual para mapear a pila de
		//minero estos valores
		
		page_directory_entry* cr3 = (page_directory_entry*) rcr3();
		uint32_t* origen_virtual = (uint32_t*) DIRECCION_VIRTUAL_DE_INICIO_DE_TAREAS + 10 * PAGE_SIZE;
		uint32_t desp = desplazamiento_para_calcular_la_direccion_de_la_pagina_en_el_mapa(morgan->x,morgan->y);
	    unsigned int dest_fisico = MAPA_BASE_FISICA + desp;
		//mapeamos pAgina temporaria
		mmu_mapear_pagina((uint32_t) origen_virtual,dest_fisico ,cr3,0x3);
		//copiamos posiciones a pila nivel 3 de nueva tarea ****** testear ***********
		origen_virtual[1023] = scheduler.tasks[scheduler.current].posiciOn_y_tesoro;
		origen_virtual[1022] = scheduler.tasks[scheduler.current].posiciOn_x_tesoro;
		origen_virtual[1021] = 0;//direcciOn de retorno (no usada)
		//desmapeamos pAgina temporaria
		mmu_desmapear_pagina((uint32_t) origen_virtual, cr3);
		//restauramos posiciones a default
		scheduler.tasks[scheduler.current].posiciOn_x_tesoro = 100;
		scheduler.tasks[scheduler.current].posiciOn_y_tesoro = 100;
	} else if(scheduler.tasks[scheduler.current].reservado_explorador) {
		//inicializamos tss, seteamos como activa , pintamos , mapeamos y copiamos cOdigo
		pirata_t * morgan = scheduler.tasks[scheduler.current].pirata;
		 
		game_pirata_relanzar(morgan,morgan->jugador,TIPO_EXPLORADOR);
		//restauramos reserva a default
		scheduler.tasks[scheduler.current].reservado_explorador = 0;
	}
	     
    return scheduler.tasks[scheduler.current].gdt_index; 
}

uint32_t consultar_buffer_pos_tesoro(uint32_t index_j){
	uint32_t res = 500;//devolvemos 500 si no hay posiciones almacenadas
	if(index_j == INDICE_JUGADOR_A){
		int k;
		uint16_t aux = scheduler.inicio_tesoros_A;
		for(k = 0; k < 20; k++){	
			if(scheduler.posiciones_tesoros_A[(aux+k)% 20][0] != 100){
				res = ((aux+k)% 20);
				//si hay siguiente posiciOn de moneda en buffer lo seteamos como Indice de primer lugar
				if(scheduler.posiciones_tesoros_A[(aux+1)%20][0] != 100)
					scheduler.inicio_tesoros_A = (aux+1)%20;
				break;
			}
		}
	}else{
		int k;
		uint16_t aux = scheduler.inicio_tesoros_B;
	    for(k = 0; k < 20; k++){	
			if(scheduler.posiciones_tesoros_B[(aux+k)% 20][0] != 100){
				res = ((aux+k)% 20);
				//si hay siguiente posiciOn de moneda en buffer avanzamos con Indice de primer lugar
				if(scheduler.posiciones_tesoros_B[(aux+1)%20][0] != 100)
					scheduler.inicio_tesoros_B = (aux+1)%20;
				break;
			}
		}
	}
	
	return res;
}

uint16_t buscar_sig_tarea(uint32_t index_j){
    int i = 0,indice = 0;
    bool nohay = 1;
	int j = (int) scheduler.current+1;
    for(i = 0; i < (MAX_CANT_TAREAS_VIVAS+1); i++){
	  indice = (j+i) % (MAX_CANT_TAREAS_VIVAS+1); 
	  
	  if((scheduler.tasks[indice].pirata != NULL) &&
	    (scheduler.tasks[indice].pirata->jugador->index == index_j)){
			 
		  //mismo jugador que llamador
       
		  if( (scheduler.tasks[indice].pirata->libre == 0)||
		  (scheduler.tasks[indice].reservado_minero)||
		  (scheduler.tasks[indice].reservado_explorador) ){
			 nohay = 0;
		     break;
		 }  /**/
	  }
	  
      
    }
    //si no encontramos activa retornamos valor por default
    
    if(nohay){
		//breakpoint();
		indice = 500;
	}
	
    return indice;
}

uint16_t buscar_tarea(uint16_t selector){
	int i;
    for(i = 0; i < MAX_CANT_TAREAS_VIVAS+1; i++){
	  
	  if(scheduler.tasks[i].gdt_index == selector)
 		     break;
	    
    }
    return i;
	}





