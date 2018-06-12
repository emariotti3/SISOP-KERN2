#include "decls.h"
#include "multiboot.h"
#include "lib/string.h"

#define BUF_SZ 256

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);
    if (mbi->flags) {
        char buf[BUF_SZ] = "cmdline: ";
        char *cmdline = (void *) mbi->cmdline;
        strlcat(buf, cmdline, BUF_SZ);
        vga_write(buf, 9, 0x07);
    }
}
