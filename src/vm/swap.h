#ifndef VM_SWAP_H
#define VM_SWAP_H

#include <bitmap.h>
#include "threads/synch.h"

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
void swap_in(void);
void swpa_out(void);

#endif
