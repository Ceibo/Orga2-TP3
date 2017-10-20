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
extern page_directory
extern mmu_inicializar_dir_kernel

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
	mov fs, ax ; mov es, ax ???
	
    ; Establecer la base de la pila
	mov esp, 0x27000
	mov ebp, esp

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar el juego

    ; Inicializar pantalla
    call screen_inicializar
    call imprimir_nombre_de_grupo

    ; Inicializar el manejador de memoria

    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    ; Cargar directorio de paginas
    mov eax, page_directory
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

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

limpiar_pantalla:
	; recorrer la memoria desde 0x0 hasta el límite del segmento de
	; video (sacarlo de la GDT) e ir escribiendo cero en cada posición
	mov ebx, 0xB8000
	mov ecx, 4000
	xor eax, eax
	.ciclo:
		mov [fs:ebx], ax
		add ebx, 2
	loop .ciclo	
ret
dd


