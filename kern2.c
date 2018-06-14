#include "decls.h"
#include "multiboot.h"
#include "lib/string.h"

#define BUF_SZ 125
#define SHIFT_MIB 11

char mem[256] = "Physical memory: ";
char tmp[64] = "";



void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);
    if (mbi->flags) {
        char buf[BUF_SZ] = "cmdline: ";
        char *cmdline = (void *) mbi->cmdline;
        strlcat(buf, cmdline, BUF_SZ);
        vga_write(buf, 9, 0x07);
    }
    /*if (fmt_int(0 , tmp, sizeof tmp)) { //TOTAL
        strlcat(mem, tmp, sizeof mem);
        strlcat(mem, "MiB total", sizeof mem);
    }
    if (fmt_int(mbi->mem_lower + mbi->mem_upper , tmp, sizeof tmp)) {
        strlcat(mem, "(", sizeof mem);
        strlcat(mem, tmp, sizeof mem);
        strlcat(mem, "KiB base, ", sizeof mem);
    }*/
    vga_write(mem, 10, 0x07);
}
