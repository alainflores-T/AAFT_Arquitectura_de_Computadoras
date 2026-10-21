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
        perror("[Prod_NO] mkfifo");
        return 1;
    }

    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("[Prod_NO] open FIFO");
        return 1;
    }

    printf("[Prod_NO] Enviando %d paquetes por FIFO...\n", NUM_PACKETS);

    double start = now_sec();

    for (int i = 0; i < NUM_PACKETS; i++) {
        Packet p;
        p.id = i;
        memset(p.payload, 'A', sizeof(p.payload));

        ssize_t off = 0;
        while (off < (ssize_t)sizeof(p)) {
            ssize_t n = write(fd, ((char *)&p) + off, sizeof(p) - off);
            if (n < 0) {
                if (errno == EINTR)
                    continue;
                perror("[Prod_NO] write");
                close(fd);
                return 1;
            }
            off += n;
        }
    }

    double end = now_sec();
    double elapsed = end - start;
    double pps = NUM_PACKETS / elapsed;
    double mbps = (NUM_PACKETS * (double)PKT_SIZE) / (1024.0 * 1024.0 * elapsed);

    printf("\n=== METRICAS PRODUCTOR NO OPTIMIZADO (Prod_NO) ===\n");
    printf("Paquetes enviados : %d\n", NUM_PACKETS);
    printf("Tiempo total      : %.4f segundos\n", elapsed);
    printf("Velocidad (PPS)   : %.0f paquetes/seg\n", pps);
    printf("Rendimiento       : %.2f MB/s\n", mbps);

    close(fd);
    unlink(FIFO_NAME);

    return 0;
}


