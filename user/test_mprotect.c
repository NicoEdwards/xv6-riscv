#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main() {
    // Obtener la dirección actual del heap y reservar una página
    char *addr = sbrk(0);
    sbrk(4096);

    // Intentar proteger la nueva página
    if (mprotect(addr, 1) == -1) {
        printf("mprotect falló\n");
    } else {
        printf("mprotect exitoso\n");
    }

    // Intentar escribir en la página protegida (debería fallar)
    char *ptr = addr;
    *ptr = 'A';  // Esto debería causar un fallo si la protección es exitosa
    printf("Valor en la dirección: %c\n", *ptr);  // Verificar el valor

    // Desproteger la página
    if (munprotect(addr, 1) == -1) {
        printf("munprotect falló\n");
    } else {
        printf("munprotect exitoso\n");
    }

    // Intentar escribir en la página desprotegida (debería funcionar)
    *ptr = 'B';
    printf("Valor en la dirección después de munprotect: %c\n", *ptr);

    exit(0);
}
