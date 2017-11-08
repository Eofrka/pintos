#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <list.h>
#include "vm/page.h"
#include "threads/synch.h"
#include "threads/palloc.h"

struct frame_table_entry
{
  void* kvaddr;                                 /* Kernel virtual address. */
  struct supplemental_page_table_entry* spte;   /* Supplemental page table entry. */  




  struct list_elem elem;                        /* List element for frame table. */
};


/* Global variables. */
struct list frame_table;
struct lock frame_lock;


/* New functions. */
void frame_init(void);
struct frame_table_entry* fte_obtain(enum palloc_flags flags);
bool fte_fetch(struct list_elem* fte_e,struct hash_elem* spte_he);
void fte_insert(struct list* ft, struct list_elem* fte_e);
bool fte_install(void* upage, void* kpage, bool writable);
void ft_print(struct list* ft);
void fte_print(struct frame_table_entry* fte);

#endif



