#include "decls.h"
#include "multiboot.h"
#include "lib/string.h"


void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);

    // A remplazar por una llamada a two_stacks(),
    // definida en stacks.S.
    asm("call two_stacks");
}
