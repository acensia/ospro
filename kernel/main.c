#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
//    printf("=========================================\n");
//    printf("2017-17338       ______           _\n");
//    printf("   _      _     |__  __|       __| |__     _              \n");
//    printf("__| |__  | |     / /\\ \\       |_______| __| |          \n");
//    printf("|__  __| | |    /_/  \\_\\        /   \\  |__  |       \n");
//    printf(" / /\\ \\  | |    __________      |() |  |__  |       \n");
//    printf("/_/  \\_\\ | |   |____  ____|     \\___/     |_|          \n");
//    printf(" __||____| |    _   ||              _______          \n");
//    printf("|________  |   | |  ||             |_____  |         \n");
//    printf("         |_|   | |______                 | |         \n");
//    printf("               |________|                |_|          \n");
//    printf("=========================================\n");
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode cache
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
