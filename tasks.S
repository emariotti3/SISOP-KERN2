//tasks.S
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

        leal -4(%ebp), %esp  //Restauro sp considerando que hice push de %esi.
        pop %esi             //Restauro callee-saved.

        popl %ebp            //Restauro fp.
        ret

.globl task_swap
task_swap:

        push %ebx           //Guardo en el stack de la tarea actual
        push %esi           //los callee saved registers.
        push %edi
        push %ebp

        mov 20(%esp), %eax //Guardo en %eax la direccion de la variable esp.
        mov %esp, %edx     //Guardo en %edx el valor actual del sp.
        mov (%eax), %esp   //Asigno a %esp la direccion del nuevo stack.
        mov %edx, (%eax)   //Guardo el viejo sp en la la variable esp.

        pop %ebp           //Restauro los registros de la nueva
        pop %edi           //tarea en ejecución. 
        pop %esi
        pop %ebx

        ret                 //Pop + jmp al return address de la nueva tarea.
