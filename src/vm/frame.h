#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <list.h>
#include "threads/synch.h"

struct frame_table_entry
{
  void* kvaddr;                                 /* Kernel virtual address. */
  //struct supplemental_page_table_entry* spte;   /* Supplemental page table entry. */  




  struct list_elem elem;                        /* List element for frame table. */
};


/* Global variables. */
struct list frame_table;
struct lock frame_lock;


/* New functions. */
void frame_init(void);
struct frame_table_entry* fte_obtain(enum palloc_flags flags);

#endif
