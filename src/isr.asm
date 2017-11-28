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

	push %1
    call print_isr
    add esp, 4

    mov eax, %1
    ;jmp $
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
_isr46:
	 cmp eax,0x1
	 je .mover
	 cmp eax ,0x2
	 je .cavar
	 cmp eax ,0x3
	 je .posicion
	 ;jmp .desalojar; argumento invAlido
	 
.mover:
	push ecx; direccion a mover
	;invocar funciOn que devuelva id de pirata actual
	push 0; 1er tarea
	call game_syscall_pirata_mover
	 
.cavar:
.posicion:
	
;.desalojar: ; dividir por 0 para que la excepciOn la mate
.fin:
	pop eax
	iret;

;; -------------------------------------------------------------------------- ;;


