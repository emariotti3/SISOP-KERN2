#ifndef SCHED_H
#define SCHED_H

#define USTACK_SIZE 4096

enum TaskStatus {
    FREE = 0,
    READY,
    RUNNING,
    DYING,
};

struct TaskFrame {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    /* below here defined by x86 hardware */
    uint32_t eip;
    uint16_t cs;
    uint16_t padding;
    uint32_t eflags;
    uint32_t kill_fn;
} __attribute__((packed));

struct Task {
    uint8_t stack[USTACK_SIZE];
    enum TaskStatus status;
    struct TaskFrame *frame;
};

void task_kill();

#endif //SCHED_H
