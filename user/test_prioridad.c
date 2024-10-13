#include "kernel/types.h"
#include "user/user.h"

int main() {
    for(int i = 0; i < 20; i++) {
        if(fork() == 0) {
            // Código del hijo
            sleep(i * 10);  // Diferir la salida para cada proceso para reducir la intercalación
            printf("Ejecutando proceso %d\n", i);
            sleep(10);  // Mantener el sleep para que los procesos permanezcan en ejecución
            exit(0);
        }
    }
    for(int i = 0; i < 20; i++) {
        wait(0);  // Esperar a que todos los procesos terminen
    }
    exit(0);
}

