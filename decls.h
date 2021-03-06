#ifndef KERN2_DECL_H
#define KERN2_DECL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "lib/string.h"

// write.c (función de kern0-vga copiada no-static).
void vga_write(const char *s, int8_t linea, uint8_t color);

// Escribe en ‘s’ el valor de ‘val’ en base 10 si su anchura
// es menor que ‘bufsize’. En ese caso devuelve true, caso de
// no haber espacio suficiente no hace nada y devuelve false.
bool fmt_int(uint64_t val, char *s, size_t bufsize);

void two_stacks_c();

void two_stacks();

// Realiza una llamada a "entry" sobre el stack proporcionado.
void task_exec(uintptr_t entry, uintptr_t stack);

//Declara vga_write2 con atributo regparam, para que
//reciba argumentos via registros de la CPU.
void __attribute__((regparm(3)))
vga_write2(const char *s, int8_t linea, uint8_t color);

void __attribute__((regparm(2)))
vga_write_cyan(const char *s, int8_t linea);

#endif
