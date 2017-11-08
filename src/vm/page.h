#ifndef VM_PAGE_H
#define VM_PAGE_H

#include <hash.h>
#include "filesys/off_t.h"

/* state of supplemental page table entry. */
enum SPTE_STATE
{
  SPTE_FILE,
  SPTE_SWAP,
  SPTE_ZERO,
  SPTE_LOADED,

};

struct supplemental_page_table_entry
{
  void* uvaddr;                   /* User virtual address. */
  enum SPTE_STATE state;          /* State of spte. */
  
  struct frame_table_entry* fte;  /* Frame table entry. */

  struct file* file;              /* File to do lazy loading. */
  off_t ofs;                      /* File offset. */
  size_t page_read_bytes;         /* Bytes to read from FILE. */
  size_t page_zero_bytes;         /* Bytes to fill zero. */

  bool writable;                  /* Writable by user process if this flag is true, otherwise read-only. */
  struct hash_elem he;            /* Hash table element. */

};


/* New Function declarations. */
unsigned spte_hash(const struct hash_elem *he, void* aux);
bool spte_less (const struct hash_elem* he_a, const struct hash_elem* he_b, void* aux);
void spt_init(struct hash* spt);
void spt_destroy(struct hash* spt);
void spte_free(struct hash_elem* spte_he, void* aux);
struct supplemental_page_table_entry* spte_create(void);
void spte_print(struct hash_elem* spte_he, void* aux);
bool spte_insert(struct hash* spt, struct hash_elem* spte_he);
void spt_print(struct hash* spt);
struct supplemental_page_table_entry* spte_find(struct hash* spt, void* uvaddr);

#endif
