#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
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


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  struct proc *p = myproc();
  // get register a0's pointer data
  uint64 getaddr = 0;
  argaddr(0, &getaddr);
  // get register a1's data
  int getnum = 0;
  argint(1, &getnum);

  uint64 getstore;
  argaddr(2, &getstore);

  uint64 bits = 0;
  for(int i = 0; i < getnum; ++i)
  {
    uint64 qaddr = getaddr + i * PGSIZE;
    pte_t *pte = walk(p->pagetable, qaddr, 0);
    if(*pte & PTE_A) // if find a PTE which is accessed
    {
      bits = bits | (1 << i);
      *pte = (*pte) & (~PTE_A);
    }
  }

  if(copyout(p->pagetable, getstore, (char*) &bits, sizeof(bits)) < 0)
    return -1;
  return 0;
}
#endif

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
