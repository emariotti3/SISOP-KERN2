#define PIC1 0x20
#define ACK_IRQ 0x20

.data
breakpoint_msg:
    .asciz "Hello, breakpoint"
divzero_msg:
    .asciz "Se divide por ++ebx"

.text
.globl ack_irq
ack_irq:
    // Indicar que se manejó la interrupción.
    movl $ACK_IRQ, %eax
    outb %al, $PIC1
    iret

.globl breakpoint
breakpoint:
    push %eax
    push %edx
    push %ecx
    mov $breakpoint_msg, %eax
    mov $14, %edx
    mov $0xB0, %ecx
    call vga_write2
    pop %ecx
    pop %edx
    pop %eax
    iret

.globl timer_asm
timer_asm:
    // Guardar registros.
    push %eax
    push %edx
    push %ecx

    call timer

    // Restaurar registros.
    pop %ecx
    pop %edx
    pop %eax

    jmp ack_irq

.global divzero
divzero:
    push %eax
    push %edx
    push %ecx
    mov $divzero_msg, %eax
    mov $17, %edx
    call vga_write_cyan
    add $1, %ebx
    pop %ecx
    pop %edx
    pop %eax
    iret

.globl keyboard_asm
keyboard_asm:
    // Guardar registros.
    push %eax
    push %edx
    push %ecx

    call keyboard

    // Restaurar registros.
    pop %ecx
    pop %edx
    pop %eax
    jmp ack_irq
