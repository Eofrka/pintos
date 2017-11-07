#include "vm/frame.h"
#include "threads/malloc.h"
#include <debug.h>
/* Initialize frame_table and frame_lock. */
void frame_init(void)
{
  lock_init(&frame_lock);
  list_init(&frame_table);
}

/* Obtain a frame. */
struct frame_table_entry* fte_obtain(enum palloc_flags flags)
{
  struct frame_table_entry* fte = (struct frame_table_entry*)malloc(sizeof(struct frame_table_entry));
  if(fte == NULL)
  {
    PANIC("not enough memory to allocate fte");
  }

  uint8_t *kpage  
  kpage = palloc_get_page (flags);
  if (kpage == NULL)
  {
    //TODO: Clock algorithm.
  }
  fte->kvaddr = kpage;
}

