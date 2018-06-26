#include "decls.h"
#include "sched.h"

#define MAX_TASK 10

// Multiboot siempre define "8" como el segmento de código.
// (Ver campo CS en `info registers` de QEMU.)
static const uint8_t KSEG_CODE = 8;
static struct Task Tasks[MAX_TASK];
static struct Task *current;

void sched_init() {
    Tasks[0].status = RUNNING;
    current = &Tasks[0];
}

void spawn(void (*entry)(void)) {
    int i=0;
    for(; i<10 && Tasks[i].status!=FREE;i++){}
    uint32_t *reg_ptr = &Tasks[i].stack[USTACK_SIZE];

    Tasks[i].status = READY;

    *(--reg_ptr) = task_kill;
    *(--reg_ptr) = 0x0200;
    *(--reg_ptr) = KSEG_CODE;
    *(--reg_ptr) = entry;

    *(--reg_ptr) = 0;
    *(--reg_ptr) = 0;
    *(--reg_ptr) = 0;
    *(--reg_ptr) = 0;
    *(--reg_ptr) = 0;
    *(--reg_ptr) = 0;
    *(--reg_ptr) = 0;
    *(--reg_ptr) = 0;


    Tasks[i].frame = (struct TaskFrame*) reg_ptr;
}

void sched(struct TaskFrame *tf) {
    struct Task *old = current;
    struct Task *new = old;

    bool found = false;
    for(int i = 0;!found && i<MAX_TASK; i++){
        new++;
        if(new==old) new++;
        if (new >= (Tasks+MAX_TASK)) new=Tasks;
        found = (new->status == READY);
    }

    if(!found) return;

    old->status = tf? READY : FREE;
    old->frame = tf;

    current = new;
    current->status = RUNNING;

    asm("movl %0, %%esp\n"
    "popa\n"
    "iret\n"
    :
    : "g"(current->frame)
    : "memory");
}

void task_kill(){
    sched(NULL);
}
