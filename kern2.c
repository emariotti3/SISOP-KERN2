#include "decls.h"
#include "multiboot.h"
#include "interrupts.h"

#ifndef USTACK_SIZE
#define USTACK_SIZE 4096
#endif

static uint8_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
static uint8_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));

void kmain(const multiboot_info_t *mbi) {
    int8_t linea;
    uint8_t color;

    vga_write("kern2 loading.............", 8, 0x70);

    two_stacks();
    two_stacks_c();
    //contador_run();
    sched_init();
    contador_spawn();

    idt_init();
    irq_init();
    asm("int3");

    asm("div %4"
        : "=a"(linea), "=c"(color)
        : "0"(18), "1"(0xE0), "b"(0), "d"(0));

    vga_write2("Funciona vga_write2?", 18, 0xE0);


}

void two_stacks_c() {
    // Inicializar al *tope* de cada pila.
    uintptr_t *a = stack1+USTACK_SIZE;
    uintptr_t *b = stack2+USTACK_SIZE;

    // Preparar, en stack1, la llamada:
    //vga_write("vga_write() from stack1", 15, 0x57);
    *(--a) = 0x57;
    *(--a) = 15;
    *(--a) = (uintptr_t) "vga_write() from stack1";

    // Preparar, en s2, la llamada:
    //vga_write("vga_write() from stack2", 16, 0xD0);

    // AYUDA 3: para esta segunda llamada, usar esta forma de
    // asignación alternativa:
    b -= 3;
    b[2] = (uintptr_t) "vga_write() from stack2";
    b[1] = 16;
    b[0] = 0xD0;

    // Primera llamada usando task_exec().
    task_exec((uintptr_t) vga_write, (uintptr_t) a);

    // Segunda llamada con ASM directo. Importante: no
    // olvidar restaurar el valor de %esp al terminar, y
    // compilar con: -fasm -fno-omit-frame-pointer.
    asm("push (%0)\n\t"
      "push 4(%0)\n\t"
      "push 8(%0)\n\t"
      "call *%1\n\t"
      "add $4, %%esp\n\t"
      "add $4, %%esp\n\t"
      "add $4, %%esp\n\t"
        : /* no outputs */
        : "r"(b), "r"((uintptr_t) vga_write));
}
