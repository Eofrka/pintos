#ifndef VM_PAGE_H
#define VM_PAGE_H

#include <hash.h>
#include "filesys/off_t.h"

#define MAX_STACK_SIZE 0x800000

/* Current spte's state. */
enum spte_state
{
  SPTE_FRAME,
  SPTE_FILE,
  SPTE_SWAP,
  SPTE_ZERO
  
};

/* Supplemental page table entry. */
struct supplemental_page_table_entry
{
  void* upage;                              /* User virtual address of page. */
  enum spte_state state;                    /* Supplemental page table entry's state. */
  struct frame_table_entry* fte;            /* Frame table entry. */
  uint32_t* pagedir;                        /* Actual pagedir. */
  bool writable;                            /* Read-only: fasle, Writable: true. */
  struct file* file;                        /* When page is in the executable file. */
  off_t ofs;                                /* Offset of the file. */
  size_t page_read_bytes;                   /* Page read bytes from the file. */
  size_t page_zero_bytes;                   /* Page zero bytes to initilize to 0. */
  size_t swap_idx;                          /* Swap idx. */
  bool is_stack_page;                       /* True if it is stack page. */

  bool is_mmap_page;                        /* Mmap page. */

  struct hash_elem h_elem;                  /* Hash table element for supplemental page table. */

};

unsigned spte_hash(const struct hash_elem* he, void* aux);
bool spte_less(const struct hash_elem* he_a, const struct hash_elem* he_b, void* aux);
void spte_free(struct hash_elem* he, void* aux);
void spt_init(struct hash* spt);
void spt_destroy(struct hash* spt);
struct supplemental_page_table_entry* spte_create(void);
bool spte_insert(struct hash* spt, struct supplemental_page_table_entry* spte);
bool spte_actual_load(void* fault_addr, bool not_present, uint8_t* esp);
struct supplemental_page_table_entry* spte_lookup(struct hash* spt, void* upage);
bool stack_growth(void* fault_page_vaddr);
bool stack_growth_need(uint8_t* fault_addr, uint8_t* esp);


#endif

