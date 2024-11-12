#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

void update_pte_permissions(pagetable_t pagetable, uint64 addr, int len, int readonly);

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

uint64
sys_mprotect(void) {
    uint64 addr;
    int len;

    if (argaddr(0, &addr) < 0 || argint(1, &len) < 0) {
    return -1; // Retorna un error si alguna de las funciones falla
    }


    // Verificar que la dirección es válida y está alineada a una página.
    if (addr % PGSIZE != 0 || len <= 0) {
        return -1; // Error si la dirección o longitud son inválidas.
    }

    update_pte_permissions(myproc()->pagetable, addr, len, 1); // Marcar como solo lectura
    return 0; 
}

uint64
sys_munprotect(void) {
    uint64 addr;
    int len;

    if (argaddr(0, &addr) < 0 || argint(1, &len) < 0) {
    return -1; // Retorna un error si alguna de las funciones falla
  }


    if (addr % PGSIZE != 0 || len <= 0) {
        return -1; // Error si la dirección o longitud son inválidas.
    }

    update_pte_permissions(myproc()->pagetable, addr, len, 0); // Restaurar permisos de escritura
    return 0;
}

