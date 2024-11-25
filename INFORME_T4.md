# Informe Tarea 4 Sistemas Operativos

Este proyecto tiene como objetivo extender el sistema operativo educativo xv6 para añadir un sistema básico de permisos en los inodes. El propósito principal fue permitir que los archivos tengan configuraciones de permisos que restrinjan las operaciones de lectura y escritura. Aunque se logró implementar gran parte de la funcionalidad solicitada, encontré problemas que impidieron completar con éxito la tarea debido a errores relacionados con los tamaños de las estructuras.

---

## Objetivo

Añadir un sistema básico de permisos a los inodes, donde el campo permissions define el acceso permitido:
- *0:* Sin permisos.
- *1:* Solo lectura.
- *2:* Solo escritura.
- *3:* Lectura y escritura (valor predeterminado).

---

## Pasos Implementados

1. *Creación de la estructura chmod.c*
   - Se implementó un programa de usuario chmod.c para modificar los permisos de archivos según lo especificado en la tarea.

2. *Validación de tamaños con check_size*
   - Se incluyó una función auxiliar check_size para confirmar que la estructura struct dinode mantuviera un tamaño alineado (128 bytes).

3. *Modificaciones realizadas*
   - Actualizé los siguientes archivos para incorporar y manejar los permisos:
     - Makefile: Añadí el archivo chmod.c y ajustamos las compilaciones necesarias.
     - usys.pl: Para exponer las nuevas llamadas al sistema.
     - user.h: Definí los prototipos de las nuevas funciones.
     - init.c: Para revisar posibles errores relacionados con la inicialización de permisos.
     - sysproc.c, sysfile.c, syscall.h, syscall.c: Añadí lógica para las llamadas al sistema relacionadas con permisos.
     - sleeplock.h, fs.h, fs.c, file.h: Actualicé las estructuras y funciones necesarias para manejar los permisos.

4. *Manejo de errores*
   - Intenté rastrear inconsistencias en los tamaños de estructuras, especialmente en struct dinode, para asegurar que los cambios no rompieran el comportamiento del sistema.

---

## Problemas Encontrados

### 1. Tamaños de Estructuras
- *Descripción:* La estructura struct dinode debía mantener un tamaño de 128 bytes. Aunque agregué un campo permissions y usamos un padding para compensar, parece que hubo un problema con la alineación de los componentes dentro del struct o a más grandes razgos algúna complicación en tamaños de structs con superbloques.
- *Impacto:* Esto afectó la lectura de los inodes desde el disco, provocando fallos en procesos esenciales como init y sh, posiblemente afectando en su funcionamiento a nivel de permisos.

### 2. Comportamiento de init y sh
- *Descripción:* Al compilar exitosamente el programa, el shell no mostraba el símbolo $, lo que indicaba que init y sh no se ejecutaban correctamente.
- *Diagnóstico:* Esto se atribuyó a errores en los tamaños de las estructuras, lo que generaba inconsistencias al leer los inodes desde el disco.

### 3. Capacidad de Maquina Virtual.
- *Descripción:* Como hipotesis alterna, es posible que la maquina virtual en la cual está Xv6, dentro de Visual Studio, Dentro de Ubuntu, no tenga la suficiente capacidad para poder compilar el cambio que realicé en la estructura de permisos.
- *Diagnóstico:* Est podría estar generando la complicación a init y sh, por lo tanto descartando $.

## Resultados

Aunque se logró implementar la mayoría de los requisitos solicitados, los problemas con los tamaños de estructuras y la alineación de dinode impidieron que init y sh funcionaran correctamente. Esto resultó en la falla del sistema al no mostrar el símbolo del shell ($), incluso después de una compilación exitosa.

---

## Conclusión

Este proyecto fue un ejercicio valioso para aprender sobre el diseño de sistemas operativos y la interacción entre estructuras en memoria y en disco. A pesar de los desafíos, se avanzó en gran medida en la implementación de permisos básicos. Sin embargo, errores relacionados con el manejo de tamaños de estructuras impidieron una solución completamente funcional.

Para futuros trabajos, se recomienda validar cuidadosamente los tamaños de estructuras y realizar pruebas exhaustivas en cada etapa de desarrollo para evitar errores similares.