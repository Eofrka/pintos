#include "vm/frame.h"
#include "vm/page.h"
#include "threads/malloc.h"
#include <debug.h>
#include "userprog/syscall.h"
#include "filesys/file.h"
#include <string.h>
#include "userprog/pagedir.h"


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
  uint8_t *kpage;  
  kpage = palloc_get_page (flags);
  if (kpage == NULL)
  {
    //TODO: Clock algorithm.
    PANIC("no free frames...clock algorithm need");
  }
  ASSERT(kpage != NULL);
  fte->kvaddr = kpage;
  fte->spte = NULL;
  fte->elem.prev = NULL;
  fte->elem.next = NULL;
  return fte;
}


/* Fetch the data into the fte's frame by using spte's information. */
bool fte_fetch(struct list_elem* fte_e,struct hash_elem* spte_he)
{
  struct supplemental_page_table_entry* spte = hash_entry(spte_he, struct supplemental_page_table_entry, he);
  struct frame_table_entry* fte= list_entry(fte_e, struct frame_table_entry, elem);
  switch(spte->state)
  {
    //4-1. case SPTE_FILE: read it from the file.
    case SPTE_FILE:
      lock_acquire(&filesys_lock);
      file_seek(spte->file, spte->ofs);
      int file_read_ret = file_read(spte->file, fte->kvaddr, spte->page_read_bytes);
      if(file_read_ret != (int)spte->page_read_bytes)
      {
        PANIC("file_read does not match");
      }
      lock_release(&filesys_lock);
      void* zero_base_kvaddr = (void*)((uint32_t)fte->kvaddr + (uint32_t)spte->page_read_bytes); 
      memset(zero_base_kvaddr, 0x00, spte->page_zero_bytes);
      break;
    //4-2. case SPTE_SWAP: read it from the swap slot.
    case SPTE_SWAP:
      break;
    //4-3. case SPTE_ZERO: zero it.
    case SPTE_ZERO:
      memset(fte->kvaddr, 0x00, (size_t)0x1000);
      break;
    case SPTE_LOADED:
      PANIC("impossible SPTE_STATE");
      break;
  }
  spte->fte = fte;
  fte->spte = spte;
  return true;

}

/* Inserts fte into the frame table. */
void fte_insert(struct list* ft, struct list_elem* fte_e)
{
  lock_acquire(&frame_lock);
  list_push_back(ft, fte_e);
  lock_release(&frame_lock);
}


/* Adds a mapping from user virtual address UPAGE to kernel
   virtual address KPAGE to the page table.
   If WRITABLE is true, the user process may modify the page;
   otherwise, it is read-only.
   UPAGE must not already be mapped.
   KPAGE should probably be a page obtained from the user pool
   with palloc_get_page().
   Returns true on success, false if UPAGE is already mapped or
   if memory allocation fails. */

bool fte_install(void* upage, void* kpage, bool writable)
{
  struct thread *t = thread_current ();

  /* Verify that there's not already a page at that virtual
     address, then map our page there. */
  return (pagedir_get_page (t->pagedir, upage) == NULL
    && pagedir_set_page (t->pagedir, upage, kpage, writable));
}
