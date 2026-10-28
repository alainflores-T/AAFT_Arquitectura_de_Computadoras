/* reto_integrador_terminal.c  (version corta)
 * Teclado -> Puerto -> Cola FIFO en RAM -> VRAM 4x4, con backspace.
 * Compilar: gcc -std=c99 -Wall -Wextra -pedantic reto_integrador_terminal.c -o terminal
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define COLA_BASE_DIR  0x70
#define COLA_CAPACIDAD 8
#define DIR_KB_HEAD    0x78
#define DIR_KB_TAIL    0x79
#define DIR_KB_COUNT   0x7A
#define VRAM_BASE_DIR  0xF0

typedef struct { uint8_t address_bus, data_bus; bool io_read, mem_read; } SystemBuses;
typedef struct { uint8_t reg_status, reg_data; } TecladoHardware;

static SystemBuses     g_bus;
static TecladoHardware g_teclado;
static uint8_t         g_ram[256];
static uint16_t        cursor_vram = VRAM_BASE_DIR;  /* uint16_t: evita 0xFF+1 = 0x00 */

/* ---------- Exp 1: puerto ---------- */
void tick_controlador_teclado(SystemBuses *bus, TecladoHardware *kb)
{
    if (bus->io_read) {
        if (bus->address_bus == 0x01) bus->data_bus = kb->reg_status;
        else if (bus->address_bus == 0x02) {
            bus->data_bus = kb->reg_data;
            kb->reg_status = 0;                      /* Handshake */
        }
    }
}

uint8_t cpu_instruccion_in(uint8_t puerto)
{
    g_bus.address_bus = puerto;
    g_bus.io_read = true;
    g_bus.mem_read = false;
    tick_controlador_teclado(&g_bus, &g_teclado);
    g_bus.io_read = false;
    return g_bus.data_bus;
}

/* ---------- Exp 2: cola ---------- */
bool cola_teclado_encolar(uint8_t ascii)
{
    uint8_t tail = g_ram[DIR_KB_TAIL], count = g_ram[DIR_KB_COUNT];
    if (count >= COLA_CAPACIDAD) return false;
    g_ram[COLA_BASE_DIR + tail] = ascii;
    g_ram[DIR_KB_TAIL]  = (uint8_t)((tail + 1) % COLA_CAPACIDAD);
    g_ram[DIR_KB_COUNT] = (uint8_t)(count + 1);
    return true;
}

bool cola_teclado_desencolar(uint8_t *salida)
{
    uint8_t head = g_ram[DIR_KB_HEAD], count = g_ram[DIR_KB_COUNT];
    if (count == 0) return false;
    *salida = g_ram[COLA_BASE_DIR + head];
    g_ram[DIR_KB_HEAD]  = (uint8_t)((head + 1) % COLA_CAPACIDAD);
    g_ram[DIR_KB_COUNT] = (uint8_t)(count - 1);
    return true;
}

/* ---------- Exp 3: VRAM ---------- */
void vram_mostrar(const char *titulo)
{
    int f, c;
    printf("%s\n +----+\n", titulo);
    for (f = 0; f < 4; f++) {
        printf(" |");
        for (c = 0; c < 4; c++) putchar(g_ram[VRAM_BASE_DIR + f * 4 + c]);
        printf("|\n");
    }
    printf(" +----+  cursor = 0x%02X\n\n", (unsigned)cursor_vram);
}

/* ---------- Reto: terminal ---------- */
void terminal_procesar_siguiente_tecla(void)
{
    uint8_t tecla;
    if (!cola_teclado_desencolar(&tecla)) return;

    if (tecla == 0x08 || tecla == 0x7F) {            /* Backspace */
        if (cursor_vram > VRAM_BASE_DIR) {
            cursor_vram--;
            g_ram[cursor_vram] = ' ';
        }
    } else if (tecla >= 32 && tecla <= 126 && cursor_vram < 0x100) {
        g_ram[cursor_vram] = tecla;                  /* Imprimible */
        cursor_vram++;
    }
}

/* Recorrido completo de una tecla: teclado -> puerto -> cola -> VRAM */
void teclear(uint8_t ascii)
{
    g_teclado.reg_data = ascii;                      /* Usuario presiona */
    g_teclado.reg_status = 1;

    if (cpu_instruccion_in(0x01) & 0x01)             /* IN 0x01: hay tecla? */
        cola_teclado_encolar(cpu_instruccion_in(0x02)); /* IN 0x02 -> cola */

    terminal_procesar_siguiente_tecla();             /* cola -> VRAM */
}

int main(void)
{
    const char *texto = "HOLA";
    int i;

    for (i = 0; i < 16; i++) g_ram[VRAM_BASE_DIR + i] = ' ';

    for (i = 0; texto[i] != '\0'; i++) teclear((uint8_t)texto[i]);
    vram_mostrar("1) Se escribe \"HOLA\":");

    teclear(0x08);
    vram_mostrar("2) Backspace (borra la A):");

    teclear('X');
    vram_mostrar("3) Se escribe 'X':");

    return 0;
}
