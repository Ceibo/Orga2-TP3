// Extern and Software Interrupt Service Routines

#include "eisr.h"
#include "screen.h"

void clock_interrupt_routine() {
	screen_actualizar_reloj_global();
}