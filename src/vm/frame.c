#include "vm/frame.h"
#include "vm/page.h"
#include "vm/swap.h"
#include "threads/malloc.h"
#include <debug.h>
#include "userprog/syscall.h"
#include "filesys/file.h"
#include <string.h>
#include <stdio.h>
#include "userprog/pagedir.h"
#include "threads/thread.h"
#include "threads/vaddr.h"


/* Initialize frame_table and frame_lock. */
void frame_init(void)
{
  lock_init(&frame_lock);
  list_init(&frame_table);
  lru_approximation_clock.hand = list_begin(&frame_table);
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

    lock_acquire(&frame_lock);

    //1. Initialize clk if it is needed.
    struct list* ft = &frame_table;
    struct clock* clk = &lru_approximation_clock;
    if(clk->hand == list_tail(ft))
    {
      clk->hand = list_begin(ft);
    }

    //2. Get initial clock hand. 
    struct list_elem* start = clk->hand;

    
    struct frame_table_entry* tmp_fte;
    struct supplemental_page_table_entry* tmp_spte;
    struct frame_table_entry* victim_fte = NULL;





    /*
    //debugging
    tmp_fte = list_entry(start, struct frame_table_entry, elem);
    tmp_spte = tmp_fte->spte;
    struct list_elem* iter= start;
    uint32_t* pd = thread_current()->pagedir;
    int i=0;
    printf("[%03d], upage[0x%08x], A: %d, D: %d, ", i,tmp_spte->uvaddr, pagedir_is_accessed(pd, tmp_spte->uvaddr), pagedir_is_dirty(pd, tmp_spte->uvaddr));
    printf("[%03d], kpage[0x%08x], A: %d, D: %d\n", i,tmp_fte->kvaddr, pagedir_is_accessed(pd, tmp_fte->kvaddr), pagedir_is_dirty(pd, tmp_fte->kvaddr));

    i++;
    iter = list_next(iter);
    if(iter == list_end(ft))
    {
      iter = list_begin(ft);
    }

    while(iter != start)
    {
      tmp_fte = list_entry(iter, struct frame_table_entry, elem);
      tmp_spte = tmp_fte->spte;
      printf("[%03d], upage[0x%08x], A: %d, D: %d, ", i,tmp_spte->uvaddr, pagedir_is_accessed(pd, tmp_spte->uvaddr), pagedir_is_dirty(pd, tmp_spte->uvaddr));
      printf("[%03d], kpage[0x%08x], A: %d, D: %d\n", i,tmp_fte->kvaddr, pagedir_is_accessed(pd, tmp_fte->kvaddr), pagedir_is_dirty(pd, tmp_fte->kvaddr));

      i++;
      iter = list_next(iter);
      if(iter == list_end(ft))
      {
        iter = list_begin(ft);
      }
    } 
    */


    //3. First search.
    tmp_fte = list_entry(clk->hand, struct frame_table_entry, elem);
    tmp_spte = tmp_fte->spte;
    //3-1. If accessed bit is setted, clear it.
    uint32_t* pd =thread_current()->pagedir; 
    if(pagedir_is_accessed(pd, tmp_spte->uvaddr))
    {
      pagedir_set_accessed(pd, tmp_spte->uvaddr, false);
    }
    //3-2. Else get the victim fte and advance clock hand.
    else
    {
      victim_fte=tmp_fte;
    }
    clk->hand = list_next(clk->hand);
    if(clk->hand == list_end(ft))
    {
      clk->hand = list_begin(ft);
    } 


    //4. search victim_fte(Accessed bit is 0) until one loop.
    if(victim_fte == NULL)
    {
      while(clk->hand != start)
      {
        tmp_fte = list_entry(clk->hand, struct frame_table_entry, elem);
        tmp_spte = tmp_fte->spte;
        if(pagedir_is_accessed(pd, tmp_spte->uvaddr))
        {
          pagedir_set_accessed(pd, tmp_spte->uvaddr, false);
          clk->hand = list_next(clk->hand);
          if(clk->hand == list_end(ft))
          {
            clk->hand = list_begin(ft);
          } 
        }
        else
        {
          victim_fte = tmp_fte;
          clk->hand = list_next(clk->hand);
          if(clk->hand == list_end(ft))
          {
            clk->hand = list_begin(ft);
          } 
          break;
        }
      }

      //5. if all accessed bit were 1, Now all accessed bit are cleared and start fte is victim_fte.
      if(victim_fte == NULL)
      {
        ASSERT(clk->hand ==start);
        victim_fte = list_entry(clk->hand, struct frame_table_entry, elem);
        ASSERT(pagedir_is_accessed(pd, victim_fte->spte->uvaddr) == 0);
        clk->hand = list_next(clk->hand);
        if(clk->hand == list_end(ft))
        {
          clk->hand = list_begin(ft);
        }       
      }    

    }

    /*
    //debugging
    tmp_fte = list_entry(start, struct frame_table_entry, elem);
    tmp_spte = tmp_fte->spte;
    struct list_elem* iter= start;
    int i=0;
    printf("[%03d], upage[0x%08x], A: %d, D: %d, ", i,tmp_spte->uvaddr, pagedir_is_accessed(pd, tmp_spte->uvaddr), pagedir_is_dirty(pd, tmp_spte->uvaddr));
    printf("[%03d], kpage[0x%08x], A: %d, D: %d\n", i,tmp_fte->kvaddr, pagedir_is_accessed(pd, tmp_fte->kvaddr), pagedir_is_dirty(pd, tmp_fte->kvaddr));

    i++;
    iter = list_next(iter);
    if(iter == list_end(ft))
    {
      iter = list_begin(ft);
    }

    while(iter != start)
    {
      tmp_fte = list_entry(iter, struct frame_table_entry, elem);
      tmp_spte = tmp_fte->spte;
      printf("[%03d], upage[0x%08x], A: %d, D: %d, ", i,tmp_spte->uvaddr, pagedir_is_accessed(pd, tmp_spte->uvaddr), pagedir_is_dirty(pd, tmp_spte->uvaddr));
      printf("[%03d], kpage[0x%08x], A: %d, D: %d\n", i,tmp_fte->kvaddr, pagedir_is_accessed(pd, tmp_fte->kvaddr), pagedir_is_dirty(pd, tmp_fte->kvaddr));

      i++;
      iter = list_next(iter);
      if(iter == list_end(ft))
      {
        iter = list_begin(ft);
      }
    } 
    
    */


    /*
    //debugging print victim_spte and victim_fte.
    printf("[victim], upage[0x%08x], A: %d, D: %d, ", victim_fte->spte->uvaddr, pagedir_is_accessed(pd, victim_fte->spte->uvaddr), pagedir_is_dirty(pd, victim_fte->spte->uvaddr));
    printf("[victim], kpage[0x%08x], A: %d, D: %d\n", victim_fte->kvaddr, pagedir_is_accessed(pd, victim_fte->kvaddr), pagedir_is_dirty(pd, victim_fte->kvaddr));
    */



    //6. Swap_out or pass to filesys or mark it SPTE_ZERO to indicate stack segment. Update the victim_spte(HEAD_JOB)
    struct supplemental_page_table_entry* victim_spte = victim_fte->spte; 


    //6-1. If victim_spte->uvaddr is dirty => swap_out()
    if(pagedir_is_dirty(pd, victim_spte->uvaddr) || pagedir_is_dirty(pd, victim_fte->kvaddr))
    {
      swap_out(victim_spte,victim_fte);
    }
    //6-2. Else if is_stack_seg == true
    else if(victim_spte->is_stack_seg)
    {
      victim_spte->state = SPTE_ZERO;
    }
    //6-3 Else (read-only || D:0 and writable)
    else
    {
      victim_spte->state =SPTE_FILE;
    }

    //7. Remove victim_fte from the frame_table and free its resources include kvaddr and itself. 
    list_remove(&victim_fte->elem);
    victim_fte->spte = NULL;
    palloc_free_page(victim_fte->kvaddr);
    SAFE_FREE(victim_fte);

    //8. Update the victim_spte.(TAIL JOB)
    victim_spte->fte = NULL;
    pagedir_clear_page(pd, victim_spte->uvaddr);

    //9. reallocate the frame at kpage.
    kpage = palloc_get_page(flags);

    lock_release(&frame_lock);
    //printf("CLOCK ALGORITHM NEED\n");
    //PANIC("no free frames...clock algorithm need");
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
    swap_in(spte, fte);
    break;
    //4-3. case SPTE_ZERO: zero it.
    case SPTE_ZERO:
    memset(fte->kvaddr, 0x00, (size_t)PGSIZE);
    break;
    case SPTE_LOADED:
    spte_print(spte_he, NULL);
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
  struct clock* clk = &lru_approximation_clock;
  list_insert(clk->hand, fte_e);
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

void ft_print(struct list* ft)
{
  printf("*****************************************************\n");
  printf("frame table entries\n");
  lock_acquire(&frame_lock);
  struct list_elem* iter; 
  for(iter = list_begin(ft); iter != list_end(ft); iter = list_next(iter))
  {
   struct frame_table_entry* fte = list_entry(iter, struct frame_table_entry, elem);
   fte_print(fte);
 }
 lock_release(&frame_lock);
 printf("*****************************************************\n");
}

void fte_print(struct frame_table_entry* fte)
{
  printf("kvaddr: [0x%08x]\n", (uint32_t)fte->kvaddr);
  //hex_dump((uintptr_t)(fte->kvaddr), fte->kvaddr, 4096, true); 
}


