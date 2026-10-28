/* exp1A_puerto_teclado.c
 * Experimento 1 - Version A:
 *   Se LEE la tecla con una funcion de C (getchar) y la CPU la ESCRIBE
 *   en el puerto con la instruccion OUT (activa IO_WRITE en el bus).
 * Compilar: gcc -std=c99 -Wall -Wextra -pedantic exp1A_puerto_teclado.c -o exp1A
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define PUERTO_KB_STATUS 0x01   /* Bit 0 = 1 -> dato disponible */
#define PUERTO_KB_DATA   0x02   /* Codigo ASCII */

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
void tick_controlador_teclado(SystemBuses *bus, TecladoHardware *kb);
void cpu_instruccion_out(uint8_t puerto, uint8_t valor);
void mostrar_bus(void);

/* ---------- Controlador del teclado (responde solo a senales de E/S) ---------- */
void tick_controlador_teclado(SystemBuses *bus, TecladoHardware *kb)
{
    if (bus->io_write) {
        if (bus->address_bus == PUERTO_KB_DATA) {
            kb->reg_data   = bus->data_bus;   /* Guarda el dato en el puerto */
            kb->reg_status = 0x01;            /* Marca dato disponible      */
        }
    } else if (bus->io_read) {
        if (bus->address_bus == PUERTO_KB_STATUS) {
            bus->data_bus = kb->reg_status;
        } else if (bus->address_bus == PUERTO_KB_DATA) {
            bus->data_bus = kb->reg_data;
            kb->reg_status = 0;               /* Handshake automatico */
        }
    }
}

/* ---------- Instruccion OUT [puerto], valor ---------- */
void cpu_instruccion_out(uint8_t puerto, uint8_t valor)
{
    g_bus.address_bus = puerto;
    g_bus.data_bus    = valor;
    g_bus.io_write    = true;     /* IO_WRITE = 1  */
    g_bus.io_read     = false;
    g_bus.mem_read    = false;    /* MEM_READ = 0  */
    g_bus.mem_write   = false;    /* MEM_WRITE = 0 -> la RAM ignora el ciclo */

    tick_controlador_teclado(&g_bus, &g_teclado);
    mostrar_bus();                /* Foto del bus durante el ciclo OUT */

    g_bus.io_write = false;       /* Fin del ciclo */
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

    printf("=== Experimento 1 - Version A: getchar() -> OUT al PUERTO ===\n");
    printf("Escribe un texto y presiona Enter (Ctrl+D para salir):\n");

    /* 1) Se lee el teclado con una funcion estandar de C */
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') {
            printf("\n");
            continue;
        }

        printf("getchar() leyo '%c' (0x%02X)\n", (char)ch, (unsigned)ch);

        /* 2) La CPU escribe ese valor en el puerto de datos: OUT [0x02], R */
        printf("  OUT [0x02], 0x%02X\n", (unsigned)ch);
        cpu_instruccion_out(PUERTO_KB_DATA, (uint8_t)ch);

        /* 3) Contenido del puerto despues de la escritura */
        printf("  Puerto 0x02 (data)   = 0x%02X '%c'\n",
               (unsigned)g_teclado.reg_data, (char)g_teclado.reg_data);
        printf("  Puerto 0x01 (status) = 0x%02X (dato disponible)\n\n",
               (unsigned)g_teclado.reg_status);
    }

    printf("Fin.\n");
    return 0;
}
