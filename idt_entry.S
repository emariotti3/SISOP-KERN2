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
.data
breakpoint_msg:
    .asciz "Hello, breakpoint"
