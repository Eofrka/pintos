#include "vm/page.h"
#include <debug.h>
#include "threads/malloc.h"
#include "threads/vaddr.h"
#include "threads/thread.h"
#include "vm/frame.h"
#include "vm/swap.h"
#include <stdio.h>


/* Returns a hash value for spt. */
unsigned spte_hash(const struct hash_elem* he, void* aux UNUSED)
{
  const struct supplemental_page_table_entry* spte = hash_entry(he, struct supplemental_page_table_entry, h_elem);
  return hash_bytes(&spte->upage, sizeof(spte->upage));

}

/* Returns true if spte_a precedes spte_b. */
bool spte_less(const struct hash_elem* he_a, const struct hash_elem* he_b, void* aux UNUSED)
{
  const struct supplemental_page_table_entry* spte_a = hash_entry(he_a, struct supplemental_page_table_entry, h_elem);
  const struct supplemental_page_table_entry* spte_b = hash_entry(he_b, struct supplemental_page_table_entry, h_elem);
  return spte_a->upage < spte_b->upage;

}

/* Frees spte. If additional free is needed, free it. */
void spte_free(struct hash_elem* he, void* aux UNUSED)
{

  struct supplemental_page_table_entry* spte = hash_entry(he, struct supplemental_page_table_entry, h_elem);

  switch(spte->state)
  {
    case SPTE_FRAME :
      ASSERT(spte->fte != NULL);
      fte_free(spte->fte);
      break;
    case SPTE_FILE :
      break;
    case SPTE_SWAP :
      swap_free(spte);
      break;
    case SPTE_ZERO :
      break;
  }
  SAFE_FREE(spte);
}

/* Initilizes spt. */
void spt_init(struct hash* spt)
{
  if(!hash_init(spt, spte_hash, spte_less, NULL))
  {
    PANIC("can not Initilize spt");
  }

}

/* Destroys spt */
void spt_destroy(struct hash* spt)
{
  hash_destroy(spt, spte_free);
}

/* Creates a supplemental page table entry. Returns NULL when not enough memory to allocate spte. */
struct supplemental_page_table_entry* spte_create(void)
{
  struct supplemental_page_table_entry* spte =(struct supplemental_page_table_entry*)malloc(sizeof(struct supplemental_page_table_entry));
  if(spte == NULL)
  {
    return NULL;
  }

  /* Initialize each members into default values. */
  spte->upage = NULL;
  spte->fte = NULL;
  spte->pagedir = NULL;
  spte->writable = false;
  spte->file = NULL;
  spte->ofs = 0;
  spte->page_read_bytes = 0;
  spte->page_zero_bytes= 0;
  spte->swap_idx = SWAP_IDX_DEFAULT;
  spte->is_stack_page = false;
  spte->is_mmap_page = false;
  spte->is_fetched = false;
  spte->h_elem.list_elem.prev = NULL;
  spte->h_elem.list_elem.next = NULL;
  return spte;


}

/* Inserts spte into spt. If the key value(upage) is already in the spt returns false. */
bool spte_insert(struct hash* spt, struct supplemental_page_table_entry* spte)
{
  if(hash_insert(spt, &spte->h_elem) != NULL)
  {
    return false;
  }
  else
  {
    return true;
  }

}

/* Actual load in page_fault(). If success returns true. */
bool spte_actual_load(void* fault_addr, bool not_present, uint8_t* esp)
{
  /* 1. Check the fault_addr is valid or not. */
  if(!not_present)
  {
    return false;
  }
  if( fault_addr >= PHYS_BASE || fault_addr < (void*)0x08048000)
  {
    return false;
  }


  /* 2. Check stack growth is needed. If it is needed, grow the stack. */
  bool sgn = stack_growth_need((uint8_t *)fault_addr, esp);
  void* fault_page_vaddr = pg_round_down(fault_addr);
  if(sgn)
  {
    if(!stack_growth(fault_page_vaddr))
    {
      return false;
    }
  }

  /* 3. Lookup fault_page_addr in spt. */
  struct thread* curr = thread_current();
  struct supplemental_page_table_entry* spte = spte_lookup(&curr->spt, fault_page_vaddr);
  if(spte == NULL)
  {
    return false;
  }

  /*4. Now the memory reference is valid. Handle page fault. */
  return handle_page_fault(spte);
}

/* Finds the spt and returns the spte containing the given upage, or NULL if no such spte exists. */
struct supplemental_page_table_entry* spte_lookup(struct hash* spt, void* upage)
{
  struct supplemental_page_table_entry spte;
  struct hash_elem* he;
  spte.upage = upage;
  he = hash_find(spt, &spte.h_elem);
  return he != NULL ? hash_entry(he, struct supplemental_page_table_entry, h_elem) : NULL;
}


/* Inserts additional stack region spte into spt. Returns true if stack growth success. */
bool stack_growth(void* fault_page_vaddr)
{
  /* 1. Create a stack spte. */
  struct supplemental_page_table_entry* spte = spte_create();
  if(spte == NULL)
  {
    return false;
  }

  /* 2. Initialize the stack spte. */
  struct thread* curr = thread_current();
  spte->upage = fault_page_vaddr;
  spte->state= SPTE_ZERO;
  spte->pagedir = curr->pagedir;
  spte->writable = true;
  spte->page_zero_bytes = (size_t)PGSIZE;
  spte->is_stack_page = true;

  /* 3. Insert the spte into spt. */
  if(!spte_insert(&curr->spt, spte))
  {
    SAFE_FREE(spte);
    return false;
  }
  else
  {
    return true;
  }
}


/* If stack growth need, returns true. */
bool stack_growth_need(uint8_t* fault_addr, uint8_t* esp)
{
  uint32_t stack_limit = (uint32_t)PHYS_BASE - (uint32_t)MAX_STACK_SIZE;
  uint32_t additional_stack_base = (uint32_t)PHYS_BASE - (uint32_t)PGSIZE;
  
  bool cond1 = (stack_limit <= (uint32_t)esp) && ((uint32_t)esp < (uint32_t)PHYS_BASE);
  bool cond2 = (stack_limit <= (uint32_t)fault_addr) && ((uint32_t)fault_addr < additional_stack_base);
  bool cond3 = ((uint32_t)fault_addr >= (uint32_t)esp) || ((uint32_t)fault_addr ==(uint32_t)esp -(uint32_t)4) || ((uint32_t)fault_addr == (uint32_t)esp - (uint32_t)32);

  return cond1 && cond2 && cond3;
}





