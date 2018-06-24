#include "decls.h"
#include <stdbool.h>

/**
 * Handler para el timer (IRQ0). Escribe un carácter cada segundo.
 */
static const uint8_t hz_ratio = 18;  // Default IRQ0 freq (18.222 Hz).

void timer() {
    static char chars[81];
    static unsigned ticks;
    static int8_t line = 20;
    static uint8_t idx = 0;

    if (++ticks % hz_ratio == 0) {
        chars[idx] = '.';
        chars[++idx] = '\0';
        vga_write(chars, line, 0x07);
    }

    if (idx >= sizeof(chars)) {
        line++;
        idx = 0;
    }
}

/**
 * Mapa de "scancodes" a caracteres ASCII en un teclado QWERTY.
 */
#define LOCK_SHIFT 0x1
#define UNLOCK_SHIFT 0xFE
#define SHIFT 0x2A
#define DELTA_CAPS -32

static const uint8_t KBD_PORT = 0x60;
static uint8_t ctrl_alt_shift = 0;


static char klayout[128] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8',             // 0-9
    '9', '0', 0,   0,   0,   0,   'q', 'w', 'e', 'r',             // 10-19
    't', 'y', 'u', 'i', 'o', 'p', 0,   0,   0,   0,               // 20-29
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0,   0,          // 30-40
    0,   '^',   0,   'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.'};  // 41-52


/**
 * Handler para el teclado (IRQ1).
 *
 * Imprime la letra correspondiente por pantalla.
 */
void keyboard() {
    uint8_t code;
    static char chars[81];
    static uint8_t idx = 0;

    asm volatile("inb %1,%0" : "=a"(code) : "n"(KBD_PORT));

    if (code >= sizeof(klayout) || !klayout[code])
        return;

    //Key was released
    if(code & 0x80){
        if((code - 128)==SHIFT){
          //Shift was released
          ctrl_alt_shift = ctrl_alt_shift & UNLOCK_SHIFT;
        }
    }else{
        if(code==SHIFT){
            //Shift is pressed
            ctrl_alt_shift = ctrl_alt_shift | LOCK_SHIFT;
        }else{
            int delta = 0;
            if((ctrl_alt_shift & LOCK_SHIFT) && (klayout[code]<=0x7A) && (klayout[code]>=0x61)){
                delta = DELTA_CAPS;
            }
            if (idx == 80) {
                while (idx--)
                    chars[idx] = ' ';
            }

            chars[idx++] = klayout[code] + delta;
            vga_write(chars, 19, 0x0A);
        }
    }

}
