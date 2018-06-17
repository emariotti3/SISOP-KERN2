.text
.globl task_exec
task_exec:
        // Preámbulo estándar
        push %ebp
        movl %esp, %ebp

        push %esi            //Guardo callee-saved.
        push %edi            //Guardo callee-saved.
        leal (%esp), %edi    //Guardo el sp actual en un callee-saved.

        movl 16(%esp), %esi  //Guardo el puntero a la funcion en %esi.

        movl 20(%esp), %esp  //Cambio el sp para que apunte al stack recibido.
        movl %esp, %ebp      //Cambio el fp para que apunte al stack recibido.

        call *%esi           //push %eip + jmp.
next:
        leal (%edi), %esp    //Restauro sp.

        pop %edi             //Restauro callee-saved.
        pop %esi             //Restauro callee-saved.

        popl %ebp           //Restauro fp.
        ret
