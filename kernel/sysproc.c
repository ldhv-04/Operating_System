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
sys_sysinfo(void)
{
  uint64 info;

  if(argaddr(0, &info) < 0){
    return -1;
  }
  return systeminfo(info);
}

uint64
sys_trace(void)
{
  int mask;
  argint(0, &mask);
  struct proc *p = myproc();
  p->trace_mask = mask;         // Set the trace mask for this process
  return 0;
}



int
sys_pgaccess(void) {
  uint64 start;
  int npages;    
  uint64 abitsaddr;  
  uint64 va;  
  uint64 mask;  
  uint64 abits;  

  struct proc *p = myproc();  // Lay con tro toi quoc gia (process) hien tai
  argaddr(0, &start);  // Lay tham so dau vao dia chi bat dau cua trang tu nguoi dung
  argint(1, &npages);  // Lay tham so dau vao so luong trang can kiem tra
  argaddr(2, &abitsaddr);  // Lay tham so dia chi de luu tru bitmask ket qua

  if (npages > 64)  
    return -1;

  mask = 1;  
  abits = 0; 
  for(va = start; va < start + PGSIZE * npages; va += PGSIZE) {  // Duyet qua tung trang
    pte_t *pte = walk(p->pagetable, va, 0);  // Lay PTE cua trang tai dia chi va
    if(*pte & PTE_A) {  // Neu bit PTE_A duoc set (trang da duoc truy cap)
      abits |= mask;  // Dat bit tương ứng trong bitmask
      *pte = *pte & (~PTE_A);  // Xoa bit PTE_A sau khi kiem tra
    }
    mask <<= 1;  // Dich bitmask sang trai cho trang tiep theo
  }

  if (copyout(p->pagetable, abitsaddr, (char *)&abits, 4) < 0)  // Sao chep bitmask vao bo nho nguoi dung
    return -1;
  return 0;  // Thanh cong
}
