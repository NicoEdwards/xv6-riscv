# Informe de Implementación Tarea 4 Final : Sistema de Permisos e Inmutabilidad en xv6

## Primera Parte: Implementación de Permisos

### Descripción:

Este informe documenta el desarrollo y los resultados de la Tarea 4 del curso de Sistemas Operativos, centrada en la implementación de un sistema de permisos para archivos en el sistema operativo educativo xv6. Este archivo representa el resultado final después de un extenso proceso de análisis, pruebas, y asesoramiento. Durante el desarrollo, enfrenté múltiples problemas relacionados con estructuras de datos que afectaron componentes como init y sh, los cuales quedaron registrados en la rama Tarea_4. Gracias a la colaboración con compañeros y numerosos intentos, logré implementar una solución funcional, que cumple con los requerimientos principales de la tarea.

### Objetivo General
Implementar un sistema de permisos en xv6 para controlar acceso de lectura, escritura y un comando `chmod` que permita cambiar dichos permisos.

### Pasos Previos
Antes de la implementación, se corrigieron errores en diversos archivos clave:

1. **riscv.h**  
   - Se añadió protección contra inclusiones múltiples:
   - **Objetivo**: Evitar múltiples inclusiones y errores de compilación.

2. **trampoline.S**  
   - Se ajustó la dirección del marco de interrupción:
   - **Objetivo**: Alinear el diseño de la memoria.

3. **memlayout.h**  
   - Se modificaron parámetros de mapeo de memoria:
   - **Objetivo**: Mejorar la compatibilidad de memoria.

4. **spinlock.h**  
   - Se añadió protección contra redefiniciones:


5. **Otros ajustes**  
   - Dependencias resueltas en archivos como `defs.h`, `proc.h` y `stat.h`.

### Implementación

1. **Modificación de la estructura `inode`**  
   - Se añadió un campo `perm` en `file.h`:
   - **Objetivo**: Almacenar permisos de archivos.

2. **Asignación de permisos iniciales**  
   - Se modificó `ialloc` en `fs.c` para inicializar con permisos predeterminados:

3. **Validación de permisos en syscalls**  
   - Cambios en `sys_read`, `sys_write` y `sys_open`:
     - Con la intención de bloquear operaciones según permisos.

4. **Implementación del comando `chmod`**  
   - Se añadió la syscall `sys_chmod` para cambiar permisos.

5. **Pruebas**  
   - Se creó `testchmod.c` para validar la funcionalidad.
     - Creación, lectura, escritura y cambio de permisos funcionaron correctamente.

---

## Segunda Parte: Implementación de Inmutabilidad

### Objetivo
Agregar un permiso especial (`5`) para marcar archivos como **inmutables**:
- Archivos inmutables son solo lectura.
- No es posible modificar permisos de archivos inmutables.

### Modificaciones Previas
1. **Corrección en `argint`**  
   - Ajuste en `syscall.c` para mejorar validación de argumentos
    

### Implementación

1. **Restricción en operaciones sobre archivos inmutables**  
   - **`sys_open`**:
     - Bloquear apertura en modo escritura para archivos inmutables.
   - **`sys_write`**:
     - Bloquear escritura para archivos con permiso `5`.

2. **Modificación de `sys_chmod`**  
   - Impedir cambios de permisos en archivos inmutables.

3. **Pruebas**  
   - Se añadió lógica en `testchmod.c` y `testchmod2.c` para verificar:
     - Archivos inmutables no permiten escritura.
     - Los permisos no pueden modificarse una vez marcados como inmutables.
     - Probar ambas partes de la Tarea $

---

## Resultados Obtenidos

1. **Primera Parte**  
   - Archivos creados con permisos predeterminados.
   - Función `chmod` operativa para cambiar permisos entre lectura, escritura y lectura/escritura.
   - Pruebas confirmaron correcta implementación.

2. **Segunda Parte**  
   - Archivos inmutables respetan restricciones.
   - Los permisos inmutables no se pueden alterar ni abrir en modo escritura.

### Conclusión
Luego de realizar multiples veces la tarea, es un alivio haber podido navegar por los errores en el Kernel, dejando todo registrado de como fue el "primer" intento en la rama Tarea_4 y dando a conocer la solución en esta rama Tarea_4Final. El resultado final, demuestra un entendimiento sólido de las estructuras y funciones del kernel de xv6, así como la capacidad de implementar características personalizadas como permisos en los inodes. Esta experiencia también subraya la importancia del trabajo colaborativo, la persistencia y el aprendizaje de los errores en el desarrollo de software a bajo nivel. En conclusión, la tarea cumplió con su propósito educativo, proporcionando una base práctica para comprender conceptos fundamentales en sistemas operativos y reforzando habilidades críticas como la depuración y el diseño de soluciones técnicas robustas.
