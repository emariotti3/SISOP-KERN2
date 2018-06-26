#include "decls.h"

static volatile void* const VGABUF = (volatile char*) 0xb8000;

#define VGA_COLS 80
#define VGA_FILAS 25
#define STEP 2
#define BASE 10

void vga_write_cyan(const char *s, int8_t linea) {
    vga_write(s, linea, 0xB0);
}

void vga_write(const char *s, int8_t linea, uint8_t color) {
    volatile char *buf = VGABUF;
    if(linea >= VGA_FILAS) return;
    if(linea < 0) linea = VGA_FILAS + linea - 1;
    buf+=linea*STEP*VGA_COLS;
    int8_t linea_actual = linea;
    int16_t actual = 0;
    while(s[actual] && (linea_actual < VGA_FILAS)){
        *buf++ = s[actual];
        *buf++ = color;
        actual++;
        linea_actual = ((actual%VGA_COLS)==0)? linea_actual+1:linea_actual;
    }
}

static size_t int_width(uint64_t val) {
    size_t base = 10, count = 0;
    uint64_t rmndr = 0;
    while(rmndr < val){
        rmndr = val % base;
        base = base * BASE;
        count++;
    }
    return count;
}

bool fmt_int(uint64_t val, char *s, size_t bufsize) {
    size_t l = int_width(val), c = 0;

    if (l >= bufsize)
        return false;

    s += l;
    *(s--) = '\0';
    size_t base = BASE, rmndr = 0, dec = 1;
    while(c < l){
        *(s--) = (char)(((val % base) - rmndr) / dec)+ '0';
        rmndr = val % base;
        dec = dec * BASE;
        base = base * BASE;
        c++;
    }
    return true;
}
