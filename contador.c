#include "decls.h"

#define COUNTLEN 20
#define TICKS (1ULL << 15)
#define DELAY(x) (TICKS << (x))
#define USTACK_SIZE 4096

static volatile char *const VGABUF = (volatile void *) 0xb8000;

static uintptr_t esp;
static uint8_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
static uint8_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));

static void yield() {
    if (esp)
        task_swap(&esp);
}

static void exit() {
    uintptr_t *tmp = esp;
    esp = NULL;
    task_swap(&tmp);
}

static void contador_yield(unsigned lim, uint8_t linea, char color) {
    char counter[COUNTLEN] = {'0'};  // ASCII digit counter (RTL).

    while (lim--) {
        char *c = &counter[COUNTLEN];
        volatile char *buf = VGABUF + 160 * linea + 2 * (80 - COUNTLEN);

        unsigned p = 0;
        unsigned long long i = 0;

        while (i++ < DELAY(6))  // Usar un entero menor si va demasiado lento.
            ;

        while (counter[p] == '9') {
            counter[p++] = '0';
        }

        if (!counter[p]++) {
            counter[p] = '1';
        }

        while (c-- > counter) {
            *buf++ = *c;
            *buf++ = color;
        }
        //Concurrencia cooperativa con yield:
        //yield();
    }
    //Llamado explicito a exit:
    //if(esp) exit();
}

void contador_run() {
    // Configurar stack1 y stack2 con los valores apropiados.
    uint8_t *aux_a = stack1 + USTACK_SIZE;
    uint8_t *aux_b = stack2 + USTACK_SIZE;
    uintptr_t *a = (uintptr_t*)aux_a;
    uintptr_t *b = (uintptr_t*)aux_b;

    *(--a) = 0x2F;
    *(--a) = 0;
    *(--a) = 100;

    *(--b) = 0x4F;
    *(--b) = 1;
    *(--b) = 50;
    *(--b) = (uintptr_t) exit;            //Direccion de retorno de contador_yield.
    *(--b) = (uintptr_t) contador_yield;  //Direccion de retorno de yield.
    *(--b) = 0;                           //Inicializo %ebx.
    *(--b) = 0;                           //Inicializo %esi.
    *(--b) = 0;                           //Inicializo %edi.
    *(--b) = (uintptr_t*)aux_b;           //Inicializo %ebp en el tope de stack2.

    // Actualizar la variable estática ‘esp’ para que apunte
    // al del segundo contador.
    esp = b;

    // Lanzar el primer contador con task_exec.
    task_exec((uintptr_t) contador_yield, (uintptr_t) a);
}


static void contador1() {
    contador_yield(15, 2, 0x2F);
}

static void contador2() {
    contador_yield(15, 3, 0x6F);
}

static void contador3() {
    contador_yield(15, 4, 0x4F);
}

void contador_spawn() {
    spawn(contador1);
    spawn(contador2);
    spawn(contador3);
}
