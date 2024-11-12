# Tarea 3: Protección de Memoria en xv6

## Objetivo
Implementar un sistema de protección de memoria en xv6 que permita marcar regiones de memoria como solo lectura.

### Modificaciones Requeridas
1. Implementación de Funciones de Protección:
   - Se implementaron las funciones mprotect y munprotect.

2. Actualización de la Tabla de Páginas:
   - Se modificó la tabla de páginas para incluir un bit que indique si una página es de solo lectura.

### Funciones Implementadas
- mprotect(void *addr, int len):
  - Marca una región de memoria, comenzando en addr con longitud len, como solo lectura.
  - Se calculan cuántas páginas están involucradas y se modifica el bit W de cada PTE para deshabilitar la escritura.
  
- munprotect(void *addr, int len):
  - Revierte la protección de solo lectura, volviendo las páginas de nuevo de lectura/escritura.
  - Similar a mprotect, pero restaura el permiso de escritura.

### Manejo de Errores
- Las funciones verifican:
  - Direcciones inválidas para addr o len.
  - Direcciones que no pertenecen al proceso.
  - Otros errores como longitud cero o fuera de los límites de la memoria del proceso.

### Pasos Realizados

1. Modificación de Archivos del Sistema:
   - syscall.h: Se añadieron las constantes SYS_mprotect y SYS_munprotect.
   - syscall.c: Se añadieron las funciones de sistema sys_mprotect y sys_munprotect.
   - sysproc.c: Se implementaron las funciones sys_mprotect y sys_munprotect que gestionan las llamadas a sistema.
   - vm.c: Se añadió la función update_pte_permissions para modificar los permisos de las PTEs.
   - usys.pl y user.h: Se declararon las funciones de usuario mprotect y munprotect.

2. Pruebas:
   - Se creó el archivo de prueba test_mprotect.c para verificar la función mprotect.
   - Se creó el archivo test_munprotect.c para probar la función munprotect.

3. Manejo de Excepciones:
   - Se modificó trap.c para capturar los intentos de escritura en una página de solo lectura y mostrar un mensaje de error.

### Problemas y Soluciones
- Problema: Error de compilación por funciones de retorno void en argint y argaddr.
  - Solución: Se modificaron las funciones para que devuelvan un valor int y permitan la verificación de errores.

- Problema: Mensajes de error de usertrap() al intentar escribir en una página protegida.
  - Solución: Se agregó un manejo de excepciones para capturar y mostrar las direcciones de fallos de protección.

### Programas de Prueba

*Programa de prueba para mprotect*:
c
#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    char *addr = sbrk(0);
    sbrk(4096);
    if (mprotect(addr, 1) == -1) {
        printf("mprotect falló\n");
    } else {
        printf("mprotect exitoso\n");
    }
    *addr = 'A'; // Esto debería causar un fallo si la protección es exitosa
    printf("Prueba de escritura completada.\n");
    exit(0);
}


**Programa de prueba para munprotect**:
c

#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    char *addr = sbrk(0);
    sbrk(4096);
    if (mprotect(addr, 1) == -1) {
        printf("mprotect falló\n");
    } else {
        printf("mprotect exitoso\n");
    }
    if (munprotect(addr, 1) == -1) {
        printf("munprotect falló\n");
    } else {
        printf("munprotect exitoso\n");
    }
    *addr = 'B'; // Esto debería funcionar sin errores tras desproteger la página
    printf("Valor después de munprotect: %c\n", *addr);
    exit(0);
}

### Conclusión

   -   Se logró implementar un sistema de protección de memoria que permite marcar regiones como solo lectura y restaurar los permisos con munprotect. Se manejaron las excepciones adecuadamente y se realizaron pruebas para validar el correcto funcionamiento de las funciones.