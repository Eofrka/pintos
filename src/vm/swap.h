#ifndef VM_SWAP_H
#define VM_SWAP_H

#include <bitmap.h>
#include "threads/synch.h"
#include "vm/frame.h"
#include "vm/page.h"
#define SWAP_IDX_DEFAULT 0xffffffff
struct swap_table
{
  struct lock lock;            /* Lock for swap table. */
  struct bitmap* used_map;     /* Used: 1, free: 0. */
  struct disk* disk;           /* Swap disk. */

};


/* Global variables. */
struct swap_table swap_table;



/* New functions declarations. */
void swap_init(void);
void swap_destroy(void);

void swap_out(struct supplemental_page_table_entry* victim_spte, struct frame_table_entry* victim_fte);
void swap_in(struct supplemental_page_table_entry* spte, struct frame_table_entry* fte);
void swap_free(struct supplemental_page_table_entry* spte);
#endif
