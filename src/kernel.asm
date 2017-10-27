; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

extern GDT_DESC
extern IDT_DESC
extern idt_inicializar
extern screen_inicializar
extern imprimir_nombre_de_grupo
extern mmu_inicializar
extern mmu_inicializar_dir_kernel
extern mmu_inicializar_dir_pirata
extern page_directory_kernel ; es una referencia al puntero de C, o sea un doble puntero a page_directory_entry
extern page_table_kernel_0

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    ;0x07 -> color
    ;0 -> fila
    ;0 -> columna
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

    ; Habilitar A20
	  call habilitar_A20

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Saltar a modo protegido
    jmp 0x40:modoprotegido

	BITS 32
	modoprotegido:

    ; Establecer selectores de segmentos
	xor eax, eax
	
	mov ax, 0x48 ; carga segmento de datos de nivel cero
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov gs, ax

	mov ax, 0x60 ; carga segmento de video
	mov fs, ax
	
    ; Establecer la base de la pila
	mov esp, 0x27000
	mov ebp, esp

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar el juego

    ; Inicializar pantalla  
    call limpiar_pantalla
    call screen_inicializar
    call imprimir_nombre_de_grupo

    ; Inicializar el manejador de memoria
    call mmu_inicializar

    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel
    call mmu_inicializar_dir_pirata

    ; Cargar directorio de paginas
    mov eax, [page_directory_kernel]
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ;call identity_mapping

    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar

    ; Cargar IDT
    lidt [IDT_DESC]

    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones
    sti

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;
%include "a20.asm"

BITS 32
limpiar_pantalla:
	; recorrer la memoria desde 0x0 hasta el límite del segmento de
	; video (sacarlo de la GDT) e ir escribiendo cero en cada posición
	xor ebx, ebx
	mov ecx, 4000  
	.ciclo:
		mov byte [fs:ebx], 219; 219: caracter ascii de celda completa
		inc ebx
		mov byte [fs:ebx], 0; color negro
        inc ebx
	loop .ciclo	
ret

identity_mapping:
    mov ecx, 1024
    mov ebx, [page_directory_kernel] ; ebx = page directory address

    .pd:
        mov dword [ebx+ecx*4-4], 0x00000002
    loop .pd    

    mov eax, [page_table_kernel_0] ; eax = page table address

    lea edx, [eax + 0x3]
    mov [ebx], edx

    mov ecx, 1024
    mov ebx, [eax]
    mov edx, 0x1000 * 1023
    
    .pt:
        mov [ebx+ecx*4-4], edx
        add dword [ebx+ecx*4-4], 0x00000003
        sub edx, 0x1000
    loop .pt

    mov dword [eax+512*4], 0x000B8003
ret
