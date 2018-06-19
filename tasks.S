.text
.globl task_exec
task_exec:
        // Preámbulo estándar
        push %ebp
        movl %esp, %ebp

        push %esi            //Guardo callee-saved.

        movl 12(%esp), %esi  //Guardo el puntero a la funcion en %esi.

        movl 16(%esp), %esp  //Cambio el sp para que apunte al stack recibido.

        call *%esi           //push %eip + jmp.
next:
        leal -4(%ebp), %esp  //Restauro sp considerando que hice un push extra.
        pop %esi             //Restauro callee-saved.

        popl %ebp            //Restauro fp.
        ret
