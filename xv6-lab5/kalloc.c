// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

// void incRefCount(uint pa);
// void decRefCount(uint pa);

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

// Structure containing (and pointing to) a list of free pages within
// memory. Basically, a wrapper for the free list and locks
struct {
  struct spinlock lock;
  int use_lock;
  // total number of free pages
  int freepages;
  // Reference count for each physical page descriptor
  // Top physical memory divided by pagesize will give total number
  // of physical page frames that could have a reference count
  struct run *freelist;
  int ref_count[PHYSTOP / PGSIZE];
} kmem;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  kmem.freepages = 0;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE){
    // All reference counts set to 0 on startup
    kmem.ref_count[V2P(p)/PGSIZE] = 0;
    kfree(p);
  }
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
// As in the previous functions, char *v is a pointer
// to physical memory represented by a virtual address
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  // If the reference count is higher than 0, then we should only
  // decrement the count by 1. However, if the reference count is
  // 0, then nothing points to the page and the resources should
  // be freed
  if (kmem.ref_count[V2P(v)/PGSIZE] > 0)
    decRefCount(V2P(v));
  else {
    // Fill with junk to catch dangling refs.
    memset(v, 1, PGSIZE);

    if(kmem.use_lock)
      acquire(&kmem.lock);
    r = (struct run*)v;
    r->next = kmem.freelist;
    kmem.freelist = r;

    kmem.freepages += 1;

    if(kmem.use_lock)
      release(&kmem.lock);
  }
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(r) {
    kmem.freelist = r->next;
    // free pages should go down when you allocate one
    kmem.freepages -= 1;
    // Number of references to that page gets initialized to 1 on allocation
    // We must use the struct r and cast (char *) to it, since r currently
    // represents the next free physical page in freelist (at this point
    // during the allocation)
    // * see page 33 in the xv6 documentation book for details
    kmem.ref_count[V2P((char *)r)/PGSIZE] += 1;

  }

  if(kmem.use_lock)
    release(&kmem.lock);

  return (char*)r;
}

int
getNumFreePages(void)
{ 
  int freepages;

  // Acquiring the lock and getting number of free pages
  if(kmem.use_lock)
    acquire(&kmem.lock);
  freepages = kmem.freepages;
  if(kmem.use_lock)
    release(&kmem.lock);

  return freepages;
}

// param uint pa is the phyiscal address
void incRefCount(uint pa){
  // error checking copied from kfree
  // checking bounds
  if (pa < (uint)V2P(end) || pa > PHYSTOP)
    panic("kfree");
  // acquiring lock and incrementing reference count
  if (kmem.use_lock)
    acquire(&kmem.lock);

  kmem.ref_count[pa/PGSIZE] += 1;

  if (kmem.use_lock)
    release(&kmem.lock);

}

// param uint pa is the physical address
void decRefCount(uint pa){
  // error checking copied from kfree
  // checking bounds
  if (pa < (uint)V2P(end) || pa > PHYSTOP)
    panic("kfree");
  // acquiring lock and decrementing refernce count
  if (kmem.use_lock)
    acquire(&kmem.lock);

  kmem.ref_count[pa/PGSIZE]  -= 1;

  if (kmem.use_lock)
    release(&kmem.lock);

}

// helper function to return reference count
// of a physical page
uint getRefCount(uint pa){

  uint refcnt;

  if (kmem.use_lock)
    acquire(&kmem.lock);

  refcnt = kmem.ref_count[pa/PGSIZE];
  
  if (kmem.use_lock)
    release(&kmem.lock);

  return refcnt;
}
