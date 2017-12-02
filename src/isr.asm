; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_tick
extern sched_tarea_actual

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
extern print_isr

%macro ISR 1
global _isr%1

_isr%1:
    xchg bx, bx; magic breakpoint ******

	push %1
    call print_isr
    add esp, 4

    mov eax, %1
    jmp $
    iret

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

global _isr32 ; (5)
extern game_tick
 
_isr32:
	call fin_intr_pic1; comunicamos al pic que ya se atendiO la interrupciOn permitiendo nuevas llamadas desde el dispositivo.
	call game_tick; rutina de reloj
	iret; retornamos de la interrupciOn habilitando interrupciones.
	;mov eax, 32
    ;jmp $

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
;BITS 32
global _isr33 ; (5)
  
 extern game_atender_teclado
_isr33:
	call fin_intr_pic1; comunicamos al pic que ya se atendiO la interrupciOn permitiendo nuevas llamadas desde el dispositivo.
	 

	push eax; guardamos registro
	in al,0x60; leemos teclado
	push eax; pasamos parAmetro a procedimiento siguiente
	call game_atender_teclado; rutina de teclado
 	add esp,4
	pop eax
	iret;
;;
;; Rutinas de atención de las SYSCALLS
global _isr46 ; (5)
extern game_syscall_pirata_mover
extern game_pirata_exploto
extern posiciOn_x_tesoro
extern posiciOn_y_tesoro
extern sched_buscar_tarea_libre

_isr46:
	 ;xchg bx, bx; magic breakpoint ******

	 pushad
	 cmp eax,0x1
	 je .mover
	 cmp eax ,0x2
	 je .cavar
	 cmp eax ,0x3
	 je .posicion
	 ;jmp .desalojar; argumento invAlido
	 
.mover:
 	push ecx; direccion a mover : arriba, abajo, der, izq.
	;invocar funciOn que devuelva id de pirata actual
	push 15; 1er tarea (temporal)
	call game_syscall_pirata_mover;  retorna en eax : -1,0 o 1
    add esp,8
    cmp eax,0
    je .fin; si eax = 0 entonces retornar 
    cmp eax,-1
    ;je .kill; si eax = -1 entonces matar tarea
    ;cmp eax ,1
    je .minero; si eax = 1 entonces switch a minero
    jmp .fin
    
.minero:
	call sched_buscar_tarea_libre; eax contiene Indice en arreglo de scheduler de tarea libre
	cmp eax,1000;  
	;je .guardar_id_actual
;si eax == 1000 entonces guardar posiciOn de tesoro descubierta 
	;para cuando alguno se libere (Indice libre correspondiente a pirata asociado a jugador llamador)
	;esto se almacena en array posiciones_tesoros correspondiente a jugador llamador en 
	;estructura de scheduler. debe buscar posiciones en array con valores igual a 100.
	; si no hay posiciones libres (con 100) se descartan.
	
;si eax != 1000 entonces eax tiene Indice vAlido en scheduler asociado a jugador llamador
	;debo guardar en sched_task_t asociado a Indice la posiciOn de tesoro y setear 
	;slot como resevado para minero.
	
	;invocar funciOn que devuelva id de pirata actual en eax
	mov ebx,eax; ebx es copia de id (se mantiene por convenciOn c)
	push ebx; 1er parAmetro posiciOn_x_tesoro
	call posiciOn_x_tesoro; eax es posiciOn x de tesoro
	mov esi, eax; esi es respaldo de posiciOn x de tesoro
	add esp,4
	push ebx; 1er parAmetro posiciOn_y_tesoro
	call posiciOn_y_tesoro; eax es posiciOn y de tesoro
	add esp,4
    
.cavar:
	 
.posicion:
.kill:
     ;invocar funciOn que devuelva id de pirata actual
	push 15;id
	call game_pirata_exploto
	jmp .fin

;.desalojar: ; dividir por 0 para que la excepciOn la mate
.fin:
	popad
	iret;

;; -------------------------------------------------------------------------- ;;


