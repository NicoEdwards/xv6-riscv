# **Tarea 2: Implementación de Sistema de Prioridades en xv6**

## **1. Descripción General**
El objetivo de esta tarea fue modificar el **planificador de procesos (scheduler)** de xv6 para implementar un **sistema de prioridades dinámico**. Además, se incorporó una mecánica de **boost automático** para ajustar las prioridades de los procesos y evitar la "injusticia" (starvation) en la ejecución.

### **Estado Original del Scheduler en xv6**
El planificador original de xv6 es un **Round Robin** simple. Funciona de la siguiente manera:
- Itera a través de la lista de procesos y selecciona el **primer proceso en el estado RUNNABLE** (listo para ejecutarse).
- Todos los procesos tienen el **mismo nivel de prioridad** y reciben la misma cantidad de tiempo de CPU.
- Aunque es simple y justo, no permite dar prioridad a procesos importantes ni manejar casos donde ciertos procesos deban ejecutarse con mayor prioridad que otros.

## **2. Modificaciones Realizadas**
A continuación, se describen los cambios realizados para implementar el sistema de prioridades:

### **Paso 1: Modificar la Estructura del Proceso**
1. **Archivo Modificado**: `proc.h`
2. **Cambios**:
   - Se añadieron dos nuevos campos en la estructura `struct proc`:
     ```c
     int priority;  // Nivel de prioridad (0 es la más alta)
     int boost;     // Ajuste automático de la prioridad (+1 o -1)
     ```
   - **Razón**: Estos campos permiten definir el nivel de prioridad de cada proceso y ajustar dinámicamente la prioridad con el boost.

### **Paso 2: Inicializar Prioridades y Boost**
1. **Archivo Modificado**: `proc.c`
2. **Cambios**:
   - Se inicializaron los valores `priority` y `boost` en la función `allocproc()`:
     ```c
     p->priority = 0;  // Prioridad inicial alta
     p->boost = 1;     // Boost inicial positivo
     ```
   - **Razón**: Establece un valor predeterminado para todos los procesos nuevos.

### **Paso 3: Ajustar el Planificador (Scheduler)**
1. **Archivo Modificado**: `proc.c`
2. **Cambios**:
   - Se modificó la lógica en la función `scheduler()` para que seleccione el proceso con **la prioridad más alta** (número más bajo).
   - **Código Modificado**:
     ```c
     void scheduler(void) {
         struct proc *p;
         struct proc *selected_proc;
         struct cpu *c = mycpu();
         c->proc = 0;

         for(;;){
             // Desactivar interrupciones para trabajar en el scheduler
             intr_off();

             // Ajustar prioridades antes de seleccionar el siguiente proceso
             adjust_priority();

             selected_proc = 0;
             // Buscar el proceso RUNNABLE con la prioridad más alta
             for(p = proc; p < &proc[NPROC]; p++){
                 acquire(&p->lock);
                 if(p->state == RUNNABLE){
                     if(selected_proc == 0 || p->priority < selected_proc->priority){
                         if (selected_proc) {
                             release(&selected_proc->lock);
                         }
                         selected_proc = p;
                         continue;
                     }
                 }
                 release(&p->lock);
             }

             if(selected_proc != 0){
                 p = selected_proc;
                 p->state = RUNNING;
                 c->proc = p;
                 swtch(&c->context, &p->context);
                 c->proc = 0;
                 release(&p->lock);
             }

             intr_on();
         }
     }
     ```
   - **Razón**: El planificador ahora elige el proceso con menor prioridad, permitiendo manejar la ejecución de procesos basados en niveles de importancia.

### **Paso 4: Implementar Ajuste Automático de Prioridad (Boost)**
1. **Archivo Modificado**: `proc.c`
2. **Cambios**:
   - Se creó la función `adjust_priority()` para ajustar la prioridad de los procesos usando un boost:
     ```c
     void adjust_priority(void) {
         struct proc *p;
         for(p = proc; p < &proc[NPROC]; p++){
             acquire(&p->lock);
             if(p->state != ZOMBIE){
                 p->priority += p->boost;
                 if(p->priority >= 9){
                     p->priority = 9;
                     p->boost = -1;
                 } else if(p->priority <= 0){
                     p->priority = 0;
                     p->boost = 1;
                 }
             }
             release(&p->lock);
         }
     }
     ```
   - **Razón**: La función ajusta las prioridades para que no queden procesos "atrapados" en baja prioridad por mucho tiempo, garantizando equidad.

### **Paso 5: Crear un Programa de Prueba**
1. **Archivo Creado**: `user/test_prioridad.c`
2. **Código**:
   ```c
   #include "kernel/types.h"
   #include "user/user.h"

   int main() {
       for(int i = 0; i < 20; i++) {
           if(fork() == 0) {
               sleep(i * 10);
               printf("Ejecutando proceso %d\n", i);
               sleep(10);
               exit(0);
           }
       }
       for(int i = 0; i < 20; i++) {
           wait(0);
       }
       exit(0);
   }
	- **Razón**: Creamos un programa de prueba que ordene 20 procesos diferentes agregando un Sleep para que no se super pongan unos con otros.
### **Paso 6: Modificación del Makefile**
- **Archivo Modificado**: `Makefile`
- **Cambios**:
  - Se añadió `test_prioridad` a la lista `UPROGS` para que se compile automáticamente:
    ```makefile
    UPROGS= \
      ...
      _test_prioridad\
    ```
  - **Razón**: Esto permite observar cómo los procesos se manejan y priorizan bajo el nuevo sistema de prioridades.

## **7. Problemas Encontrados y Soluciones**
1. **Problema**: `panic: release`
   - **Causa**: Falta de sincronización (locks) en el planificador y ajuste de prioridades.
   - **Solución**: Se implementaron locks alrededor de las operaciones críticas para asegurar la integridad de los datos.

2. **Problema**: Salida desordenada en consola
   - **Causa**: Varios procesos escribiendo a la consola al mismo tiempo.
   - **Solución**: Se añadió un sleep en el programa de prueba para escalonar las escrituras y evitar la superposición.

3. **Otros ajustes menores**: Se añadieron declaraciones de funciones en `proc.h` para evitar errores de compilación relacionados con funciones no reconocidas.
