#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>

#define FIFO_NAME   "/tmp/bus_fifo_no"
#define NUM_PACKETS 10000000
#define PKT_SIZE    64

typedef struct {
    int id;
    char payload[PKT_SIZE - sizeof(int)];
} Packet;

static double now_sec(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(void)
{
    if (mkfifo(FIFO_NAME, 0666) == -1 && errno != EEXIST) {
        perror("[Cons_NO] mkfifo");
        return 1;
    }

    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("[Cons_NO] open FIFO");
        return 1;
    }

    printf("[Cons_NO] Recibiendo %d paquetes por FIFO...\n", NUM_PACKETS);

    size_t received = 0;
    double start = now_sec();

    while (received < NUM_PACKETS) {
        Packet p;
        ssize_t off = 0;

        while (off < (ssize_t)sizeof(p)) {
            ssize_t n = read(fd, ((char *)&p) + off, sizeof(p) - off);
            if (n < 0) {
                if (errno == EINTR)
                    continue;
                perror("[Cons_NO] read");
                close(fd);
                return 1;
            }
            if (n == 0) {
                fprintf(stderr, "[Cons_NO] EOF prematuro. Recibidos: %zu\n", received);
                close(fd);
                return 1;
            }
            off += n;
        }

        if (p.id != (int)received) {
            fprintf(stderr, "[Cons_NO] Paquete fuera de orden. Esperado %zu, recibido %d\n",
                    received, p.id);
            close(fd);
            return 1;
        }

        received++;
    }

    double end = now_sec();
    double elapsed = end - start;
    double pps = received / elapsed;
    double mbps = (received * (double)PKT_SIZE) / (1024.0 * 1024.0 * elapsed);

    printf("\n=== METRICAS CONSUMIDOR NO OPTIMIZADO (Cons_NO) ===\n");
    printf("Paquetes recibidos: %zu\n", received);
    printf("Tiempo total      : %.4f segundos\n", elapsed);
    printf("Velocidad (PPS)   : %.0f paquetes/seg\n", pps);
    printf("Rendimiento       : %.2f MB/s\n", mbps);

    close(fd);

    return 0;
}


