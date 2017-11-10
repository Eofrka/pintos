#include "vm/frame.h"
#include "threads/malloc.h"
#include "userprog/syscall.h"
#include "filesys/file.h"
#include <string.h>
#include <stdio.h>
#include "userprog/pagedir.h"
#include "threads/vaddr.h"
#include "vm/swap.h"

/* Initilizes frame lock and frame table. */
void frame_init(void)
{
  lock_init(&frame_lock);
  list_init(&frame_table);
  frame_clock.clock_hand = list_tail(&frame_table);
}

/* Removes frame table entry from frame table and frees it. Also free frame which the entry points and clear it from pagedir. */ 
void fte_free(struct frame_table_entry* fte)
{
  lock_acquire(&frame_lock);
  
  if(frame_clock.clock_hand == &fte->elem)
  {
    frame_clock.clock_hand= list_next(&fte->elem);
    if(frame_clock.clock_hand == list_tail(&frame_table))
    {
      frame_clock.clock_hand = list_begin(&frame_table);
    }
  }
  list_remove(&fte->elem);
  ASSERT(fte->kpage != NULL);
  struct supplemental_page_table_entry* spte = fte->spte;
  ASSERT(spte != NULL);
  ASSERT(spte->pagedir != NULL);
  palloc_free_page(fte->kpage);
  pagedir_clear_page(spte->pagedir, spte->upage);
  SAFE_FREE(fte);
  lock_release(&frame_lock);
}

/* Handles page fault. */
bool handle_page_fault(struct supplemental_page_table_entry* spte)
{
  //1. Obtain fte.
  struct thread* curr = thread_current();
  lock_acquire(&frame_lock);
  struct frame_table_entry* fte = fte_obtain(PAL_USER);
  lock_release(&frame_lock);
  if(fte == NULL)
  {
    printf("frame_obtain() failed\n");
    hash_delete(&curr->spt, &spte->h_elem);
    SAFE_FREE(spte);
    /* Not enough kernel pool memory to allocate fte. */
    return false;
  }
  //2. Fetch data into fte using spte.
  if(!fte_fetch(fte,spte))
  {
    printf("frame_fetch() failed\n");
    palloc_free_page (fte->kpage);
    SAFE_FREE(fte);
    hash_delete(&curr->spt, &spte->h_elem);
    SAFE_FREE(spte);
    return false;
  }
  //3. Install the page.
  if(!fte_install(fte, spte))
  {
    printf("frame_install_page() failed\n");
    palloc_free_page(fte->kpage);
    SAFE_FREE(fte);
    hash_delete(&curr->spt, &spte->h_elem);
    SAFE_FREE(spte);
    return false;
  }

  fte->spte= spte;
  spte->fte = fte;
  spte->state = SPTE_FRAME;
  return true;

}

/* Obtains a frame to store the page. If user pool is full, do page replacement algorithm. */
struct frame_table_entry* fte_obtain(enum palloc_flags flags)
{
  
  struct frame_table_entry* fte = (struct frame_table_entry*)malloc(sizeof(struct frame_table_entry));
  if(fte == NULL)
  {
    /* Not enough kernel pool memory to allocate fte. */
    PANIC("not enough memory to allocate fte");
  }

  fte->kpage = NULL;
  fte->spte = NULL;
  fte->elem.prev = NULL;
  fte->elem.next = NULL;

  /* Allocate frame. */

  void* kpage = palloc_get_page(flags);
  if(kpage == NULL)
  {
    /* Page replacement algoritm. */
    kpage = frame_realloc(flags);
    /**************************************************************************************************************************/
  }
  /* Connect fte and frame. */
  fte->kpage = kpage;
  fte_insert(fte);
  return fte;

}

/* Fetches the data into the frame, by reading it from the file system, or swap slots, or zeroing it. */
bool fte_fetch(struct frame_table_entry* fte, struct supplemental_page_table_entry* spte)
{
  ASSERT(fte->kpage != NULL);
  switch(spte->state)
  {
    case SPTE_FRAME :
      PANIC("SPTE_FRAME is not available spte_state to fetch");
      break;
    case SPTE_FILE :
      lock_acquire(&filesys_lock);
      file_seek(spte->file, spte->ofs);
      if (file_read (spte->file, fte->kpage, spte->page_read_bytes) != (int) spte->page_read_bytes)
      {
          lock_release(&filesys_lock);
          return false; 
      }
      memset ((uint8_t*)fte->kpage + spte->page_read_bytes, 0, spte->page_zero_bytes);
      lock_release(&filesys_lock);
      break;
    case SPTE_SWAP :
      swap_in(spte, fte);
      break;
    case SPTE_ZERO :
      ASSERT(spte->is_stack_page == true);
      memset (fte->kpage, 0, PGSIZE);
      break;

  }
  return true;
}


/* Inserts fte into frame_table. */
void fte_insert(struct frame_table_entry* fte)
{
  ASSERT(frame_clock.clock_hand != NULL);
  ASSERT(frame_clock.clock_hand != list_head(&frame_table));
  list_insert(frame_clock.clock_hand, &fte->elem);
}

/* Installs the frame into pagedir. */
bool fte_install(struct frame_table_entry* fte, struct supplemental_page_table_entry* spte)
{
  void* upage = spte->upage;
  void* kpage = fte->kpage;
  bool writable = spte->writable;
  uint32_t* pagedir = spte->pagedir;

  /* Verify that there's not already a page at that virtual
     address, then map our page there. */
  return (pagedir_get_page (pagedir, upage) == NULL && pagedir_set_page (pagedir,upage,kpage, writable));

}

/* Advances frame table iter. Caller must synchronize. */
void frame_advance_iter(struct list_elem** iter_ptr)
{
  *iter_ptr = list_next(*iter_ptr);
  if(*iter_ptr == list_tail(&frame_table))
  {
    *iter_ptr=list_begin(&frame_table);
  }
}

/* Reallocates frame. */
void* frame_realloc(enum palloc_flags flags)
{

  void* kpage = NULL;

  /* Find victim fte. */
  /**************************************************************************************************************************/
  struct frame_table_entry* victim_fte;
  struct clock* fc = &frame_clock;
  if(fc->clock_hand == list_tail(&frame_table))
  {
    fc->clock_hand = list_begin(&frame_table);
  }

  struct list_elem* start = fc->clock_hand;
  struct list_elem* last;
  if(start ==list_begin(&frame_table))
  {
    last = list_prev(list_tail(&frame_table));
  }
  else
  {
    last = list_prev(start);
  }
  struct list_elem* iter = start;
  struct frame_table_entry* iter_fte;


  while(1)
  {
    iter_fte = list_entry(iter, struct frame_table_entry, elem);
    ASSERT(iter_fte->spte != NULL);

    uint32_t* pd = iter_fte->spte->pagedir;
    void* upage = iter_fte->spte->upage;
    if(pagedir_is_accessed(pd, upage) && pagedir_is_accessed(pd, iter_fte->kpage))
    {
      pagedir_set_accessed(pd, upage ,false);
      pagedir_set_accessed(pd, iter_fte->kpage, false);
      if(iter == last)
      {
        frame_advance_iter(&iter);
        fc->clock_hand=iter;
        victim_fte = list_entry(iter, struct frame_table_entry, elem);
        frame_advance_iter(&iter);
        fc->clock_hand=iter;
        break;
      }
      frame_advance_iter(&iter);
      fc->clock_hand=iter;
    }
    else
    {
      victim_fte = iter_fte;
      frame_advance_iter(&iter);
      fc->clock_hand=iter;
      break;
    }
  }
  /**************************************************************************************************************************/
  struct supplemental_page_table_entry* victim_spte = victim_fte->spte;
  uint32_t* victim_pd = victim_spte->pagedir;

  void* victim_upage = victim_spte->upage;

  bool kpage_dirty, upage_dirty;
  kpage_dirty = pagedir_is_dirty(victim_spte->pagedir, victim_fte->kpage);
  upage_dirty = pagedir_is_dirty(victim_spte->pagedir, victim_spte->upage);



  if(!kpage_dirty && !upage_dirty)
  {
    if(victim_spte->is_stack_page)
    {
      victim_spte->state = SPTE_ZERO;
    }
    else
    {
      victim_spte->state = SPTE_FILE;
    }
    victim_spte->fte = NULL;
  }
  else
  {
    swap_out(victim_spte, victim_fte);
  }
    

  pagedir_clear_page(victim_pd, victim_upage);
  list_remove(&victim_fte->elem);
  palloc_free_page(victim_fte->kpage);
  SAFE_FREE(victim_fte);

  kpage = palloc_get_page(flags);
  ASSERT(kpage != NULL);
  return kpage;
  /**************************************************************************************************************************/

}

