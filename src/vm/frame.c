#include "vm/frame.h"
#include "threads/malloc.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include <string.h>
#include <stdio.h>
#include "userprog/pagedir.h"
#include "threads/vaddr.h"
#include "vm/swap.h"
#include "threads/thread.h"

/* Initilizes frame_lock and frame_table. */
void frame_init(void)
{
  lock_init(&frame_lock);
  list_init(&frame_table);
  frame_clock.clock_hand = list_tail(&frame_table);
}

/* Removes fte from frame_table and frees it. Also frees frame which the entry points and clear it from pagedir. */ 
void fte_free(struct frame_table_entry* fte)
{

  /* If the clock_hand points to the fte which we are going to free, Advance the clock_hand.
     This pointer manipulation is very important!!. */
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
  lock_release(&frame_lock);
  ASSERT(fte->kpage != NULL);
  struct supplemental_page_table_entry* spte = fte->spte;
  ASSERT(spte != NULL);
  ASSERT(spte->pagedir != NULL);

  if(spte->is_mmap_page)
  {
    void* upage = spte->upage;
    /* If the page is dirty, write it into file. */
    bool dirty = pagedir_is_dirty(spte->pagedir, upage);
    dirty = dirty || spte->swap_idx != SWAP_IDX_DEFAULT;
    if(dirty)
    {
      //lock_acquire(&filesys_lock);
      file_write_at(spte->file, fte->kpage, spte->page_read_bytes, spte->ofs);
      //lock_release(&filesys_lock);
    }
  }

  palloc_free_page(fte->kpage);
  pagedir_clear_page(spte->pagedir, spte->upage);
  SAFE_FREE(fte);

  
}

/* Handles page fault. */
bool handle_page_fault(struct supplemental_page_table_entry* spte)
{
  /* 1. Obtain fte. */
  struct thread* curr = thread_current();
  lock_acquire(&frame_lock);
  struct frame_table_entry* fte = fte_obtain(spte,PAL_USER);
  if(fte == NULL)
  {
    hash_delete(&curr->spt, &spte->h_elem);
    SAFE_FREE(spte);
    lock_release(&frame_lock);
    return false;
  }
  lock_release(&frame_lock);


  /* 2. Fetch data into fte by using spte's information. */
  if(!fte_fetch(fte,spte))
  {
    lock_acquire(&frame_lock);
    list_remove(&fte->elem);
    lock_release(&frame_lock);
    palloc_free_page (fte->kpage);
    SAFE_FREE(fte);

    hash_delete(&curr->spt, &spte->h_elem);
    SAFE_FREE(spte);
    return false;
  }

  /* 3. Install the page. swapped_in page's dirty bit must be cleared because of pagedir_set_page(). */
  if(!fte_install(fte, spte))
  {
    lock_acquire(&frame_lock);
    list_remove(&fte->elem);
    lock_release(&frame_lock);
    palloc_free_page(fte->kpage);
    SAFE_FREE(fte);

    hash_delete(&curr->spt, &spte->h_elem);
    SAFE_FREE(spte);
    return false;
  }

  /* 4. Update the spte's state into SPTE_FRAME. */
  spte->state = SPTE_FRAME;
  return true;

}

/* Obtains a frame to store the page. If user pool is full, do page replacement algorithm. This function includes allocating a frame, allocating a fte, connecting the spte and the fte, and inserting the fte into the frame_table. Programmer must do synchronization. */
struct frame_table_entry* fte_obtain(struct supplemental_page_table_entry* spte, enum palloc_flags flags)
{
  /* 1. Allocate a frame. */
  void* kpage = palloc_get_page(flags);
  if(kpage == NULL)
  {
    /* If user pool is full, do page replacement algoritm. */
    kpage = frame_realloc(flags);
  }

  /* 2. Allocate a fte. */
  struct frame_table_entry* fte = (struct frame_table_entry*)malloc(sizeof(struct frame_table_entry));
  if(fte == NULL)
  {
    palloc_free_page(kpage);
    return NULL;
  }

  /* 3. Initialize the fte. */
  fte->kpage = kpage;
  fte->spte = spte;
  fte->elem.prev = NULL;
  fte->elem.next = NULL;

  spte->fte = fte;
  /* 4. Insert the fte into frame_table. */ 
  fte_insert(fte);
  return fte;

}

/* Fetches the data into a frame, by reading it from the file system, or swap slots, or zeroing it. */
bool fte_fetch(struct frame_table_entry* fte, struct supplemental_page_table_entry* spte)
{
  ASSERT(fte->kpage != NULL);
  spte->is_fetched = false;
  switch(spte->state)
  {
    case SPTE_FRAME :
    PANIC("SPTE_FRAME is not available state to fetch");
    break;
    case SPTE_FILE :
    lock_acquire(&filesys_lock);
    file_seek(spte->file, spte->ofs);
    lock_release(&filesys_lock);
    if (file_read (spte->file, fte->kpage, spte->page_read_bytes) != (int) spte->page_read_bytes)
    {
      //lock_release(&filesys_lock);
      return false; 
    }
    memset ((uint8_t*)fte->kpage + spte->page_read_bytes, 0, spte->page_zero_bytes);
    //lock_release(&filesys_lock);
    break;
    case SPTE_SWAP :
    swap_in(spte, fte);
    break;
    case SPTE_ZERO :
    ASSERT(spte->is_stack_page == true);
    memset (fte->kpage, 0, PGSIZE);
    break;
    default:
    PANIC("not available state to fetch");
    break;

  }
  spte->is_fetched = true;
  return true;
}


/* Inserts fte into frame_table. */
void fte_insert(struct frame_table_entry* fte)
{
  /*
  struct list_elem* elem = frame_clock.clock_hand;
  bool interior_true= (elem != NULL && elem->prev != NULL && elem->next != NULL);
  bool tail_true = elem != NULL && elem->prev != NULL && elem->next == NULL;
  if(!interior_true && !tail_true)
  {
    printf("clock_hand:       [0x%08x]\n", elem); 
    printf("clcok_hand->prev: [0x%08x]\n", elem->prev);
    printf("clock_hand->next: [0x%08x]\n", elem->next);
  }
  */
  /*
  struct list_elem* elem = frame_clock.clock_hand;
  bool head_true = elem != NULL && elem->prev == NULL && elem->next != NULL;
  if(head_true)
  {
    printf("@\n");
  }
  */
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

/* Advances frame_table iter. Programmer must do synchronization. */
void frame_advance_iter(struct list_elem** iter_ptr)
{
  *iter_ptr = list_next(*iter_ptr);
  if(*iter_ptr == list_tail(&frame_table))
  {
    *iter_ptr=list_begin(&frame_table);
  }
}

/* Reallocates frame. Programmer must do synchronization. */
void* frame_realloc(enum palloc_flags flags)
{

  void* kpage = NULL;
  /* Find victim fte. */
  /**************************************************************************************************************************/
  struct frame_table_entry* victim_fte;
  struct clock* fc = &frame_clock;

  /* If clock_hand == list_tail(&frame_table), it means that the first time to find a victim
  fte. Therefore start the clock_hand from list_begin(&frame_tagble). */
  if(fc->clock_hand == list_tail(&frame_table))
  {
    fc->clock_hand = list_begin(&frame_table);
  }

  /* Calculate the start fte and last fte for one loop searching. */
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

  /* victim finding loop. */
  struct list_elem* iter = start;
  struct frame_table_entry* iter_fte;
  while(true)
  {

    iter_fte = list_entry(iter, struct frame_table_entry, elem);
    //printf("iter_fte->kpage: [0x%08x], iter_fte->spte: [0x%08x]\n", iter_fte->kpage, iter_fte->spte);
    ASSERT(iter_fte->spte != NULL);
    if(iter_fte->spte->is_fetched == false)
    {
      frame_advance_iter(&iter);
      fc->clock_hand=iter;
      continue;
    }
    uint32_t* pd = iter_fte->spte->pagedir;
    void* upage = iter_fte->spte->upage;
    /* If upage is accessed, set accessed bit of upage's pte into 0. */
    if(pagedir_is_accessed(pd, upage))
    {
      pagedir_set_accessed(pd, upage ,false);

      /* If iter is last, Get the victim_fte and set clock_hand right after the victim_fte. Then break.*/ 
      if(iter == last)
      {
        frame_advance_iter(&iter);
        fc->clock_hand=iter;
        victim_fte = list_entry(iter, struct frame_table_entry, elem);
        frame_advance_iter(&iter);
        fc->clock_hand=iter;
        break;
      }
      /* Else, just advance the iter and clock_hand. */
      frame_advance_iter(&iter);
      fc->clock_hand=iter;
    }
    /* If upage is not accssed, Get the victim_fte and set the clock_hand right after the victim_fte. Then braek.*/
    else
    {
      victim_fte = iter_fte;
      frame_advance_iter(&iter);
      fc->clock_hand=iter;
      break;
    }
  }
  /**************************************************************************************************************************/

  /* Now, we get the victim_fte and victim_spte. Do rest of job for page replacement. */
  struct supplemental_page_table_entry* victim_spte = victim_fte->spte;
  uint32_t* victim_pd = victim_spte->pagedir;

  void* victim_upage = victim_spte->upage;
  bool upage_dirty = pagedir_is_dirty(victim_pd, victim_upage);
  bool have_been_swapped = victim_spte->swap_idx != SWAP_IDX_DEFAULT;
  

  /*
  //swap disk level replacement is required!
  if(upage_dirty &&  have_been_swapped)
  {
    swap_out(victim_spte, victim_fte);
  }
  else if(upage_dirty && !have_been_swapped)
  {
    swap_out(victim_spte, victim_fte);
  } 
  else if(!upage_dirty && have_been_swapped)
  {
    victim_spte->state = SPTE_SWAP;
    victim_spte->fte = NULL;
    
  }
  */

  /* Compare the upage with the original upage. If it is dirty, swap_out.
     The spte which has been swapped is dirty!. */
  if(upage_dirty || have_been_swapped)
  {
    if(victim_spte->is_mmap_page)
    {
      //lock_acquire(&filesys_lock);
      file_write_at(victim_spte->file, victim_fte->kpage, victim_spte->page_read_bytes, victim_spte->ofs);
      victim_spte->state = SPTE_FILE;
      victim_spte->fte = NULL;
      //lock_release(&filesys_lock);
    }
    else
    {
      swap_out(victim_spte, victim_fte);
    }
  }
  /* If the victim_spte is stack page and it is not dirty, just change state into SPTE_ZERO. */
  else if(victim_spte->is_stack_page)
  {
    /* May not be happened. */
    victim_spte->state = SPTE_ZERO;
    victim_spte->fte = NULL;
  }
  /* If the victim_spte is not dirty and it is not stack page, just chang state into SPTE_FILE. */ 
  else
  {
    victim_spte->state = SPTE_FILE;
    victim_spte->fte = NULL;

  }

  /* Remove the victim_fte from the frame_table and free its kpage and itself. */
  list_remove(&victim_fte->elem);
  palloc_free_page(victim_fte->kpage);
  victim_fte->kpage = NULL;
  SAFE_FREE(victim_fte);

  /* Clear the victim_upage mapping from victim pagedir. */
  pagedir_clear_page(victim_pd, victim_upage);

  /* Reallocate a kpage! */ 
  kpage = palloc_get_page(flags);
  ASSERT(kpage != NULL);
  return kpage;
  /**************************************************************************************************************************/

}

