/* exp1B_puerto_teclado.c
 * Experimento 1 - Version B:
 *   El usuario escribe normalmente en el teclado y la CPU LEE el puerto
 *   con la instruccion IN (activa IO_READ en el bus).
 * Compilar: gcc -std=c99 -Wall -Wextra -pedantic exp1B_puerto_teclado.c -o exp1B
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define PUERTO_KB_STATUS 0x01   /* Bit 0 = 1 -> tecla lista */
#define PUERTO_KB_DATA   0x02   /* Codigo ASCII de la tecla */

/* ---------- Modelo del hardware ---------- */
typedef struct {
    uint8_t address_bus;   /* Bus de direcciones */
    uint8_t data_bus;      /* Bus de datos */
    bool    io_read;       /* Bus de control: IO_READ   */
    bool    io_write;      /* Bus de control: IO_WRITE  */
    bool    mem_read;      /* Bus de control: MEM_READ  */
    bool    mem_write;     /* Bus de control: MEM_WRITE */
} SystemBuses;

typedef struct {
    uint8_t reg_status;
    uint8_t reg_data;
} TecladoHardware;

static SystemBuses     g_bus;
static TecladoHardware g_teclado;

/* ---------- Prototipos ---------- */
void    tick_controlador_teclado(SystemBuses *bus, TecladoHardware *kb);
uint8_t cpu_instruccion_in(uint8_t puerto);
void    teclado_recibir_tecla(TecladoHardware *kb, uint8_t ascii);
void    mostrar_bus(void);

/* ---------- Controlador del teclado ---------- */
void tick_controlador_teclado(SystemBuses *bus, TecladoHardware *kb)
{
    if (bus->io_read) {
        if (bus->address_bus == PUERTO_KB_STATUS) {
            bus->data_bus = kb->reg_status;
        } else if (bus->address_bus == PUERTO_KB_DATA) {
            bus->data_bus = kb->reg_data;
            kb->reg_status = 0;          /* Handshake automatico */
        }
    }
}

/* ---------- Instruccion IN R, [puerto] ---------- */
uint8_t cpu_instruccion_in(uint8_t puerto)
{
    uint8_t dato;

    g_bus.address_bus = puerto;
    g_bus.io_read   = true;          /* IO_READ = 1  */
    g_bus.io_write  = false;
    g_bus.mem_read  = false;         /* MEM_READ = 0 -> la RAM ignora el ciclo */
    g_bus.mem_write = false;

    tick_controlador_teclado(&g_bus, &g_teclado);

    dato = g_bus.data_bus;
    if (puerto == PUERTO_KB_DATA)
        mostrar_bus();               /* Foto del bus durante el ciclo IN [0x02] */
    g_bus.io_read = false;           /* Fin del ciclo */
    return dato;
}

/* ---------- El hardware del teclado deja la tecla en sus registros ---------- */
void teclado_recibir_tecla(TecladoHardware *kb, uint8_t ascii)
{
    kb->reg_data   = ascii;
    kb->reg_status = 0x01;           /* Bit 0 = tecla lista */
}

/* ---------- Muestra el estado del bus ---------- */
void mostrar_bus(void)
{
    printf("  Bus -> ADDR=0x%02X DATA=0x%02X | IO_READ=%d IO_WRITE=%d MEM_READ=%d MEM_WRITE=%d\n",
           (unsigned)g_bus.address_bus, (unsigned)g_bus.data_bus,
           g_bus.io_read, g_bus.io_write, g_bus.mem_read, g_bus.mem_write);
}

/* ---------- Programa principal ---------- */
int main(void)
{
    int ch;
    uint8_t status, dato;

    printf("=== Experimento 1 - Version B: teclado -> IN desde el PUERTO ===\n");
    printf("Escribe un texto y presiona Enter (Ctrl+D para salir):\n");

    while ((ch = getchar()) != EOF) {
        if (ch == '\n') {
            printf("\n");
            continue;
        }

        /* 1) Se presiono una tecla: el hardware la pone en el puerto */
        teclado_recibir_tecla(&g_teclado, (uint8_t)ch);
        printf("Tecla presionada\n");

        /* 2) ESPERAR_TECLA: IN R1,[0x01] / CMP R1,0 / JZ ESPERAR_TECLA */
        do {
            status = cpu_instruccion_in(PUERTO_KB_STATUS);
        } while ((status & 0x01) == 0);
        printf("  IN R1, [0x01] -> 0x%02X (hay tecla lista)\n", (unsigned)status);

        /* 3) LEER_TECLA: IN R2,[0x02] */
        printf("  IN R2, [0x02]\n");
        dato = cpu_instruccion_in(PUERTO_KB_DATA);
        printf("  R2 = 0x%02X '%c'\n", (unsigned)dato, (char)dato);

        /* 4) Verificar handshake */
        status = cpu_instruccion_in(PUERTO_KB_STATUS);
        printf("  IN R1, [0x01] -> 0x%02X %s\n\n", (unsigned)status,
               status == 0 ? "(handshake OK: puerto limpio)" : "(FALLO)");
    }

    printf("Fin.\n");
    return 0;
}
