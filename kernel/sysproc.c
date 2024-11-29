#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_send(void) {
    int sender_pid = myproc()->pid;  // Obtiene el PID del proceso remitente
    char content[128];

    // Verifica que el argumento content sea válido
    if (argstr(0,content,sizeof(content)) < 0)
        return -1;

    acquire(&queue_lock);

    // Verifica que haya espacio en la cola
    if ((queue_end + 1) % MSG_QUEUE_SIZE == queue_start) {
        release(&queue_lock);
        return -1;  // Cola llena
    }

    // Agrega el mensaje a la cola
    msg_queue[queue_end].sender_pid = sender_pid;
    safestrcpy(msg_queue[queue_end].content, content, sizeof(msg_queue[queue_end].content));
    queue_end = (queue_end + 1) % MSG_QUEUE_SIZE;

    // Despierta procesos bloqueados en sys_receive
    wakeup(&msg_queue);

    release(&queue_lock);
    return 0;  // Éxito
}



// Falta arreglar correctamente esta función receive, tiene problemas con proc y con la variable message_queue.

// Función sys_receive: recibe un mensaje de la cola de mensajes
int sys_receive(void) {
    char content[128];
    
    // Usar argstr para obtener el contenido del mensaje de los argumentos de la llamada
    if (argstrargstr(0,content,sizeof(content)) < 0) 
        return -1;  // Error al obtener el argumento
    

    // Obtener el PID del proceso que realiza la llamada
    int pid = proc->pid;

    // Adquirir el bloqueo para acceder de manera segura a la cola de mensajes
    acquire(&queue_lock);

    // Esperar hasta que haya mensajes en la cola (bloquear el proceso)
    while (queue_start == queue_end) {
        // La cola está vacía, se bloquea el proceso
        sleep(&queue_start, &queue_lock);
    }

    // Leer el mensaje desde la cola de mensajes
    struct message msg = message_queue[queue_start];
    if (msg.sender_pid != pid) {
        // Si el mensaje no fue enviado por este proceso, ignorarlo
        release(&queue_lock);
        return -1;  // Error si el mensaje no corresponde al proceso
    }

    // Copiar el contenido del mensaje al buffer proporcionado por el proceso
    strncpy(content, msg.content, sizeof(msg.content));

    // Actualizar el índice de inicio de la cola
    queue_start = (queue_start + 1) % MSG_QUEUE_SIZE;

    // Despertar a cualquier proceso que esté esperando en la cola
    wakeup(&queue_start);

    // Liberar el bloqueo
    release(&queue_lock);

    return 0;  // Mensaje recibido con éxito
}