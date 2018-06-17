#include "decls.h"
#include "multiboot.h"

#define USTACK_SIZE 4096

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);

    two_stacks();
    two_stacks_c();
}

static uint8_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
//static uint8_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));


void two_stacks_c() {
    // Inicializar al *tope* de cada pila.
    uintptr_t *a = stack1;
    //uintptr_t *b = stack2;

    // Preparar, en stack1, la llamada:
    //vga_write("vga_write() from stack1", 15, 0x57);

    *(a++) = (uintptr_t) "vga_write() from stack1";
    *(a++) = 15;
    *(a++) = 0x57;

    // Preparar, en s2, la llamada:
    //vga_write("vga_write() from stack2", 16, 0xD0);

    // AYUDA 3: para esta segunda llamada, usar esta forma de
    // asignación alternativa:
    //b -= 3;
    //b[0] = ...
    //b[1] = ...
    //b[2] = ...

    // Primera llamada usando task_exec().
    //task_exec((uintptr_t) vga_write, (uintptr_t) stack1):

    asm("push %1\n\t"
      "push %0\n\t"
      "call task_exec\n\t"
      "add $4, %%esp\n\t"
      "add $4, %%esp\n\t"
      : /* no outputs */
      : "r"((uintptr_t) stack1), "r"((uintptr_t) vga_write));

    // Segunda llamada con ASM directo. Importante: no
    // olvidar restaurar el valor de %esp al terminar, y
    // compilar con: -fasm -fno-omit-frame-pointer.
    //asm("...; call *%1; ..."
    //    : /* no outputs */
    //    : "r"(s2), "r"(vga_write));
}
