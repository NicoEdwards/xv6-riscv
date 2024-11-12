#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main() {
    char *addr = sbrk(0);  // Obtener la dirección actual del heap
    sbrk(4096);  // Reservar una página

    // Proteger la nueva página
    if (mprotect(addr, 1) == -1) {
        printf("mprotect falló\n");
        exit(1);
    }
    printf("mprotect exitoso\n");

    // Revertir la protección
    if (munprotect(addr, 1) == -1) {
        printf("munprotect falló\n");
        exit(1);
    }
    printf("munprotect exitoso\n");

    // Intentar escribir de nuevo (debería funcionar)
    char *ptr = addr;
    *ptr = 'B';
    printf("Valor en la dirección (desprotegida): %c\n", *ptr);

    exit(0);
}
