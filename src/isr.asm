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

;;debug
extern debugger

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
extern print_isr
extern game_pirata_exploto
extern game_id_pirata_actual

%macro ISR 1
global _isr%1

_isr%1:
    ;guardamos en estructura debugger info de pirata muerto

	mov dword [debugger + 00], eax
    mov dword [debugger + 04], ebx
    mov dword [debugger + 08], ecx
    mov dword [debugger + 12], edx
    mov dword [debugger + 16], esi
    mov dword [debugger + 20], edi
    mov dword [debugger + 24], ebp
    mov dword [debugger + 28], esp
 
	mov eax, [esp+12] ; eip
	mov dword [debugger + 32], eax ;eip  \
		
	mov ax, cs
	mov word [debugger + 36], ax
	mov ax, ds
	mov word [debugger + 38], ax
	mov ax, es
	mov word [debugger + 40], ax
	mov ax, fs
	mov word [debugger + 42], ax
	mov ax, gs
	mov word [debugger + 44], ax
	mov ax, ss
	mov word [debugger + 46], ax
    
    xor eax, eax
    pushf    ; obtenemos el registro
    pop ax   ; eflags
    mov dword [debugger + 48], eax
    
    mov eax, cr0
  	mov dword [debugger + 52], eax
  	mov eax, cr2
  	mov dword [debugger + 56], eax
  	mov eax, cr3
  	mov dword [debugger + 60], eax
  	mov eax, cr4
  	mov dword [debugger + 64], eax
  	
  	mov eax, [esp]
  	mov dword [debugger + 68], eax
  	mov eax, [esp+4]
  	mov dword [debugger + 72], eax
  	mov eax, [esp+8]
  	mov dword [debugger + 76], eax
  	mov eax, [esp+12]
  	mov dword [debugger + 80], eax
    mov eax, [esp+16]
  	mov dword [debugger + 84], eax


    
    call game_id_pirata_actual; ax tiene id de pirata actual
	shl eax,16
	shr eax,16; limpiamos word de arriba de eax
 	push eax;
 	sti
	call game_pirata_exploto
	add esp,4
    
     
    jmp 0x70:0 ;voy a idle

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
extern sched_atender_tick
extern debugger_activo

_isr32:
	pushad
	;pushfd;agregado *****
	call fin_intr_pic1; comunicamos al pic que ya se atendiO la interrupciOn permitiendo 
	;nuevas llamadas desde el dispositivo.
	;xchg bx, bx; magic breakpoint *****
	call debugger_activo
	cmp eax, 1
    je .fin
    xor eax, eax
    
	call sched_atender_tick; rutina de sched-reloj , ax es selector a siguiente tarea
	
	str cx
	cmp ax,cx; si siguiente es igual a actual no se switchea
	je .fin
	;xchg bx, bx; magic breakpoint *****
	mov [sched_tarea_selector],ax
	jmp far [sched_tarea_offset]
	
.fin:
    ;popfd;agregado *****
	popad
	iret; retornamos de la interrupciOn habilitando interrupciones.
	 


;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
;BITS 32
global _isr33 ; (5)
  
 extern game_atender_teclado
  
_isr33:
	pushad
	;pushfd;agregado *****
	;nuevas llamadas desde el dispositivo.
	xor eax,eax
	in al,0x60; leemos teclado
	;test al,10000000b
	;jnz .fin; no hay informaciOn util. finalizamos
	push eax; pasamos parAmetro a procedimiento siguiente
	;xchg bx, bx; magic breakpoint *****
	call game_atender_teclado; rutina de teclado
 	add esp,4
 	call fin_intr_pic1; comunicamos al pic que ya se atendiO la interrupciOn permitiendo 

.fin:
    ;popfd;agregado *****
	popad
	iret;
	
 
;; Rutinas de atención de las SYSCALLS
;;----------------------------------------------------------------------------------;;
global _isr70 ; (5)
extern game_syscall_pirata_mover
extern game_pirata_exploto
extern game_syscall_cavar
extern game_id_pirata_actual
extern game_syscall_pirata_posicion

_isr70:
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
	mov ebx,ecx; backup direcciOn
	call game_id_pirata_actual; ax tiene id de pirata actual
	shl eax,16
	shr eax,16; limpiamos word de arriba de eax
 	push ebx; direccion a mover : arriba, abajo, der, izq.	
	push eax; id de pirata actual
	call game_syscall_pirata_mover;  retorna en eax : -1,0
    add esp,8
    ;cmp eax,0
    ;je .fin; si eax = 0 entonces retornar 
    ;cmp eax,-1
    ;je .kill; si eax = -1 entonces matar tarea
    jmp .fin
    
.cavar:
    call game_id_pirata_actual; ax tiene id de pirata actual
	shl eax,16
	shr eax,16; limpiamos word de arriba de eax
 	push eax; id de pirata actual
    call game_syscall_cavar
    add esp,4
	jmp .fin
	
.posicion:
    mov ebx,ecx; backup caso
	call game_id_pirata_actual; ax tiene id de pirata actual
	shl eax,16
	shr eax,16; limpiamos word de arriba de eax
 	push ebx; caso a consultar	
	push eax; id de pirata actual
    call game_syscall_pirata_posicion; eax es posiciOn: 8 bits menos significativos son x,los siguientes son y
	add esp,8
	mov word [sched_tarea_selector],0x70; saltamos a la idle
	jmp far [sched_tarea_offset]
 	pop edi
 	pop esi
 	pop ebp
 	add esp,4
 	pop ebx
 	pop edx
 	pop ecx
 	add esp,4; ignoramos eax guardada (ahI estA posiciOn)
	iret;
	
;.desalojar: ; dividir por 0 para que la excepciOn la mate
.fin:
	;saltar a idle seteando parAmetro prox de scheduler con siguiente tarea de jugador contrario
	;o en caso de no haber otro jugador la que siga (incluso la misma) tal que en prOxima
	; interrupciOn de reloj al chequearse si current es 0 se usa ese prox para saltar a esa tarea
    ; manteniendo la interrupciOn 0x46 transparente
     
	
	mov word [sched_tarea_selector],0x70; saltamos a la idle
	jmp far [sched_tarea_offset]
 	popad
	iret;

;; -------------------------------------------------------------------------- ;;


