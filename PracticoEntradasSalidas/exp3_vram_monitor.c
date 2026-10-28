/* exp3_vram_monitor.c
 * Experimento 3: Monitor por memoria de video (VRAM 4x4 en 0xF0-0xFF) y scroll.
 * Compilar: gcc -std=c99 -Wall -Wextra -pedantic exp3_vram_monitor.c -o exp3
 */
#include <stdio.h>
#include <stdint.h>

/* ---------- Mapa de memoria ---------- */
#define RAM_SIZE       256
#define VRAM_BASE_DIR  0xF0   /* 0xF0 - 0xFF */
#define VRAM_FILAS     4
#define VRAM_COLUMNAS  4
#define CARACTER_VACIO 0x20   /* Espacio en blanco */

static uint8_t g_ram[RAM_SIZE];

/* ---------- Prototipos ---------- */
void vram_limpiar(void);
void vram_escribir(int fila, int columna, uint8_t ascii);
void vram_escribir_texto(int fila, const char *texto);
void vram_scroll_arriba(void);
void vram_mostrar(const char *titulo);

/* ---------- Llena toda la VRAM con espacios ---------- */
void vram_limpiar(void)
{
    int i;
    for (i = 0; i < VRAM_FILAS * VRAM_COLUMNAS; i++)
        g_ram[VRAM_BASE_DIR + i] = CARACTER_VACIO;
}

/* ---------- Escribe un caracter en (fila, columna) ---------- */
void vram_escribir(int fila, int columna, uint8_t ascii)
{
    uint8_t dir;

    if (fila < 0 || fila >= VRAM_FILAS || columna < 0 || columna >= VRAM_COLUMNAS)
        return;                                         /* Fuera de la pantalla */

    dir = (uint8_t)(VRAM_BASE_DIR + (fila * VRAM_COLUMNAS) + columna);
    g_ram[dir] = ascii;
}

/* ---------- Escribe hasta 4 caracteres en una fila ---------- */
void vram_escribir_texto(int fila, const char *texto)
{
    int col;
    for (col = 0; col < VRAM_COLUMNAS && texto[col] != '\0'; col++)
        vram_escribir(fila, col, (uint8_t)texto[col]);
}

/* ---------- Scroll: cada fila sube una posicion, la ultima queda en blanco ---------- */
void vram_scroll_arriba(void)
{
    int fila, col;
    uint8_t dir_destino, dir_origen;

    for (fila = 0; fila < VRAM_FILAS - 1; fila++) {
        for (col = 0; col < VRAM_COLUMNAS; col++) {
            dir_destino = (uint8_t)(VRAM_BASE_DIR + (fila * VRAM_COLUMNAS) + col);
            dir_origen  = (uint8_t)(VRAM_BASE_DIR + ((fila + 1) * VRAM_COLUMNAS) + col);
            g_ram[dir_destino] = g_ram[dir_origen];
        }
    }
    for (col = 0; col < VRAM_COLUMNAS; col++)
        g_ram[VRAM_BASE_DIR + ((VRAM_FILAS - 1) * VRAM_COLUMNAS) + col] = CARACTER_VACIO;
}

/* ---------- Dibuja el monitor leyendo 0xF0..0xFF ---------- */
void vram_mostrar(const char *titulo)
{
    int fila, col;

    printf("%s\n", titulo);
    printf("         +------+\n");
    for (fila = 0; fila < VRAM_FILAS; fila++) {
        printf("  0x%02X   | ", (unsigned)(VRAM_BASE_DIR + fila * VRAM_COLUMNAS));
        for (col = 0; col < VRAM_COLUMNAS; col++) {
            uint8_t c = g_ram[VRAM_BASE_DIR + fila * VRAM_COLUMNAS + col];
            putchar((c >= 32 && c <= 126) ? (int)c : '?');
        }
        printf(" |  fila %d\n", fila);
    }
    printf("         +------+\n\n");
}

/* ---------- Programa de prueba ---------- */
int main(void)
{
    printf("=== Experimento 3: Monitor VRAM 4x4 y scroll ===\n\n");

    /* Prueba 1: pantalla limpia */
    vram_limpiar();
    vram_mostrar("Pantalla limpia:");

    /* Prueba 2: un caracter en una celda concreta (fila 2, columna 1) */
    vram_escribir(2, 1, 'X');
    printf("vram_escribir(2, 1, 'X') -> direccion 0xF0 + 2*4 + 1 = 0x%02X\n",
           (unsigned)(VRAM_BASE_DIR + 2 * VRAM_COLUMNAS + 1));
    vram_mostrar("Despues de escribir 'X':");

    /* Prueba 3: llenar las 4 filas */
    vram_limpiar();
    vram_escribir_texto(0, "HOLA");
    vram_escribir_texto(1, "SOY ");
    vram_escribir_texto(2, "UNA ");
    vram_escribir_texto(3, "VRAM");
    vram_mostrar("Pantalla llena:");

    /* Prueba 4: scroll hacia arriba */
    vram_scroll_arriba();
    vram_mostrar("Despues de 1 scroll (HOLA desaparece, fila 3 en blanco):");

    /* Prueba 5: escribir en la fila liberada */
    vram_escribir_texto(3, "4x4!");
    vram_mostrar("Escribiendo en la fila liberada:");

    return 0;
}
