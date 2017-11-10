#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <list.h>
#include "vm/page.h"
#include "threads/synch.h"
#include "threads/palloc.h"


/* Frame table entry. */
struct frame_table_entry
{
  void* kpage;                                   /* Kernel virtual address of frame. */   
  struct supplemental_page_table_entry* spte;    /* Supplemental page table entry. */
  struct list_elem elem;                        /* List element of frame table. */                

};


/* Clock for page replacement algorithm. */
struct clock
{
  struct list_elem* clock_hand;                   /* Clock hand which points fte's elem. */

};

/* Gloabal variables. */
struct lock frame_lock;                           /* Frame lock. */
struct list frame_table;                          /* Frame table. */
struct clock frame_clock;                         /* Frame clock. */

void frame_init(void);
void fte_free(struct frame_table_entry* fte);
bool handle_page_fault(struct supplemental_page_table_entry* spte);
struct frame_table_entry* fte_obtain(enum palloc_flags flags);
bool fte_fetch(struct frame_table_entry* fte, struct supplemental_page_table_entry* spte);
void fte_insert(struct frame_table_entry* fte);
void fte_remove(struct frame_table_entry* fte);
bool fte_install(struct frame_table_entry* fte, struct supplemental_page_table_entry* spte);
void frame_advance_iter(struct list_elem** iter_ptr);
void* frame_realloc(enum palloc_flags flags);


#endif

